-- 车站表【车站编号，车站名，所属城市】
-- Station (SID int, SName char(20), CityName char(20))
-- 其中，主码为车站编号。
create table station(
SID int,
sname char(20),
cityname char(22),
constraint PK_station primary key(SID)
);

-- 车次表【列车流水号，发车日期，列车名称，起点站编号，终点站编号，开出时刻，终点时刻】
-- Train (TID int, SDate date, TName char(20), SStationID int, AStationID int, STime datetime, ATime datetime)
-- 其中，TID为主码，(列车名称，发车日期)为候选码；SStationID和AStationID都来源于车站表的SID。
create table train(
TID int,
SDate date,
TName char(20), 
SStationID int,
AStationID int, 
STime datetime, 
ATime datetime,
constraint PK_train primary key (TID),
constraint UK_date_tname unique(SDate,TName),
constraint FK_train_SStationId foreign key(SStationID) REFERENCES station(SID),
constraint FK_train_AStationId foreign key(AStationID) REFERENCES station(SID)
);
set FOREIGN_KEY_CHECKS =0;
drop table train;
set FOREIGN_KEY_CHECKS =1;
-- 建立部分车次表，以转存excel表数据
-- 增加的startID：起点站编号、destID：终点站编号;depart 出发时间；days:第几天到达
create table train0(
trainno char(11) primary key,
startID int,
start char(10),
destID int,
dest char(10),
depart time,
arrival time,
days int,
distance int
);
update train0 join station on train0.start = station.sname
set startid = station.SID;
update train0 join station on train0.dest = station.sname
set destid = station.SID;

create procedure TrainArrange()
begin
declare TID1 int;
declare SDate1 date;
declare TName1 char(20);
declare SStationID1 int;
declare AStationID1 int;
declare STime1 time;
declare ATime1 time;
declare d int;
declare arrivalDate Date;
declare done boolean default 0;
declare train_cur cursor for select trainno,startID,destID,depart,arrival,days from train0 order by trainno;
declare continue handler for not FOUND set done =1;
open train_cur;
fetch train_cur into TName1,SStationID1,AStationID1,STime1,ATime1,d;
set TID1=1;
while not done do 
set SDate1='2020-01-10';
while(SDate1<='2020-01-25') do
if(d<2) then set arrivalDate = SDate1;
else set arrivalDate = date_add(SDate1,interval d-1 day);
end if;
	-- 2020-01-23至2020-4-07武汉出发和武汉到达的车次停运
if not (
((arrivalDate>'2020-01-22' and arrivalDate<'2020-04-08') and 
(AStationID1 in (select SID from station where cityname ='武汉'))) or
((SDate1>'2020-01-22' and SDate1 <'2020-04-08')and 
(SStationID1 in (select SID from station where cityname ='武汉')))
) 
then insert into train values(TID1,SDate1,TName1,SStationID1,AStationID1,
concat(SDate1, " ", STime1),
concat(arrivalDate, " ", ATime1));
set TID1 =TID1 +1;
end if;
set SDate1 =date_add(SDate1,interval 1 day);
end while;
fetch train_cur into TName1,SStationID1,AStationID1,STime1,ATime1,d;
end while;
	close train_cur;
end ;
drop procedure TrainArrange;


call TrainArrange();
-- 车程表【列车流水号，车站序号，车站编号，到达时刻，离开时刻】
-- TrainPass (TID int, SNo smallint, SID int, STime datetime, ATime datetime)
-- 其中，主码为(TID,SNo)。SID来源于车站表的SID。
create table trainpass(
TID int, 
SNo smallint, 
SID int, 
STime datetime, 
ATime datetime,
CONSTRAINT PK_TrainPass primary key(TID,SNo),
CONSTRAINT FK_TP_TID FOREIGN key(TID) REFERENCES Train(TID),
CONSTRAINT FK_TP_SID FOREIGN key(SID) REFERENCES station(SID)
);
-- 中间表，导入部分车次的运行时刻表：车次，站次，···
create table timetable(
trainno char(11),
SNo smallint,
SID int,
sname char(20),
STime time,
ATime time,
D int,
PRIMARY key(trainno,SNo)
);
-- 填写SID车站的编号
update timetable join station on station.sname = timetable.sname
set timetable.SID = station.SID;

-- 遍历train表，每一行只取TID,SDtate和TName
-- 根据TNAME从timeable 中取出SNO,SID,ATIME,D,WTHER TRAIN0 = TNMAE对应的额遍历
-- 遍历每一个站点，从出发日期算出到站时间，离站时间然后插入
delimiter $$
create procedure DetailArrange()
begin
declare TID1,SNO1,SID1 INT;
declare SDate1 date;
declare TName1 char(20);
declare STime1 time;
declare ATime1 time;
declare d1 int;
declare arrivalDate Date;
declare departDate Date;

declare outer_done boolean default 0;
declare inner_done boolean default 0;

declare train_cur cursor for select TID,SDate,TName from train order by TID;
declare tt_cur cursor for
select SNO,SID,STime,ATime,D from timetable where trainno =TName1 order by SNO;
declare continue HANDLER for not found
begin
set outer_done=1;
set inner_done=1;
end;
open train_cur;
fetch train_cur into TID1,SDate1,TName1;
while not outer_done do 
set inner_done =0;
open tt_cur;
fetch tt_cur into SNO1,SID1,STime1,ATime1,D1;
while not inner_done do
if(D1<2) then set arrivalDate =SDate1;
else set arrivalDate = date_add(SDate1,interval D1-1 day);
end if;
if STime1 <ATime1 then set departDate = arrivalDate;
else set departDate=date_add(arrivalDate,interval 1 day);
end if;
if not ((arrivalDate>'2020-01-22' and arrivalDate<'2020-04-08')
and
(SID1 IN (SELECT SID FROM station where cityname='武汉'))) then
insert into trainpass values(TID1,SNO1,SID1,
concat(arrivalDate ," ", STime1),
concat(departDate ," ", ATime1));
end if;
fetch tt_cur into SNO1,SID1,STime1,ATime1,D1;
END WHILE;
SET outer_done=0;
close tt_cur;
fetch train_cur into TID1 ,SDate1,TName1;
end while;
close train_cur;
end$$
drop procedure DetailArrange;
delimiter;
call DetailArrange();
-- 乘客表【乘客身份证号，姓名，性别，年龄】
-- Passenger (PCardID char(18), PName char(20), Sex bit, Age smallint)
-- 其中，主码为乘客身份证号；性别取值为0/1（“1”表示“男”，“0”表示“女”）。
create table Passenger (
PCardID char(18),
PName char(20), 
Sex bit, 
Age smallint,
CONSTRAINT PK_passenger primary key(PCardID)
);
-- 键姓名表
-- sno为自增长列，
-- create table nmlist(
-- sno int primary key auto_increment,
-- pid char(18),
-- nm char(10),
-- sex bit,
-- age smallint
-- );


-- 乘车记录表【记录编号，乘客身份证号，列车流水号，出发站编号，到达站编号，车厢号，席位排号，席位编号，席位状态】
-- TakeTrainRecord (RID int, PCardID char(18), TID int, SStationID int, AStationID int, CarrigeID smallint, SeatRow smallint，SeatNo char(1)，SStatus int)
-- 其中，主码、外码请依据应用背景合理定义。
-- CarrigeID若为空，则表示“无座”；
-- SeatNo只能取值为’A’、’B’、’C’、’E’、’F’，或为空值；
-- SStatus只能取值’0’（退票）、’1’（正常）、’2’（乘客没上车）。

create table TakeTrainRecord (
RID int,
PCardID char(18), 
TID int, 
SStationID int, 
AStationID int, 
CarrigeID smallint, 
SeatRow smallint,
SeatNo char(1),
SStatus int,
CONSTRAINT PK_TTR_RID PRIMARY key(RID),
CONSTRAINT FK_TTR_PCardID foreign key (PCardID) REFERENCES passenger(PCardID),
CONSTRAINT FK_TTR_SStationID foreign key(SStationID) REFERENCES station(SID),
CONSTRAINT FK_TTR_AStationID foreign key(AStationID) REFERENCES station(SID),
CONSTRAINT CK_TTR_SeatNo check(SeatNo in('A','B','C','D','E','F')),
CONSTRAINT CK_TTR_SStatus check(SStatus in (0,1,2))
);
-- 诊断表【诊断编号，病人身份证号，诊断日期，诊断结果，发病日期】
-- DiagnoseRecord (DID int, PCardID char(18), DDay date, DStatus smallint, FDay date)
-- 其中，主码为DID；DStatus包括：1：新冠确诊；2：新冠疑似；3：排除新冠
create table DiagnoseRecord (
DID int, 
PCardID char(18), 
DDay date, 
DStatus smallint, 
FDay date,
CONSTRAINT PK_DR_DID PRIMARY key(DID),
CONSTRAINT CK_DR_DStatus check(DStatus in(1,2,3))
);
-- 乘客紧密接触者表【接触日期, 被接触者身份证号，状态，病患身份证号】
-- TrainContactor (CDate date, CCardID char(18), DStatus smallint, PCardID char(18))
-- 其中，主码为全码。DStatus包括：1：新冠确诊；2：新冠疑似；3：排除新冠
create table TrainContactor (
CDate date, 
CCardID char(18), 
DStatus smallint, 
PCardID char(18),
CONSTRAINT PK_TC_all PRIMARY key (CDate,CCardID,DStatus,PCardID),
CONSTRAINT CK_TC_DStatus check(DStatus in(1,2,3))
);
#1
/*分别用一条sql语句完成对乘车记录表基本的增、删、改的操作*/
INSERT INTO TakeTrainRecord VALUES(41,'65432319991017375X',1,1599,1621,7,2,'C',2);
UPDATE TakeTrainRecord SET SStatus = 1 where RID = 41;
DELETE FROM TakeTrainRecord WHERE RID = 41;
#2 /*将乘车记录表中的从武汉出发的乘客的乘车记录插入到一个新表WH_TakeTrainRecord中*/
CREATE TABLE WH_TakeTrainRecord AS (SELECT * FROM TakeTrainRecord where SStationID IN (SELECT SID FROM station WHERE cityname='武汉'));
#3
/*数据导入导出*/
SELECT * FROM WH_TakeTrainRecord INTO OUTFILE "D:/MySQL/MySQL Server 8.0/Uploads/wh_taketrainrecord.txt";
-- 限制了本地文件加载
set global local_infile = 1;
LOAD DATA LOCAL INFILE "D:/MySQL/MySQL Server 8.0/Uploads/wh_taketrainrecord.txt" INTO TABLE WH_TakeTrainRecord;
#4
/*观察性实验*/
DROP TABLE IF EXISTS test;
CREATE TABLE test(
	a int,
	b int,
	c int
);
INSERT INTO test VALUES (1,2,3);
INSERT INTO test VALUES (1,2,3);
INSERT INTO test VALUES (1,2,3);
#均发生了修改
SELECT * FROM test;
DROP TABLE test;
#5
/*创建一个新冠确诊病人的乘火车记录视图，其中的属性包括：身份证号、姓名、年龄、乘坐列车编号、发车日期、车厢号，席位排号，席位编号。按身份证号升序排序，如果身份证号一致，按发车日期降序排序（注意，如果病人买了票但是没坐车，不计入在内）。*/
DROP VIEW IF EXISTS digTakeRecord;
CREATE VIEW digTakeRecord 
AS
SELECT passenger.PCardID,PName,Age,train.TID,SDate,CarrigeID,SeatRow,SeatNo
FROM passenger,train,taketrainrecord,diagnoserecord
WHERE passenger.PCardID=diagnoserecord.PCardID 
AND passenger.PCardID=diagnoserecord.PCardID 
AND train.TID=taketrainrecord.TID
AND diagnoserecord.DStatus=1
AND taketrainrecord.SStatus=1
ORDER BY passenger.PCardID,train.SDate DESC;

#6
/*编写一个触发器，用于实现以下完整性控制规则：
1) 当新增一个确诊患者时，若该患者在发病前14天内有乘车记录，则将其同排及前后排乘客自动加入“乘客紧密接触者表”，其中：接触日期为乘车日期。
2) 当一个紧密接触者被确诊为新冠时，从“乘客紧密接触者表”中修改他的状态为“1”。
*/

CREATE
    TRIGGER trigger1 AFTER UPDATE ON DiagnoseRecord
    FOR EACH ROW 
BEGIN
	declare Cdate date;
	declare Cardid char(18);
	declare Dstatus int;
	declare Pcardid char(18);
	set Dstatus =2;
	set Pcardid = new.PCardID;
	select TakeTrainRecord.PCardID into Cardid where TakeTrainRecord.TID in (select train.TID from TakeTrainRecord,train where TakeTrainRrcord.TID = train.ITD and
new.Pcardid = TakeTrainRecord.PCardID and
new.Dstatus =1 and
train.SDate>=date_sub(new.FDay,interval 14 day)and
train.SDate<=new.FDay
);
if Cardid!=null then
insert into TrainContactor values(Cdate,Cardid,Dstatus,Pcardid);
	END IF;
	update TrainContactor set DStatus = new.DStatus where(TrainContactor.CCardID = new.PCardID);
    END;

/*1)查询确诊者“张三”的在发病前14天内的乘车记录*/
select TakeTrainRecord.*,passenger.PName from TakeTrainRecord,train,DiagnoseRecord,passenger where 
TakeTrainRecord.TID = train.TID and 
DiagnoseRecord.PCardID=TakeTrainRecord.PCardID and
passenger.PCardID =DiagnoseRecord.PCardID AND
passenger.PName= '张三' AND
DiagnoseRecord.DStatus = 1 AND 
train.SDate <= DiagnoseRecord.FDay AND 
train.SDate >= DATE_SUB(DiagnoseRecord.FDay,INTERVAL 14 DAY);
/*2）查询所有从城市“武汉”出发的乘客乘列车所到达的城市名； */
/*DISTINCT过滤掉重复值*/
select DISTINCT station.cityname from station,TakeTrainRecord where 
station.SID = TakeTrainRecord.AStationID and
TakeTrainRecord.SStationID IN
(SELECT station.SID from station where station.cityname='武汉');
/*3）计算每位新冠患者从发病到确诊的时间间隔（天数）及患者身份信息，并将结果按照发病时间天数的降序排列；*/
-- DATEDIFF() 函数返回两个日期之间的时间
select datediff (DiagnoseRecord.DDay,DiagnoseRecord.FDay)As DayInterval,passenger.* from passenger,DiagnoseRecord where
passenger.PCardID =DiagnoseRecord.PCardID and
DiagnoseRecord.DStatus=1 ORDER BY DayInterval desc;
/*4）查询“2020-01-22”从“武汉”发出的所有列车；*/
select train.* from train where train.SDate ='2020-01-22' and
train.SStationID in (select distinct SID FROM station where
station.cityname='武汉');
/*5）查询“2020-01-22”途经“武汉”的所有列车；*/
SELECT train.*
FROM train,trainpass
WHERE train.TID=trainpass.TID AND train.SDate='2020-01-22' 
AND trainpass.SID IN
		(SELECT SID 
		 FROM station 
		 WHERE CityName='武汉');
/*6）查询“2020-01-22”从武汉离开的所有乘客的身份证-号、所到达的城市、到达日期； */
select passenger.PCardID,station.cityname,train.ATime from passenger,TakeTrainRecord,station,train where passenger.PCardID=TakeTrainRecord.PCardID and
station.SID=TakeTrainRecord.AStationID and
train.TID=TakeTrainRecord.AStationID and
TakeTrainRecord.TID in
(select DISTINCT train.TID from train where train.SDate ='2020-01-22'and
train.SStationID in
(select distinct SID FROM station where
station.cityname='武汉'))
GROUP BY(cityname);
/*7）统计“2020-01-22” 从武汉离开的所有乘客所到达的城市及达到各个城市的武汉人员数。*/
select station.cityname,count(cityname) as number from passenger,taketrainrecord,station,train where
passenger.PCardID = taketrainrecord.PCardID and
station.SID = taketrainrecord.AStationID and
train.TID = taketrainrecord.TID and
taketrainrecord.TID in
(select DISTINCT train.TID from train where train.SDate ='2020-01-22'and
train.SStationID in
(select distinct SID FROM station where
station.cityname='武汉'))
GROUP BY(cityname);
/*8）查询2020年1月到达武汉的所有人员；*/
select distinct passenger.* from passenger,taketrainrecord,station,train where passenger.PCardID = taketrainrecord.PCardID and
taketrainrecord.AStationID in
(select distinct SID FROM station where
station.cityname='武汉')
and taketrainrecord.TID = train.TID and
train.ATime<'2020-02-01';
/*9） 查询2020年1月乘车途径武汉的外地人员（身份证非“420”开头）；*/
select distinct passenger.* from passenger,train,trainpass,taketrainrecord where
passenger.PCardID=taketrainrecord.PCardID and
trainpass.TID=train.TID and
left(passenger.PCardID,3)!='420' and
trainpass.SID in
(select distinct SID FROM station where
station.cityname='武汉') and
taketrainrecord.TID=train.TID and
train.STime>='2020-01-01' and
train.STime<'2020-02-01';
/*10）统计“2020-01-22”乘坐过‘G007’号列车的新冠患者在火车上的密切接触乘客人数（每位新冠患者的同车厢人员都算同车密切接触）。*/
select count(*) as counts from taketrainrecord where
(taketrainrecord.TID,taketrainrecord.CarrigeID)in
(select taketrainrecord.TID,taketrainrecord.CarrigeID from
passenger,taketrainrecord,train,diagnoserecord where
taketrainrecord.TID=train.TID and
train.ATime='2020-01-22' and
train.TName='G007' and
passenger.PCardID=taketrainrecord.PCardID and
diagnoserecord.PCardID=passenger.PCardID and
diagnoserecord.DStatus=1);
/*11）查询一趟列车的一节车厢中有3人及以上乘客被确认患上新冠的列车名、出发日期，车厢号；*/ 
select train.TName,train.SDate,taketrainrecord.CarrigeID from
train,taketrainrecord,diagnoserecord where
train.TID=taketrainrecord.TID and
taketrainrecord.PCardID=diagnoserecord.PCardID and
diagnoserecord.DStatus=1
GROUP BY train.TName,train.SDate,taketrainrecord.CarrigeID having count(*)>=3;
/*12）查询没有感染任何周边乘客的新冠乘客的身份证号、姓名、乘车日期； */
-- ?
SELECT DISTINCT passenger.PCardID,PName,CDate
FROM passenger,traincontactor
WHERE passenger.PCardID=traincontactor.PCardID AND CCardID NOT IN
(SELECT DISTINCT CCardID FROM traincontactor WHERE DStatus=1);
/*13）查询到达 “北京”、或“上海”，或“广州”（即终点站）的列车名，要求where子句中除了连接条件只能有一个条件表达式；*/
select DISTINCT train.TName from train where
train.AStationID in
(select station.SID from station where
station.cityname='北京' or station.cityname='上海' or
station.cityname='广州');
/*14）查询“2020-01-22”从“武汉站”出发，然后当天换乘另一趟车的乘客身份证号和首乘车次号，结果按照首乘车次号降序排列，同车次则按照乘客身份证号升序排列；*/
select train.TID,passenger.PCardID from train,passenger,taketrainrecord WHERE
passenger.PCardID = taketrainrecord.PCardID and
taketrainrecord.TID = train.TID and
train.TID in
(select train.TID from train where train.SDate='2020-01-22' and train.SStationID in
(select DISTINCT SID from station where
station.sname='武汉'))and
((taketrainrecord.AStationID,passenger.PCardID)in
(select taketrainrecord.SStationID,passenger.PCardID from train,passenger,taketrainrecord where
passenger.PCardID=taketrainrecord.PCardID and 
taketrainrecord.TID = train.TID and train.TID in
(select train.TID from train where train.SDate='2020-01-22'))) 
ORDER BY train.TID desc,PCardID;
/*15）查询所有新冠患者的身份证号，姓名及其2020年以来所乘坐过的列车名、发车日期，要求即使该患者未乘坐过任何列车也要列出来；*/
-- LEFT JOIN 关键字会从左表那里返回所有的行，即使在右表中没有匹配的行
select temp.PCardID,PName,Tname,SDate from
(select passenger.PCardID,PName from
passenger,diagnoserecord where
passenger.PCardID=diagnoserecord.PCardID and
diagnoserecord.DStatus=1)
temp left join
(select TName,SDate,PCardID from taketrainrecord,train where
taketrainrecord.TID=train.TID and
train.SDate>='2020-01-01')
temp1 on temp.PCardID = temp1.PCardID  ORDER BY temp.PCardID,temp1.SDate desc;
/*16）查询所有发病日期相同而且确诊日期相同的病患统计信息，包括：发病日期、确诊日期和患者人数，结果按照发病日期降序排列的前提下再按照确诊日期降序排列。*/
select FDay,DDay,count(*) as total from
diagnoserecord where DStatus =1 group by
FDay,DDay ORDER BY FDay desc,DDay desc;
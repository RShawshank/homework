/*采用B/S或C/S模式实现一个汽车租借信息系统。完成用户、车辆、经手员工、租借情况、车辆损毁情况、交通违规罚款等信息的管理。
要求：
1）实现不同权限的浏览和更新。
2）能够根据车辆使用情况计算押金退还金额。
3）能查询客户的租借历史记录，并进行信誉度评价，进行会员制和非会员制的客户管理。
4）能够管理车辆报修信息；
5）能够生成租借公司的日、月、季度、年财务报表。
*/
DROP TABLE IF EXISTS `carinfo`;
CREATE TABLE `carinfo` (
                           `ID` int(11) NOT NULL,
                           `BRAND` varchar(11) NOT NULL,
                           `RENT` int(11) NOT NULL,
                           `STATU` int(11) NOT NULL,
                           `PLEDGE` int(11) NOT NULL,
                           `VALID` int(11) NOT NULL,
                           `CID` varchar(11) DEFAULT NULL,
                           `LASTLOGIN` datetime DEFAULT NULL,
                           PRIMARY KEY (`ID`),
                           KEY `K5_idx` (`CID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='车辆信息表';
DROP TABLE IF EXISTS `customerinfo`;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `customerinfo` (
  `ID` int(11) NOT NULL,
  `CNAME` varchar(11) NOT NULL,
  `MEMBER` tinyint(1) NOT NULL,
  `PASSWD` varchar(11) NOT NULL,
  `DLRQ` datetime DEFAULT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='顾客信息表';

DROP TABLE IF EXISTS `diary`;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `diary` (
  `INFOID` int(11) NOT NULL,
  `CARID` int(11) NOT NULL,
  `CUSID` int(11) NOT NULL,
  `STUFFID` int(11) NOT NULL,
  `DTIME` datetime NOT NULL,
  `DEVENT` int(11) NOT NULL,
  `DETAIL` varchar(255) DEFAULT NULL,
  `COST` int(11) NOT NULL,
  PRIMARY KEY (`INFOID`),
  KEY `K1` (`CARID`),
  KEY `K3` (`CUSID`),
  KEY `K3_idx` (`STUFFID`),
  CONSTRAINT `K1` FOREIGN KEY (`CARID`) REFERENCES `carinfo` (`ID`),
  CONSTRAINT `K2` FOREIGN KEY (`CUSID`) REFERENCES `customerinfo` (`ID`),
  CONSTRAINT `K3` FOREIGN KEY (`STUFFID`) REFERENCES `employeeinfo` (`ID`),
  CONSTRAINT `diary_chk_1` CHECK ((`INFOID` > 0))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='日记';


DROP TABLE IF EXISTS `employeeinfo`;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `employeeinfo` (
  `ID` int(11) NOT NULL,
  `ENAME` varchar(11) NOT NULL,
  `TITLE` int(11) NOT NULL,
  `PASSWD` varchar(11) NOT NULL,
  `DLRQ` datetime NOT NULL,
  PRIMARY KEY (`ID`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='员工信息表';

DROP TABLE IF EXISTS `info1`;
SET @saved_cs_client     = @@character_set_client;
SET character_set_client = utf8mb4;
/*!50001 CREATE VIEW `info1` AS SELECT 
 1 AS `infoid`,
 1 AS `carid`,
 1 AS `cusid`,
 1 AS `stuffid`,
 1 AS `cname`,
 1 AS `ename`*/;
SET character_set_client = @saved_cs_client;


DROP TABLE IF EXISTS `profit`;
 SET character_set_client = utf8mb4 ;
CREATE TABLE `profit` (
  `PID` int(11) NOT NULL,
  `PEVENT` int(11) NOT NULL,
  `PDATETIME` datetime NOT NULL,
  `PPROFIT` double NOT NULL,
  PRIMARY KEY (`PID`),
  CONSTRAINT `profit_chk_1` CHECK ((`PID` > 0))
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;
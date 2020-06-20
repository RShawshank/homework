DROP TABLE IF EXISTS `T_BRXX`;
CREATE TABLE `T_BRXX` (
  `BRBH` char(6) NOT NULL,
  `BRMC` char(10) NOT NULL,
  `DLKL` char(8) NOT NULL,
  `YCJE` decimal(10,2) NOT NULL,
  `DLRQ` timestamp  NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY  (`BRBH`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `T_BRXX` WRITE;
INSERT INTO `T_BRXX` VALUES ('001','张一','123456',1000.00,now()),('002','张二','123456',5000.00,now()),('003','张三','123456',50.00,now()),('004','张四','123456',500.00,now()),('005','张五','123456',0.00,now()),('006','张六','123456',40.00,now());
UNLOCK TABLES;

DROP TABLE  IF EXISTS `T_KSYS`;
CREATE TABLE `T_KSYS` (
  `YSBH` char(6),
  `KSBH` char(6) NOT NULL,
  `YSMC` char(10) NOT NULL,
  `PYZS` char(4),
  `DLKL` char(8) NOT NULL,
  `SFZJ` int NOT NULL,
  `DLRQ` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY  (`YSBH`),
    KEY `KSBH` (`KSBH`),
  CONSTRAINT `T_KSYS_ibfk_1` FOREIGN KEY(`KSBH`) REFERENCES `T_KSXX`(`KSBH`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `T_KSYS` WRITE;
INSERT INTO `T_KSYS` VALUES ('001','01','刘医生','liu','123456',0,'2020-05-03 16:45:15'),('002','01','饶医生','rao','123456',1,'0000-00-00 00:00:00'),('003','02','周医生','zhou','123456',0,'0000-00-00 00:00:00'),('004','02','胡医生','hu','123456',1,'0000-00-00 00:00:00'),('005','03','方医生','fang','123456',0,'0000-00-00 00:00:00'),('006','03','王医生','wang','123456',1,'0000-00-00 00:00:00'),('007','04','杜医生','du','123456',0,'0000-00-00 00:00:00'),('008','04','李医生','li','123456',1,'0000-00-00 00:00:00'),('009','05','付医生','fu','123456',0,'0000-00-00 00:00:00'),('010','05','钱医生','qian','123456',1,'0000-00-00 00:00:00'),('011','06','乔医生','qiao','123456',0,'0000-00-00 00:00:00'),('012','06','丁医生','ding','123456',1,'0000-00-00 00:00:00');
UNLOCK TABLES;

DROP TABLE  IF EXISTS `T_HZXX`;
CREATE TABLE `T_HZXX` (
  `HZBH` char(6),
  `HZMC` char(12) NOT NULL,
  `PYZS` char(4) NOT NULL,
  `KSBH` char(6) NOT NULL,
  `SFZJ` int NOT NULL,
  `GHRS` int NOT NULL,
  `GHFY`decimal(10,2) NOT NULL,
  PRIMARY KEY  (`HZBH`),
  FOREIGN KEY(`KSBH`) references `T_KSXX`(`KSBH`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
LOCK TABLES `T_HZXX` WRITE;
INSERT INTO `T_HZXX` VALUES ('001','专家1','zj1','01',1,100,100.70),('002','主治1','zz1','01',0,200,50.12),('003','专家2','zj2','01',1,100,121.65),('004','主治2','zz2','01',0,200,19.11),('005','专家1','zj1','02',1,50,128.32),('006','主治1','zz2','01',0,211,34.31),('007','专家2','zj2','02',1,50,143.23),('008','主治2','zz2','02',0,200,11.35),('009','专家1','zj1','03',1,100,122.32),('010','主治1','zz1','03',0,200,15.39),('011','专家2','zz2','03',1,50,223.49),('012','主治2','zz2','03',0,100,34.21),('013','专家1','zj1','04',1,100,157.32),('014','主治1','zz1','04',0,200,16.45),('015','专家2','zj2','04',1,100,133.56),('016','主治2','zz2','04',0,200,17.43),('017','专家1','zj1','05',1,50,254.61),('018','主治1','zz1','05',0,200,34.59),('019','专家2','zj2','05',1,50,139.49),('020','主治2','zz2','05',0,100,18.43),('021','专家1','zj1','06',1,200,342.32),('022','主治1','zz1','06',0,400,37.42),('023','专家2','zj2','06',1,20,229.37),('024','主治2','zz2','06',0,100,13.46);
UNLOCK TABLES;
DROP TABLE  IF EXISTS `T_KSXX`;
CREATE TABLE `T_KSXX` (
  `KSBH` char(6),
  `KSMC` char(10) NOT NULL,
  `PYZS` char(8) NOT NULL,
  PRIMARY KEY  (`KSBH`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

LOCK TABLES `T_KSXX` WRITE;
INSERT INTO `T_KSXX` VALUES ('001','内科','neike'),('002','外科','waike'),('03','眼科','yanke'),('04','儿科','erke'),('05','眼科','yanke'),('06','骨科','guke');
UNLOCK TABLES;
DROP TABLE  IF EXISTS `T_GHXX`;
CREATE TABLE `T_GHXX` (
  `GHBH` char(6),
  `HZBH` char(6) NOT NULL,
  `YSBH` char(6) NOT NULL,
  `BRBH` char(6) NOT NULL,
  `GHRC` int NOT NULL,
  `THBZ` int NOT NULL,
  `GHFY` decimal(8,2) NOT NULL ,
  `RQSJ` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
  PRIMARY KEY  (`GHBH`),
  KEY `HZBH` (`HZBH`),
  KEY `YSBH` (`YSBH`),
  KEY `BRBH` (`BRBH`),
  FOREIGN KEY(`HZBH`) references `T_HZXX`(`HZBH`),
  FOREIGN KEY(`YSBH`) references `T_KSYS`(`YSBH`),
  FOREIGN KEY(`BRBH`) references `T_BRXX`(`BRBH`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
LOCK TABLES `T_GHXX` WRITE;
UNLOCK TABLES;

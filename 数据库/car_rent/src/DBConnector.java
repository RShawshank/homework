import emtity.*;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.collections.ObservableSet;
import javafx.scene.chart.PieChart;
import javafx.scene.chart.XYChart;

import javax.management.ObjectName;
import javax.print.DocFlavor;
import javax.swing.*;
import java.nio.file.LinkOption;
import java.security.interfaces.RSAKey;
import java.sql.*;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
class DbUtils {
    public static void closeConn(Connection conn) {
        if (conn != null) {
            try {
                conn.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
            conn = null;
        }
    }

    public static void closeStat(PreparedStatement stat) {
        if (stat != null) {
            try {
                stat.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
            stat = null;
        }
    }

    public static void closeCursor(ResultSet cursor) {
        if (cursor != null) {
            try {
                cursor.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
            cursor = null;
        }
    }

    public static void closeResource(ResultSet cursor, PreparedStatement stat) {
        closeCursor(cursor);
        closeStat(stat);
    }
}

public class DBConnector {
    private static DBConnector instance;
    private Connection connection;
    private PreparedStatement Pstatement;

    private DBConnector() {
        //将mysql驱动注册到DriverManager中去
        /*
        Class.forName() 方法要求JVM查找并加载指定的类到内存中；
         */
        try {
            Class.forName("com.mysql.jdbc.Driver");
        } catch (ClassNotFoundException e) {
            System.out.println("找不到驱动类，加载失败！");
            e.printStackTrace();
        }
    }

    //单例模式
    public static synchronized DBConnector getInstance() {
        if (instance == null)
            instance = new DBConnector();
        return instance;
    }

    //创建数据库的连接
    public void connectionDatabase(String hostname, String DBname, Integer port, String username, String password) throws SQLException {
        String url = "jdbc:mysql://" + hostname + ":" + port + "/" + DBname + "?zeroDateTimeBehavior=convertToNull&autoReconnect=true&characterEncoding=UTF-8&characterSetResults=UTF-8";
        connection = DriverManager.getConnection(url, username, password);
    }

    public Connection getConnection() {
        return connection;
    }

    //检查用户登录是否正确
    public boolean CheckPassword(int type, int chooseId, String password) throws SQLException {
        String customer_passwd = "select PASSWD from customerinfo where ID=?";
        String employee_passwd = "select PASSWD from employeeinfo where ID=?";
        if (type == 0) {
            Pstatement = connection.prepareStatement(customer_passwd);
            Pstatement.setInt(1, chooseId);
        } else if (type == 1) {
            Pstatement = connection.prepareStatement(employee_passwd);
            Pstatement.setInt(1, chooseId);
        } else
            return false;
        ResultSet resultSet = Pstatement.executeQuery();
        if (resultSet.next()) {
            String passwd = resultSet.getString("PASSWD");
            //密码正确，更新登录时间
            if (passwd.equals(password)) {
                SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
                if (type == 0) {
                    String updatecustomerinfo = "update customerinfo set LOGINTIME = "
                            + "'" + df.format(new Date()) + "' where id=?";
                    Pstatement = connection.prepareStatement(updatecustomerinfo);
                    Pstatement.setInt(1, chooseId);
                } else if (type == 1) {
                    String updateemployeeinfo = "update employeeinfo set LOGINTIME = "
                            + "'" + df.format(new Date()) + "' where id=?";
                    Pstatement = connection.prepareStatement(updateemployeeinfo);
                    Pstatement.setInt(1, chooseId);
                } else return false;
                Pstatement.executeUpdate();
            } else {
                return false;
            }
        }
        return true;
    }

    //获得汽车品牌列表
    public ObservableList getBandFromCarinfo() throws SQLException {
        String sql = "select distinct BRAND from CARINFO";
        Pstatement = connection.prepareStatement(sql);
        ResultSet resultSet = Pstatement.executeQuery();
        String brand;
        ObservableList<String> brandList = FXCollections.observableArrayList();
        while (resultSet.next()) {
            brand = resultSet.getString("BRAND");
            brandList.add(brand);
        }
        return brandList;
    }

    //获得员工姓名列表
    public ObservableList getNAMEfromemployeeinfo() {
        ObservableList<String> nameList = FXCollections.observableArrayList();
        try {
            String sql = "select NAME from EMPLOYEEINFO ";
            Pstatement = connection.prepareStatement(sql);
            ResultSet resultSet = Pstatement.executeQuery();
            String name;
            while (resultSet.next()) {
                name = resultSet.getString("NAME");
                nameList.add(name);
            }
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return nameList;
    }

    public void getInfoFromCarinfo(String band, ObservableList id, ObservableList statu, ObservableList pledge, ObservableList rent) throws SQLException {
        String sql = "select ID,BRAND,RENT_PAY,STATUS,PLEDGE_PAY,VALID FROM CARINFO WHERE BRAND=? AND VALID=0";
        Pstatement = connection.prepareStatement(sql);
        Pstatement.setString(1, band);
        ResultSet resultSet = Pstatement.executeQuery();
        id.clear();
        statu.clear();
        pledge.clear();
        rent.clear();
        while (resultSet.next()) {
            id.add(resultSet.getString("ID"));
            statu.add(resultSet.getString("STATUS"));
            rent.add(resultSet.getString("RENT_PAY"));
            pledge.add(resultSet.getString("PLEDGE_PAY"));
        }
    }

    //根据排序进行查找id
    public int getIDfromemployeeinfo(int index) throws SQLException {
        String sql = "select ID FROM EMPLOYEEINFO ORDER BY ID LIMIT ?,1";
        Pstatement = connection.prepareStatement(sql);
        Pstatement.setInt(1, index);
        ResultSet resultSet = Pstatement.executeQuery();
        resultSet.next();
        int id = resultSet.getInt("ID");
        return id;
    }

    //更新登录时间
    public void updateTimecustomerOremployee(int type, int id) throws SQLException {
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String sql;
        if (type == 0) {
            sql = "update customerinfo set LOGINTIME = "
                    + "'" + df.format(new Date()) + "' where id= ?";
        } else if (type == 1) {
            sql = "update employeeinfo set LOGINTIME = "
                    + "'" + df.format(new Date()) + "' where id= ?";
        } else
            return;
        Pstatement = connection.prepareStatement(sql);
        Pstatement.setInt(1, id);
        Pstatement.executeUpdate();
    }

    //更新日志，载入租车信息
    public void updateDiary(int carid, int loginid, int stuffid, int rent) throws SQLException {
       /* String sql = "select count(*) as number from rentdiary";
        Pstatement = connection.prepareStatement(sql);
        ResultSet resultSet = Pstatement.executeQuery();
        resultSet.next();
        int number = resultSet.getInt("number");
        number--;
        String sql2 = "select INFOID from rentdiary ORDER BY INFOID limit ?,1";
        Pstatement = connection.prepareStatement(sql2);
        Pstatement.setInt(1, number);
        resultSet = Pstatement.executeQuery();
        resultSet.next();
        int infoid = resultSet.getInt("INFOID");
        infoid++;*/
        String sql = "select max(infoid) as number from rentdiary";
        Pstatement = connection.prepareStatement(sql);
        ResultSet resultSet = Pstatement.executeQuery();
        resultSet.next();
        int infoid = resultSet.getInt("number");
        infoid++;
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String sql3 = "insert into rentdiary(INFOID,CARID,CUSTOMERID,STUFFID,RENT_TIME,REVENT,RENT_COST) VALUES(?,?,?,?" + ",'" + df.format(new Date()) + "'," + "1,?)";
        Pstatement = connection.prepareStatement(sql3);
        Pstatement.setInt(1, infoid);
        Pstatement.setInt(2, carid);
        Pstatement.setInt(3, loginid);
        Pstatement.setInt(4, stuffid);
        Pstatement.setInt(5, rent);
        Pstatement.execute();
    }

    //租车后，更新汽车信息，同时将汽车计入收入表中
    public void updatecarinfo(int carid, int loginid, int rent) throws SQLException {
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String sql = "update carinfo set VALID=1 where id=?";
        String sql2 = "update carinfo  set LASTLOGIN='" + df.format(new Date()) + "' where id=?";
        String sql3 = "update carinfo  set CUSTOMERID=? where id=?";
        //String sql4 = "select count(*) as num from profit";
        String sql4="select max(pid)  pid from profit";
        Pstatement = connection.prepareStatement(sql);
        Pstatement.setInt(1, carid);
        Pstatement.executeUpdate();
        Pstatement = connection.prepareStatement(sql2);
        Pstatement.setInt(1, carid);
        Pstatement.executeUpdate();
        Pstatement = connection.prepareStatement(sql3);
        Pstatement.setInt(1, loginid);
        Pstatement.setInt(2, carid);
        Pstatement.executeUpdate();
        Pstatement = connection.prepareStatement(sql4);
        ResultSet resultSet = Pstatement.executeQuery();
        resultSet.next();
        int profitid = resultSet.getInt("pid");
        profitid++;
        String sql5 = "insert into profit values("+profitid+","+"1"+",'"+df.format(new Date())+"',"+rent+")";
        Pstatement = connection.prepareStatement(sql5);
        Pstatement.setInt(1, profitid);
        Pstatement.setInt(2,1);
        Pstatement.setDouble(3, rent);
        Pstatement.execute();
    }

    //客户端的客户信息录入
    public ObservableList getPersoninfo(int infoid, ObservableList list) throws SQLException {
        String id = null, name = null, member = null, dlrq = null, moral = null;
        String sql = "select ID,NAME,MEMBER,LOGINTIME FROM customerinfo WHERE ID = ?";
        Pstatement = connection.prepareStatement(sql);
        Pstatement.setInt(1, infoid);
        ResultSet resultSet = Pstatement.executeQuery();
        while (resultSet.next()) {
            id = resultSet.getString("ID");
            name = resultSet.getString("NAME");
            String member_temp = resultSet.getString("MEMBER");
            dlrq = resultSet.getString("LOGINTIME");
            //获得是否为会员
            if (member_temp.equals("0")) {
                member = "否";
            } else
                member = "是";
        }
        String sql2 = "select COUNT(REVENT) AS NUM from rentdiary WHERE customerid=?";
        Pstatement = connection.prepareStatement(sql2);
        Pstatement.setInt(1, infoid);
        resultSet = Pstatement.executeQuery();
        int temp_num = 0;
        while (resultSet.next()) {
            temp_num = resultSet.getInt("NUM");
        }
        switch (temp_num) {
            case 0:
                moral = "优秀";
                break;
            case 1:
            case 2:
                moral = "良好";
                break;
            case 3:
            case 4:
                moral = "一般";
                break;
            default:
                moral = "较差";
                break;
        }
        list.add(new CustomerPerson(id, name, member, moral, dlrq));
        return list;
    }

    //客户端汽车信息录入
    public ObservableList getCarinfo(int infoid) throws SQLException {
        ObservableList<customerReturnCar> list = FXCollections.observableArrayList();
        String valid = null, rent = null, id = null, brand = null, status = null, pledge = null, cid = null, lastlogin = null;
        String sql = "select * from carinfo where CUSTOMERID = ?";
        Pstatement = connection.prepareStatement(sql);
        Pstatement.setInt(1, infoid);
        ResultSet resultSet = Pstatement.executeQuery();
        while (resultSet.next()) {
            id = resultSet.getString("ID");
            brand = resultSet.getString("BRAND");
            rent = resultSet.getString("RENT_PAY");
            status = resultSet.getString("STATUS");
            pledge = resultSet.getString("PLEDGE_PAY");
            String tempvalid = resultSet.getString("VALID");
            if (tempvalid.equals("0"))
                valid = "否";
            else
                valid = "是";
            System.out.println(id);
            System.out.println(brand);
            list.add(new customerReturnCar(id, brand, rent, status, pledge, valid));
        }
        return list;
    }

    public void DeleteReturnedCar(int loginid, customerReturnCar customerReturnCar) throws SQLException {
        //获得租车花费，花费的金额根据租车的时间来计算
        String sql = "select lastlogin from carinfo where ID = ?";
        Pstatement = connection.prepareStatement(sql);
        Pstatement.setInt(1, Integer.parseInt(customerReturnCar.getCarId()));
        ResultSet resultSet = Pstatement.executeQuery();
        resultSet.next();
        String date_lastlogin = resultSet.getString("lastlogin");
        SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String currenttime = df.format(new Date());
        String sql2 = "select sum(rent_cost) as num4 from rentdiary where CARID = ? and RENT_TIME BETWEEN '"+date_lastlogin+"' AND '"+currenttime+"'";
        Pstatement = connection.prepareStatement(sql2);
        Pstatement.setInt(1, Integer.parseInt(customerReturnCar.getCarId()));
        resultSet = Pstatement.executeQuery();
        resultSet.next();
        int data_cost = resultSet.getInt("num4");//租车花费
        int addtional_pay = 0;//额外的费用
        int return_pay = 0;//返回的费用
        int pledge_pay = Integer.parseInt(customerReturnCar.getCarPledge());//租车押金
        if (data_cost > pledge_pay)//租车的花费大于押金
        {
            return_pay = 0;
            addtional_pay = data_cost - pledge_pay;
        } else {
            return_pay = data_cost - pledge_pay;
            addtional_pay = 0;
        }
        //车损定责没写
        //下面针对rentdiary进行补充，将还车信息输入其中
        String sql3 = "select max(infoid) as number from rentdiary";
        Pstatement = connection.prepareStatement(sql3);
        resultSet = Pstatement.executeQuery();
        resultSet.next();
        int data_infoID = resultSet.getInt("number");
        data_infoID++;
        String sql5 = "select STUFFID FROM rentdiary where infoid = ?";
        Pstatement = connection.prepareStatement(sql5);
        Pstatement.setInt(1,Integer.parseInt(customerReturnCar.getCarId()));
        resultSet = Pstatement.executeQuery();
        resultSet.next();
        int data_stuffid = resultSet.getInt("STUFFID");
        String sql6 = "insert into rentdiary(infoid,carid,customerid,stuffid,rent_time,revent,rent_cost) values (?,?,?,?,'"+currenttime+"',?,?)";
        Pstatement = connection.prepareStatement(sql6);
        Pstatement.setInt(1, data_infoID);
        Pstatement.setInt(2, Integer.parseInt(customerReturnCar.getCarId()));
        Pstatement.setInt(3, loginid);
        Pstatement.setInt(4, data_stuffid);
        Pstatement.setInt(5, 2);//2表示还车
        Pstatement.setInt(6, data_cost);
        Pstatement.execute();
        //更新收入表
        //当客户需要支付一定费用时
        if (return_pay != pledge_pay) {
            String sql7 = "select max(pid) as number from profit";
            Pstatement = connection.prepareStatement(sql7);
            resultSet = Pstatement.executeQuery();
            resultSet.next();
            int data_pid = resultSet.getInt("number");
            data_pid++;
            int profit_income = pledge_pay - return_pay + addtional_pay;
            String sql9 = "insert into profit(PID,PEVENT,PDATETIME,PPROFIT) VALUES(?,?,'"+currenttime+"',?)";
            Pstatement = connection.prepareStatement(sql9);
            Pstatement.setInt(1, data_pid);
            Pstatement.setInt(2, 2);
            Pstatement.setInt(3,profit_income);
            Pstatement.execute();
        }
        //修改汽车信息表
        String sql10 = "update carinfo set lastlogin=null,valid=0  where id=?";
        Pstatement = connection.prepareStatement(sql10);
        Pstatement.setInt(1, Integer.parseInt(customerReturnCar.getCarId()));
        Pstatement.execute();
        //输出信息
        if (addtional_pay == 0) {
            JOptionPane.showMessageDialog(null, "还车成功！退还结算后的押金：" + return_pay);
            return;
        } else {
            JOptionPane.showMessageDialog(null, "还车成功！还需补交金额：" + addtional_pay);
            return;
        }
    }

    //服务端加载全部汽车的信息
    public ObservableList getAllCarinfo(ObservableList list ) throws SQLException {
        String sql = "select * from carinfo order by id";
        Pstatement = connection.prepareStatement(sql);
        ResultSet resultSet = Pstatement.executeQuery();
        while (resultSet.next()) {
            list.add(new StuffCar(resultSet.getString("ID"), resultSet.getString("BRAND"), resultSet.getString("rent_pay"), resultSet.getString("status"), resultSet.getString("pledge_pay"), resultSet.getString("valid")));
        }
        return list;
    }

    //服务端下架汽车信息行
    public void deleteReturnCar(String carid, String valid) throws SQLException {
        if (valid.equals("1")) {
            JOptionPane.showMessageDialog(null, "选中的车尚没有归还，无法下架！");
            return;
        }
        String sql = "delete from carinfo where ID = ?";
        Pstatement = connection.prepareStatement(sql);
        Pstatement.setInt(1, Integer.parseInt(carid));
        JOptionPane.showMessageDialog(new JFrame().getContentPane(),
                "删除的汽车ID为：" + carid + "!", "通知", JOptionPane.INFORMATION_MESSAGE);
        Pstatement.execute();
    }

    //服务端上架汽车信息
    public void addCarinfo(String carid, String carbrand, String carstatus, String carrent, String carpledge, String carvalid) throws SQLException {
        String sql = "select ID from carinfo where id = ?";
        Pstatement = connection.prepareStatement(sql);
        Pstatement.setInt(1,Integer.parseInt(carid));
        ResultSet resultSet=Pstatement.executeQuery();
        if(resultSet.next())
        {
            JOptionPane.showMessageDialog(null,"请勿插入重复的汽车ID！");
            return;
        }
       String sql1="insert into carinfo(ID,BRAND,RENT_PAY,STATUS,PLEDGE_PAY,VALID) VALUES (?,?,?,?,?,?)";
        Pstatement=connection.prepareStatement(sql1);
        Pstatement.setInt(1,Integer.parseInt(carid));
        Pstatement.setString(2,carbrand);
        Pstatement.setInt(3,Integer.parseInt(carrent));
        Pstatement.setInt(4,Integer.parseInt(carstatus));
        Pstatement.setInt(5,Integer.parseInt(carpledge));
        Pstatement.setInt(6,Integer.parseInt(carvalid));
        Pstatement.execute();
    }
    //服务端加载全部顾客信息
    public ObservableList getAllcustomerinfo() throws SQLException {
        ObservableList<StuffCustomer> list = FXCollections.observableArrayList();
        String sql="select * from customerinfo order by id";
        Pstatement=connection.prepareStatement(sql);
        ResultSet resultSet= Pstatement.executeQuery();
        while(resultSet.next()) {
            String moral = null;
            String sql2 = "select count(revent) as num from rentdiary where customerid = ?";
            PreparedStatement temp = connection.prepareStatement(sql2);
            temp.setInt(1, resultSet.getInt("ID"));
            ResultSet resultSet1 = temp.executeQuery();
            resultSet1.next();
                int num = resultSet1.getInt("num");
                switch (num) {
                    case 0:
                        moral = "优秀";
                        break;
                    case 1:
                    case 2:
                        moral = "良好";
                        break;
                    case 3:
                    case 4:
                        moral = "一般";
                        break;
                    default:
                        moral = "较差";
                        break;
                }
                list.add(new StuffCustomer(String.valueOf(resultSet.getInt("ID")), resultSet.getString("NAME"), resultSet.getString("PASSWD"), moral, resultSet.getString("member")));
        }
        return list;
    }
    //服务端删除客户信息
    public void deleteCusotomerinfo(String cusid) throws SQLException {
        String sql ="select id from carinfo where customerid = ?";
        Pstatement=connection.prepareStatement(sql);
        Pstatement.setInt(1,Integer.parseInt(cusid));
        ResultSet resultSet=Pstatement.executeQuery();
        if(resultSet.next())
        {
            JOptionPane.showMessageDialog(null,"该客户正在租用汽车，不能删除！！");
            return;
        }
        String sql2="delete from customerinfo where id = ?";
        Pstatement=connection.prepareStatement(sql2);
        Pstatement.setInt(1,Integer.parseInt(cusid));
        Pstatement.execute();
    }
    //服务端添加客户信息
    public void addCustomerinfo(String cusid,String name,String member,String passwd) throws SQLException {
        String sql="select id from customerinfo where id = ?";
        Pstatement=connection.prepareStatement(sql);
        Pstatement.setInt(1,Integer.parseInt(cusid));
        ResultSet resultSet=Pstatement.executeQuery();
        if(resultSet.next())
        {
            JOptionPane.showMessageDialog(null,"请勿插入重复的客户ID！");
            return;
        }
        String sql2="insert into customerinfo(id,name,member,passwd) values (?,?,?,?)";
        Pstatement=connection.prepareStatement(sql2);
        Pstatement.setInt(1,Integer.parseInt(cusid));
        Pstatement.setString(2,name);
        Pstatement.setInt(3,Integer.parseInt(member));
        Pstatement.setString(4,passwd);
        Pstatement.execute();
    }
    //服务端加载全部员工信息
    public ObservableList getAllstuffinfo(ObservableList list) throws SQLException {
        String sql="select * from employeeinfo order by id";
        Pstatement=connection.prepareStatement(sql);
        ResultSet resultSet=Pstatement.executeQuery();
        while(resultSet.next())
        {
            list.add(new StuffPerson(resultSet.getString("ID"),resultSet.getString("NAME"),resultSet.getString("title"),resultSet.getString("passwd")));
        }
        return list;
    }
    public void deleteStuffinfo(String stuffid) throws SQLException {
        String sql="delete from employeeinfo where id = ?";
        Pstatement=connection.prepareStatement(sql);
        Pstatement.setInt(1,Integer.parseInt(stuffid));
        Pstatement.execute();
    }
    public void addStuffinfo(String stuffid,String stuffname,String stufftitle,String stuffpasswd) throws SQLException {
        String sql="select * from employeeinfo where id =?";
        Pstatement=connection.prepareStatement(sql);
        Pstatement.setInt(1,Integer.parseInt(stuffid));
        ResultSet resultSet=Pstatement.executeQuery();
        if(resultSet.next())
        {
            JOptionPane.showMessageDialog(null,"请勿插入重复的员工ID！");
            return;
        }
        String sql2="insert into employeeinfo(id,name,title,passwd) values (?,?,?,?)";
        Pstatement=connection.prepareStatement(sql2);
        Pstatement.setInt(1,Integer.parseInt(stuffid));
        Pstatement.setString(2,stuffname);
        Pstatement.setInt(3,Integer.parseInt(stufftitle));
        Pstatement.setString(4,stuffpasswd);
        Pstatement.execute();
    }
    public ObservableList getAlldirayinfo(ObservableList list) throws SQLException {
        String sql="select * from rentdiary order by infoid";
        Pstatement=connection.prepareStatement(sql);
        ResultSet resultSet=Pstatement.executeQuery();
        while(resultSet.next())
        {
            list.add(new StuffDiary(resultSet.getString("INFOID"),resultSet.getString("CARID"),resultSet.getString("CUSTOMERID"),resultSet.getString("STUFFID"),resultSet.getString("RENT_TIME"),resultSet.getString("REVENT"),resultSet.getString("rent_cost")));
        }
        return list;
    }
    public void deleteDiaryinfo(String infoid,String event) throws SQLException {
        if(event.equals("租车")||event.equals("还车"))
        {
            JOptionPane.showMessageDialog(null,"此时汽车正在租车或还车阶段，无法删除日志！");
            return;
        }
        String sql="delete from rentdiary where id = ?";
        Pstatement=connection.prepareStatement(sql);
        Pstatement.setInt(1,Integer.parseInt(infoid));
        Pstatement.execute();
    }
    public void addDiaryinfo(String infoid,String cusid,String stuffid,String carid,String time,String event,String cost) throws SQLException {
        String sql="select * from rentdiary where id =?";
        Pstatement=connection.prepareStatement(sql);
        Pstatement.setInt(1,Integer.parseInt(infoid));
        ResultSet resultSet=Pstatement.executeQuery();
        if(resultSet.next())
        {
            JOptionPane.showMessageDialog(null,"请勿插入重复的日志ID！");
            return;
        }
        String sql2="insert into rentdiary(infoid,carid,customerid,stuffid,rent_time,revent,rent_cost) values (?,?,?,?,?,?,?)";
        Pstatement=connection.prepareStatement(sql2);
        Pstatement.setInt(1,Integer.parseInt(infoid));
        Pstatement.setInt(2,Integer.parseInt(carid));
        Pstatement.setInt(3,Integer.parseInt(cusid));
        Pstatement.setInt(4,Integer.parseInt(stuffid));
        Pstatement.setString(5,time);
        Pstatement.setString(6,event);
        Pstatement.setString(7,cost);
        Pstatement.execute();
    }
    //获得每个季度的收入
    public int[] getprofit_season(int[]a) throws SQLException {
        String sql="select sum(pprofit) as sum1 from profit where month(pdatetime)>=1 and month(pdatetime)<=3  order  by month(pdatetime)";
        String sql2="select sum(pprofit) as sum2 from profit where month(pdatetime)>3 and month(pdatetime)<=6 order  by month(pdatetime)";
        String sql3="select sum(pprofit) as sum3 from profit where month(pdatetime)>6 and month(pdatetime)<=9  order  by month(pdatetime)";
        String sql4="select sum(pprofit) as sum4 from profit where month(pdatetime)>9 and month(pdatetime)<=12  order  by month(pdatetime)";
        Pstatement=connection.prepareStatement(sql);
        ResultSet resultSet=Pstatement.executeQuery();
        resultSet.next();
        a[0]=resultSet.getInt("sum1");
        Pstatement=connection.prepareStatement(sql2);
         resultSet=Pstatement.executeQuery();
        resultSet.next();
        a[1]=resultSet.getInt("sum2");
        Pstatement=connection.prepareStatement(sql3);
        resultSet=Pstatement.executeQuery();
        resultSet.next();
        a[2]=resultSet.getInt("sum3");
        Pstatement=connection.prepareStatement(sql4);
        resultSet=Pstatement.executeQuery();
        resultSet.next();
        a[3]=resultSet.getInt("sum4");
        return a;
    }
    public ObservableList getprofit_year(ObservableList list) throws SQLException {
        String sql="SELECT year(pdatetime) as num,sum(pprofit) as sum from profit  group by year(pdatetime) order  by year(pdatetime)";
        Pstatement=connection.prepareStatement(sql);
        ResultSet resultSet=Pstatement.executeQuery();
        while(resultSet.next())
        {
            list.add(new PieChart.Data(resultSet.getString("num"),resultSet.getInt("sum")));
        }
        return list;
    }
    public ObservableList getprofitinfo(ObservableList list) throws SQLException {
        String sql="select BRAND,COUNT(BRAND) AS NUM FROM CARINFO GROUP BY BRAND";
        Pstatement=connection.prepareStatement(sql);
        ResultSet resultSet=Pstatement.executeQuery();
        while(resultSet.next())
        {
            list.add(new PieChart.Data(resultSet.getString("brand"),resultSet.getInt("NUM")));
        }
        return list;
    }

}



import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import java.math.BigDecimal;
import java.sql.*;
import java.util.LinkedList;
//清除操作痕迹
class DBUtils{
    public static void closeConnection(Connection conn)
    {
        if (conn!=null)
        {
            try
            {
                conn.close();
            }catch (SQLException e)
            {
                e.printStackTrace();
            }
        }
        conn=null;
    }
    public static void closeStatement(PreparedStatement state)
    {
        if (state!=null)
        {
            try{
                state.close();
            }catch (SQLException e)
            {
                e.printStackTrace();
            }
        }
        state=null;
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
}
public class DBConnector {
    private static DBConnector instance = null;
    private Connection connection;
    private PreparedStatement Pstatement;
    private DBConnector() {
        //将mysql驱动注册到DriverManager中去
        /*
        Class.forName() 方法要求JVM查找并加载指定的类到内存中；
         */
        try {
            Class.forName("com.mysql.jdbc.Driver");
        }catch (ClassNotFoundException e)
        {
            System.out.println("找不到驱动类，加载失败！");
            e.printStackTrace();
        }
    }
//单例模式
    public static DBConnector getInstance() {
        if (instance == null)
            instance = new DBConnector();
        return instance;
    }
    //创建数据库的连接
    public void connectionDatabase(String hostname,String DBname,Integer port,String username,String password) throws SQLException {
        String url = "jdbc:mysql://"+hostname+":"+port+"/"+DBname+"?zeroDateTimeBehavior=convertToNull&autoReconnect=true&characterEncoding=UTF-8&characterSetResults=UTF-8";
        connection=DriverManager.getConnection(url,username,password);
    }
    //检查用户登录是否正确，并且更新用户登录时间
    public boolean checkUsernameAndPassword(String id,String password,String logintype) throws SQLException {
        String urlPatient = "SELECT DLKL from T_BRXX WHERE BRBH=?";
        String urlDoctor = "SELECT DLKL from T_KSYS WHERE YSBH=?";
        String urlPatientUpdate = "UPDATE T_BRXX SET DLRQ=(SELECT CURRENT_TIMESTAMP()) WHERE BRBH=?";
        String urlDoctorUpdate ="UPDATE T_KSYS SET DLRQ=(SELECT CURRENT_TIMESTAMP()) WHERE YSBH=?";
        boolean status = false;
        if (logintype.equals("医生"))
        {
            Pstatement=connection.prepareStatement(urlDoctor);
            Pstatement.setString(1,id);
        }
        else if(logintype.equals("病患"))
        {
            Pstatement=connection.prepareStatement(urlPatient);
            Pstatement.setString(1,id);
        }
        ResultSet cursor = Pstatement.executeQuery();
        while(cursor.next())
        {
            String rightpwd = cursor.getString("DLKL").trim();
            System.out.println(rightpwd);
            if (password.equals(rightpwd)) {
                status = true;
                break;
            }
        }
        cursor=null;
        Pstatement=null;
        if (status)
        {
            if (logintype.equals("医生"))
            {
                Pstatement=connection.prepareStatement(urlDoctorUpdate);
                Pstatement.setString(1,id);
            }
            else if(logintype.equals("病患"))
            {
                Pstatement=connection.prepareStatement(urlPatientUpdate);
                Pstatement.setString(1,id);
            }
            Pstatement.executeUpdate();
        }
        return status;
    }
    //获得科室信息表
    public LinkedList<String> getT_KSXX() throws SQLException {
        String table= "SELECT * FROM T_KSXX";
            //创建执行语句
            Pstatement = connection.prepareStatement(table);
            ResultSet cursor = Pstatement.executeQuery();
            LinkedList<String> T_KSXX = new LinkedList<String>();
            while(cursor.next())
            {
                String KSBH = cursor.getString("KSBH").trim();
                String KSMC = cursor.getString("KSMC").trim();
                String PYZS = cursor.getString("PYZS").trim();
                T_KSXX.add(KSBH+" "+KSMC+" "+PYZS);
            }
            return T_KSXX;
    }
    //获得一个科室内全部医生的信息
    public LinkedList<String> getDoctorsFromDepartment(String department,boolean isP) throws SQLException
    {
        String table="SELECT YSBH,YSMC,PYZS,SFZJ FROM T_KSYS WHERE KSBH=?";
        ResultSet cursor = null;
        //专家号只能专家挂，专家可以挂普通号
        PreparedStatement Pstatement = connection.prepareStatement(table);
        Pstatement.setString(1,department);
        cursor = Pstatement.executeQuery();

        LinkedList<String> list = new LinkedList<String>();
        while(cursor.next())
        {
            String departmentId = cursor.getString("YSBH").trim();
            String departmentName = cursor.getString("YSMC").trim();
            String departmentPYZS =cursor.getString("PYZS").trim();
            boolean isp = cursor.getBoolean("SFZJ");
            if(isP==true) {
                if(isp==isP)
                    if(!list.contains(departmentId + " " + departmentName + " " + departmentPYZS))
                list.add(departmentId + " " + departmentName + " " + departmentPYZS);
            }
            else {
                if (!list.contains(departmentId + " " + departmentName + " " + departmentPYZS))
                    list.add(departmentId + " " + departmentName + " " + departmentPYZS);
            }

        }
        return list;
    }
    //获得一个科室的全部挂号种类
    /**
     * @param isProfressional 是否为专家号
     * */
    public LinkedList<String> getRegisterTypeFromDepartment(String department,boolean isProfressional)throws SQLException
    {
        String table="SELECT HZBH,HZMC,PYZS,GHRS,GHFY,SFZJ FROM T_HZXX WHERE KSBH=?  AND (GHRS > (SELECT IFNULL(MAX(GHRC),0) FROM T_GHXX WHERE HZBH=T_HZXX.HZBH AND TO_DAYS(RQSJ)=TO_DAYS(NOW())));";
        Pstatement = connection.prepareStatement(table);
        Pstatement.setString(1, department);
        LinkedList<String> list = new LinkedList<String>();
        ResultSet cursor = Pstatement.executeQuery();
        while (cursor.next())
        {
            String registerTypeId = cursor.getString("HZBH").trim();
            String registerTypeName = cursor.getString("HZMC").trim();
            String registerTypePYZS = cursor.getString("PYZS").trim();
            BigDecimal paymentAmount = cursor.getBigDecimal("GHFY");
            int isp=cursor.getInt("SFZJ");
           if(isp==0) {
                if(isProfressional==false)
                list.add(registerTypeId + " " + registerTypeName + " " + registerTypePYZS + " " + paymentAmount);
            }
            else
                list.add(registerTypeId + " " + registerTypeName + " " + registerTypePYZS + " " + paymentAmount);
        }
        return list;
    }
    public boolean IsRegister(String register)throws SQLException
    {
        String table="SELECT * FROM T_HZXX WHERE HZBH=? AND GHRS>(SELECT IFNULL(MAX(GHRC),0) FROM T_GHXX WHERE HZBH=? AND TO_DAYS(RQSJ)=TO_DAYS(NOW()))";
        Pstatement = connection.prepareStatement(table);
        Pstatement.setString(1,register);
        Pstatement.setString(2,register);
        ResultSet cursor = Pstatement.executeQuery();
        boolean flag = cursor.next();
        return flag;
    }
    //查看预存金额是否足够支付
    public boolean IsPay(String patientId,String Isregister)throws SQLException {
        String table = "SELECT * FROM T_BRXX WHERE BRBH=? AND YCJE>=(SELECT GHFY FROM T_HZXX WHERE HZBH=?)";
        Pstatement = connection.prepareStatement(table);
        Pstatement.setString(1, patientId);
        Pstatement.setString(2, Isregister);
        ResultSet cursor = Pstatement.executeQuery();
        boolean flag = cursor.next();
        return flag;
    }
    //获得病人的预存金额
    public BigDecimal getPatientPaymentAmount(String patientId)throws SQLException
    {
        String table = "SELECT YCJE FROM T_BRXX WHERE BRBH=?";
        Pstatement = connection.prepareStatement(table);
        Pstatement.setString(1,patientId);
        ResultSet cursor = Pstatement.executeQuery();
        cursor.next();//解决报错Before start of result set
        BigDecimal bigDecimal = cursor.getBigDecimal("YCJE");
        return bigDecimal;
    }
    public String getDoctorName(String doctorId)throws  SQLException
    {
        String table = "SELECT YSMC FROM T_KSYS WHERE YSBH=?";
        Pstatement = connection.prepareStatement(table);
        Pstatement.setString(1, doctorId);
        ResultSet cursor = Pstatement.executeQuery();
        cursor.next();
        String doctorName = cursor.getString("YSMC").trim();
        return doctorName;

    }
    public ObservableList<patientController>getPatients(String doctorId)throws SQLException
    {
        ObservableList<patientController> patients = FXCollections.observableArrayList();
        String table="SELECT GHBH,BRMC,RQSJ,SFZJ FROM T_GHXX,T_BRXX,T_HZXX WHERE T_GHXX.YSBH=? AND T_GHXX.BRBH=T_BRXX.BRBH AND T_GHXX.HZBH=T_HZXX.HZBH ORDER BY GHBH ASC";
        Pstatement=connection.prepareStatement(table);
        Pstatement.setString(1,doctorId);
        ResultSet cursor=Pstatement.executeQuery();
        while(cursor.next())
        {
            String registerId = cursor.getString("GHBH").trim();
            String patientName = cursor.getString("BRMC").trim();
            String registerDateTime = cursor.getString("RQSJ").trim();
            String isProfresstional = cursor.getBoolean("SFZJ")==true?"专家号":"普通号";
            patientController temp = new patientController(registerId,patientName,registerDateTime,isProfresstional);
            patients.add(temp);
        }
        return patients;
    }
    public ObservableList<Income> getIncome()throws SQLException
    {
        ObservableList<Income> incomes = FXCollections.observableArrayList();
        String table=String.format("SELECT %s FROM %s WHERE %s GROUP BY %s",
                "KSMC,T_GHXX.YSBH,YSMC,T_HZXX.SFZJ,COUNT(*),SUM(T_GHXX.GHFY)", "T_GHXX,T_KSXX,T_KSYS,T_HZXX",
                "T_GHXX.YSBH=T_KSYS.YSBH AND T_KSYS.KSBH=T_KSXX.KSBH AND T_GHXX.HZBH=T_HZXX.HZBH AND T_GHXX.RQSJ BETWEEN UNIX_TIMESTAMP(CAST(SYSDATE() AS DATE)) AND NOW() AND T_GHXX.THBZ=false",
                "T_GHXX.YSBH, T_GHXX.HZBH");
        Pstatement = connection.prepareStatement(table);
        ResultSet cursor = Pstatement.executeQuery();
        while(cursor.next())
        {
            String departmentName = cursor.getString("KSMC").trim();
            String doctorId = cursor.getString("T_GHXX.YSBH").trim();
            String doctorName = cursor.getString("YSMC").trim();
            String isProfressional = cursor.getBoolean("T_HZXX.SFZJ")==true?"专家号":"普通号";
            int PatientNumber=cursor.getInt("COUNT(*)");
            BigDecimal income= cursor.getBigDecimal("SUM(T_GHXX.GHFY)");
            Income temp=new Income(departmentName,doctorId,doctorName,isProfressional,String.valueOf(PatientNumber),income.toString());
            incomes.add(temp);
        }
        return incomes;
    }

    //挂号
    /**
     * @param patientId 病人编号
     * @param doctorId 医生编号
     * @param paymentAmount 挂号费用
     * @param registerName 号种名称
     * @param JKJE 交款金额+余额
     * */
    public String RegisterInfo(String patientId,String doctorId,String registerName,BigDecimal paymentAmount,BigDecimal JKJE) throws SQLException {
        /*connection.setAutoCommit(false);
        String table = String.format(
                "INSERT IGNORE INTO T_GHXX (GHBH,HZBH,YSBH,BRBH,GHRC,GHFY) SELECT %s,?,?,?,%s,GHFY FROM T_HZXX WHERE HZBH=? AND GHRS>(%s)",
                "LPAD(CAST((SELECT COUNT(*)+1 FROM T_GHXX) AS CHAR(6)), 6, 0)",
                "CAST((SELECT COUNT(*)+1 FROM T_GHXX WHERE HZBH=? AND TO_DAYS(RQSJ)=TO_DAYS(NOW())) AS INT)",
                "SELECT COUNT(*) FROM T_GHXX WHERE HZBH=? AND TO_DAYS(RQSJ)=TO_DAYS(NOW())");
        Pstatement = connection.prepareStatement(table);
        Pstatement.setString(1,registerName);
        Pstatement.setString(2,doctorId);
        Pstatement.setString(3,patientId);
        Pstatement.setString(4,registerName);
        Pstatement.setString(5,registerName);
        Pstatement.setString(6,registerName);
        int LeftRowNumber = Pstatement.executeUpdate();
        System.out.println("leftrownumber:"+LeftRowNumber);
        DBUtils.closeStatement(Pstatement);
        if (LeftRowNumber==0)
        {
            DBUtils.closeConnection(connection);
        }
        Pstatement=connection.prepareStatement("SELECT GHBH,GHFY FROM T_GHXX WHERE HZBH=? AND YSBH=? AND BRBH=? ORDER BY RQSJ DESC LIMIT 1");
        Pstatement.setString(1,registerName);
        Pstatement.setString(2,doctorId);
        Pstatement.setString(3,patientId);
        ResultSet cursor = Pstatement.executeQuery();
        String registerId = cursor.getString("GHBH");
        BigDecimal payableAmount = cursor.getBigDecimal("GHFY");
        if(paymentAmount.compareTo(new BigDecimal(0))>0)
            Pstatement = UPdatePaymentAmount(Pstatement,patientId);
        else
            Pstatement = UPdatepayableAmount(Pstatement,patientId,payableAmount);
        LeftRowNumber = Pstatement.executeUpdate();
        connection.commit();
        return registerId;*/
        try {
            String table = "select * from t_ghxx order by ghbh desc limit 1";
            Pstatement = connection.prepareStatement(table);
            ResultSet cursor = Pstatement.executeQuery();
            int registerId, count, maxRegister = 0;//挂号、当前挂号人数、最大挂号人数
            if (!cursor.next()) registerId = 1;
            else {
                registerId = Integer.parseInt(cursor.getString("GHBH")) + 1;
            }
            cursor = Pstatement.executeQuery("select  * from t_ghxx where HZBH=" + registerName
                    + " and unix_timestamp(RQSJ)>=unix_timestamp(NOW()) order by GHRC desc limit 1");
            if (!cursor.next()) count = 0;
            else
                count = cursor.getInt("GHRC");
            cursor = Pstatement.executeQuery("select * from t_hzxx where HZBH = " + registerName);
            while (cursor.next()) {
                maxRegister = cursor.getInt("GHRS");
                if (count >= maxRegister) {
                    return "当前号种挂号人数已满!请选择其他号种";
                }
                Pstatement.executeUpdate(String.format("insert into t_ghxx values(\"%06d\",\"%s\",\"%s\",\"%s\",%d,0,%.2f, current_timestamp)", registerId, registerName, doctorId, patientId, count + 1, paymentAmount));
                Pstatement.executeUpdate(String.format("update t_brxx set ycje = %.2f where brbh = %s", JKJE.subtract(paymentAmount), patientId));
                BigDecimal refund = JKJE.subtract(paymentAmount);
                RegisterController.setPatientYCYE(refund);
                return "success " + registerId;
            }
        }catch (SQLException e)
        {
            e.printStackTrace();
        }
        return "0";
    }

    /**
     * 获取病人信息
     * @param brbh 病人编号
     * @return 病人信息
     */
    public ResultSet getPatientInfo(String brbh) {
        try {
            String table="select * from t_brxx where brbh ="+brbh;
            Pstatement = connection.prepareStatement(table);
            return Pstatement.executeQuery();
        } catch (SQLException e) {
            return null;
        }
    }

public PreparedStatement UPdatePaymentAmount(PreparedStatement preparedStatement,String patientId) throws SQLException {
    String table="SELECT GHBH,GHFY FROM T_GHXX WHERE HZBH=? AND YSBH=? AND BRBH=? ORDER BY RQSJ DESC LIMIT 1";
    preparedStatement = connection.prepareStatement(table);
    preparedStatement.setString(1,patientId);
    return preparedStatement;
    }

    //更新病患的预存金额
    public PreparedStatement UPdatepayableAmount(PreparedStatement preparedStatement,String patientId,BigDecimal payableAmount) throws SQLException {
    String table="UPDATE T_BRXX SET YCJE=YCJE-? WHERE BRBH=?";
    preparedStatement = connection.prepareStatement(table);
    preparedStatement.setBigDecimal(1,payableAmount);
    preparedStatement.setString(2,patientId);
    return preparedStatement;
    }

}

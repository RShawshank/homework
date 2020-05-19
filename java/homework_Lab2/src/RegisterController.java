
import javafx.application.Platform;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.event.EventHandler;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.layout.StackPane;
import javafx.stage.Stage;
import org.controlsfx.control.textfield.AutoCompletionBinding;
import org.controlsfx.control.textfield.TextFields;

import javax.swing.*;
import java.io.IOException;
import java.math.BigDecimal;
import java.sql.SQLException;
import java.util.LinkedList;
import java.util.List;

public class RegisterController {
    @FXML
    private TextField department_Field;//科室名称
    @FXML
    private TextField doctorName_Field;//医生名称
    @FXML
    public ChoiceBox<String> registerType_Field;//挂号种类
    @FXML
    private TextField registerName_Field;//号种名称
    @FXML
    private TextField paymentAmount_Field;//交款金额
    @FXML
    private TextField amountPayable_Field;//应缴金额
    @FXML
    private TextField change_Field;//找零金额
    @FXML
    private TextField registerId_Field;//挂号号码
    @FXML
    private Label welcomeLabel;  //欢迎label
    private static String patientName;
    private static BigDecimal patientYCYE;
    private BigDecimal patientJKJE=new BigDecimal(0);//病人交款金额
    private String departmentId;
    private String doctorId=null;
    public static String patientId;
    private String Isregister=null;
    private String registerName;
    private boolean isProfessional = false;
    private BigDecimal amountPayable;
    private String registerID;

    @FXML
    private Button confirmButton;
    @FXML
    private Button clearButton;
    @FXML
    private Button exitButton;
    @FXML
    public void initialize() throws SQLException {
        welcomeLabel.setText("欢迎进入医院挂号系统，" + patientName + "!\t当前账户余额:" + patientYCYE + "元");
       departmentName_Fieldinit();
    }
//根据编号、科室名、拼音进行科室的选择
    private void departmentName_Fieldinit()
    {
        try {
            DBConnector dbConnector = DBConnector.getInstance();
            LinkedList<String> department = dbConnector.getT_KSXX();
            for(String s:department)
                System.out.println(s);
                //实现自动填充
                AutoCompletionBinding<String> bind = TextFields.bindAutoCompletion(department_Field, FXCollections.observableArrayList(department));
                //实现动态改变绑定下拉匹配列表
                bind.setOnAutoCompleted(new EventHandler<>() {
                    public void handle(AutoCompletionBinding.AutoCompletionEvent<String> stringAutoCompletionEvent) {
                        String[] connect = stringAutoCompletionEvent.getCompletion().split(" ");
                        departmentId = connect[0];
                        department_Field.setText(connect[1]);
                        registerType_Field.setDisable(false);
                        registerType_Fieldinit();
                    }
                });
        }
        catch (SQLException e)
        {
            e.printStackTrace();
        }
    }
    //选择科室后，再进行专家号的选择
    private void registerType_Fieldinit() {
       registerType_Field.getSelectionModel().selectedItemProperty().addListener((ObservableValue<? extends String> observable, String oldValue, String newValue) -> {
            if (newValue.equals("普通号")) {
                isProfessional = false;
                registerType_Field.setValue("普通号");
            } else {
                isProfessional = true;
                registerType_Field.setValue("专家号");
            }
            doctorName_Field.setDisable(false);
           doctorNameinit(isProfessional);
        });
    }
    //选择医生
    private void doctorNameinit(boolean isProfessional)
    {
        try {
        DBConnector dbConnector = DBConnector.getInstance();
            List<String> doctors=new LinkedList<>();
                   doctors= dbConnector.getDoctorsFromDepartment(departmentId,isProfessional);
            for(String s:doctors)
                System.out.println(s);
          /*  doctorName_Field.getItems().addAll(doctors);
            if(!doctorName_Field.isShowing())
                doctorName_Field.show();
            String[] connect = doctorName_Field.getValue().split(" ");
            for(String s:connect)
            {
                System.out.println(s);
            }
            doctorId=connect[0];
            doctorName_Field.setValue(connect[1]);
            registerName_Field.setDisable(false);
            registerName_Fieldinit();*/
            AutoCompletionBinding<String> bind = TextFields.bindAutoCompletion(doctorName_Field, FXCollections.observableArrayList(doctors));
            bind.setOnAutoCompleted(new EventHandler<>() {
                public void handle(AutoCompletionBinding.AutoCompletionEvent<String> stringAutoCompletionEvent) {
                    String[] connect = stringAutoCompletionEvent.getCompletion().split(" ");
                    doctorId=connect[0];
                    doctorName_Field.setText(connect[1]);
                    registerName_Field.setDisable(false);
                    registerName_Fieldinit();
                }
            });

        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    private void registerName_Fieldinit() {
        try{
            DBConnector dbConnector =DBConnector.getInstance();
            LinkedList<String> registerTypes = dbConnector.getRegisterTypeFromDepartment(departmentId, isProfessional);
            AutoCompletionBinding<String> bind = TextFields.bindAutoCompletion(registerName_Field,FXCollections.observableArrayList(registerTypes));
            bind.setOnAutoCompleted(new EventHandler<>() {
                public void handle(AutoCompletionBinding.AutoCompletionEvent<String> stringAutoCompletionEvent)
                {
                    String[] connect = stringAutoCompletionEvent.getCompletion().split(" ");
                    Isregister = connect[0];
                    try {
                        if (!dbConnector.IsRegister(Isregister)) {
                            JOptionPane.showMessageDialog(null, "无法挂号，号已挂满！");
                        } else {
                            registerID=connect[0];
                            registerName_Field.setText(connect[1]);
                            registerName = registerName_Field.getText().trim();
                            amountPayable = new BigDecimal(connect[3]);
                            amountPayable_Field.setText(connect[3]);
                            amountPayable_Field.setDisable(false);
                            //预存金额足够
                            if (dbConnector.IsPay(patientId, Isregister)) {
                                paymentAmount_Field.setText(String.valueOf(patientYCYE));
                                 patientYCYE = dbConnector.getPatientPaymentAmount(patientId);
                                BigDecimal change =patientYCYE.subtract(amountPayable);
                                System.out.println("payable:"+amountPayable);
                                System.out.println("paymentAmount"+patientYCYE);
                                System.out.println("change"+change);
                                if(change.compareTo(new BigDecimal(0))>=0)
                                {
                                    change_Field.setText(change.toString());
                                }else
                                {
                                    change_Field.clear();
                                }
                            } else
                                {
                                paymentAmount_Field.setDisable(false);
                                paymentAmount_Field.textProperty()
                                        .addListener(new ChangeListener<String>() {
                                            public void changed(ObservableValue<? extends String> observableValue, String s, String t1) {
                                                System.out.println("t1:" + t1);
                                                if (!t1.equals("")) {
                                                    try{
                                                        patientJKJE = new BigDecimal(t1);
                                                        DBConnector dbConnector = DBConnector.getInstance();
                                                        patientYCYE = dbConnector.getPatientPaymentAmount(patientId);
                                                        BigDecimal change = patientJKJE.add(patientYCYE).subtract(amountPayable);//余额加上存入的钱
                                                        System.out.println("payment"+patientJKJE);
                                                        System.out.println("paymentAmount"+patientYCYE);
                                                        System.out.println("change"+change);
                                                        if(change.compareTo(new BigDecimal(0))>=0)
                                                        {
                                                            change_Field.setText(change.toString());
                                                        }else
                                                        {
                                                            change_Field.clear();
                                                            JOptionPane.showMessageDialog(null, "金额不足！");
                                                        }
                                                    }catch (NumberFormatException e)
                                                    {
                                                        e.printStackTrace();
                                                    } catch (SQLException throwables) {
                                                        throwables.printStackTrace();
                                                    }
                                                }
                                            }
                                        });
                            }
                        }
                    }
                    catch (SQLException E)
                    {
                        E.printStackTrace();
                    }
                }
            });
        }
        catch (SQLException e)
        {
            e.printStackTrace();
        }
    }

         /*
                    private void addPaymentAmount_FieldChangeListener ()
    {
        paymentAmount_Field.textProperty()
                .addListener(new ChangeListener<String>() {
                    public void changed(ObservableValue<? extends String> observableValue, String s, String t1) {
                        System.out.println("t1:"+t1);
                        if(!t1.equals(""))
                        {
                            try{
                                BigDecimal payment = new BigDecimal(t1);
                                DBConnector dbConnector = DBConnector.getInstance();
                                BigDecimal paymentAmount = dbConnector.getPatientPaymentAmount(patientId);
                                BigDecimal change = payment.add(paymentAmount).subtract(amountPayable);
                               System.out.println("payment"+payment);
                                System.out.println("paymentAmount"+paymentAmount);

                                System.out.println("change"+change);
                                if(change.compareTo(new BigDecimal(0))>=0)
                                {
                                    change_Field.setText(change.toString());
                                }else
                                {
                                    change_Field.clear();
                                }
                            }catch (NumberFormatException e)
                            {
                                e.printStackTrace();
                            } catch (SQLException throwables) {
                                throwables.printStackTrace();
                            }
                        }
                    }
                });
    }
    */

    @FXML
    void onClearButtonClick() {
        department_Field.clear();
        doctorName_Field.clear();
        doctorName_Field.setDisable(true);
        registerType_Field.setValue("普通号");
        registerType_Field.setDisable(true);
        registerName_Field.clear();
        registerName_Field.setDisable(true);
        registerId_Field.clear();
        registerId_Field.setDisable(true);
        paymentAmount_Field.clear();
        paymentAmount_Field.setDisable(true);
        amountPayable_Field.clear();
        amountPayable_Field.setDisable(true);
        change_Field.clear();
        change_Field.setDisable(true);
        departmentName_Fieldinit();
    }
    @FXML
    void onConfirmButtonClick() {
        if (department_Field.getText().equals("") || doctorName_Field.getText().equals("")
                || registerName_Field.getText().equals("")) {
            JOptionPane.showMessageDialog(null, "输入格式错误！");
            return;
        }
        try {
            DBConnector dbConnector = DBConnector.getInstance();
            BigDecimal all =patientJKJE.add(patientYCYE);
            String registerId = dbConnector.RegisterInfo(patientId, doctorId,registerID, amountPayable,all);
            registerId_Field.setText(registerId);
            registerId_Field.setDisable(false);
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    @FXML
    void onExitButtonClick() {
        Stage nextStage = (Stage) exitButton.getScene().getWindow();
        nextStage.close();
        Parent root = null;
        try {
            root = FXMLLoader.load(getClass().getResource("login.fxml"));
        } catch (IOException e) {
            e.printStackTrace();
        }
        Scene scene = new Scene(root, 600, 400);
        scene.setRoot(root);
        Stage stage = new Stage();
        stage.setTitle("简易医院挂号系统");
        stage.setScene(scene);
        stage.show();
    }

    public static void setPatientId(String patientId) {
        RegisterController.patientId = patientId;
    }
    public static void setPatientName(String patientName) {
        RegisterController.patientName = patientName;
    }
    public static void setPatientYCYE(BigDecimal patientYCYE) {
        RegisterController.patientYCYE = patientYCYE;
    }
}

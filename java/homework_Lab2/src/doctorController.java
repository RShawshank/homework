import javafx.beans.property.SimpleStringProperty;
import javafx.beans.value.ObservableValue;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.scene.control.*;
import javafx.util.Callback;


import java.sql.SQLException;
import java.time.LocalDate;

public class doctorController {
    @FXML
    Label welcomeLabel;
    private ObservableList<patientController> patientData;//病人列表
    @FXML
    private TableView<patientController> patientTable;
    @FXML
    private TableColumn<patientController, String> registerId;//挂号编号
    @FXML
    private TableColumn<patientController, String> patientName;//病人名称
    @FXML
    private TableColumn<patientController, String> registerDatetime;//挂号日期时间
    @FXML
    private TableColumn<patientController, String> isProfressional;//号种类别

    private ObservableList<Income> incomeData;//收入列表
    @FXML
    private TableView<Income> incomeTable;
    @FXML
    private TableColumn<Income, String> departmentName;//科室名称
    @FXML
    private TableColumn<Income, String> doctorId;//医生编号
    @FXML
    private TableColumn<Income, String> doctorName;//医生名称
    @FXML
    private TableColumn<Income, String>income_isProfressional;//号种类别
    @FXML
    private TableColumn<Income, String> registerAmount;//挂号人次
    @FXML
    private TableColumn<Income, String> incomeAll;//收入合计
    @FXML
    private DatePicker pickDateStart;
    @FXML
    private DatePicker pickDateEnd;
    public  static String doctor_Id;
    public static String doctor_name;
    public static void setDoctor_Id(String doctor_Id) {
        doctorController.doctor_Id = doctor_Id;
    }

    public static void setDoctor_name(String doctor_name) {
        doctorController.doctor_name = doctor_name;
    }

    @FXML
    public void initialize() throws SQLException {
        Labelinit();
        patientInfoinit();
        incomeInfoinit();
    }
    public void Labelinit(){
            pickDateStart.setValue(LocalDate.now());
            pickDateEnd.setValue(LocalDate.now());
            welcomeLabel.setText("欢迎，"+doctor_name+"登录！");

    }
    public void patientInfoinit() throws SQLException {
        registerId.setCellValueFactory(new Callback<TableColumn.CellDataFeatures<patientController, String>, ObservableValue<String>>() {
            public ObservableValue<String> call(TableColumn.CellDataFeatures<patientController, String> patientStringCellDataFeatures) {
                return new SimpleStringProperty(patientStringCellDataFeatures.getValue().registerIdProperty());
            }
        });
        registerDatetime.setCellValueFactory(new Callback<TableColumn.CellDataFeatures<patientController, String>, ObservableValue<String>>() {
            public ObservableValue<String> call(TableColumn.CellDataFeatures<patientController, String> patientStringCellDataFeatures) {
                return new SimpleStringProperty(patientStringCellDataFeatures.getValue().registerDatetimeProperty());
            }
        });
        patientName.setCellValueFactory(new Callback<TableColumn.CellDataFeatures<patientController, String>, ObservableValue<String>>() {
            public ObservableValue<String> call(TableColumn.CellDataFeatures<patientController, String> patientStringCellDataFeatures) {
                return new SimpleStringProperty(patientStringCellDataFeatures.getValue().patientNameProperty());
            }
        });
        isProfressional.setCellValueFactory(new Callback<TableColumn.CellDataFeatures<patientController, String>, ObservableValue<String>>() {
            public ObservableValue<String> call(TableColumn.CellDataFeatures<patientController, String> patientStringCellDataFeatures) {
                return new SimpleStringProperty(patientStringCellDataFeatures.getValue().isProfressionalProperty());
            }
        });
        DBConnector connector = DBConnector.getInstance();
        patientData=connector.getPatients(doctor_Id);
        patientTable.setItems(patientData);
    }
    public void incomeInfoinit() throws SQLException {
        departmentName.setCellValueFactory(new Callback<TableColumn.CellDataFeatures<Income, String>, ObservableValue<String>>() {
            public ObservableValue<String> call(TableColumn.CellDataFeatures<Income, String> incomeStringCellDataFeatures) {
                return new SimpleStringProperty(incomeStringCellDataFeatures.getValue().departmentNameProperty());
            }
        });
        doctorId.setCellValueFactory(new Callback<TableColumn.CellDataFeatures<Income, String>, ObservableValue<String>>() {
            public ObservableValue<String> call(TableColumn.CellDataFeatures<Income, String> incomeStringCellDataFeatures) {
                return new SimpleStringProperty(incomeStringCellDataFeatures.getValue().doctorIdProperty());
            }
        });
        doctorName.setCellValueFactory(new Callback<TableColumn.CellDataFeatures<Income, String>, ObservableValue<String>>() {
            public ObservableValue<String> call(TableColumn.CellDataFeatures<Income, String> incomeStringCellDataFeatures) {
                return new SimpleStringProperty(incomeStringCellDataFeatures.getValue().doctorNameProperty());
            }
        });
        income_isProfressional.setCellValueFactory(new Callback<TableColumn.CellDataFeatures<Income, String>, ObservableValue<String>>() {
            public ObservableValue<String> call(TableColumn.CellDataFeatures<Income, String> incomeStringCellDataFeatures) {
                return new SimpleStringProperty(incomeStringCellDataFeatures.getValue().isProfressionalProperty());
            }
        });
        registerAmount.setCellValueFactory(new Callback<TableColumn.CellDataFeatures<Income, String>, ObservableValue<String>>() {
            public ObservableValue<String> call(TableColumn.CellDataFeatures<Income, String> incomeStringCellDataFeatures) {
                return new SimpleStringProperty(incomeStringCellDataFeatures.getValue().register_NumberProperty());
            }
        });
        incomeAll.setCellValueFactory(new Callback<TableColumn.CellDataFeatures<Income, String>, ObservableValue<String>>() {
            @Override
            public ObservableValue<String> call(TableColumn.CellDataFeatures<Income, String> incomeStringCellDataFeatures) {
                return new SimpleStringProperty(incomeStringCellDataFeatures.getValue().incomeProperty());
            }
        });
        DBConnector connector = DBConnector.getInstance();
        incomeData=connector.getIncome();
        incomeTable.setItems(incomeData);

    }
}

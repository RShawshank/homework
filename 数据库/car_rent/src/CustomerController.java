import emtity.CustomerPerson;
import emtity.customerReturnCar;
import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.control.cell.PropertyValueFactory;
import javafx.stage.Stage;

import javax.swing.*;
import java.io.IOException;
import java.net.URL;
import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.ResourceBundle;

public class CustomerController implements Initializable{
    @FXML
    public ComboBox<String> combo_brand,combo_statu,combo_rent,combo_pledge,combo_id,combo_sname;
    @FXML
    private Button btn_exit;
    @FXML
    Label label_title;
    @FXML
    private TableColumn<CustomerPerson, String> person_id,person_cname,person_member,person_dlrq,person_moral;//对应顾客信息的列
    @FXML
    private TableColumn<customerReturnCar,String> car_id,car_brand,car_status,car_rent,car_pledge,car_valid;//对应租车信息的列
    @FXML
    TableView<CustomerPerson> table_personainfo;//顾客信息列表
    @FXML
    TableView<customerReturnCar> tableCustomerReturnCar;//还车列表
    DBConnector connector;
    ObservableList<String> ob_brand = FXCollections.observableArrayList();//汽车品牌
    ObservableList<String> ob_statu = FXCollections.observableArrayList();//汽车状况
    ObservableList<String> ob_rent = FXCollections.observableArrayList();//汽车租金
    ObservableList<String> ob_pledge = FXCollections.observableArrayList();//汽车押金
    ObservableList<String> ob_id = FXCollections.observableArrayList();
    ObservableList<String> ob_sname = FXCollections.observableArrayList();
    ObservableList<CustomerPerson> personList = FXCollections.observableArrayList();
    ObservableList<customerReturnCar> ReturnCarList = FXCollections.observableArrayList();
    ArrayList<String>bandList,statuList,rentList,pledgeList,IdList,NameList;
    public static String customer_id;
    public static String customer_name;
    public static void setCustomer_id(String id)
    {
        customer_id=id;
    }
    public static void setCustomer_name(String name)
    {
        customer_name=name;
    }
    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        connector=DBConnector.getInstance();
        bandList = new ArrayList<>();
        statuList = new ArrayList<>();
        rentList = new ArrayList<>();
        pledgeList = new ArrayList<>();
        IdList = new ArrayList<>();
        NameList = new ArrayList<>();
        label_title.setText("欢迎，"+customer_name+"登录！租车系统");
        //载入信息
        try{
            ob_brand=connector.getBandFromCarinfo();
            combo_brand.setItems(ob_brand);
            ob_sname =connector.getNAMEfromemployeeinfo();
            combo_sname.setItems(ob_sname);
            combo_sname.getSelectionModel().select(0);
        }catch (SQLException e)
        {
            e.printStackTrace();
        }
        combo_brand.getSelectionModel().selectedItemProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observableValue, String s, String t1) {
                try {
                        String brand = combo_brand.getSelectionModel().getSelectedItem();
                        connector.getInfoFromCarinfo(brand,ob_id,ob_statu,ob_pledge,ob_rent);
                        combo_id.setItems(ob_id);
                        combo_id.getSelectionModel().select(0);
                } catch (SQLException throwables) {
                    throwables.printStackTrace();
                }
            }
        });
        //确定了汽车的牌号，其租金、押金和车况就一一确定了。
        combo_id.getSelectionModel().selectedItemProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observableValue, String s, String t1) {
                int tempid = combo_id.getSelectionModel().getSelectedIndex();
                String tempId = combo_id.getSelectionModel().getSelectedItem();
                ObservableList<String> ob_statu_temp = FXCollections.observableArrayList();
                ObservableList<String> ob_rent_temp = FXCollections.observableArrayList();
                ObservableList<String> ob_pledge_temp = FXCollections.observableArrayList();
                    if(tempId==null)return;
                    if(tempid>=0)
                    {
                        ob_statu_temp.add(ob_statu.get(tempid));
                        ob_pledge_temp.add(ob_pledge.get(tempid));
                        ob_rent_temp.add(ob_rent.get(tempid));
                     combo_statu.setItems(ob_statu_temp);
                     combo_pledge.setItems(ob_pledge_temp);
                     combo_rent.setItems(ob_rent_temp);
                     combo_rent.getSelectionModel().select(0);
                     combo_pledge.getSelectionModel().select(0);
                     combo_statu.getSelectionModel().select(0);
                    }
            }
        });
    }
    //退出返回到登录界面
    @FXML
    public void onExitClick()
    {
        Stage nextSage=(Stage)btn_exit.getScene().getWindow();
        nextSage.close();
        Parent root = null;
        try
        {
            root = FXMLLoader.load(getClass().getResource("login.fxml"));
        }catch (IOException e)
        {
            e.printStackTrace();
        }
        Scene scene =new Scene(root,800,600);
        scene.setRoot(root);
        Stage stage = new Stage();
        stage.setScene(scene);
        stage.show();

    }
    //租车
    @FXML
    public void onComfirmClick()
    {
        String temp_stuff = combo_sname.getSelectionModel().getSelectedItem();
        String temp_status = combo_statu.getSelectionModel().getSelectedItem();
        String temp_rent = combo_rent.getSelectionModel().getSelectedItem();
        String temp_brand = combo_brand.getSelectionModel().getSelectedItem();
        String temp_id = combo_id.getSelectionModel().getSelectedItem();
        String temp_pledge = combo_pledge.getSelectionModel().getSelectedItem();
        int temp_index = combo_sname.getSelectionModel().getSelectedIndex();
        if(temp_brand==null||temp_brand.isEmpty()||temp_stuff==null||temp_stuff.isEmpty()||temp_id==null||temp_id.isEmpty())
        {
            JOptionPane.showMessageDialog(null,"信息不全！请填写完整信息");
            return;
        }
        try
        {

            int stuffid = connector.getIDfromemployeeinfo(temp_index);
            System.out.println(stuffid);
            connector.updateDiary(Integer.parseInt(temp_id),Integer.parseInt(customer_id),stuffid,Integer.parseInt(temp_rent));
            connector.updatecarinfo(Integer.parseInt(temp_id),Integer.parseInt(customer_id),Integer.parseInt(temp_rent));
            JOptionPane.showMessageDialog(null,"租车成功！");
            combo_id.getSelectionModel().clearSelection();
            combo_brand.getSelectionModel().clearSelection();
            combo_rent.getSelectionModel().clearSelection();
            combo_statu.getSelectionModel().clearSelection();
            combo_pledge.getSelectionModel().clearSelection();
        }
        catch (SQLException E)
        {
            E.printStackTrace();
        }
    }
    @FXML
    public void onClearClick()
    {
        combo_statu.getSelectionModel().clearSelection();
        combo_brand.getSelectionModel().clearSelection();
        combo_id.getSelectionModel().clearSelection();
        combo_pledge.getSelectionModel().clearSelection();
        combo_rent.getSelectionModel().clearSelection();
        combo_sname.getSelectionModel().clearSelection();
    }
    //载入个人信息
    @FXML
    public void getpersoninfo()  {
        try{
        person_id.setCellValueFactory(new PropertyValueFactory<CustomerPerson,String>("customer_id"));
        person_cname.setCellValueFactory(new PropertyValueFactory<CustomerPerson,String>("customer_name"));
        person_member.setCellValueFactory(new PropertyValueFactory<CustomerPerson,String>("customer_member"));
        person_dlrq.setCellValueFactory(new PropertyValueFactory<CustomerPerson,String>("customer_dlrq"));
        person_moral.setCellValueFactory(new PropertyValueFactory<CustomerPerson,String>("customer_moral"));
        personList.clear();
        personList=connector.getPersoninfo(Integer.valueOf(customer_id),personList);
    }catch (SQLException e)
        {
            e.printStackTrace();
        }
        finally {
            table_personainfo.setItems(personList);
        }
    }
    //载入当前客户租车的信息
    @FXML
    public void returnCar() throws SQLException {
            car_id.setCellValueFactory(new PropertyValueFactory<customerReturnCar,String>("CarId"));
            car_brand.setCellValueFactory(new PropertyValueFactory<customerReturnCar,String>("CarBrand"));
            car_pledge.setCellValueFactory(new PropertyValueFactory<customerReturnCar,String>("CarPledge"));
            car_status.setCellValueFactory(new PropertyValueFactory<customerReturnCar,String>("CarStatus"));
            car_rent.setCellValueFactory(new PropertyValueFactory<customerReturnCar,String>("CarRent"));
            car_valid.setCellValueFactory(new PropertyValueFactory<customerReturnCar,String>("CarValid"));
            ReturnCarList.clear();
            ReturnCarList=connector.getCarinfo(Integer.valueOf(customer_id));
        tableCustomerReturnCar.setItems(ReturnCarList);
    }
    //还车
    @FXML
    public void onReturncarClick() throws SQLException {
        customerReturnCar temp_return_car = tableCustomerReturnCar.getSelectionModel().getSelectedItem();
        if(temp_return_car==null)
        {
            JOptionPane.showMessageDialog(null,"请选中你要进行退号操作的操作行！");
            return;
        }
        connector.DeleteReturnedCar(Integer.valueOf(customer_id),temp_return_car);
    }

}

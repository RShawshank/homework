import javafx.beans.value.ChangeListener;
import javafx.beans.value.ObservableValue;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ComboBox;
import javafx.scene.control.PasswordField;
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

public class loginController implements Initializable {
    @FXML
    public ComboBox<String> Combo_account,Combo_type;
    @FXML
    private PasswordField text_password;
    @FXML
    private Button loginButton;
    ObservableList<String> ename_stuff = FXCollections.observableArrayList();
    ObservableList<String> cname_customer = FXCollections.observableArrayList();
    ArrayList<String> eid_stuff,cid_customer;
    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {
        eid_stuff = new ArrayList<>();
        cid_customer = new ArrayList<>();
        Combo_type.getItems().addAll("顾客","员工");
        Combo_type.getSelectionModel().select(0);
        try{
            DBConnector connector = DBConnector.getInstance();
            Connection connection = connector.getConnection();
            String sql = "select ID,NAME from customerinfo";
            Statement statement =connection.createStatement();
            ResultSet resultSet =statement.executeQuery(sql);
            String CID,CNAME;
            while(resultSet.next())
            {
                CID=resultSet.getString("ID");
                CNAME=resultSet.getString("NAME");
                cname_customer.add(CNAME);
                cid_customer.add(CID);
            }
            sql="select ID,NAME from employeeinfo";
            resultSet=statement.executeQuery(sql);
            String EID,ENAME;
            while(resultSet.next())
            {
                EID=resultSet.getString("ID");
                ENAME=resultSet.getString("NAME");
                ename_stuff.add(ENAME);
                eid_stuff.add(EID);
            }
            resultSet.close();
            statement.close();
        } catch (SQLException throwables) {
            throwables.printStackTrace();
        }
        Combo_account.setItems(cname_customer);
        Combo_account.getSelectionModel().select(0);
        cname_customer.removeAll();
        Combo_type.getSelectionModel().selectedItemProperty().addListener(new ChangeListener<String>() {
            @Override
            public void changed(ObservableValue<? extends String> observableValue, String s, String t1) {
                int index = Combo_type.getSelectionModel().getSelectedIndex();
                if(index==0)
                {
                    Combo_account.setItems(cname_customer);
                }
                else if(index==1)
                {
                    Combo_account.setItems(ename_stuff);
                }
                Combo_account.getSelectionModel().select(0);
            }
        });
    }
    @FXML
    public void onLoginClick() throws SQLException, IOException {
        if(checkPasswordAndUsername()!=true)
            return;
        DBConnector connector = DBConnector.getInstance();
        int type=Combo_type.getSelectionModel().getSelectedIndex();
        String password = text_password.getText();
        int account_index=Combo_account.getSelectionModel().getSelectedIndex();
        String chooseId=type==0?cid_customer.get(account_index):eid_stuff.get(account_index);
       //登录密码正确
        if(connector.CheckPassword(type,Integer.parseInt(chooseId),password)==true)
        {
            System.out.println("密码验证成功！\n");
            String path=null;
            SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            if(type==0)//顾客
            {
                path="customer.fxml";
                CustomerController.setCustomer_name(Combo_account.getValue());
                CustomerController.setCustomer_id(chooseId);
            }
            else if(type==1)
            {
                path="stuff.fxml";
                StuffController.setStuff_name(Combo_account.getValue());
                StuffController.setStuff_id(chooseId);
            }
            else return;
            connector.updateTimecustomerOremployee(type,Integer.parseInt(chooseId));
            text_password.clear();
            Stage tempStage = (Stage)loginButton.getScene().getWindow();
            tempStage.close();
            Parent root = FXMLLoader.load(getClass().getResource(path));
            Scene scene = new Scene(root,1000,800);
            scene.setRoot(root);
            Stage stage = new Stage();
            stage.setScene(scene);
            stage.show();
        }
        else
        {
            JOptionPane.showMessageDialog(null, "密码和账号不匹配！");
        }
    }
    private boolean checkPasswordAndUsername()
    {
        if(Combo_account.getValue()==null||Combo_account.getValue().toString().isEmpty()==true)
        {
            JOptionPane.showMessageDialog(null,"账号不可为空！");
            return false;
        }
        if(Combo_type.getValue()==null||Combo_type.getValue().toString().isEmpty()==true)
        {
            JOptionPane.showMessageDialog(null,"类型不可为空！");
            return false;
        }
        if(text_password.getText().equals(" "))
        {
            JOptionPane.showMessageDialog(null,"密码不可为空！");
            return false;
        }
        return true;
    }

}

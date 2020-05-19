import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.fxml.Initializable;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.ChoiceBox;
import javafx.scene.control.TextField;
import javafx.stage.Stage;

import javax.swing.*;
import java.io.IOException;
import java.net.URL;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.util.ResourceBundle;
public class loginController implements Initializable {
    @FXML
    private Button loginButton;
    @FXML
    private TextField usernameField;
    @FXML
    private TextField passwordField;
    @FXML
    public ChoiceBox<String> loginType;
    String logintype=null;

    @Override
    public void initialize(URL url, ResourceBundle resourceBundle) {

    }
    @FXML
    public void onLoginClick() {
        if (checkPasswardAndUsername() != true)
            return;
            logintype = "病患";
        if (loginType.getValue().equals("医生")) {
            logintype = "医生";
        }
        try {
            DBConnector db = DBConnector.getInstance();
            if (db.checkUsernameAndPassword(usernameField.getText().trim(), passwordField.getText().trim(), logintype)) {
                String path = null;
                String title = null;
                if (logintype == "医生") {

                    doctorController.setDoctor_Id(usernameField.getText().trim());
                    doctorController.setDoctor_name(DBConnector.getInstance().getDoctorName(usernameField.getText().trim()));
                    path = "doctor.fxml";
                    title = "医生系统";
                } else {
                    RegisterController.setPatientId(usernameField.getText().trim());
                    title = "挂号系统";
                    path = "Register.fxml";
                    ResultSet cursor = (ResultSet) DBConnector.getInstance().getPatientInfo(usernameField.getText().trim());
                    while(cursor.next())
                    {
                        RegisterController.setPatientName(cursor.getString("BRMC").trim());
                        RegisterController.setPatientYCYE(cursor.getBigDecimal("YCJE"));
                    }
                }
                Stage tempStage = (Stage) loginButton.getScene().getWindow();
                tempStage.close();
                Parent root = FXMLLoader.load(getClass().getResource(path));
                Scene scene = new Scene(root, 600, 400);
                    scene.setRoot(root);
                    Stage stage = new Stage();
                    stage.setTitle(title);
                    stage.setScene(scene);
                    stage.show();
            }
            else
            {
                JOptionPane.showMessageDialog(null, "密码和账号不匹配！");
            }
        } catch (SQLException | IOException e) {
            e.printStackTrace();
        }
    }

    private boolean checkPasswardAndUsername() {
        if (usernameField.getText().equals(" ")) {
            JOptionPane.showMessageDialog(null, "用户名不可为空，请重新输入用户名");
            return false;
        }
        if (passwordField.getText().equals(" ")) {
            JOptionPane.showMessageDialog(null, "密码不可为空，请重新输入密码");
            return false;
        }
        return true;
    }
}

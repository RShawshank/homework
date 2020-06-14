import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.control.SplitPane;
import javafx.stage.Stage;

import java.io.IOException;
import java.sql.SQLException;

public class App extends Application {
    public static void main(String[] args) {
        launch(args);
    }

    public void start(Stage stage) throws Exception {
        try
        {
            DBConnector.getInstance().connectionDatabase("localhost","car_rent",3306,"root","123456");
            stage.setTitle("汽车租借系统");
            Parent root_login= FXMLLoader.load(getClass().getResource("login.fxml"));
            Scene scene = new Scene(root_login,800,600);
            stage.setScene(scene);
            stage.show();
        }
        catch (SQLException e)
        {
            System.out.println("打开数据库失败！\n");
            e.printStackTrace();
        }
    }
}

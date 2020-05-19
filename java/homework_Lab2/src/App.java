import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.io.IOException;
import java.sql.SQLException;

public class App extends Application {
    public static void main(String[] args) {
        launch(args);
    }

    @Override
    public void start(Stage primaryStage) throws IOException {
        try {
            DBConnector.getInstance().connectionDatabase("localhost","my_information",3306,"root","123456");
        } catch (SQLException throwables) {
            System.out.println("打开数据库失败！\n");
            throwables.printStackTrace();
        }
        primaryStage.setTitle("医院简易登录系统");
        Parent root = FXMLLoader.load(getClass().getResource("login.fxml"));
        Scene scene = new Scene(root, 600, 400);
        primaryStage.setScene(scene);
        primaryStage.show();
    }

}

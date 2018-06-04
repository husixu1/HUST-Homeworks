package hims;

import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.stage.Stage;

import java.sql.SQLException;

public class Main extends Application {
    @Override
    public void start(Stage primaryStage) throws Exception{
        // intialize connector
        try {
            DBConnector.getInstance().connectDataBase("localhost", 3306, "java_lab2", "java", "javajava");
        } catch (SQLException e) {
            System.err.println("failed to connect to sql database");
            System.exit(0);
        }

        //Parent root = FXMLLoader.load(getClass().getResource("Patient.fxml"));
        Parent root = FXMLLoader.load(getClass().getResource("Login.fxml"));
        primaryStage.setTitle("挂号管理系统");
        //primaryStage.setScene(new Scene(root, 600, 200));
        primaryStage.setMinWidth(400);
        primaryStage.setMinHeight(190);
        primaryStage.setScene(new Scene(root));
        primaryStage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}

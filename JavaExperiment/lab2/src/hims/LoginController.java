package hims;

import com.jfoenix.controls.*;
import com.sun.javafx.robot.impl.FXRobotHelper;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.scene.input.KeyCode;

import java.io.IOException;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

import static java.lang.System.exit;

public class LoginController {
    @FXML JFXTextField inputUsername;
    @FXML JFXPasswordField inputPassword;
    @FXML JFXButton buttonLoginDoctor;
    @FXML JFXButton buttonLoginPatient;
    @FXML JFXButton buttonExit;
    @FXML Label labelStatus;

    @FXML
    void initialize() {
        buttonLoginDoctor.setOnKeyReleased(keyEvent -> {
            try {
                if (keyEvent.getCode() == KeyCode.ENTER)
                    doctorLogin();
            } catch (IOException e) {}
        });

        buttonLoginPatient.setOnKeyReleased(keyEvent -> {
            try {
                if (keyEvent.getCode() == KeyCode.ENTER)
                    patientLogin();
            } catch (IOException e) { }
        });

        buttonExit.setOnKeyReleased(keyEvent -> {
            if (keyEvent.getCode() == KeyCode.ENTER)
                exit(0);
        });
    }

    @FXML
    void doctorLogin() throws IOException {
        if (!validateUserNameAndPassword())
            return;

        ResultSet result = DBConnector.getInstance().getDoctorInfo(inputUsername.getText().trim());
        if (result == null) {
            labelStatus.setText("读取数据库错误，请联系管理员。");
            labelStatus.setStyle("-fx-text-fill: red;");
        }

        try {
            if (!result.next()) {
                labelStatus.setText("用户不存在");
                labelStatus.setStyle("-fx-text-fill: red;");
                return;
            } else if (!result.getString(Config.NameTableColumnDoctorPassword).equals(inputPassword.getText())) {
                labelStatus.setText("密码错误");
                labelStatus.setStyle("-fx-text-fill: red;");
                return;
            }

            DoctorController.doctorName = result.getString(Config.NameTableColumnDoctorName);
            DoctorController.doctorNumber = result.getString(Config.NameTableColumnDoctorNumber);

            DBConnector.getInstance().updateDoctorLoginTime(
                    result.getString(Config.NameTableColumnDoctorNumber),
                    LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss")));

            Scene scene = new Scene(FXMLLoader.load(getClass().getResource("Doctor.fxml")));
            scene.getStylesheets().add(getClass().getResource("Main.css").toExternalForm());
            FXRobotHelper.getStages().get(0).setScene(scene);
        } catch (SQLException e){
            // progrom shouldn't came here
            e.printStackTrace();
            return;
        }
    }

    @FXML
    void patientLogin() throws IOException {
        if (!validateUserNameAndPassword())
            return;

        ResultSet result = DBConnector.getInstance().getPatientInfo(inputUsername.getText().trim());
        if (result == null) {
            labelStatus.setText("读取数据库错误，请联系管理员。");
            labelStatus.setStyle("-fx-text-fill: red;");
        }

        try {
            if (!result.next()) {
                labelStatus.setText("用户不存在");
                labelStatus.setStyle("-fx-text-fill: red;");
                return;
            } else if (!result.getString(Config.NameTableColumnPatientPassword).equals(inputPassword.getText())) {
                labelStatus.setText("密码错误");
                labelStatus.setStyle("-fx-text-fill: red;");
                return;
            }

            // fill info and login to patient page
            PatientController.patientName = result.getString(Config.NameTableColumnPatientName);
            PatientController.patientBalance = result.getDouble(Config.NameTableColumnPatientBalance);
            PatientController.patientNumber = result.getString(Config.NameTableColumnPatientNumber);

            DBConnector.getInstance().updatePatientLoginTime(
                    result.getString(Config.NameTableColumnPatientNumber),
                    LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss")));

            Scene scene = new Scene(FXMLLoader.load(getClass().getResource("Patient.fxml")));
            scene.getStylesheets().add(getClass().getResource("Main.css").toExternalForm());
            FXRobotHelper.getStages().get(0).setScene(scene);
        } catch (SQLException e) {
            // progrom shouldn't came here
            e.printStackTrace();
            return;
        }
    }

    private boolean validateUserNameAndPassword() {
        if(inputUsername.getText().isEmpty()){
            inputUsername.setStyle("-fx-background-color: pink;");
            labelStatus.setText("请输入用户名");
            labelStatus.setStyle("-fx-text-fill: red;");
            return false;
        }
        if(inputPassword.getText().isEmpty()) {
            inputPassword.setStyle("-fx-background-color: pink;");
            labelStatus.setText("请输入密码");
            labelStatus.setStyle("-fx-text-fill: red;");
            return false;
        }

        labelStatus.setText("登录中...");
        labelStatus.setStyle("");
        return true;
    }

    @FXML
    void onInputUsernameAction(){
        inputUsername.setStyle("");
    }

    @FXML
    void onInputPasswordAction(){
        inputPassword.setStyle("");
    }

    @FXML
    void buttonExitClicked(){
        exit(0);
    }
}

// TODO: update login time
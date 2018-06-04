package hims;

import com.jfoenix.controls.*;
import com.jfoenix.controls.datamodels.treetable.RecursiveTreeObject;
import com.sun.javafx.robot.impl.FXRobotHelper;
import javafx.beans.property.SimpleStringProperty;
import javafx.beans.property.StringProperty;
import javafx.collections.FXCollections;
import javafx.collections.ObservableList;
import javafx.fxml.FXML;
import javafx.fxml.FXMLLoader;
import javafx.scene.Scene;
import javafx.scene.control.*;
import javafx.util.StringConverter;
import javafx.event.*;

import java.io.IOException;
import java.sql.*;

import java.time.LocalDate;
import java.time.LocalDateTime;
import java.time.LocalTime;
import java.time.format.DateTimeFormatter;


public class DoctorController {
    private static final class Register extends RecursiveTreeObject<Register> {
        public StringProperty number;
        public StringProperty namePatient;
        public StringProperty dateTimeDisplay;
        public StringProperty isSpecialistDisplay;
        public Register(String number, String namePatient, Timestamp dateTime, boolean isSpecialist){
            this.number = new SimpleStringProperty(number);
            this.namePatient = new SimpleStringProperty(namePatient);
            this.dateTimeDisplay = new SimpleStringProperty(dateTime.toLocalDateTime().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss")));
            this.isSpecialistDisplay = new SimpleStringProperty(isSpecialist ? "专家号" : "普通号");
        }
    }

    private static final class Income extends RecursiveTreeObject<Income> {
        public StringProperty departmentName;
        public StringProperty doctorNumber;
        public StringProperty doctorName;
        public StringProperty registerType;
        public StringProperty registerPopulation;
        public StringProperty incomeSum;
        public Income(String depName, String docNum, String docName, boolean isSpec, int regNumPeople, Double incomSum){
            this.departmentName = new SimpleStringProperty(depName);
            this.doctorNumber = new SimpleStringProperty(docNum);
            this.doctorName = new SimpleStringProperty(docName);
            this.registerType = new SimpleStringProperty(isSpec ? "专家号" : "普通号");
            this.registerPopulation = new SimpleStringProperty(Integer.toString(regNumPeople));
            this.incomeSum = new SimpleStringProperty(String.format("%.2f", incomSum));
        }
    }

    // set by LoginController
    public static String doctorName;
    public static String doctorNumber;

    @FXML private Label labelWelcome;
    @FXML private JFXDatePicker pickerDateStart;
    @FXML private JFXDatePicker pickerDateEnd;
    @FXML private JFXTimePicker pickerTimeStart;
    @FXML private JFXTimePicker pickerTimeEnd;

    @FXML private JFXTabPane mainPane;
    @FXML private Tab tabRegister;
    @FXML private Tab tabIncome;

    @FXML private JFXTreeTableView<Register> tableRegister;
    @FXML private TreeTableColumn<Register, String> columnRegisterNumber;
    @FXML private TreeTableColumn<Register, String> columnRegisterPatientName;
    @FXML private TreeTableColumn<Register, String> columnRegisterDateTime;
    @FXML private TreeTableColumn<Register, String> columnRegisterType;
    private TreeItem<Register> rootRegister;

    @FXML private JFXTreeTableView<Income> tableIncome;
    @FXML private TreeTableColumn<Income, String> columnIncomeDepartmentName;
    @FXML private TreeTableColumn<Income, String> columnIncomeDoctorNumber;
    @FXML private TreeTableColumn<Income, String> columnIncomeDoctorName;
    @FXML private TreeTableColumn<Income, String> columnIncomeRegisterType;
    @FXML private TreeTableColumn<Income, String> columnIncomeRegisterPopulation;
    @FXML private TreeTableColumn<Income, String> columnIncomeSum;
    private TreeItem<Income> rootIncome;

    private ObservableList<Register> listRegister = FXCollections.observableArrayList();
    private ObservableList<Income> listIncome = FXCollections.observableArrayList();

    @FXML JFXCheckBox checkBoxAllTime;
    @FXML JFXCheckBox checkBoxToday;
    @FXML JFXButton buttonFilter;

    @FXML
    void initialize(){
        labelWelcome.setText(String.format("欢迎，%s！", doctorName));

        // set two date converter (formatter)
        pickerDateStart.setConverter(new DateConverter());
        pickerDateEnd.setConverter(new DateConverter());
        // default to current date
        pickerDateStart.setValue(LocalDate.now());
        pickerDateEnd.setValue(LocalDate.now());

        // set time selector to 24h
        pickerTimeStart.setIs24HourView(true);
        pickerTimeEnd.setIs24HourView(true);
        // default to 00:00 to 23:59
        pickerTimeStart.setValue(LocalTime.MIN);
        pickerTimeEnd.setValue(LocalTime.MAX);

        // initiailze register list
        columnRegisterNumber.setCellValueFactory(
                (TreeTableColumn.CellDataFeatures<Register, String> param) -> param.getValue().getValue().number);
        columnRegisterPatientName.setCellValueFactory(
                (TreeTableColumn.CellDataFeatures<Register, String> param) -> param.getValue().getValue().namePatient );
        columnRegisterDateTime.setCellValueFactory(
                (TreeTableColumn.CellDataFeatures<Register, String> param) -> param.getValue().getValue().dateTimeDisplay );
        columnRegisterType.setCellValueFactory(
                (TreeTableColumn.CellDataFeatures<Register, String> param) -> param.getValue().getValue().isSpecialistDisplay );

        rootRegister = new RecursiveTreeItem<>(listRegister, RecursiveTreeObject::getChildren);
        tableRegister.setRoot(rootRegister);

        // template
        // listRegister.add(new Register("a", "b", new Timestamp(1000),true));
        columnIncomeDepartmentName.setCellValueFactory(
                (TreeTableColumn.CellDataFeatures<Income, String> param) -> param.getValue().getValue().departmentName);
        columnIncomeDoctorNumber.setCellValueFactory(
                (TreeTableColumn.CellDataFeatures<Income, String> param) -> param.getValue().getValue().doctorNumber);
        columnIncomeDoctorName.setCellValueFactory(
                (TreeTableColumn.CellDataFeatures<Income, String> param) -> param.getValue().getValue().doctorName);
        columnIncomeRegisterType.setCellValueFactory(
                (TreeTableColumn.CellDataFeatures<Income, String> param) -> param.getValue().getValue().registerType);
        columnIncomeRegisterPopulation.setCellValueFactory(
                (TreeTableColumn.CellDataFeatures<Income, String> param) -> param.getValue().getValue().registerPopulation);
        columnIncomeSum.setCellValueFactory(
                (TreeTableColumn.CellDataFeatures<Income, String> param) -> param.getValue().getValue().incomeSum);

        rootIncome = new RecursiveTreeItem<>(listIncome, RecursiveTreeObject::getChildren);
        tableIncome.setRoot(rootIncome);

    }

    @FXML
    private void buttomFilterPressed() {
        if(mainPane.getSelectionModel().getSelectedItem() == tabRegister) {
            ResultSet result;
            if (checkBoxAllTime.isSelected()) {
                result = DBConnector.getInstance().getRegisterForDoctor(
                        doctorNumber,
                        "0000-00-00 00:00:00",
                        LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"))
                );
            } else if (checkBoxToday.isSelected()) {
                result = DBConnector.getInstance().getRegisterForDoctor(
                        doctorNumber,
                        LocalDate.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd")) + " 00:00:00",
                        LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"))
                );
            } else {
                result = DBConnector.getInstance().getRegisterForDoctor(
                        doctorNumber,
                        pickerDateStart.getValue().format(DateTimeFormatter.ofPattern("yyyy-MM-dd")) +
                                pickerTimeStart.getValue().format(DateTimeFormatter.ofPattern(" HH:mm:ss")),
                        pickerDateEnd.getValue().format(DateTimeFormatter.ofPattern("yyyy-MM-dd")) +
                                pickerTimeEnd.getValue().format(DateTimeFormatter.ofPattern(" HH:mm:ss"))
                );
            }

            try {
                listRegister.clear();
                while (result.next()) {
                    listRegister.add(new Register(
                            result.getString(Config.NameTableColumnRegisterNumber),
                            result.getString(Config.NameTableColumnPatientName),
                            result.getTimestamp(Config.NameTableColumnRegisterDateTime),
                            result.getBoolean(Config.NameTableColumnCategoryRegisterIsSpecialist)
                    ));
                }
            } catch (SQLException e) {
                e.printStackTrace();
                return;
            }
        } else if (mainPane.getSelectionModel().getSelectedItem() == tabIncome) {
            ResultSet result;
            if (checkBoxAllTime.isSelected()) {
                result = DBConnector.getInstance().getIncomeInfo(
                        "0000-00-00 00:00:00",
                        LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"))
                );
            } else if (checkBoxToday.isSelected()) {
                result = DBConnector.getInstance().getIncomeInfo(
                        LocalDate.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd")) + " 00:00:00",
                        LocalDateTime.now().format(DateTimeFormatter.ofPattern("yyyy-MM-dd HH:mm:ss"))
                );
            } else {
                result = DBConnector.getInstance().getIncomeInfo(
                        pickerDateStart.getValue().format(DateTimeFormatter.ofPattern("yyyy-MM-dd")) +
                                pickerTimeStart.getValue().format(DateTimeFormatter.ofPattern(" HH:mm:ss")),
                        pickerDateEnd.getValue().format(DateTimeFormatter.ofPattern("yyyy-MM-dd")) +
                                pickerTimeEnd.getValue().format(DateTimeFormatter.ofPattern(" HH:mm:ss"))
                );
            }

            try {
                listIncome.clear();
                while (result.next()) {
                    listIncome.add(new Income(
                                    result.getString("depname"),
                                    result.getString(Config.NameTableColumnDoctorNumber),
                                    result.getString("docname"),
                                    result.getBoolean(Config.NameTableColumnCategoryRegisterIsSpecialist),
                                    result.getInt(Config.NameTableColumnRegisterCurrentRegisterCount),
                                    result.getDouble("sum")
                            )
                    );
                }
            } catch (SQLException e) {
                e.printStackTrace();
                return;
            }
        }
    }

    @FXML
    private void tabSelectionChanged(Event event) {
        if(((Tab)(event.getTarget())).isSelected());
    }

    @FXML
    private void buttonExitClicked() throws IOException {
        Scene scene = new Scene(FXMLLoader.load(getClass().getResource("Login.fxml")));
        FXRobotHelper.getStages().get(0).setScene(scene);
    }

    @FXML
    void checkBoxAllTimeSelected(){
        if (checkBoxAllTime.isSelected()) {
            checkBoxToday.setSelected(false);
            pickerDateStart.setDisable(true);
            pickerDateEnd.setDisable(true);
            pickerTimeStart.setDisable(true);
            pickerTimeEnd.setDisable(true);
        } else if (!checkBoxToday.isSelected()) {
            pickerDateStart.setDisable(false);
            pickerDateEnd.setDisable(false);
            pickerTimeStart.setDisable(false);
            pickerTimeEnd.setDisable(false);
        }
    }

    @FXML
    void checkBoxTodaySelected(){
        if (checkBoxToday.isSelected()) {
            checkBoxAllTime.setSelected(false);
            pickerDateStart.setDisable(true);
            pickerDateEnd.setDisable(true);
            pickerTimeStart.setDisable(true);
            pickerTimeEnd.setDisable(true);
        } else if(!checkBoxAllTime.isSelected()){
            pickerDateStart.setDisable(false);
            pickerDateEnd.setDisable(false);
            pickerTimeStart.setDisable(false);
            pickerTimeEnd.setDisable(false);
        }
    }
}

class DateConverter extends StringConverter<LocalDate> {
    DateTimeFormatter formatter = DateTimeFormatter.ofPattern("yyyy-MM-dd");
    @Override
    public String toString(LocalDate localDate) {
        if(localDate != null){
            return formatter.format(localDate);
        } else {
            return "";
        }
    }

    @Override
    public LocalDate fromString(String s) {
        if(s != null && !s.isEmpty()) {
            return LocalDate.parse(s, formatter);
        } else {
            return null;
        }
    }
}

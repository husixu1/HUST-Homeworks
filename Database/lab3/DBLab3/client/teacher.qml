import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import QtQml.Models 2.3
import QtGraphicalEffects 1.0

import 'qrc:/styles'
import DBLab3.TeacherController 0.1

Pane {
    id: teacherPane
    width: 1024
    height: 768
    anchors.fill: parent

    TabBar {
        id: tabBar
        width: parent.width
        TabButton {
            text: qsTr("Self")
        }
        TabButton {
            text: qsTr("Course")
        }
        TabButton {
            text: qsTr("Task")
        }
    }

    StackLayout {
        width: parent.width
        anchors.bottom: errorLabel.top
        anchors.top: tabBar.bottom
        currentIndex: tabBar.currentIndex
        // Self Information tab
        Item {
            id: selfTab
            Grid{
                anchors.fill: parent
                rows: 2
                columns: 2
                spacing: 10
                Pane{
                    id: selfPane
                    Material.elevation: 4
                    width: parent.width / 2 - parent.spacing / 2
                    height: parent.height / 2 - parent.spacing / 2
                    Column {
                        spacing: 10
                        Label {
                            font.pixelSize: window.titleFontSize
                            text: qsTr("Personal Information")
                            Material.foreground: Material.Blue
                        }
                        Label { id: selfRealName; text: qsTr("Name: ") }
                        Label { id: selfUserName; text: qsTr("User: ") }
                        Label { id: selfIdentity; text: qsTr("Id:   ") }
                        Button{ visible: false; text: qsTr("Change Username")}
                        Button{ visible: false; text: qsTr("Change Password")}
                    }
                }
                Pane{
                    id: selfCoursePane
                    Material.elevation: 4
                    width: parent.width / 2 - parent.spacing / 2
                    height: parent.height / 2 - parent.spacing / 2
                    Label {
                        id: selfCourseTitle
                        font.pixelSize: titleFontSize
                        text: qsTr("Teaching Courses")
                        Material.foreground: Material.Purple
                    }
                    ListView {
                        anchors.top: selfCourseTitle.bottom
                        anchors.bottom: parent.bottom
                        width: parent.width
                        clip: true
                        spacing: -10
                        model: teachingCourseModel

                        ScrollIndicator.vertical: ScrollIndicator{
                            orientation: Qt.Vertical
                        }

                        delegate: Button{
                            flat: true
                            anchors.left: parent.left
                            anchors.right: parent.right
                            Label{
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: modelData
                            }
                            onClicked: {
                                unregMenu.popup()
                            }
                            Menu{
                                id: unregMenu
                                MenuItem {
                                    font.pixelSize: window.subTitleFontSize
                                    text: "unregister"
                                    onClicked: {
                                        controller.unregisterCourse(index);
                                    }
                                }
                            }
                        }
                    }
                }
                // done and undone task pane
                Pane{
                    id: selfTaskPane
                    Material.elevation: 4
                    width: parent.width / 2 - parent.spacing / 2
                    height: parent.height / 2 - parent.spacing / 2
                    Label {
                        id: selfTaskTitle
                        font.pixelSize: titleFontSize
                        text: qsTr("Assigned Tasks")
                        Material.foreground: Material.Pink
                    }
                    Button {
                        id: addTaskShowPaneButton
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        text: "Add a Task"
                        onClicked: addTaskPane.visible = true
                    }
                    ListView {
                        anchors.top: selfTaskTitle.bottom
                        anchors.bottom: addTaskShowPaneButton.top
                        width: parent.width
                        clip: true
                        spacing: -10
                        model: assignedTaskModel

                        ScrollIndicator.vertical: ScrollIndicator{
                            orientation: Qt.Vertical
                        }
                        delegate: Button{
                            flat: true
                            anchors.left: parent.left
                            anchors.right: parent.right
                            Label{
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: modelData
                            }
                        }
                    }
                }
                Pane{
                    id: selfApplicationPane
                    Material.elevation: 4
                    width: parent.width / 2 - parent.spacing / 2
                    height: parent.height / 2 - parent.spacing / 2
                    Label {
                        id: selfApplications
                        font.pixelSize: titleFontSize
                        text: qsTr("Applications")
                        Material.foreground: Material.Green
                    }
                    ListView {
                        anchors.top: selfApplications.bottom
                        anchors.bottom: parent.bottom
                        width: parent.width
                        clip: true
                        spacing: -10
                        model: relaventAppModel

                        ScrollIndicator.vertical: ScrollIndicator{
                            orientation: Qt.Vertical
                        }

                        delegate: Button {
                            flat: true
                            anchors.left: parent.left
                            anchors.right: parent.right
                            Label {
                                anchors.verticalCenter: parent.verticalCenter
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                text: modelData
                            }
                        }
                    }
                }
            }

            Pane{
                id: addTaskPane
                anchors.centerIn: parent
                width: parent.width * 0.66
                height: parent.height * 0.66
                Material.elevation: 6
                visible: false
                Label{
                    id: addTaskLabel
                    anchors.top: parent.top
                    anchors.horizontalCenter: parent.horizontalCenter
                    font.pixelSize: window.titleFontSize
                    Material.foreground: Material.DeepOrange
                    text: "Add a Task"
                }
                Label {
                    id: selectCourseLabel
                    anchors.top: addTaskLabel.bottom
                    anchors.left: parent.left
                    font.pixelSize: window.subTitleFontSize
                    text: "Select a course:"
                }
                ComboBox {
                    model: teachingCourseModel
                    id: courseSelectionBox
                    anchors.top: selectCourseLabel.bottom
                    width: parent.width
                }
                Label {
                    id: addTaskQuestionLabel
                    anchors.top: courseSelectionBox.bottom
                    anchors.left: parent.left
                    font.pixelSize: window.subTitleFontSize
                    text:  "What's the question?"
                }
                ScrollView {
                    anchors.top: addTaskQuestionLabel.bottom
                    anchors.bottom: addTaskButton.top
                    width: parent.width
                    clip: true
                    ScrollBar.vertical: ScrollBar{
                        orientation: Qt.Vertical
                        anchors.right: parent.right
                        anchors.top: parent.top
                        height: parent.height - addTaskQuesionArea.bottomPadding
                    }
                    TextArea {
                        id: addTaskQuesionArea
                        width: parent.width
                    }
                }
                Button {
                    id: addTaskButton
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    text: "Add"
                    Material.background: Material.Cyan
                    onClicked: {
                        controller.addTask(
                                   courseSelectionBox.currentIndex,
                                   addTaskQuesionArea.text)
                        // hide the pane
                        addTaskPane.visible = false
                    }
                }
                Button {
                    text: "Cancel"
                    anchors.bottom: parent.bottom
                    anchors.right: addTaskButton.left
                    anchors.rightMargin: 10
                    onClicked: addTaskPane.visible = false
                }
            }

            RoundButton {
                width: 50
                height: 50
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                Material.background: Material.Pink
                Material.foreground: "white"
                text: "\u21bb"
                font.pixelSize: window.titleFontSize
                onClicked: controller.initialize()  // refresh content
            }
        }

        // Course Selection tab
        Item {
            id: courseTab

            Button {
                id: courseRegisterButton
                anchors.top: parent.top
                anchors.right: parent.right
                text: "Register"
                Material.background: Material.Cyan
                onClicked: controller.selectCourseToTeach()
            }

            Button {
                id: courseRefreshButton
                anchors.top: parent.top
                anchors.right: courseRegisterButton.left
                anchors.rightMargin: 10
                text: "Refresh"
                onClicked: controller.updateAllCourseInfo()
            }

            Row{
                id: courseFilterRow
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: courseRefreshButton.left
                height: courseRefreshButton.height

                spacing: 10
                TextField {
                    width: parent.width / 3 - parent.spacing
                    anchors.bottom: parent.bottom
                    id: courseIdFilter
                    placeholderText: "id"
                    onTextChanged: {
                        allCourseModel.sortingId = text
                        allCourseModel.refresh()
                    }
                }
                TextField {
                    width: parent.width / 3 - parent.spacing
                    anchors.bottom: parent.bottom
                    id: courseNameFilter
                    placeholderText: "name"
                    onTextChanged: {
                        allCourseModel.sortingName = text
                        allCourseModel.refresh()
                    }
                }
                TextField {
                    width: parent.width / 3 - parent.spacing
                    anchors.bottom: parent.bottom
                    id: courseSpanFilter
                    placeholderText: "span"
                    onTextChanged: {
                        allCourseModel.sortingSpan = isNaN(parseInt(text)) ? 0 : (parseInt(text))
                        console.log(allCourseModel.sortingSpan)
                        allCourseModel.refresh()
                    }
                }
            }

            // display all available courses
            TableView {
                id: courseTable
                anchors.top: courseFilterRow.bottom
                anchors.bottom: parent.bottom
                width: parent.width
                alternatingRowColors: false
                backgroundVisible: true
                model: allCourseModel
                sortIndicatorVisible: true
                style: TableHeaderStyle { }

                TableViewColumn{
                    title: qsTr("select")
                    // NOTE: this is a brilliant idea (to use an hidden data)
                    role: "id"
                    delegate : CheckBox{
                        visible: !controller.getIsCoursePending(styleData.value);
                        checked: controller.getIsCourseSelected(styleData.value)
                        enabled: !controller.getIsCourseSelected(styleData.value)
                        anchors.fill: parent
                        onClicked: {
                            controller.prepareCourses(styleData.value, checked)
                        }
                    }
                }
                TableViewColumn {
                    id: testcol
                    role: "id"
                    title: qsTr("Course Id")
                }
                TableViewColumn {
                    role: "name"
                    title: qsTr("Name")
                }
                TableViewColumn{
                    role: "span"
                    title: qsTr("Span")
                }
                TableViewColumn{
                    role: "season"
                    title: qsTr("Season")
                }
            }
        }

        // Task Tab
        Item {
            id: taskTab
            // display current task/student info and misc controls
            Pane{
                id: taskRow
                width: parent.width
                height: taskSubmitButton.height
                Material.elevation: 2
                padding: 0
                Button {
                    id: taskSubmitButton
                    anchors.right: parent.right
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Grade"
                    Material.background: Material.Cyan
                    onClicked:  controller.gradeStudent(gradeSpinBox.value);
                }
                SpinBox {
                    id: gradeSpinBox
                    anchors.right: taskSubmitButton.left
                    anchors.rightMargin: 10
                    from: 0
                    to: 100
                    stepSize: 1
                    editable: true
                }
                Button{
                    id: taskShowListButton
                    anchors.right: gradeSpinBox.left
                    anchors.rightMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: "List"
                    onClicked: {
                        taskListPane.state =
                                ((taskListPane.state == "show") ?
                                     "hide" : "show")
                        taskStudentListPane.state = "hide"
                    }

                }

                Label{
                    id: taskInfo
                    anchors.right: taskShowListButton.left
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: {
                        if (controller.currentTaskId.length === 0)
                            return ""
                        if (controller.currentStudentInfo.length === 0)
                            return "Test " + controller.currentTaskId
                        else
                            return "Test " + controller.currentTaskId
                                    + ", Student " + controller.currentStudentInfo
                    }
                }
            }

            SplitView{
                anchors.top: taskRow.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                orientation: Qt.Horizontal

                // TODO: make scrollable, also make student's page's \
                // textares scrollable
                // the question
                TextArea{
                    id: taskQuestion
                    width: parent.width / 2
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                    text: controller.currentTaskQuestion
                }
                // the answering area
                TextArea {
                    id: taskAnswer
                    readOnly: true
                    wrapMode: TextEdit.Wrap
                    text: controller.currentTaskAnswer
                }
            }

            // student list
            Pane {
                id: taskStudentListPane
                width: 0.2*parent.width < 350 ? 350 : 0.2*parent.width
                height: parent.height
                anchors.top: parent.top
                leftPadding: 0
                rightPadding: 0
                bottomPadding: 0
                Material.elevation: 4
                //Material.background: Material.Pink

                ListView{
                    ScrollIndicator.vertical: ScrollIndicator{
                        orientation: Qt.Vertical
                    }
                    clip: true
                    model: relaventStudentsModel
                    anchors.fill: parent
                    header: Label{
                        text: "Select a Student"
                        font.pixelSize: window.titleFontSize
                        width: parent.width
                        horizontalAlignment: Text.AlignHCenter
                        Material.foreground: Material.Cyan
                    }

                    // task buttons
                    delegate: Button{
                        flat: true
                        width: parent.width
                        contentItem: Label{
                            text: modelData
                            horizontalAlignment: Text.AlignLeft
                        }
                        onClicked: {
                            taskListPane.state = "hide"
                            taskStudentListPane.state = "hide"
                            controller.getSpecificAnswer(index);
                        }
                    }
                }

                states: [
                    State {
                        name: "show"
                        PropertyChanges {
                            target: taskStudentListPane
                            x: - teacherPane.leftPadding + taskListPane.width
                        }
                    },
                    State {
                        name: "hide"
                        PropertyChanges {
                            target: taskStudentListPane
                            x: - width - teacherPane.leftPadding
                        }
                    }
                ]
                // initial state
                state: "hide"
                // define transition animations
                transitions: Transition {
                    PropertyAnimation {
                        targets: taskStudentListPane
                        properties: "x"
                        duration: 300
                        easing.type: Easing.OutExpo
                    }
                }

                Button{
                    id: taskCloseListButton
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    flat: true
                    text: "«"
                    onClicked: {
                        taskListPane.state =
                                ((taskListPane.state == "show") ?
                                     "hide" : "show")
                        taskStudentListPane.state =
                                ((taskStudentListPane.state == "show") ?
                                     "hide" : "show")
                    }
                } // Button
            } // Pane

            // problem list
            Pane {
                id: taskListPane
                width: 0.2*parent.width < 350 ? 350 : 0.2*parent.width
                height: parent.height
                anchors.top: parent.top
                leftPadding: 0
                rightPadding: 0
                bottomPadding: 0
                Material.elevation: 4
                //Material.background: Material.Pink

                ListView{
                    ScrollIndicator.vertical: ScrollIndicator{
                        orientation: Qt.Vertical
                    }
                    clip: true
                    model: assignedTaskModel
                    anchors.fill: parent
                    header: Label{
                        text: "Select a Task"
                        font.pixelSize: window.titleFontSize
                        width: parent.width
                        horizontalAlignment: Text.AlignHCenter
                        Material.foreground: Material.Blue
                    }

                    // task buttons
                    delegate: Button{
                        flat: true
                        width: parent.width
                        contentItem: Label{
                            text: modelData
                            horizontalAlignment: Text.AlignLeft
                        }
                        onClicked: {
                            taskStudentListPane.state = "show"
                            controller.getStudentsForTask(index);
                        }
                    }
                }

                states: [
                    State {
                        name: "show"
                        PropertyChanges {
                            target: taskListPane
                            x: - teacherPane.leftPadding
                        }
                    },
                    State {
                        name: "hide"
                        PropertyChanges {
                            target: taskListPane
                            x: - width - teacherPane.leftPadding
                        }
                    }
                ]
                // initial state
                state: "hide"
                // define transition animations
                transitions: Transition {
                    PropertyAnimation {
                        targets: taskListPane
                        properties: "x"
                        duration: 300
                        easing.type: Easing.OutExpo
                    }
                }
            } // Pane

        } // Item
    } // Pane

    Label{
        id: errorLabel
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        Material.foreground: Material.Pink

        // undisplay text after certain time
        Timer{
            id: errorLabelTimer
            repeat: false
            interval: 2500
            onTriggered: {
                errorLabel.text = ""
            }
        }
    }
    TeacherController {
        id: controller

        username: window.username
        password: window.password
        identity: window.identity
        realname: window.realname

        Component.onCompleted: {
            // initialize backend
            controller.initialize()

            // initialize GUI
            selfIdentity.text += identity
            selfUserName.text += username
            selfRealName.text += realname
        }

        onDisplayError: {
            errorLabel.text = message
            errorLabel.Material.foreground = Material.Pink
            errorLabelTimer.restart()
        }

        onDisplayMessage: {
            errorLabel.text = message
            errorLabel.Material.foreground = Material.Green
            errorLabelTimer.restart()
        }
    }
}


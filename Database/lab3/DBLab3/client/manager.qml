import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.3
import QtQml.Models 2.3
import QtGraphicalEffects 1.0

import 'qrc:/styles'
import DBLab3.ManagerController 0.1

Pane{
    TabBar {
        id: tabBar
        width: parent.width
        currentIndex: managerSwipe.currentIndex
        TabButton {
            text: qsTr("Information")
        }
        TabButton {
            text: qsTr("Applications")
        }
    }

    SwipeView {
        clip: true
        id: managerSwipe
        width: parent.width
        //anchors.fill: parent
        anchors.bottom: errorLabel.top
        anchors.top: tabBar.bottom
        currentIndex: tabBar.currentIndex
        Item{
            id: informationTab
            Pane {
                anchors.margins: 4
                id: selfInformationPane
                Material.elevation: 4
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                padding: 0
                height: selfChangeUserName.height
                Label{
                    id: selfIdentity
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 10
                }
                Label{
                    id: selfUserName
                    anchors.left: selfIdentity.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                }
                Label{
                    id: selfRealName
                    anchors.left: selfUserName.right
                    anchors.leftMargin: 10
                    anchors.right: selfChangeUserName.left
                    anchors.verticalCenter: parent.verticalCenter
                }
                Button{
                    visible: false
                    id: selfChangeUserName
                    anchors.right: selfChangePassword.left
                    anchors.rightMargin: 10
                    text: "Change Username"
                }
                Button{
                    visible: false
                    id: selfChangePassword
                    anchors.right: parent.right
                    text: "Change Password"
                    anchors.rightMargin: 10
                }
            }
            Pane{
                id: studentListPane
                anchors.top: selfInformationPane.bottom
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                width: parent.width / 3 - 8
                anchors.margins: 4
                Material.elevation: 4
                Layout.fillWidth: true
                Label {
                    id: studentListLabel
                    Material.foreground: Material.Purple
                    font.pixelSize: window.titleFontSize
                    text: "Students"
                }

                ListView {
                    anchors.top: studentListLabel.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    clip: true
                    model: studentModel
                    spacing: -10

                    delegate: Button{
                        flat: true
                        width: parent.width
                        Label{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.leftMargin: 10
                            text: modelData
                        }
                    }

                    ScrollBar.vertical: ScrollBar{
                        orientation: Qt.Vertical
                    }
                }
                RoundButton {
                    width: 50
                    height: 50
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    Material.background: Material.Purple
                    Material.foreground: "white"
                    text: "\u21bb"
                    font.pixelSize: window.titleFontSize
                    onClicked: controller.getStudentsList()
                }
            }
            Pane{
                id: courseListPane
                anchors.top: selfInformationPane.bottom
                anchors.bottom: parent.bottom
                anchors.left: studentListPane.right
                anchors.right: teacherListPane.left
                anchors.margins: 4
                Material.elevation: 4
                Layout.fillWidth: true
                Label {
                    id: courseListLabel
                    Material.foreground: Material.Pink
                    font.pixelSize: window.titleFontSize
                    text: "Courses"
                }
                ListView {
                    anchors.top: courseListLabel.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    clip: true
                    model: courseModel
                    spacing: -10

                    delegate: Button{
                        flat: true
                        width: parent.width
                        Label{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.leftMargin: 10
                            text: modelData
                        }
                    }

                    ScrollBar.vertical: ScrollBar{
                        orientation: Qt.Vertical
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
                    onClicked: controller.getCoursesList()
                }
            }
            Pane{
                id: teacherListPane
                anchors.top: selfInformationPane.bottom
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                anchors.margins: 4
                width: parent.width / 3 - 8
                Material.elevation: 4
                Layout.fillWidth: true
                Label {
                    id: teacherListLabel
                    Material.foreground: Material.Red
                    font.pixelSize: window.titleFontSize
                    text: "Teachers"
                }
                ListView {
                    anchors.top: teacherListLabel.bottom
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                    clip: true
                    model: teacherModel
                    spacing: -10
                    //width: parent.width

                    delegate: Button {
                        flat: true
                        width: parent.width
                        Label{
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.leftMargin: 10
                            text: modelData
                        }
                    }

                    ScrollBar.vertical: ScrollBar{
                        orientation: Qt.Vertical
                    }
                }
                RoundButton {
                    width: 50
                    height: 50
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    Material.background: Material.Red
                    Material.foreground: "white"
                    text: "\u21bb"
                    font.pixelSize: window.titleFontSize
                    onClicked: controller.getTeachersList()
                }
            }
        }
        // application tab
        Item{
            id: applicationTab
            Pane {
                id: teacherAppListPane
                anchors.left: parent.left
                anchors.margins: 5
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                width: parent.width / 2 - 8
                Material.elevation: 4

                Label {
                    id: teacherApplicationLabel
                    Material.foreground: Material.Blue
                    font.pixelSize: window.titleFontSize
                    text: "Teachers' Applications"
                }
                ListView {
                    anchors.top: teacherApplicationLabel.bottom
                    anchors.bottom: acceptTeacherAppButton.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    clip: true
                    model: teacherAppModel
                    spacing: -10
                    //width: parent.width

                    delegate: Button {
                        CheckBox {
                            id: teacherAppCheckBox
                            anchors.left: parent.left
                            onCheckedChanged: controller.prepareTeacherApp(index, checked);
                        }
                        Label {
                            anchors.left: teacherAppCheckBox.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            text: modelData
                        }
                        flat: true
                        width: parent.width
                        onClicked: {
                            teacherAppCheckBox.checked =
                                    teacherAppCheckBox.checked ? false : true
                        }
                    }

                    ScrollIndicator.vertical: ScrollIndicator{
                        orientation: Qt.Vertical
                    }
                }
                Button {
                    id: acceptTeacherAppButton
                    anchors.bottom: parent.bottom
                    anchors.right: declineTeacherAppButton.left
                    anchors.rightMargin: 10
                    Material.background: Material.Blue
                    text: "Accept"
                    onClicked: controller.executeTeacherAppList(true);
                }
                Button {
                    id: declineTeacherAppButton
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    text: "Decline"
                    onClicked: controller.executeTeacherAppList(false);
                }
                RoundButton {
                    width: 50
                    height: 50
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    Material.background: Material.Blue
                    Material.foreground: "white"
                    text: "\u21bb"
                    font.pixelSize: window.titleFontSize
                    onClicked: controller.getTeacherApplicationList()
                }
            }
            Pane {
                id: studentAppListPane
                anchors.left: teacherAppListPane.right
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.margins: 5
                Material.elevation: 4
                Label {
                    id: studentApplicationLabel
                    Material.foreground: Material.Green
                    font.pixelSize: window.titleFontSize
                    text: "Students' Applications"
                }
                ListView {
                    anchors.top: studentApplicationLabel.bottom
                    anchors.bottom: acceptStudentAppButton.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    clip: true
                    model: studentAppModel
                    spacing: -10
                    //width: parent.width

                    delegate: Button {
                        CheckBox {
                            id: studentAppCheckBox
                            anchors.left: parent.left
                            onCheckedChanged: controller.prepareStudentApp(index, checked);
                        }
                        Label {
                            anchors.left: studentAppCheckBox.right
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            text: modelData
                        }
                        flat: true
                        width: parent.width
                        onClicked: {
                            studentAppCheckBox.checked =
                                    studentAppCheckBox.checked ? false : true
                        }
                    }

                    ScrollIndicator.vertical: ScrollIndicator{
                        orientation: Qt.Vertical
                    }
                }
                Button {
                    id: acceptStudentAppButton
                    anchors.bottom: parent.bottom
                    anchors.right: declineStudentAppButton.left
                    anchors.rightMargin: 10
                    Material.background: Material.Green
                    text: "Accept"
                    onClicked: controller.executeStudentAppList(true);
                }
                Button {
                    id: declineStudentAppButton
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    text: "Decline"
                    onClicked: controller.executeStudentAppList(false);
                }
                RoundButton {
                    width: 50
                    height: 50
                    anchors.left: parent.left
                    anchors.bottom: parent.bottom
                    Material.background: Material.Green
                    Material.foreground: "white"
                    text: "\u21bb"
                    font.pixelSize: window.titleFontSize
                    onClicked: controller.getStudentApplicationList()
                }
            }
        }
    }
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

    ManagerController {
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

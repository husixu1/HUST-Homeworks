import QtQuick 2.0
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import DBLab3.LoginController 0.1

Item {
    anchors.fill: parent
    Label {
        text: qsTr("Login");
        anchors.leftMargin: horizontalMargin
        font.pointSize: 12
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.topMargin: verticalMargin
    }

    TextField {
        property string defaultPlaceHolder : "Username"

        id: username
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 2 * horizontalMargin
        placeholderText: defaultPlaceHolder

        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: -height

        Label {
            id: usernameError
            text: qsTr("✩ Please input username")
            anchors.verticalCenter: parent.verticalCenter
            Material.foreground: Material.Pink
            visible: false
        }

        // remove error hint
        onFocusChanged: {
            if (focus) {
                placeholderText = defaultPlaceHolder
                usernameError.visible = false
            }
        }
    }

    TextField {
        property string defaultPlaceHolder : "Password"

        id: password
        anchors.horizontalCenter: parent.horizontalCenter
        width: parent.width - 2 * horizontalMargin
        placeholderText: defaultPlaceHolder
        anchors.verticalCenter: parent.verticalCenter
        anchors.verticalCenterOffset: 0.3 * height
        echoMode: TextInput.Password

        Label {
            id: passwordError
            text: qsTr("✩ Please input password")
            anchors.verticalCenter: parent.verticalCenter
            Material.foreground: Material.Pink
            visible: false
        }

        // remove error hint
        onFocusChanged: {
            if (focus) {
                placeholderText = defaultPlaceHolder
                passwordError.visible = false
            }
        }
    }

    // check if required textfield is empty
    function checkTextField(){
        var result = true
        if(!username.text.length){
            usernameError.visible = true;
            username.placeholderText = "";
            result = false
        }
        if(!password.text.length){
            passwordError.visible = true;
            password.placeholderText = "";
            result = false
        }
        return result
    }

    Button {
        id: managerLogin
        x: 20
        y: 140
        text: qsTr("I'm manager")

        onClicked: {
            if (checkTextField()) {
                controller.tryLogin(username.text, password.text, LoginController.Manager)
            }
        }
    }

    Button {
        id: teacherLogin
        x: 145
        y: 140
        text: qsTr("I'm teacher")

        onClicked: {
            if (checkTextField()) {
                controller.tryLogin(username.text, password.text, LoginController.Teacher)
            }
        }
    }

    Button {
        id: studentLogin
        x: 272
        y: 140
        text: qsTr("I'm student")

        onClicked: {
            if (checkTextField()) {
                controller.tryLogin(username.text, password.text, LoginController.Student)
            }
        }
    }

    Label {
        Material.foreground: Material.Pink
        id: errorText
        y: 190
        width: parent.width - anchors.leftMargin
        text: qsTr("")
        anchors.left: parent.left
        anchors.leftMargin: 20
        font.pixelSize: 12
    }

    // the backend
    LoginController {
        id: controller
        // display error text
        onDisplayError: {
            errorText.text = message
        }
        onLogin: {
            window.username = user
            window.password = pass
            window.identity = identity
            window.realname = realName
            switch(role){
                case LoginController.Manager:
                    pageLoader.source = "manager.qml"
                    break;
                case LoginController.Teacher:
                    pageLoader.source = "teacher.qml"
                    break;
                case LoginController.Student:
                    pageLoader.source = "student.qml"
                    break;
            }
        }
    }
}

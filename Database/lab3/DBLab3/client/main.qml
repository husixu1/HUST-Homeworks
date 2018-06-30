import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Controls.Material 2.2
import DBLab3.LoginController 0.1

ApplicationWindow {
    id: window
    visible: true
    visibility: "Windowed"
    title: qsTr("Hello World")

    font.pixelSize: 14
    property int titleFontSize: 24
    property int subTitleFontSize: 18

    property int horizontalMargin: 20
    property int verticalMargin: 10

    // user spec
    property string username
    property string password
    // FIXME: switch back to none
    property string identity// : "M201800002"
    property string realname

    Loader{
        anchors.fill: parent
        //FIXME: switch back to login.qml in release version
        source: "login.qml"
        id: pageLoader
        focus: true
        active: true
        onSourceChanged: {
            // set window geometry
            if (pageLoader.source == "qrc:/login.qml"){
                window.width = 400
                window.height = 215
                // make window unresizeable
                // this is a hack, i don't know why but it works
                // if maxval and minval are same, window cannot be resized even
                // they are changed later
                window.maximumHeight = 214
                window.minimumHeight = 215
                window.maximumWidth  = 399
                window.minimumWidth  = 400
            } else {
                // make window resizeable again
                window.maximumHeight = 65535
                window.minimumHeight = 400
                window.maximumWidth = 65535
                window.minimumWidth = 800
                window.width = 1024
                window.height = 768
            }
            //TODO: add more
        }
    }
    onWidthChanged: {
        update()
    }
}

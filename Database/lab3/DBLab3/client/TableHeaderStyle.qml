import QtQuick 2.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2

TableViewStyle{
    headerDelegate: Pane {
        height: titleLabel.implicitHeight * 2
        width: titleLabel.implicitWidth
        Material.background: styleData.containsMouse ? (
            styleData.pressed ? Material.LightBlue: Material.Cyan) : "white"
        Material.elevation: 2
        Label {
            id: titleLabel
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: styleData.textAlignment
            anchors.leftMargin: 12
            text: styleData.value
            elide: Text.ElideRight
            color: textColor
            renderType: Text.NativeRendering
        }
    }
}

import QtQuick 2.4

Item {
    width: 400
    height: 400

    Rectangle {
        id: rectangle
        x: 27
        y: 166
        width: 126
        height: 65
        color: "#ffffff"

        Text {
            id: text1
            x: 52
            y: 26
            text: qsTr("Move")
            font.pixelSize: 12
        }
    }

    Rectangle {
        id: rectangle2
        x: 222
        y: 166
        width: 126
        height: 65
        color: "#ffffff"

        Text {
            id: text2
            x: 52
            y: 26
            text: qsTr("Shoot")
            font.pixelSize: 12
        }
    }

    Connections {
        target: rectangle
        onClicked: print("clicked")
    }

    Connections {
        target: rectangle2
        onClicked: print("clicked")
    }
}

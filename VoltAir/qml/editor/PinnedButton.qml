import QtQuick 2.0

/**
*   Simple component to pin and unpin a widget.
*/
Item {
    id:pinned
    width: 10
    height: 10
    anchors.top: parent.top
    anchors.right: parent.right
    anchors.rightMargin: -10

    property int pinned_val: 0

    Rectangle{
        color: "red"
        anchors.fill: parent
    }
    Component.onCompleted: {}
}


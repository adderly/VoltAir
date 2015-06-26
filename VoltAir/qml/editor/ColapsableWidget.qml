import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import VoltAir 1.0

Rectangle {
    id: widgetRoot
    width: 200
    height: 160
    color: "#00000000"
    anchors.left: parent.left
    anchors.right: parent.right
    property alias title:title_lbl
    property alias icon:image1
    property alias content:content
    property bool content_hidden:false
    property bool hide_content_when_hidden:true
    property int _width
    property int _height



    Layout.alignment: Qt.AlignTop
    Layout.fillHeight: false
    Layout.minimumHeight: header.height

    state:"active"
    states:[
        State{
          name:"hidden"
          AnchorChanges{target: content; anchors.bottom: header.bottom}
          PropertyChanges{target: icon; rotation:0}
          PropertyChanges{target: widgetRoot; height: 0}
          PropertyChanges{target: widgetRoot; implicitHeight: 0}
          PropertyChanges{target: content; visible:!hide_content_when_hidden;}
        },
        State{
          name:"active"
          AnchorChanges{target: content; anchors.bottom: widgetRoot.bottom}
          PropertyChanges{target: icon; rotation:90}

        }
    ]
    transitions: [
        Transition {
            from: "hidden"
            to: "active"
             // smoothly reanchor myRect and move into new position
            AnchorAnimation { duration: 300 }
            RotationAnimation{ direction: RotationAnimation.Clockwise}
            PropertyAnimation{ properties: "implicitHeight";duration: 300}
        },Transition {
            from: "active"
            to: "hidden"
            AnchorAnimation { duration: 300 }
            RotationAnimation{ direction:RotationAnimation.Counterclockwise}
            PropertyAnimation{ properties: "implicitHeight";duration: 300}
        }
    ]

    Item {
        id: header
        y: 0
        height: 21
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        RowLayout {
            id: rowLayout1
            spacing: 1
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.leftMargin: 0
            anchors.topMargin: 0
            anchors.fill: parent

            Row {
                id: row1
                spacing: 5
                anchors.fill: parent

                Image {
                    id: image1
                    width: 20
                    height: 20
                    anchors.verticalCenter: parent.verticalCenter
                    source: Util.getPathToAsset("assets/editor/widget_arrow.png")
                }

                Label {
                    id: title_lbl
                    width: 51
                    text: qsTr("Label_____")
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
        }

        MouseArea{
            anchors.fill: parent
            onClicked: {
                toggle()
            }
        }

        Rectangle {
            id: rectangle1
            color: "#00000000"
            z: -1
            border.width: 1
            anchors.fill: parent
        }
    }

    Rectangle {
        id: content
        x: 0
        height: 120
        width: 200
        color: "#291f1f"
        border.width: 1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.top: header.bottom
        anchors.topMargin: 0
    }

    Rectangle {
        id: rectangle2
        width: 200
        height: 200
        color: "#ffffff"
        z: -1
        anchors.fill: parent
    }

    function toggle(){
        if(widgetRoot.state == "active"){
           widgetRoot.state = "hidden"
        }else {
            widgetRoot.state = "active"
        }
    }

    onChildrenRectChanged: {

    }
    Component.onCompleted: {
        widgetRoot.height= childrenRect.height
        //widgetRoot.width= childrenRect.width

        widgetRoot._height= childrenRect.height
        widgetRoot._width= childrenRect.width
        Layout.maximumHeight=  height
    }
}


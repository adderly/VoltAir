import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.1
import VoltAir 1.0
//import com.iktwo.components 1.0

Rectangle
{
    id:mainWindow
    width: 800
    height: 540
    color: "#00000000"
    property alias editorRootItem:rootItem

    states:[
        State{
            name:"editor"
        },
        State{
            name:"game"
        }
    ]

    Item {
        id: menuWrapper
        height: 20
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.rightMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 0

        TopMenuBar{
            id:myTopMenu
        }

       states: State {
         name: "hasMenuBar"
         when: myTopMenu && !myTopMenu.__isNative

         ParentChange {
             target: myTopMenu.__contentItem
             parent: mainWindow
         }

         PropertyChanges {
             target: myTopMenu.__contentItem
             x: 0
             y: 0
             width: menuWrapper.width
         }
       }
   }

    EditorRoot {
        id: rootItem
        objectName:"rootItem"
        color: "transparent"
        visible: true
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
        anchors.leftMargin: 0
        anchors.topMargin: 1
        anchors.fill: parent


        // Background must be transparent in order to render our scene behind it.

        Camera {
            id: camera
            objectName: "camera"


            // Control movement of the camera (lookAt and fov) based on locations of PlayerActors
            CameraMovementLogic {
                minFov: 16
            }

            Loader {
                id: levelLoader
                objectName: "levelLoader"

                // This is our translated signal
                signal loadCompleted(bool success)

                asynchronous: true

                onStatusChanged: {
                    if (levelLoader.status == Loader.Ready) {
                        loadCompleted(true)
                    } else if (levelLoader.status == Loader.Error) {
                        loadCompleted(false)
                    }
                }
            }
        }

        InputArea {
            id: inputArea1
//            anchors.rightMargin: 0
//            anchors.bottomMargin: 0
//            anchors.leftMargin: 0
//            anchors.topMargin: 0
//            visible: true
            anchors.fill: parent
            objectName: "inputArea"
        }

        Item {
            id: root
            property string colorKey

            width: 64; height: 64

//            MouseArea {
//                id: mouseArea

//                width: 64; height: 64
//                anchors.centerIn: parent

//                drag.target: tile

//                onReleased: parent = tile.Drag.target !== null ? tile.Drag.target : root

//                Rectangle {
//                    id: tile

//                    width: 64; height: 64
//                    anchors.verticalCenter: parent.verticalCenter
//                    anchors.horizontalCenter: parent.horizontalCenter

//                    color: colorKey

//                    Drag.keys: [ colorKey ]
//                    Drag.active: mouseArea.drag.active
//                    Drag.hotSpot.x: 32
//                    Drag.hotSpot.y: 32
//                    states: State {
//                        when: mouseArea.drag.active
//                        ParentChange { target: tile; parent: root }
//                        AnchorChanges { target: tile; anchors.verticalCenter: undefined; anchors.horizontalCenter: undefined }
//                    }

//                }
//            }
            Rectangle {
                              id: tile

                              width: 106; height: 58
                              anchors.verticalCenterOffset: 51
                              anchors.horizontalCenterOffset: 254
                              anchors.verticalCenter: parent.verticalCenter
                              anchors.horizontalCenter: parent.horizontalCenter}

//            SizeMoveHandle{
//                id:handle
//            }
        }

    }

    Rectangle {
        id: propertiesPanel
        x: 611
        width: 189
        color: "#7e7c7c"
        anchors.top: headPanel.bottom
        anchors.topMargin: 0
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.bottom: footPanel.top
        anchors.bottomMargin: 0



        ColumnLayout {
            id: columnLayout1
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            spacing: 1

            ColapsableWidget {
                id: colapsableWidget1
                height: 130
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0
            }

            ColapsableWidget {
                id: colapsableWidget2
                height: 130
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0

            }

//            TabView{
//                id:propertiesTabView
//                width: 200
//                anchors.right: parent.right
//                anchors.bottom: parent.bottom
//                anchors.top: parent.top
//                anchors.topMargin: 217
//                anchors.margins: 4
//                Tab { title: "Tab 1" }
//                Tab { title: "Tab 2" }
//                Tab { anchors.left: parent.left; anchors.top: parent.top; anchors.bottom: parent.bottom; title: "Tab 3" }

//                style: TabViewStyle {
//                    frameOverlap: 1
//                    tab: Rectangle {
//                        color: styleData.selected ? "gray" :"darkgray"
//                        border.color:  "black"
//                        implicitWidth: Math.max(text.width + 3.3, 80)
//                        implicitHeight: 20
//                        radius: 2
//                        Text {
//                            id: text
//                            anchors.centerIn: parent
//                            text: styleData.title
//                            font.pointSize: 10
//                            color: styleData.selected ? "white" : "black"
//                        }
//                    }
//                    frame: Rectangle { color: "lightgray" }
//                }
//                SizeMoveHandle{
//                    id:handle2
//                }

//            }
        }
    }


    Rectangle {
        id: toolBar
        width: 189
        color: "#7e7c7c"
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.top: headPanel.bottom
        anchors.topMargin: 0
        anchors.bottom: footPanel.top
        anchors.bottomMargin: 0

        PinnedButton{
            height: 10
            anchors.rightMargin: -12

        }



        ColumnLayout {
            id: columnLayout2
            anchors.right: parent.right
            anchors.rightMargin: 0
            anchors.left: parent.left
            anchors.leftMargin: 0
            spacing: 1

            ColapsableWidget {
                id: colapsableWidget3
                height: 130
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0

                GridView {
                    id: gridView1
                    parent:colapsableWidget3.content
                    anchors.fill: parent
                    delegate: Item {
                        x: 5
                        height: 50
                        Column {
                            Rectangle {
                                width: 40
                                height: 40
                                color: colorCode
                                anchors.horizontalCenter: parent.horizontalCenter
                            }

                            Text {
                                x: 5
                                text: name
                                font.bold: true
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                            spacing: 5
                        }
                    }
                    model: ListModel {
                        ListElement {
                            name: "Grey"
                            colorCode: "grey"
                        }

                        ListElement {
                            name: "Red"
                            colorCode: "red"
                        }

                        ListElement {
                            name: "Blue"
                            colorCode: "blue"
                        }

                        ListElement {
                            name: "Green"
                            colorCode: "green"
                        }
                    }
                    cellHeight: 70
                    cellWidth: 70
                }
            }

            ColapsableWidget {
                id: colapsableWidget4
                height: 130
                anchors.right: parent.right
                anchors.rightMargin: 0
                anchors.left: parent.left
                anchors.leftMargin: 0


                Rectangle{
                    parent:colapsableWidget4.content
                    anchors.fill: parent
                    color:"pink"
                }
            }

//            TabView{
//                id:propertiesTabView
//                width: 200
//                anchors.right: parent.right
//                anchors.bottom: parent.bottom
//                anchors.top: parent.top
//                anchors.topMargin: 217
//                anchors.margins: 4
//                Tab { title: "Tab 1" }
//                Tab { title: "Tab 2" }
            //                Tab { anchors.left: parent.left; anchors.top: parent.top; anchors.bottom: parent.bottom; title: "Tab 3" }

            //                style: TabViewStyle {
            //                    frameOverlap: 1
            //                    tab: Rectangle {
            //                        color: styleData.selected ? "gray" :"darkgray"
            //                        border.color:  "black"
            //                        implicitWidth: Math.max(text.width + 3.3, 80)
            //                        implicitHeight: 20
            //                        radius: 2
            //                        Text {
            //                            id: text
            //                            anchors.centerIn: parent
            //                            text: styleData.title
            //                            font.pointSize: 10
            //                            color: styleData.selected ? "white" : "black"
            //                        }
            //                    }
            //                    frame: Rectangle { color: "lightgray" }
            //                }
            //                SizeMoveHandle{
            //                    id:handle2
            //                }

            //            }
        }
    }



    Item {
        id: footPanel
        y: 378
        height: 10
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
    }

    Item {
        id: headPanel
        height: 20
        anchors.top: menuWrapper.bottom
        anchors.topMargin: 2
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.left: parent.left
        anchors.leftMargin: 0

        Item {
            id: actionPanel
            height: 20

            RowLayout {
                id: rowLayout1
                width: 188
                height: 20
                anchors.left: parent.left
                anchors.leftMargin: 0
            }
        }
    }

}


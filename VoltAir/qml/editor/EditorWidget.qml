import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.1
import VoltAir 1.0

Rectangle
{
    id:mainWindow
    width: 680
    height: 540
    color: "#00000000"

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
        anchors.rightMargin: 0
        anchors.bottomMargin: 0
           anchors.leftMargin: 0
           anchors.topMargin: 0
           anchors.fill: parent

           TopMenuBar {
             id: myTopMenu
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

    Rectangle {
        id: rootItem
        objectName:"rootItem"
        color: "transparent"
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
            objectName: "inputArea"
            anchors.fill: parent
        }
    }

    Rectangle {
        id: propertiesPanel
        x: 598
        width: 120
        color: "#00000000"
        anchors.top: parent.top
        anchors.topMargin: 20
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 21
        anchors.right: parent.right
        anchors.rightMargin: 0
    }

    Rectangle {
        id: toolBar
        x: 8
        width: 127
        color: "#00000000"
        anchors.top: parent.top
        anchors.topMargin: 15
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 14
        anchors.left: parent.left
        anchors.leftMargin: 0

        GridView {
            id: gridView1
            x: 8
            y: 139
            width: 83
            height: 289
            cellWidth: 70
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
                        anchors.horizontalCenter: parent.horizontalCenter
                        font.bold: true
                    }
                    spacing: 5
                }
            }
            cellHeight: 70
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
        }

        ColapsableList{}
    }
}


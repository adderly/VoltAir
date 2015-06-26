import QtQuick 2.2
import QtQuick.Controls 1.2

MenuBar {
    id:menuBar

    Menu{
        title:"File"
        MenuItem{
            text: "New Level"
            onTriggered: {
                editor.newLevel(qsTr(""))
            }
        }

        MenuItem{
            text: "Open"
        }
        MenuItem{
            text: "Exit"
            onTriggered: {
                 editor.onMenuExit()
            }
        }
    }

    Menu{
        title:"View"
        MenuItem{
            text:"View Option"
        }
    }
    Menu{
        title:"About"
        MenuItem{
            text:"About this awesome thing"
        }
    }
}


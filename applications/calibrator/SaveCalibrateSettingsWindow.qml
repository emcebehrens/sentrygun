import QtQuick 2.6
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

Item {
    id: dialogComponent
    anchors.fill: parent    

    property string settingsName

    Component.onCompleted: {        
        console.log("PrintPDFNotificationWindow onCompleted")
        //glue.sig_printToPDFDone.connect(dialogComponent.slot_printToPDFDone)
    }

    Component.onDestruction: {
        console.log("PrintPDFNotificationWindow onDestruction")
        //glue.sig_printToPDFDone.disconnect(dialogComponent.slot_printToPDFDone)
    }

    // Add a simple animation to fade in the popup
    // let the opacity go from 0 to 1 in 400ms
    PropertyAnimation { target: dialogComponent; property: "opacity";
                                  duration: 400; from: 0; to: 1;
                                  easing.type: Easing.InOutQuad ; running: true }

    // This rectange is the a overlay to partially show the parent through it
    // and clicking outside of the 'dialog' popup will do 'nothing'
    Rectangle {
        anchors.fill: parent
        id: overlay
        color: "#000000"
        opacity: 0.8
        // add a mouse area so that clicks outside
        // the dialog window will not do anything
        MouseArea {
            anchors.fill: parent
        }
    }

    // This rectangle is the actual popup
    Rectangle {
        id: dialogWindow
        width: 500
        height: 150
        radius: 4
        anchors.centerIn: parent

        Column {
            anchors.centerIn: parent
            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Override settings for " + settingsName + " ?"
                font.pointSize: 10
            }
            Rectangle {
                color: "transparent"
                width: 75
                height: 30
            }
            Row {
                anchors.horizontalCenter: parent.horizontalCenter
                Button {
                    text: "Cancel"
                    style: ButtonStyle {
                        background: Rectangle {
                            implicitWidth: 50
                            implicitHeight: 25
                            border.width: control.activeFocus ? 2 : 1
                            border.color: "#888"
                            gradient: Gradient {
                                GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                                GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                            }
                        }
                    }
                    onClicked: {
                        dialogComponent.destroy()
                    }
                }
                Rectangle {
                    color: "transparent"
                    width: 75
                    height: 30
                }
                Button {
                    text: "Yes, save it "
                    style: ButtonStyle {
                        background: Rectangle {
                            implicitWidth: 75
                            implicitHeight: 25
                            border.width: control.activeFocus ? 2 : 1
                            border.color: "#888"
                            gradient: Gradient {
                                GradientStop { position: 0 ; color: control.pressed ? "#ccc" : "#eee" }
                                GradientStop { position: 1 ; color: control.pressed ? "#aaa" : "#ccc" }
                            }
                        }
                    }
                }

            }
        }
    }
}

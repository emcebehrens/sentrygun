import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import CppQmlGlue 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

Window {
    visible: true
    width: 800
    height: 400
    //color: "transparent"

    property bool controllerConnected: false
    property real sliderTiltMin: 4000
    property real sliderTiltMax: 8000
    property real sliderPanMin: 4000
    property real sliderPanMax: 8000

    property real panSpeed: 0
    property real panAcc: 0
    property real tiltSpeed: 0
    property real tiltAcc: 0

    Component.onCompleted: {
        console.log("onCompleted.. connecting signals")
        glue.sig_servoControllerState.connect(slot_servoControllerState)
        //glue.sig_havePanPosition.connect(slot_havePanPosition)
        //glue.sig_haveTiltPosition.connect(slot_haveTiltPosition)
    }

    function slot_servoControllerState(state) {
        console.log("slot_servoControllerState", state)

        if (state) {
            textControllerState.text = "Meastro Servo Controller is Connected"
            controllerConnected = true
        } else {
            textControllerState.text = "No Maestro USB Servo Controller Found"
            controllerConnected = false
        }
    }

    FontLoader {
        id: basicFont;
        source: "qrc:/weblysleekuil.ttf"
    }

    FontLoader {
        id: basicFontBold;
        source: "qrc:/weblysleekuisb.ttf"
    }

    CppQmlGlue {
        id: glue

    }

    Rectangle {
        id: recMain
        width: parent.width
        height: parent.height
        color: "transparent"

        Column {
            anchors.centerIn: parent

            Rectangle {
                id: recConnection
                height: 40
                width: recMain.width
                //color: "gray"

                Text {
                    id: textControllerState
                    anchors.centerIn: parent
                    font.family: basicFont.name
                    text: "No Maestro USB Servo Controller Found"
                    font.pointSize: 15
                }
            }

            Rectangle {
                id: recMainArea
                height: recMain.height - recConnection.height
                width: recMain.width

                Row {
                    anchors.centerIn: parent
                    spacing: 10

                    Rectangle {
                        id: recTilt
                        enabled: controllerConnected
                        width: (recMainArea.width / 2) - 10
                        height: recMainArea.height - 10
                        color: "transparent"
                        border.color: "lightgray"
                        border.width: 1
                        radius: 2
                        anchors.verticalCenter: parent.verticalCenter
                        opacity: (controllerConnected) ? 1 : 0.5

                        Rectangle {
                            id: recTiltTop
                            width: parent.width
                            height: 200
                            color: "transparent"
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.topMargin: 20
                        }

                        Rectangle {
                            id: recTiltBot
                            width: parent.width - 40
                            height: recTilt.height - recTiltTop.height - 60
                            //color: "lightgray"
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                            anchors.top: recTiltTop.bottom
                            anchors.topMargin: 20

                            Column {
                                anchors.centerIn: parent
                                spacing: 10

                                GridLayout {
                                    columns: 4
                                    rows: 2
                                    columnSpacing: 10
                                    //anchors.centerIn: parent
                                    anchors.horizontalCenter: parent.horizontalCenter

                                    Text {
                                        text: "Speed"
                                        font.family: basicFont.name
                                        font.pointSize: 10
                                    }
                                    SpinBox {
                                        id: spinSpeed
                                        value: tiltSpeed
                                        maximumValue: 400
                                        minimumValue: 0
                                    }

                                    Text {
                                        text: "Min Target"
                                        font.family: basicFont.name
                                        font.pointSize: 10
                                    }
                                    SpinBox {
                                        id: spinMinTarget
                                        value: sliderTiltMin
                                        maximumValue: 9000
                                        minimumValue: 0
                                    }

                                    Text {
                                        text: "Acceleration"
                                        font.family: basicFont.name
                                        font.pointSize: 10
                                    }
                                    SpinBox {
                                        id: spinAcc
                                        value: tiltAcc
                                        maximumValue: 400
                                        minimumValue: 0
                                    }

                                    Text {
                                        text: "Max Target"
                                        font.family: basicFont.name
                                        font.pointSize: 10
                                    }
                                    SpinBox {
                                        id: spinMaxTarget
                                        value: sliderTiltMax
                                        maximumValue: 9000
                                        minimumValue: 0
                                    }

                                }// GridLayout

                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Apply"
                                    font.family: basicFont.name
                                    font.pointSize: 15
                                    color: "#07913A"

                                    MouseArea{
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            console.log("Apply Tilt Settings")

                                            sliderTiltMin = spinMinTarget.value
                                            sliderTiltMax = spinMaxTarget.value

                                            console.log("tiltSpeed", tiltSpeed, spinSpeed.value)
                                            if (spinSpeed.value != tiltSpeed) {
                                                tiltSpeed = spinSpeed.value
                                                glue.setTiltSpeed(tiltSpeed)
                                            }

                                            console.log("tiltAcc", tiltAcc, spinAcc.value)
                                            if (spinAcc.value != tiltAcc) {
                                                tiltAcc = spinAcc.value
                                                glue.setTiltAcceleration(tiltAcc)
                                            }
                                        }
                                    }
                                }

                            }
                        }
                    }

                    Rectangle {
                        id: recPan
                        enabled: controllerConnected
                        width: (recMainArea.width / 2) - 10
                        height: recMainArea.height - 10
                        color: "transparent"
                        border.color: "lightgray"
                        border.width: 1
                        radius: 2
                        anchors.verticalCenter: parent.verticalCenter
                        opacity: (controllerConnected) ? 1 : 0.5

                        Rectangle {
                            id: recPanLeft
                            width: 200
                            height: parent.height
                            color: "transparent"
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                            anchors.top: parent.top
                            anchors.topMargin: 20

                        }

                        Rectangle {
                            id: recPanRight
                            width: recPan.width - recPanLeft.width - 60
                            height: parent.height - 40
                            //color: "lightgray"
                            anchors.left: recPanLeft.right
                            anchors.leftMargin: 20
                            anchors.top: parent.top
                            anchors.topMargin: 20

                            Column {
                                anchors.centerIn: parent
                                spacing: 10

                                GridLayout {
                                    columns: 2
                                    rows: 4
                                    columnSpacing: 10
                                    anchors.horizontalCenter: parent.horizontalCenter

                                    Text {
                                        text: "Speed"
                                        font.family: basicFont.name
                                        font.pointSize: 10
                                    }
                                    SpinBox {
                                        id: spinPanSpeed
                                        value: panSpeed
                                        maximumValue: 400
                                        minimumValue: 0
                                    }

                                    Text {
                                        text: "Acceleration"
                                        font.family: basicFont.name
                                        font.pointSize: 10
                                    }
                                    SpinBox {
                                        id: spinPanAcc
                                        value: panAcc
                                        maximumValue: 400
                                        minimumValue: 0
                                    }

                                    Text {
                                        text: "Min Target"
                                        font.family: basicFont.name
                                        font.pointSize: 10
                                    }
                                    SpinBox {
                                        id: spinPanMinTarget
                                        value: sliderPanMin
                                        maximumValue: 9000
                                        minimumValue: 0
                                    }

                                    Text {
                                        text: "Max Target"
                                        font.family: basicFont.name
                                        font.pointSize: 10
                                    }
                                    SpinBox {
                                        id: spinPanMaxTarget
                                        value: sliderPanMax
                                        maximumValue: 9000
                                        minimumValue: 0
                                    }

                                }

                                Text {
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    text: "Apply"
                                    font.family: basicFont.name
                                    font.pointSize: 15
                                    color: "#0E6EB8"

                                    MouseArea{
                                        anchors.fill: parent
                                        hoverEnabled: true
                                        cursorShape: Qt.PointingHandCursor
                                        onClicked: {
                                            console.log("Apply Pan Settings")

                                            sliderPanMax = spinPanMaxTarget.value
                                            sliderPanMin = spinPanMinTarget.value

                                            console.log("panSpeed", panSpeed, spinPanSpeed.value)
                                            if (spinPanSpeed.value != panSpeed) {
                                                panSpeed = spinPanSpeed.value
                                                glue.setPanSpeed(panSpeed)
                                            }

                                            console.log("panAcc", panAcc, spinPanAcc.value)
                                            if (spinPanAcc.value != panAcc) {
                                                panAcc = spinPanAcc.value
                                                glue.setPanAcceleration(panAcc)
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

    }



}

import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QmlCppGlue 1.0

Window {
    visible: true

    width: 600
    height: 600

    QmlCppGlue {
        id: glue
    }

    Rectangle {
        anchors.fill: parent

        Column {
            spacing: 10

            Button {
                text: "getPosition"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    console.log("setTarget")
                    glue.getPosition();
                }
            }

            Button {
                text: "goHome"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    console.log("setTarget")
                    glue.goHome();
                }
            }

            Button {
                text: "demoPan"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    console.log("demoPan")
                    glue.doDemoPan();
                }
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                color: "transparent"
                height: 40
                width: 400

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    Button {
                        text: "setTarget"
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            console.log("setTarget", inputSetTarget.text)
                            glue.setTarget(parseInt(inputSetTarget.text))
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        height: 30
                        width: 20
                    }

                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        color: "lightgray"
                        height: 30
                        width: 40

                        TextInput {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            id: inputSetTarget
                            height: parent.height
                            width: parent.width

                        }
                    }
                }
            }

            Rectangle {
                anchors.horizontalCenter: parent.horizontalCenter
                color: "transparent"
                height: 40
                width: 400

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    Button {
                        text: "setSpeed"
                        anchors.verticalCenter: parent.verticalCenter
                        onClicked: {
                            console.log("setSpeed", inputSetSpeed.text)
                            glue.setSpeed(parseInt(inputSetSpeed.text))
                        }
                    }
                    Rectangle {
                        color: "transparent"
                        height: 30
                        width: 20
                    }

                    Rectangle {
                        anchors.verticalCenter: parent.verticalCenter
                        color: "lightgray"
                        height: 30
                        width: 40

                        TextInput {
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.verticalCenter: parent.verticalCenter
                            id: inputSetSpeed
                            height: parent.height
                            width: parent.width

                        }
                    }
                }
            }

            Button {
                text: "setAcceleration"
                anchors.horizontalCenter: parent.horizontalCenter
                onClicked: {
                    console.log("setAcceleration")
                }
            }
        }
    }

    /*MainForm {
        anchors.fill: parent
        mouseArea.onClicked: {
            Qt.quit();
        }
    }*/
}

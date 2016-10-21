import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QmlCppGlue 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3

Window {    
    visible: true
    width: 800
    height: 400
    //color: "transparent"

    Component.onCompleted: {
        console.log("onCompleted.. connecting signals")
        glue.sig_servoControllerState.connect(slot_servoControllerState)
    }

    function slot_servoControllerState(state) {
        console.log("slot_servoControllerState", state)

        if (state) {
            textControllerState.text = "Meastro Servo Controller is Connected"
        } else {
            textControllerState.text = "No Maestro USB Servo Controller Found"
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

    QmlCppGlue {
        id: glue

        tiltSliderValue: sliderTilt.value
        onSig_tiltSliderValueChanged: {
            //console.log(tiltSliderValue)
            glue.setTiltTarget(tiltSliderValue)
        }

        panSliderValue: sliderPan.value
        onSig_panSliderValueChanged: {
            //console.log(panSliderValue)
            glue.setPanTarget(panSliderValue)
        }
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
                        width: (recMainArea.width / 2) - 10
                        height: recMainArea.height - 10
                        color: "transparent"
                        border.color: "lightgray"
                        border.width: 1
                        radius: 2
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            id: recTiltTop
                            width: parent.width
                            height: 200
                            color: "transparent"
                            anchors.left: parent.left
                            anchors.top: parent.top
                            anchors.topMargin: 20

                            Image {
                                id: imageTilt
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                anchors.verticalCenter: parent.verticalCenter
                                source: "tilt.png"
                                fillMode: Image.PreserveAspectFit
                                sourceSize.width: recTilt.width - 10
                                sourceSize.height: recTilt.width - 10
                            }

                            Rectangle {
                                width: parent.width - imageTilt.width
                                height: parent.height
                                anchors.left: imageTilt.right
                                color: "transparent"

                                Row {
                                    anchors.centerIn: parent
                                    spacing: 20

                                    Slider {
                                        id: sliderTilt
                                        orientation: Qt.Vertical
                                        height: 200
                                        value: 6000
                                        maximumValue: 8000
                                        minimumValue: 4000

                                        style: SliderStyle {
                                                groove: Rectangle {
                                                    color: "#07913A"
                                                    implicitWidth: 200
                                                    implicitHeight: 8
                                                    radius: 8
                                                }
                                        }
                                    }

                                    Rectangle {
                                        height: 200
                                        width: 40
                                        //color: "lightgray"

                                        Column {
                                            anchors.centerIn: parent
                                            spacing: 10

                                            Rectangle {
                                                id: recTiltRL
                                                height: 20
                                                width:20

                                                Image {
                                                    anchors.horizontalCenter: parent.horizontalCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    source: "last.png"
                                                    fillMode: Image.PreserveAspectFit
                                                    sourceSize.width: parent.width
                                                    sourceSize.height: parent.height
                                                }

                                                transform: Rotation {
                                                    // Transform origin is the middle point of the lower border
                                                    origin {
                                                        x: recTiltRL.width /2
                                                        y: recTiltRL.height /2
                                                    }
                                                    axis {x: 0; y: 0; z: 1}
                                                    angle: 270
                                                }

                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        console.log("Tilt Reverse Last")
                                                    }
                                                }
                                            }

                                            Rectangle {
                                                id: recTiltRR
                                                height: 20
                                                width:20

                                                Image {
                                                    anchors.horizontalCenter: parent.horizontalCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    source: "forward.png"
                                                    fillMode: Image.PreserveAspectFit
                                                    sourceSize.width: parent.width
                                                    sourceSize.height: parent.height
                                                }

                                                transform: Rotation {
                                                    // Transform origin is the middle point of the lower border
                                                    origin {
                                                        x: recTiltRR.width /2
                                                        y: recTiltRR.height /2
                                                    }
                                                    axis {x: 0; y: 0; z: 1}
                                                    angle: 270
                                                }

                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        console.log("Tilt Reverse")
                                                    }
                                                }
                                            }

                                            Rectangle {
                                                height: 20
                                                width:20

                                                Image {
                                                    anchors.horizontalCenter: parent.horizontalCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    source: "home.png"
                                                    fillMode: Image.PreserveAspectFit
                                                    sourceSize.width: parent.width
                                                    sourceSize.height: parent.height
                                                }

                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        console.log("Tilt Home")
                                                    }
                                                }
                                            }

                                            Rectangle {
                                                id: recTiltFF
                                                height: 20
                                                width:20

                                                Image {
                                                    anchors.horizontalCenter: parent.horizontalCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    source: "forward.png"
                                                    fillMode: Image.PreserveAspectFit
                                                    sourceSize.width: parent.width
                                                    sourceSize.height: parent.height
                                                }

                                                transform: Rotation {
                                                    // Transform origin is the middle point of the lower border
                                                    origin {
                                                        x: recTiltFF.width /2
                                                        y: recTiltFF.height /2
                                                    }
                                                    axis {x: 0; y: 0; z: 1}
                                                    angle: 90
                                                }

                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        console.log("Tilt Forward")
                                                    }
                                                }
                                            }

                                            Rectangle {
                                                id: recTiltFL
                                                height: 20
                                                width:20

                                                Image {
                                                    anchors.horizontalCenter: parent.horizontalCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    source: "last.png"
                                                    fillMode: Image.PreserveAspectFit
                                                    sourceSize.width: parent.width
                                                    sourceSize.height: parent.height
                                                }

                                                transform: Rotation {
                                                    // Transform origin is the middle point of the lower border
                                                    origin {
                                                        x: recTiltFL.width /2
                                                        y: recTiltFL.height /2
                                                    }
                                                    axis {x: 0; y: 0; z: 1}
                                                    angle: 90
                                                }

                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        console.log("Tilt Forward Last")
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
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

                            GridLayout {
                                columns: 4
                                rows: 2
                                columnSpacing: 10
                                anchors.centerIn: parent

                                Text {
                                    text: "Speed"
                                    font.family: basicFont.name
                                    font.pointSize: 10
                                }
                                SpinBox {
                                    id: spinSpeed
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
                                    value: 4000
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
                                    value: 8000
                                    maximumValue: 9000
                                    minimumValue: 0
                                }

                            }
                        }
                    }

                    Rectangle {
                        id: recPan
                        width: (recMainArea.width / 2) - 10
                        height: recMainArea.height - 10
                        color: "transparent"
                        border.color: "lightgray"
                        border.width: 1
                        radius: 2
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            id: recPanLeft
                            width: 200
                            height: parent.height
                            color: "transparent"
                            anchors.left: parent.left
                            anchors.leftMargin: 20
                            anchors.top: parent.top
                            anchors.topMargin: 20

                            Image {
                                id: imagePan
                                //anchors.left: parent.left
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                source: "pan.png"
                                fillMode: Image.PreserveAspectFit
                                sourceSize.width: recPan.width - 10
                                sourceSize.height: recPan.width - 10
                            }

                            Rectangle {
                                width: parent.width
                                height: parent.height - imagePan.height
                                anchors.top: imagePan.bottom
                                color: "transparent"

                                Column {
                                    anchors.centerIn: parent
                                    spacing: 20

                                    Slider {
                                        id: sliderPan
                                        width: 200
                                        value: 6000
                                        maximumValue: 8000
                                        minimumValue: 4000

                                        style: SliderStyle {
                                                groove: Rectangle {
                                                    color: "#0E6EB8"
                                                    implicitWidth: 200
                                                    implicitHeight: 8
                                                    radius: 8
                                                }
                                        }
                                    }

                                    Rectangle {
                                        height: 40
                                        width: recPanLeft.width
                                        //color: "lightgray"

                                        Row {
                                            anchors.centerIn: parent
                                            spacing: 10

                                            Rectangle {
                                                id: recPanRL
                                                height: 20
                                                width:20

                                                Image {
                                                    anchors.horizontalCenter: parent.horizontalCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    source: "last.png"
                                                    fillMode: Image.PreserveAspectFit
                                                    sourceSize.width: parent.width
                                                    sourceSize.height: parent.height
                                                }

                                                transform: Rotation {
                                                    // Transform origin is the middle point of the lower border
                                                    origin {
                                                        x: recPanRL.width /2
                                                        y: recPanRL.height /2
                                                    }
                                                    axis {x: 0; y: 0; z: 1}
                                                    angle: 180
                                                }

                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        console.log("Pan Reverse Last")
                                                    }
                                                }
                                            }

                                            Rectangle {
                                                id: recPanRR
                                                height: 20
                                                width:20

                                                Image {
                                                    anchors.horizontalCenter: parent.horizontalCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    source: "forward.png"
                                                    fillMode: Image.PreserveAspectFit
                                                    sourceSize.width: parent.width
                                                    sourceSize.height: parent.height
                                                }

                                                transform: Rotation {
                                                    // Transform origin is the middle point of the lower border
                                                    origin {
                                                        x: recPanRR.width /2
                                                        y: recPanRR.height /2
                                                    }
                                                    axis {x: 0; y: 0; z: 1}
                                                    angle: 180
                                                }

                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        console.log("Pan Reverse")
                                                    }
                                                }
                                            }

                                            Rectangle {
                                                height: 20
                                                width:20

                                                Image {
                                                    anchors.horizontalCenter: parent.horizontalCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    source: "home.png"
                                                    fillMode: Image.PreserveAspectFit
                                                    sourceSize.width: parent.width
                                                    sourceSize.height: parent.height
                                                }

                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        console.log("Pan Home")
                                                    }
                                                }
                                            }

                                            Rectangle {
                                                height: 20
                                                width:20

                                                Image {
                                                    anchors.horizontalCenter: parent.horizontalCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    source: "forward.png"
                                                    fillMode: Image.PreserveAspectFit
                                                    sourceSize.width: parent.width
                                                    sourceSize.height: parent.height
                                                }

                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        console.log("Pan Forward")
                                                    }
                                                }
                                            }

                                            Rectangle {
                                                height: 20
                                                width:20

                                                Image {
                                                    anchors.horizontalCenter: parent.horizontalCenter
                                                    anchors.verticalCenter: parent.verticalCenter
                                                    source: "last.png"
                                                    fillMode: Image.PreserveAspectFit
                                                    sourceSize.width: parent.width
                                                    sourceSize.height: parent.height
                                                }

                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        console.log("Pan Forward Last")
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
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

                            GridLayout {
                                columns: 2
                                rows: 4
                                columnSpacing: 10
                                anchors.centerIn: parent

                                Text {
                                    text: "Speed"
                                    font.family: basicFont.name
                                    font.pointSize: 10
                                }
                                SpinBox {
                                    id: spinPanSpeed
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
                                    value: 4000
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
                                    value: 8000
                                    maximumValue: 9000
                                    minimumValue: 0
                                }

                            }
                        }
                    }
                }
            }
        }

    }



}

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
        glue.sig_havePanPosition.connect(slot_havePanPosition)
        glue.sig_haveTiltPosition.connect(slot_haveTiltPosition)
    }

    function slot_servoControllerState(state) {
        console.log("slot_servoControllerState", state)

        if (state) {
            textControllerState.text = "Maestro Servo Controller is Connected"
            controllerConnected = true
        } else {
            textControllerState.text = "No Maestro USB Servo Controller Found"
            controllerConnected = false
        }
    }

    function slot_havePanPosition(pos) {
        console.log("slot_havePanPosition", pos)
        sliderPanActualPos.value = pos
    }

    function slot_haveTiltPosition(pos) {
        console.log("slot_haveTiltPosition", pos)
        sliderTiltActualPos.value = pos
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

                            Image {
                                id: imageTilt
                                anchors.left: parent.left
                                anchors.leftMargin: 20
                                anchors.verticalCenter: parent.verticalCenter
                                source: "tilt.png"
                                //fillMode: Image.PreserveAspectFit
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

                                    Rectangle {
                                        height: 200
                                        width: 60
                                        //color: "lightgray"

                                        Row {
                                            anchors.centerIn: parent
                                            spacing: 10

                                            Slider {
                                                id: sliderTiltActualPos
                                                orientation: Qt.Vertical
                                                height: 200
                                                value: 6000
                                                maximumValue: sliderTiltMax
                                                minimumValue: sliderTiltMin

                                                style: SliderStyle {
                                                    groove: Rectangle {
                                                        color: "lightgray"
                                                        implicitWidth: 200
                                                        implicitHeight: 1
                                                    }
                                                    handle: Rectangle {
                                                        anchors.centerIn: parent
                                                        color: "#07913A"
                                                        implicitWidth: 2
                                                        implicitHeight: 20
                                                    }
                                                }
                                            }

                                            Slider {
                                                id: sliderTilt
                                                orientation: Qt.Vertical
                                                height: 200
                                                value: 6000
                                                maximumValue: sliderTiltMax
                                                minimumValue: sliderTiltMin

                                                style: SliderStyle {
                                                        groove: Rectangle {
                                                            color: "#07913A"
                                                            implicitWidth: 200
                                                            implicitHeight: 8
                                                            radius: 8
                                                        }
                                                }
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
                                                    //fillMode: Image.PreserveAspectFit
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
                                                        sliderTilt.value = sliderTiltMax
                                                        glue.setTiltTarget(sliderTilt.value)
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
                                                    //fillMode: Image.PreserveAspectFit
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
                                                        sliderTilt.value = sliderTilt.value + 5
                                                        glue.setTiltTarget(sliderTilt.value)
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
                                                    //fillMode: Image.PreserveAspectFit
                                                    sourceSize.width: parent.width
                                                    sourceSize.height: parent.height
                                                }

                                                MouseArea{
                                                    anchors.fill: parent
                                                    hoverEnabled: true
                                                    cursorShape: Qt.PointingHandCursor
                                                    onClicked: {
                                                        console.log("Tilt Home")
                                                        glue.setTiltTarget((sliderTiltMin + sliderTiltMax)/2)
                                                        sliderTilt.value = (sliderTiltMin + sliderTiltMax)/2
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
                                                        sliderTilt.value = sliderTilt.value - 5
                                                        glue.setTiltTarget(sliderTilt.value)
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
                                                        sliderTilt.value = sliderTiltMin
                                                        glue.setTiltTarget(sliderTilt.value)
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

                            Image {
                                id: imagePan
                                //anchors.left: parent.left
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.top: parent.top
                                source: "pan.png"
                                //fillMode: Image.PreserveAspectFit
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

                                    Rectangle {
                                        height: 60
                                        width: recPanLeft.width
                                       // color: "lightgray"

                                        Column {
                                            anchors.centerIn: parent
                                            spacing: 10

                                            Slider {
                                                id: sliderPanActualPos
                                                width: 200
                                                value: 6000
                                                maximumValue: sliderPanMax
                                                minimumValue: sliderPanMin

                                                style: SliderStyle {
                                                    groove: Rectangle {
                                                        color: "lightgray"
                                                        implicitWidth: 200
                                                        implicitHeight: 1
                                                    }
                                                    handle: Rectangle {
                                                        anchors.centerIn: parent
                                                        color: "#0E6EB8"
                                                        implicitWidth: 2
                                                        implicitHeight: 20
                                                    }
                                                }
                                            }

                                            Slider {
                                                id: sliderPan
                                                width: 200
                                                value: 6000
                                                maximumValue: sliderPanMax
                                                minimumValue: sliderPanMin

                                                style: SliderStyle {
                                                        groove: Rectangle {
                                                            color: "#0E6EB8"
                                                            implicitWidth: 200
                                                            implicitHeight: 8
                                                            radius: 8
                                                        }
                                                }
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
                                                        sliderPan.value = sliderPanMin
                                                        glue.setPanTarget(sliderPan.value)
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
                                                        sliderPan.value = sliderPan.value - 5
                                                        glue.setPanTarget(sliderPan.value)
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
                                                        glue.setPanTarget((sliderPanMin + sliderPanMax)/2)
                                                        sliderPan.value = (sliderPanMin + sliderPanMax)/2
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
                                                        sliderPan.value = sliderPan.value + 5
                                                        glue.setPanTarget(sliderPan.value)
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
                                                        sliderPan.value = sliderPanMax
                                                        glue.setPanTarget(sliderPan.value)
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

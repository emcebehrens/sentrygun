import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import CppQmlGlue 1.0
import QtQuick.Layouts 1.3

Window {
    id: mainwindow
    visible: true
    width: 640
    height: 200
    maximumHeight: height
    maximumWidth: width
    minimumHeight: height
    minimumWidth: width

    property bool controllerConnected: false
    property real sliderTiltMin: 4000
    property real sliderTiltMax: 8000
    property real sliderPanMin: 4000
    property real sliderPanMax: 8000

    Component.onCompleted: {
        console.log("onCompleted.. connecting signals")
        glue.sig_servoControllerState.connect(slot_servoControllerState)
        glue.sig_havePanPosition.connect(slot_havePanPosition)
        glue.sig_haveTiltPosition.connect(slot_haveTiltPosition)


        spinMaxTarget.value = glue.maxServoTilt;
        spinMinTarget.value = glue.minServoTilt;
        sliderTilt.value = ((spinMaxTarget.value - spinMinTarget.value) / 2) + spinMinTarget.value

        spinPanMinTarget.value = glue.minServoPan;
        spinPanMaxTarget.value = glue.maxServoPan;
        sliderPan.value = ((spinPanMaxTarget.value - spinPanMinTarget.value) / 2) + spinPanMinTarget.value
        //console.log("sliderTilt.value", sliderTilt.value, spinMaxTarget.value, spinMinTarget.value)

        textfieldDistX.text = glue.getCaldistanceX()
        textfieldDistY.text = glue.getCaldistanceY()
        textfieldDepth.text = glue.getDepthDistance()
    }

    function slot_servoControllerState(state) {
        console.log("slot_servoControllerState", state)

        if (state) {
            textControllerState.text = "Maestro Connected"
            textControllerState.color = "black"
            controllerConnected = true
        } else {
            textControllerState.text = "Not Connected"
            controllerConnected = false
        }
    }

    function slot_havePanPosition(pos) {
        console.log("slot_havePanPosition", pos)
        sliderPan.value = pos
    }

    function slot_haveTiltPosition(pos) {
        console.log("slot_haveTiltPosition", pos)
        sliderTilt.value = pos
    }

    CppQmlGlue {
        id: glue

        Component.onCompleted: {
            Qt.application.aboutToQuit.connect(glue.shutdown)
        }

        /*tiltSliderValue: sliderTilt.value
        onSig_tiltSliderValueChanged: {
            glue.setTiltTarget(tiltSliderValue)
        }

        panSliderValue: sliderPan.value
        onSig_panSliderValueChanged: {
            glue.setPanTarget(panSliderValue)
        }*/
    }

    Rectangle {
        id: recCheckBox
        color: "white"
        height: parent.height
        width: parent.width

        Row {
            anchors.left: parent.left
            anchors.top: parent.top

            Rectangle {
                color: "transparent"
                width: 10
                height: 200
            }

            Column {

                GridLayout {
                    columns: 4
                    rows: 4
                    columnSpacing: 10

                    Text {
                        text: "Servo Controller"
                        //font.family: basicFont.name
                        font.pointSize: 10
                    }
                    Text {
                        id: textControllerState
                        text: "Not Connected"
                        //font.family: basicFont.name
                        font.pointSize: 10
                    }

                    Text {
                        id: textTarckEnable
                        opacity: 0.5
                        text: "Tracking Enabled"
                        //font.family: basicFont.name
                        font.pointSize: 10
                    }
                    CheckBox {
                        id: trackEnCheckbox
                        enabled: false
                        checked: false
                        onClicked: {
                            console.log("Tracking toggled")
                            if (calibrateCheckbox.checked) {
                                recSetStepsizeX.txtC = "black"
                                recSetStepsizeX.enabled = false
                                recSetStepsizeX.opacity = 0.5
                                textfieldStepsizeX.enabled = false
                                mouseAreaSetStepsizeX.enabled = false
                                mouseAreaSetStepsizeX.hoverEnabled = false
                                textDistX.opacity = 0.5
                                textfieldDistX.enabled = false

                                recSetStepsizeY.txtC = "black"
                                recSetStepsizeY.enabled = false
                                recSetStepsizeY.opacity = 0.5
                                textfieldStepsizeY.readOnly = false
                                textfieldStepsizeY.enabled = false
                                mouseAreaSetStepsizeY.enabled = false
                                mouseAreaSetStepsizeY.hoverEnabled = false
                                textDistY.opacity = 0.5
                                textfieldDistY.enabled = false
                                radioLimitsdepths.checked = true



                                recTiltControls.enabled = false
                                sliderTilt.enabled = false
                                recPanControls.enabled = false
                                recCalibrateOpt.enabled = false
                                recTiltControls.opacity = 0.5
                                sliderTilt.opacity = 0.5
                                recPanControls.opacity = 0.5
                                recCalibrateOpt.opacity = 0.5
                                calibrateCheckbox.checked = false
                            }

                            glue.trackingEnabled(trackEnCheckbox.checked)
                        }
                    }

                    Text {
                        text: "Settings Name"
                        //font.family: basicFont.name
                        font.pointSize: 10
                    }
                    ComboBox {
                        id: comboSettings
                        width: 200
                        Layout.minimumWidth: 200
                        model: glue.comboBoxSettingsNamesListModel
                    }

                    /*Text {
                        text: " Save"
                        font.pointSize: 10
                    }*/

                    Button {
                        id: butSave
                        text: "Save"

                        enabled: false

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
                            console.log("Save")

                            var wnd = Qt.createComponent("qrc:/SaveCalibrateSettingsWindow.qml")
                            .createObject(mainwindow, { settingsName: comboSettings.currentText });
                        }
                    }

                    /*Rectangle {
                        color: "lightgray"
                        width: 50
                        height: 25
                        Text {
                            anchors.centerIn: parent
                            text: "Save"
                            font.pointSize: 10
                            color: "white"
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor

                            onClicked: {
                                console.log("Save")
                            }
                        }
                    }*/
                    Rectangle {
                        color: "transparent"
                        width: 10
                        height: 10
                    }

                    Text {
                        text: "Camera"
                        //font.family: basicFont.name
                        font.pointSize: 10
                    }
                    ComboBox {
                        id: deviceBox
                        width: 200
                        Layout.minimumWidth: 200
                        //anchors.left: imageSizeSetButton.right
                        model: glue.comboBoxCameraListModel
                    }

                    Text {
                        text: "Camera Enabled"
                        //font.family: basicFont.name
                        font.pointSize: 10
                    }
                    CheckBox {
                        id: camEnCheckbox
                        checked: false
                        onClicked: {
                            console.log("camEnCheckbox toggled")

                            if (!camEnCheckbox.checked) {
                                trackEnCheckbox.checked = false
                                deviceBox.enabled = true
                            } else {
                                deviceBox.enabled = false
                            }

                            trackEnCheckbox.enabled = true
                            textTarckEnable.opacity = 1
                            glue.cameraEnable(camEnCheckbox.checked, deviceBox.currentIndex )
                        }
                    }

                    Text {
                        text: "Calibrate"
                        //font.family: basicFont.name
                        font.pointSize: 10
                    }

                    Row {
                        CheckBox {
                            id: calibrateCheckbox
                            anchors.verticalCenter: parent.verticalCenter
                            checked: false
                            onClicked: {
                                console.log("calibrate toggled")
                                if (calibrateCheckbox.checked) {
                                    if (controllerConnected) {
                                        recTiltControls.enabled = true
                                        sliderTilt.enabled = true
                                        recPanControls.enabled = true
                                        recCalibrateOpt.enabled = true
                                        recTiltControls.opacity = 1
                                        sliderTilt.opacity = 1
                                        recPanControls.opacity = 1
                                        recCalibrateOpt.opacity = 1

                                        // Disable tracking also
                                        if (trackEnCheckbox.checked) {
                                            glue.trackingEnabled(false)
                                            trackEnCheckbox.checked = false
                                        }

                                        butSave.enabled = true
                                        butSaveAs.enabled = true

                                        glue.getPanTiltPositions()
                                    } else {
                                        calibrateCheckbox.checked = false
                                        textControllerState.color = "red"
                                    }
                                } else {
                                    recTiltControls.enabled = false
                                    sliderTilt.enabled = false
                                    recPanControls.enabled = false
                                    recCalibrateOpt.enabled = false
                                    recTiltControls.opacity = 0.5
                                    sliderTilt.opacity = 0.5
                                    recPanControls.opacity = 0.5
                                    recCalibrateOpt.opacity = 0.5

                                    butSave.enabled = false
                                    butSaveAs.enabled = false
                                }
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: 20
                            height: 10
                        }

                        TextField {
                            id: textfieldNewSetting
                            placeholderText: "New Settings Name"
                            width: 162
                        }

                    }

                    Button {
                        id: butSaveAs
                        text: "SaveAs"

                        enabled: false

                        style: ButtonStyle {
                            background: Rectangle {
                                implicitWidth: 55
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
                            console.log("SaveAs")
                            if (textfieldNewSetting.text != "") {
                                console.log("not empty")

                                // add new setting
                            }

                        }
                    }


                    Rectangle {
                        color: "transparent"
                        width: 10
                        height: 10
                    }
                }

                Rectangle {
                    id: recCalibrateOpt
                    enabled: false
                    opacity: 0.5
                    //color: "lightgreen"
                    color: "transparent"
                    width: 400
                    height: 61

                    Column {
                        //anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            color: "transparent"
                            width: 400
                            height: 30

                            Row {
                                anchors.verticalCenter: parent.verticalCenter
                                ExclusiveGroup { id: calibrateOptionGroup}

                                RadioButton {
                                    id: radioLimitsdepths
                                    text: "Tracking Depth"
                                    checked: true
                                    exclusiveGroup: calibrateOptionGroup
                                    onClicked: {
                                        console.log("Limits and Depth")

                                        recSetStepsizeX.txtC = "black"
                                        recSetStepsizeX.enabled = false
                                        recSetStepsizeX.opacity = 0.5

                                        //textfieldStepsizeX.readOnly = false
                                        textfieldStepsizeX.enabled = false

                                        mouseAreaSetStepsizeX.enabled = false
                                        mouseAreaSetStepsizeX.hoverEnabled = false
                                        textDistX.opacity = 0.5
                                        textfieldDistX.enabled = false

                                        recSetStepsizeY.txtC = "black"
                                        recSetStepsizeY.enabled = false
                                        recSetStepsizeY.opacity = 0.5
                                        textfieldStepsizeY.readOnly = false
                                        textfieldStepsizeY.enabled = false
                                        mouseAreaSetStepsizeY.enabled = false
                                        mouseAreaSetStepsizeY.hoverEnabled = false
                                        textDistY.opacity = 0.5
                                        textfieldDistY.enabled = false

                                        textfieldDepth.text = glue.getDepthDistance()

                                    }
                                }

                                Rectangle {
                                    color: "transparent"
                                    width: 10
                                    height: 10
                                }

                                RadioButton {
                                    id: radioStepsize
                                    text: "Work out stepsize"
                                    exclusiveGroup: calibrateOptionGroup
                                    onClicked: {
                                        console.log("Work out stepsize")

                                        recSetStepsizeX.txtC = "blue"
                                        recSetStepsizeX.enabled = true
                                        recSetStepsizeX.opacity = 1

                                        textfieldStepsizeX.readOnly = true
                                        textfieldStepsizeX.enabled = true

                                        mouseAreaSetStepsizeX.enabled = true
                                        mouseAreaSetStepsizeX.hoverEnabled = true
                                        textDistX.opacity = 1
                                        textfieldDistX.enabled = true

                                        recSetStepsizeY.txtC = "blue"
                                        recSetStepsizeY.enabled = true
                                        recSetStepsizeY.opacity = 1
                                        textfieldStepsizeY.readOnly = true
                                        textfieldStepsizeY.enabled = true
                                        mouseAreaSetStepsizeY.enabled = true
                                        mouseAreaSetStepsizeY.hoverEnabled = true
                                        textDistY.opacity = 1
                                        textfieldDistY.enabled = true

                                        textfieldDepth.text = glue.getCaldistanceZ()
                                    }
                                }
                            }
                        }

                        Rectangle {
                            //color: "lightblue"
                            color: "transparent"
                            width: 400
                            height: 30

                            Row {
                                //anchors.verticalCenter: parent.verticalCenter
                                anchors.centerIn: parent

                                Rectangle {
                                    id: recSetStepsizeX
                                    color: "transparent"
                                    opacity: 0.5
                                    enabled: false
                                    width: 65
                                    height: 30

                                    property color txtC: "black"

                                    Column {
                                        anchors.verticalCenter: parent.verticalCenter
                                        Text {
                                            text: "StepsizeX"
                                            font.pointSize: 10
                                            color: recSetStepsizeX.txtC
                                        }
                                        Text {
                                            text: "0.25us/radian"
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            font.pointSize: 6
                                            color: recSetStepsizeX.txtC
                                        }
                                    }

                                    MouseArea {
                                        id: mouseAreaSetStepsizeX
                                        anchors.fill: parent
                                        enabled: false
                                        hoverEnabled: false
                                        cursorShape: Qt.PointingHandCursor

                                        onClicked: {
                                            console.log("mouseAreaSetStepsizeX")

                                            var stps = glue.calculateStepsizeX(spinPanMaxTarget.value,
                                                                    spinPanMinTarget.value,
                                                                    parseFloat(textfieldDistX.text),
                                                                    parseFloat(textfieldDepth.text))
                                            textfieldStepsizeX.text = stps


                                        }
                                    }

                                }

                                Rectangle {
                                    color: "transparent"
                                    width: 5
                                    height: 20
                                }
                                TextField {
                                    id: textfieldStepsizeX
                                    readOnly: false
                                    enabled: false
                                    text: "400"
                                    width: 50
                                    validator: IntValidator {
                                        bottom: 0;
                                        top: 9999;
                                    }

                                    onEditingFinished: {
                                        console.log("I'm here!");
                                    }
                                }

                                Rectangle {
                                    color: "transparent"
                                    width: 25
                                    height: 20
                                }

                                Text {
                                    id: textDistX
                                    opacity: 0.5
                                    text: "Distance X"
                                    font.pointSize: 10
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                Rectangle {
                                    color: "transparent"
                                    width: 5
                                    height: 20
                                }
                                TextField {
                                    id: textfieldDistX
                                    enabled: false
                                    text: "0.00"
                                    width: 50
                                    validator: DoubleValidator {
                                        bottom: 0.00;
                                        top: 9999.99;
                                        decimals: 2;
                                        notation: DoubleValidator.StandardNotation
                                    }

                                    onEditingFinished: {
                                        console.log("I'm here!");
                                    }
                                }
                            }
                        }
                    }
                }

                Rectangle {
                    id: recPanControls
                    enabled: false
                    opacity: 0.5
                    //color: "lightgray"
                    color: "transparent"
                    width: 400
                    height: 30

                    Row {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter

                        Text {
                            text: "Max "
                            //font.family: basicFont.name
                            font.pointSize: 10
                            anchors.verticalCenter: parent.verticalCenter
                        }
                        SpinBox {
                            id: spinPanMaxTarget
                            value: 9000
                            maximumValue: 9000
                            minimumValue: 0
                            anchors.verticalCenter: parent.verticalCenter
                            width: 55
                            onEditingFinished: {
                                glue.maxServoPan = spinPanMaxTarget.value
                            }
                        }

                        Slider {
                            id: sliderPan
                            width: 200
                            value: 6000
                            anchors.verticalCenter: parent.verticalCenter
                            maximumValue: spinPanMaxTarget.value
                            minimumValue: spinPanMinTarget.value

                            rotation: 180
                            transformOrigin: Item.Center

                            onValueChanged: {
                                glue.setPanTarget(sliderPan.value)
                            }

                            style: SliderStyle {
                                    groove: Rectangle {
                                        color: (sliderPan.enabled) ? "#0E6EB8":"lightgray"
                                        implicitWidth: 200
                                        implicitHeight: 8
                                        radius: 8
                                        Text {
                                            anchors.centerIn: parent
                                            text: sliderPan.value.toFixed(0)
                                            rotation: 180
                                            transformOrigin: Item.Center
                                        }
                                    }
                            }
                        }

                        SpinBox {
                            id: spinPanMinTarget                            
                            value: 4000
                            maximumValue: 9000
                            minimumValue: 0
                            anchors.verticalCenter: parent.verticalCenter
                            width: 55
                            onEditingFinished: {
                                glue.minServoPan = spinPanMinTarget.value
                            }
                        }
                        Text {
                            text: " Min"
                            //font.family: basicFont.name
                            font.pointSize: 10
                            anchors.verticalCenter: parent.verticalCenter
                        }
                    }
                }

            }

            Rectangle {
                id: recTiltControls
                enabled: false
                opacity: 0.5
                //color: "lightgreen"
                color: "transparent"
                width: 160
                height: 200

                Column {
                    anchors.right: parent.right

                    Row {
                        anchors.right: parent.right
                        Rectangle {
                            color: "transparent"
                            width: 30
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            Text {
                                text: "Min"
                                //font.family: basicFont.name
                                font.pointSize: 10
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        SpinBox {
                            id: spinMinTarget
                            value: 4000
                            maximumValue: 9000
                            minimumValue: 0
                            width: 55
                            onEditingFinished: {
                                glue.minServoTilt = spinMinTarget.value
                            }
                        }

                    }

                    Rectangle {
                        color: "transparent"
                        width: 160
                        height: 144

                        Column {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right
                            anchors.rightMargin: 15

                            Row {
                                Rectangle {
                                    id: recSetStepsizeY
                                    opacity: 0.5
                                    enabled: false
                                    color: "transparent"
                                    width: 65
                                    height: 30

                                    property color txtC: "black"

                                    Column {
                                        anchors.verticalCenter: parent.verticalCenter
                                        Text {
                                            text: "StepsizeY"
                                            font.pointSize: 10
                                            color: recSetStepsizeY.txtC
                                        }
                                        Text {
                                            text: "0.25us/radian"
                                            anchors.horizontalCenter: parent.horizontalCenter
                                            font.pointSize: 6
                                            color: recSetStepsizeY.txtC
                                        }
                                    }

                                    MouseArea {
                                        id: mouseAreaSetStepsizeY
                                        anchors.fill: parent
                                        enabled: false
                                        hoverEnabled: false
                                        cursorShape: Qt.PointingHandCursor

                                        onClicked: {
                                            console.log("mouseAreaSetStepsizeY")
                                            var stps = glue.calculateStepsizeY(spinMaxTarget.value,
                                                                               spinMinTarget.value,
                                                                               parseFloat(textfieldDistY.text),
                                                                               parseFloat(textfieldDepth.text))
                                            textfieldStepsizeY.text = stps

                                        }
                                    }
                                }
                                Rectangle {
                                    color: "transparent"
                                    width: 5
                                    height: 20
                                }
                                TextField {
                                    id: textfieldStepsizeY
                                    readOnly: false
                                    enabled: false
                                    text: "400"
                                    width: 50
                                    validator: IntValidator {
                                        bottom: 0;
                                        top: 9999;
                                    }

                                    onEditingFinished: {
                                        console.log("I'm here!");
                                    }
                                }
                            }

                            Row {
                                Rectangle {
                                    color: "transparent"
                                    width: 65
                                    height: 30

                                    Text {
                                        id: textDistY
                                        opacity: 0.5
                                        text: "Distance Y"
                                        font.pointSize: 10
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
                                Rectangle {
                                    color: "transparent"
                                    width: 5
                                    height: 20
                                }
                                TextField {
                                    id: textfieldDistY
                                    enabled: false
                                    text: "0.00"
                                    width: 50
                                    validator: DoubleValidator {
                                        bottom: 0.00;
                                        top: 9999.99;
                                        decimals: 2;
                                        notation: DoubleValidator.StandardNotation
                                    }

                                    onEditingFinished: {
                                        console.log("I'm here!");
                                    }
                                }
                            }

                            Rectangle {
                                color: "transparent"
                                width: 65
                                height: 30
                            }

                            Row {
                                Rectangle {
                                    color: "transparent"
                                    width: 65
                                    height: 30

                                    Text {
                                        text: "Depth Z"
                                        font.pointSize: 10
                                        anchors.verticalCenter: parent.verticalCenter
                                    }
                                }
                                Rectangle {
                                    color: "transparent"
                                    width: 5
                                    height: 20
                                }
                                TextField {
                                    id: textfieldDepth
                                    text: "0.00"
                                    width: 50
                                    validator: DoubleValidator {
                                        bottom: 0.00;
                                        top: 9999.99;
                                        decimals: 2;
                                        notation: DoubleValidator.StandardNotation
                                    }

                                    onEditingFinished: {
                                        console.log("I'm here!");

                                        if (radioLimitsdepths.checked) {
                                            glue.setDepth(parseFloat(textfieldDepth.text))
                                        }
                                    }
                                }
                            }
                        }
                    }

                    Row {
                        anchors.right: parent.right
                        Rectangle {
                            color: "transparent"
                            width: 30
                            height: 20
                            anchors.verticalCenter: parent.verticalCenter
                            Text {
                                text: "Max"
                                //font.family: basicFont.name
                                font.pointSize: 10
                                anchors.verticalCenter: parent.verticalCenter
                            }
                        }
                        SpinBox {
                            id: spinMaxTarget
                            value: 8000
                            //value: glue.maxServoTilt
                            maximumValue: 9000
                            minimumValue: 0
                            width: 55
                            onEditingFinished: {
                                console.log("spinMaxTarget onEditingFinished", spinMaxTarget.value)
                                glue.maxServoTilt = spinMaxTarget.value
                            }
                        }
                    }
                }
            }

            Rectangle {                
                //color: "gray"
                color: "transparent"
                width: 30
                height: 200                
                //opacity: 0.5

                Slider {
                    enabled: false
                    opacity: 0.5
                    id: sliderTilt
                    orientation: Qt.Vertical
                    height: 180
                    //value: ((spinMaxTarget.value - spinMinTarget.value) / 2) + spinMinTarget.value
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter

                    rotation: 180
                    transformOrigin: Item.Center

                    minimumValue: spinMinTarget.value
                    maximumValue: spinMaxTarget.value
                   // maximumValue: 8000

                    onValueChanged: {
                        console.log("sliderTilt onValueChanged", sliderTilt.value);

                        glue.setTiltTarget(sliderTilt.value)
                    }

                    style: SliderStyle {
                            groove: Rectangle {
                                //color: "#07913A"
                                color: (sliderPan.enabled) ? "#07913A":"lightgray"
                                implicitWidth: 200
                                implicitHeight: 8
                                radius: 8
                                Text {
                                    anchors.centerIn: parent
                                    text: sliderTilt.value.toFixed(0)
                                }
                            }
                    }
                }
            }
        }
    }

    /*MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }*/
}

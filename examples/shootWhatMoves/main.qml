import QtQuick 2.6
import QtQuick.Window 2.2

import CppQmlGlue 1.0

Window {
    visible: true

    CppQmlGlue {
        id: glue

        Component.onCompleted: {
            console.log("glue completed")
            //glue.sig_updateAngle.connect(slot_updateAngle)
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    Text {
        text: qsTr("Shoot Everything That moves!")
        anchors.centerIn: parent
    }
}

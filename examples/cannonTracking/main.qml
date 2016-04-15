import QtQuick 2.2
import QtQuick.Controls 1.1

import CppQmlGlue 1.0

ApplicationWindow {
    visible: true
    width: 200
    height: 200
    title: qsTr(" ")

    function slot_updateAngle(an) {
        console.log("slot_updateAngle", an)

        reMainRotation.angle = an
    }

    CppQmlGlue {
        id: glue

        Component.onCompleted: {
            console.log("glue completed")
            glue.sig_updateAngle.connect(slot_updateAngle)
        }
    }


    Rectangle {

        width: parent.width
        height: parent.height

        Image {
            anchors.centerIn: parent
            id: gun
            width: 100
            height: 100
            //source: "file:///barrel.png"
            fillMode: Image.PreserveAspectFit

            transform: Rotation {
                id: reMainRotation
                origin.x: 50;
                origin.y: 50;
                axis { x: 0; y: 0; z: 1 }
                angle: 0 }
        }

        MouseArea {
            anchors.fill: parent
            onClicked: {
                reMainRotation.angle += 1
                //console.log("clicked")
            }
        }
    }

}

import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4

Window {
    visible: true
    width: 1920
    height: 810
    onBeforeRendering: Work.start();

    Rectangle{
        anchors.fill: parent

        gradient: Gradient {
            GradientStop { position: 0.0; color: "darkgrey" }
            GradientStop { position: 0.5; color: "white" }
        }

        Rectangle{
            color: "transparent"
            height: 700
            width: 700
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            transform: Rotation { origin.x: 0; origin.y: 0; axis { x: 0; y: 1; z: 0 } angle: 15}

            Image {
                id: needle
                property double angle: 180
                source: "qrc:/gauges/needle.png"
                height: 120
                anchors.verticalCenterOffset: -275
                anchors.horizontalCenterOffset: -3
                width: 120
                anchors.centerIn: rpm
                transform: Rotation { origin.x: 60; origin.y: 330; axis { x: 0; y: 0; z: 1 } angle: needle.angle }
            }

            Image{
                id: rpm
                y: 0
                source: "qrc:/gauges/RPMGauge.png"
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                height: 750
                anchors.bottomMargin: -50
                anchors.leftMargin: 0
                width: 750
            }

        }

        Rectangle{
            color: "transparent"
            height: 700
            width: 700
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            transform: Rotation { origin.x: 0; origin.y: 0; axis { x: 0; y: 1; z: 0 } angle: -15 }
            Image{
                id: mph
                x: 96
                y: -31
                source: "qrc:/gauges/MPHGauge.png"
                anchors.top: parent.top
                anchors.right: parent.right
                height: 633
                anchors.rightMargin: 64
                anchors.topMargin: 0
                width: 540

                        Text{
                            id: liveMPH
                            anchors.centerIn: parent
                            font.pointSize: 80
                            color: "#0059ff"
                            text: "0"
                            font.family: "Calibri"
                            anchors.verticalCenterOffset: -10
                        }
            }

        }

    }
    Connections{
        target: Work
        onObdRPM: needle.angle = rpm * 0.045 + 180
        onObdMPH: liveMPH.text = speed - 1
//        onObdFuelStatus: liveFuel.text = fuel + "% fuel"
//        onObdCoolantTemp: liveCoolantTemp.text = coolantTemp + "c"
//        onObdThrottlePosition: liveThrottlePosition.text = "Throttle: " + throttle + "%"
        //onObdTroubleCode: liveTroubleCode.text = troublecode
    }

}

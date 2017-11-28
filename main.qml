import QtQuick 2.7
import QtQuick.Window 2.2
import QtGraphicalEffects 1.0

Window {
    id:mainwin
    property string wincolor: "white"
    visible: true
    width: 1920
    height: 810
    onBeforeRendering: Work.start();
    color: mainwin.wincolor

    Rectangle{
        id: background
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: .80; color: "transparent" }
            GradientStop { position: 1; color: "white" }
        }

        Rectangle{
            color: mainwin.wincolor
            height: 400
            width: 400
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            transform: Rotation { origin.x: 400; origin.y: 400; axis { x: 0; y: 0; z: 1 } angle: 45}
            gradient: Gradient {
                GradientStop { position: .5; color: mainwin.wincolor }
                GradientStop { position: .5; color: "transparent" }
            }
        }

        Rectangle{
            color: mainwin.wincolor
            height: 400
            width: 400
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            transform: Rotation { origin.x: 0; origin.y: 400; axis { x: 0; y: 0; z: 1 } angle: -45}
            gradient: Gradient {
                GradientStop { position: .5; color: mainwin.wincolor }
                GradientStop { position: .5; color: "transparent" }
            }
        }
        Rectangle{
            id: rpmrect
            color: "transparent"
            height: 550
            width: 550
            anchors.left: parent.left
            anchors.bottom: parent.bottom

            Image {
                id: rpmneedle
                property double angle: -36
                source: "qrc:/gauges/needle.png"
                anchors.verticalCenterOffset: 128.75
                anchors.horizontalCenterOffset: 4
                anchors.centerIn: rpm
                transform: Rotation { origin.x: 5; origin.y: 0; axis { x: 0; y: 0; z: 1 } angle: rpmneedle.angle;
                    Behavior on angle { SpringAnimation { spring: 5; damping: 0.4 ; modulus: 360 }}} //324
                smooth: true
            }

            Image{
                id: fuelpneedle
                property int angle: 90
                source: "qrc:/gauges/fueltempneedle.png"
                anchors.verticalCenterOffset: 80
                anchors.horizontalCenterOffset: 5
                anchors.centerIn: rpm
                transform: Rotation { origin.x: 5; origin.y: 0; axis { x: 0; y: 0; z: 1 } angle: fuelpneedle.angle;
                    Behavior on angle { SpringAnimation { spring: 5; damping: 0.4 ; modulus: 360 }}}
            }

            Image{
                id: tempneedle
                property int angle: 270
                source: "qrc:/gauges/fueltempneedle.png"
                anchors.verticalCenterOffset: 80
                anchors.horizontalCenterOffset: 5
                anchors.centerIn: rpm
                transform: Rotation { origin.x: 5; origin.y: 0; axis { x: 0; y: 0; z: 1 } angle: tempneedle.angle;
                    Behavior on angle { SpringAnimation { spring: 5; damping: 0.4 ; modulus: 360 }}}
            }

            Image{
                id: rpm
                source: "qrc:/gauges/RPMGauge.png"
                anchors.bottom: parent.bottom
                anchors.left: parent.left
            }
        }

        Rectangle{
            id: mphrect
            color: "transparent"
            anchors.right: background.right
            anchors.bottom: background.bottom
            height: 560
            width: 600

            Image {
                id: mphneedle
                property double angle: 36
                anchors.verticalCenterOffset: 137.75
                anchors.horizontalCenterOffset: 13
                source: "qrc:/gauges/needle.png"
                anchors.centerIn: mph
                transform: Rotation { origin.x: 5; origin.y: 0; axis { x: 0; y: 0; z: 1 } angle: mphneedle.angle;
                    Behavior on angle { SpringAnimation { spring: 5; damping: 0.4 ; modulus: 360 }}}
                smooth: true
            }

            Image{
                id: mph
                source: "qrc:/gauges/MPHGauge.png"
                anchors.centerIn: mphrect

                Text{
                    id: liveMPH
                    anchors.centerIn: parent
                    anchors.horizontalCenterOffset: 10
                    font.pointSize: 70
                    color: "#0059ff"
                    text: "0"
                    font.family: "Calibri"
                }

                Image{
                    visible: false
                    id: checkengine
                    source: "qrc:/gauges/checkengine.png"
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: 110
                    anchors.horizontalCenterOffset: 8
                    Text{
                        id: troubleCode
                        font.pointSize: 13
                        font.bold: true
                        color: "gold"
                        anchors.centerIn: parent
                        anchors.verticalCenterOffset: 2
                    }
                }
            }

            Image {
                id: skull
                source: "qrc:/gauges/skull.png"
                anchors.centerIn: parent
                height: 300
                width: 300
                opacity: 0
                anchors.verticalCenterOffset: 10
                anchors.horizontalCenterOffset: 10
                ColorOverlay{
                    anchors.fill: parent
                    source: parent
                    color: "#ffffff"
                }
            }

        }
    }

    Connections{
        target: Work
        onObdRPM: {rpmneedle.angle = rpm * 0.036 - 36;
            skull.opacity = ((rpm * .1) -450) * .01;
            liveMPH.opacity = 1 - (skull.opacity)
        }

        onObdMPH: {liveMPH.text = speed - 1;
            mphneedle.angle = ((speed - 1) * -1.8) + 36
        }

        onObdFuelStatus: {fuelpneedle.angle = ((fuel - 3) * 1.6) + 100}
        onObdCoolantTemp: {tempneedle.angle = ((coolantTemp * -1) * .57) + 80;
        }
        onObdTroubleCode: {troubleCode.text = troublecode;
            checkengine.visible = true
        }
    }
}

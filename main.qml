import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtGraphicalEffects 1.0

Window {
    id:mainwin
    property string wincolor: "black"
    visible: true
    width: 1820
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

            Rectangle{
                id: fuelrect
                height: 75.2
                width: 49.5
                anchors.centerIn: rpm
                anchors.verticalCenterOffset: -54
                anchors.horizontalCenterOffset: -56.1
                radius: 6
                gradient: Gradient {
                    GradientStop { position: 0.3; color: "grey" }
                    GradientStop { position: 1; color: "darkgrey" }
                }

                Rectangle{
                    id: fuelprogressbar
                    property int fuelpercentage: 12
                    radius: 10
                    anchors.bottom: parent.bottom
                    color: "lightgreen"
                    width: parent.width
                    height: parent.height * (fuelprogressbar.fuelpercentage * .01)
                    gradient: Gradient {
                        GradientStop { position: 0.5; color: "lightgreen" }
                        GradientStop { position: 1; color: "green" }
                    }

                }

                Text {
                    id: fuelpercent
                    anchors.centerIn: fuelrect
                    anchors.verticalCenterOffset: 10
                    font.family: "Arial"
                    text: "--%"
                    font.pointSize: 11
                    color: "#0059ff"
                    font.bold: true
                }
            }

            Image{
                id: rpm
                source: "qrc:/gauges/RPMGauge.png"
                anchors.bottom: parent.bottom
                anchors.left: parent.left

                Image{
                    id: tempneedle
                    property int angle: 270
                    height: 120
                    width: 120
                    source: "qrc:/gauges/tempneedle.png"
                    anchors.centerIn: parent
                    anchors.horizontalCenterOffset: 3
                    anchors.verticalCenterOffset: 15
                    transform: Rotation { origin.x: 60; origin.y: 92; axis { x: 0; y: 0; z: 1 } angle: tempneedle.angle;
                    Behavior on angle { SpringAnimation { spring: 5; damping: 0.4 ; modulus: 360 }}}
                }

                Text{
                    id: texttemp
                    anchors.centerIn: tempneedle
                    anchors.verticalCenterOffset: 32
                    font.pointSize: 15
                    color: "#0059ff"
                    text: "--"
                    font.family: "Calibri"
                }

            }

        }

        Rectangle{
            id: mphrect
            color: "transparent"
            anchors.right: background.right
            anchors.bottom: background.bottom
            height: 565
            width: 565


            Image {
                id: mphneedle
                property double angle: 36
                anchors.verticalCenterOffset: 137.75
                anchors.horizontalCenterOffset: 13
                source: "qrc:/gauges/needle.png"
                anchors.centerIn: mph
                transform: Rotation { origin.x: 5; origin.y: 0; axis { x: 0; y: 0; z: 1 } angle: mphneedle.angle;
                Behavior on angle { SpringAnimation { spring: 5; damping: 0.3 ; modulus: 360 }}}
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
            }
        }
    }
    Connections{
        target: Work
        onObdRPM: rpmneedle.angle = rpm * 0.036 - 36
        onObdMPH: {liveMPH.text = speed - 1; mphneedle.angle = ((speed - 1) * -1.8) + 36}
        onObdFuelStatus: {fuelprogressbar.fuelpercentage = fuel; fuelpercent.text = fuel - 3 + "%"}
        onObdCoolantTemp: {tempneedle.angle = (coolantTemp * (-9/14)) - 90; texttemp.text = ((coolantTemp - 40) * 1.8 + 32).toFixed(0)}
        //onObdThrottlePosition: liveThrottlePosition.text = "Throttle: " + throttle + "%"
        //onObdTroubleCode: liveTroubleCode.text = troublecode
    }

}

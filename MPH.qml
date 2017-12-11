import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    anchors.fill: parent

    Rectangle{
        id: mphrect
        color: "transparent"
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.rightMargin: 25
        height: 550
        width: 550

        RectangularGlow {
            id: meffect
            anchors.fill: mphrect
            glowRadius: 0
            spread: 0
            cornerRadius: 360
            color: mainwin.gaugeglow
        }

        Rectangle{
            id:mphblurr
            height: mph.height -34
            width: mph.width -34
            anchors.centerIn: mph
            anchors.horizontalCenterOffset: 12
            anchors.verticalCenterOffset: 10
            color: mainwin.wincolor
            radius: 360
            opacity: .1
            FastBlur{
                anchors.fill: parent
                source: mph
                radius: 64
            }
        }

        Image {
            id: mphneedle
            property double angle: 36
            property double mshade: -25
            anchors.verticalCenterOffset: 137.75
            anchors.horizontalCenterOffset: 13
            source: "qrc:/gauges/needle.png"
            anchors.centerIn: mph
            transform: Rotation { origin.x: 5; origin.y: 0; axis { x: 0; y: 0; z: 1 } angle: mphneedle.angle;
                Behavior on angle { SpringAnimation { spring: 5; damping: 0.4 ; modulus: 360 }}}
            smooth: true
            layer.enabled: true
            layer.effect: DropShadow {
                id: mphnshadow
                horizontalOffset: mphneedle.mshade
                verticalOffset: -3
                radius: 8
                samples: 16
                color: "#80000000"
            }
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
                    color: "blue"
                    anchors.centerIn: parent
                    anchors.verticalCenterOffset: 2
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
                    color: "transparent"
                }
            }
        }
    }

    Connections{
        target: Work

        onObdMPH: {liveMPH.text = (speed - .9).toFixed(0);
            mphneedle.angle = (((speed - .1) * -1.8) + 36).toFixed(0);
            mphneedle.mshade = (speed * 0.3571428) - 25;
            troubleCode.color = "gold"
        }
        onObdTroubleCode: {troubleCode.text = troublecode;
            checkengine.visible = true;
        }
    }
}

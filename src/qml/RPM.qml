import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {

    anchors.fill: parent

    Rectangle{
        id: rpmrect
        color: "transparent"
        height: 550
        width: 550
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        RectangularGlow {
            id: reffect
            anchors.fill: rpm
            glowRadius: 0
            spread: 0
            cornerRadius: 360
            color: mainwin.gaugeglow
        }

        Rectangle{
            id:rpmblurr
            height: rpm.height -32
            width: rpm.width -32
            anchors.centerIn: rpm
            anchors.horizontalCenterOffset: 5
            anchors.verticalCenterOffset: 1
            color: mainwin.wincolor
            radius: 360
            opacity: .1
            FastBlur{
                anchors.fill: parent
                source: rpm
                radius: 64
            }
        }

        Image {
            id: rpmneedle
            property double angle: -36
            property double rshade: 25
            source: "qrc:/gauges/needle.png"
            anchors.verticalCenterOffset: 128.75
            anchors.horizontalCenterOffset: 4
            anchors.centerIn: rpm
            transform: Rotation { origin.x: 5; origin.y: 0; axis { x: 0; y: 0; z: 1 } angle: rpmneedle.angle;
                Behavior on angle { SpringAnimation { spring: 5; damping: 0.4 ; modulus: 360 }}} //324
            smooth: true
            layer.enabled: true
            layer.effect: DropShadow {
                id: mphrshadow
                horizontalOffset: rpmneedle.rshade
                verticalOffset: -3
                radius: 8
                samples: 16
                color: "#80000000"
            }
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
            ColorOverlay{
                id:rpm5k
                anchors.fill: parent
                source: parent
            }

            Text {
                id: txtgear
                text: ""
                font.pointSize: 70
                anchors.centerIn: parent
                anchors.horizontalCenterOffset: 5
                color: "#0059ff"
            }
        }
    }

    Connections{
        target: Work
        onObdRPM: {rpmneedle.angle = rpm * 0.036 - 36;
            if(daytime > 400){
                gaugeglow = "lightgrey";
                mustangrect.color = "darkgrey";
                background.color = "darkgrey"
                stdrect.color = "black"
            }
            if(rpm >= 5500)
                rpm5k.color = "red";
            rpmneedle.rshade = -1 * ((((rpm * 0.01) * 2) * 0.3571428) - 25)
        }

        onObdFuelStatus: {fuelpneedle.angle = ((fuel - 3) * 1.6) + 100}
        onObdCoolantTemp: {tempneedle.angle = ((coolantTemp * -1) * .57) + 80;
        }
    }
}

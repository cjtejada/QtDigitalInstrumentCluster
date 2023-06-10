import QtQuick 2.0
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

Window {
    id:mainwin
    property int daytime: 0400
    property string wincolor: "black"
    property string gaugeglow: "white"
    visible: true
    width: 1920
    height: 650
    onAfterRendering: Work.start();
    color: mainwin.wincolor

    Rectangle{
        id: background
        anchors.fill: parent
        color: "grey"

        TabBar{
            id: optiontabs
            anchors.centerIn: parent
            anchors.verticalCenterOffset: -310
            width: 360

            TabButton {
                width: 120
                text: "Standard"
            }
            TabButton {
                width: 120
                text: "Mustang"
            }
            TabButton {
                width: 120
                text: "Navigation"
            }
        }

        StackLayout {
            id: stcklayout
            width: parent.width
            height: 620
            currentIndex: optiontabs.currentIndex
            anchors.bottom : background.bottom
            Item{
                id: standard
                Rectangle{
                    id: stdrect
                    anchors.fill: parent
                    color: "#cecece"
                }
            }

            Item {
                id: mustangtab
                Rectangle{
                    id: mustangrect
                    anchors.fill: parent
                    color: "#a8a8a8"
                    Mustang{}
                }
            }
            Item {
                id: navtab
                Rectangle{
                    anchors.fill: parent
                    Navigation{}
                }
            }
        }

        RPM{}   //RPM Gauge
        MPH{}   //MPH Gauge
    }
}

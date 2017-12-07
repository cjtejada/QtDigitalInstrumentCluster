import QtQuick 2.0
import QtLocation 5.8
import QtPositioning 5.8

Item {
    id: mapItem
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent

        Plugin {
            id: mapPlugin
            name: "osm"
        }

        Map {
            id: dashmap
            property double lat: 0
            property double lon: 0
            copyrightsVisible: false
            anchors.fill: parent
            plugin: mapPlugin
            center: QtPositioning.coordinate(dashmap.lat,dashmap.lon)
            zoomLevel: 20
            tilt: 60
            gesture.enabled: true
            //gesture: MapGestureArea{panActive: true}
            AnimatedImage {
                id: navcenter
                height: 50
                width: 50
                source: "qrc:/gauges/navcenter.gif"
                anchors.centerIn: parent
                transform: Rotation { origin.x: 0; origin.y: 50; axis { x: 1; y: 0; z: 0 } angle: 60;
                }
            }
        }
    }

    Connections{
        target: Work
        onGpsLat: {
            console.log(Lat);
            dashmap.lat = (Lat).toFixed(9)
        }
        onGpsLong: {
            console.log(Long);
            dashmap.lon = (Long).toFixed(9);
            dashmap.pan(310,960)
        }
    }
}

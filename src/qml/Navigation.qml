import QtQuick 2.15
import QtLocation 5.15
import QtPositioning 5.15

Item {
    id: mapItem
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent

        Plugin {
            id: mapPlugin
            name: "osm"
            PluginParameter {
                name: "osm.mapping.providersrepository.disabled"
                value: "true"
            }
            PluginParameter {
                name: "osm.mapping.providersrepository.address"
                value: "http://maps-redirect.qt.io/osm/5.6/"
            }
        }

        PositionSource {
            id: positionSource
            updateInterval: 1000
            active: true
        }

        MapQuickItem {
            id: navcenterMapItem
            sourceItem: AnimatedImage {
                height: 50
                width: 50
                source: "qrc:/gauges/navcenter.gif"
                anchors.centerIn: parent
                transform: Rotation {
                    origin.x: 0; origin.y: 50; axis { x: 1; y: 0; z: 0 } angle: 45;
                }
            }
            coordinate: positionSource.position.latitudeValid ? positionSource.position.coordinate : QtPositioning.coordinate(44,-103)

//            anchorPoint: Qt.point(-poiTheQtComapny.sourceItem.width * 0.5,poiTheQtComapny.sourceItem.height * 1.5)
        }



        Map {

            id: dashmap
            copyrightsVisible: false
            anchors.fill: parent
            plugin: mapPlugin
            center: positionSource.position.latitudeValid ? positionSource.position.coordinate : QtPositioning.coordinate(44,-103)
            bearing: positionSource.position.directionValid ? positionSource.position.direction : 0
            zoomLevel: 20
            tilt: 45
            gesture.enabled: true
            //gesture: MapGestureArea{panActive: true}

        }
        Component.onCompleted: {
            dashmap.addMapItem(navcenterMapItem)
        }
    }
}

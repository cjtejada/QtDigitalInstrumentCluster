import QtQuick 2.0
import QtLocation 5.8
import QtPositioning 5.8

Item {
    anchors.fill: parent
    Rectangle{
        anchors.fill: parent

        Plugin {
            id: mapPlugin
            name: "osm"
        }

        Map {
            anchors.fill: parent
            plugin: mapPlugin
            center: QtPositioning.coordinate(44.0267621, -103.26569239999998)
            zoomLevel: 18
        }

    }
}

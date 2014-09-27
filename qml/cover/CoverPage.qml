import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    Image {
        id: image
        source: "../images/sailkino-appcover.png"
    }
    Label {
        id: label
        anchors.centerIn: parent
        text: "SailKino"
    }
}




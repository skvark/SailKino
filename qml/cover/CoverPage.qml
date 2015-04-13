import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailkino.events 1.0
import harbour.sailkino.eventsmodel 1.0

CoverBackground {

    property bool active: status === Cover.Active;
    property bool ready: false;
    property string eventid;
    anchors.fill: parent

    SlideshowView {

        id: view
        anchors.fill: parent
        delegate:

            Image {
                property variant eventData: model
                id: coverpic
                anchors.fill: parent;
                source: mediumimageportrait
                NumberAnimation on opacity {
                    from: 0
                    to: 1
                    duration: 1000
                }
           }
    }


    Timer {
        id: timer
        interval: 3000;
        running: ready && active;
        repeat: true;
        onTriggered: {
            view.incrementCurrentIndex()
        }
    }

    CoverActionList {
        enabled: true
        CoverAction {
            iconSource: "image://theme/icon-cover-new.png"
            onTriggered: {
                pageStack.push(Qt.resolvedUrl("../pages/SingleEvent.qml"), { id: view.currentItem.eventData.id, comingsoonmodel: false })
                app.activate();
            }
        }
    }

    Connections {
        target: kinoAPI

        onLoading: {

            if(yesno) {
                ready = false;
            } else {
                ready = true;
                view.model = kinoAPI.inTheatres;
            }
        }
    }
}




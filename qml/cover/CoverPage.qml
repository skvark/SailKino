import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailkino.events 1.0
import harbour.sailkino.eventsmodel 1.0

CoverBackground {

    property bool active: status === Cover.Active || applicationActive
    property bool ready: false;
    property int index: 0;
    property EventsModel events;
    anchors.fill: parent

    CoverActionList {
        enabled: ready && active
        CoverAction {
            iconSource: "image://theme/icon-cover-new"
            onTriggered: {
                pageStack.push(Qt.resolvedUrl("../pages/SingleEvent.qml"), { id: events.get(index - 1).id, comingsoonmodel: false })
                app.activate();
            }
        }
    }

    CoverPlaceholder {
        id: holder
        property string title
        visible: true
        text: "SailKino"
        onTitleChanged: {
            if(title !== "")
                text = "SailKino\n\n"+title
        }
        z: -1
    }

    Image {
        id: coverpic
        visible: ready && active
        anchors.fill: parent

        NumberAnimation on opacity {
            id: fadeout
            running: false
            from: 1
            to: 0
            duration: 1000
            onStopped: {
                if(events.get(index - 1).mediumimageportrait !== undefined)
                {
                    coverpic.source = events.get(index - 1).mediumimageportrait
                    coverpic.visible = true;
                }
                else if (events.get(index - 1).smallimageportrait !== undefined)
                {
                    coverpic.source = events.get(index - 1).smallimageportrait
                    coverpic.visible = true;
                }
                else
                {
                    coverpic.visible = false;
                    coverpic.source = ""
                    if(events.get(index - 1).title !== undefined)
                        holder.title = events.get(index - 1).title
                }
            }
        }

        NumberAnimation on opacity {
            id: fadein
            running: false
            from: 0
            to: 1
            duration: 1000
        }

        onStatusChanged:
            if (coverpic.status === Image.Ready) {
                fadein.start()
            }
    }

    Timer {
        id: timer
        interval: 7000;
        triggeredOnStart: true;
        running: ready && active;
        repeat: true;
        onTriggered: {

            if(events.count() - 1 > index) {
                ++index;
            } else {
                index = 1;
            }

            if(index === 1) {
                fadeout.onStopped()
            } else {
                fadeout.start()
            }

        }
    }

    Connections {
        target: kinoAPI

        onLoading: {

            if(yesno) {
                ready = false;
            } else {
                events = kinoAPI.inTheatres;
                index = 0;
                ready = true;
            }
        }
    }
}




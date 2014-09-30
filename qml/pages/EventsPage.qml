import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    BusyIndicator {
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        running: loading
    }

    SlideshowView {

        id: menuView
        itemWidth: width
        itemHeight: height
        height: window.height
        clip: true

        anchors {
            top: parent.top;
            left: parent.left;
            right: parent.right
        }

        model: VisualItemModel {
                id: days
                Events { id: inSelectedTheatre }
                Events { id: inTheatres }
                Events { id: comingSoon }
        }
    }

    property var date: new Date();
    property bool loading: true;

    Connections {
        target: kinoAPI
        onSchedule: { inSelectedTheatre.setModel(kinoAPI.getModel(0), "In Selected Theatre") }
        onEvents: { inTheatres.setModel(kinoAPI.getModel(0), "Now in Theatres") }
        onComingSoonEvents: { comingSoon.setModel(kinoAPI.getModel(1), "Coming Soon") }
        onLoading: {
                    if(yesno) {
                        loading = true;
                    } else {
                        loading = false;
                    }
                   }
    }
}



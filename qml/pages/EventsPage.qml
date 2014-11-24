import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailkino.eventsmodel 1.0

Page {
    id: page
    Component.onCompleted: init()

    BusyIndicator {
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        running: loading
    }

    SilicaFlickable {

        width: parent.width

        ViewPlaceholder {
            id: pholder
            enabled: loading == false && holder == true
            text: qsTr("Please select country, language and area in the settings to see events.")
        }
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
                id: movieViews
                Events { id: inSelectedTheatre }
                Events { id: comingSoon }
        }
    }

    property bool loading: false;
    property bool holder: false;
    property string info;

    function init() {
        if (kinoAPI.areaSelectedEarlier()) {
            holder = false
            kinoAPI.init();
        } else {
            holder = true
        }
    }

    Connections {
        target: kinoAPI

        onReady: {
            inSelectedTheatre.setModel(kinoAPI.inTheatres,
                                       kinoAPI.getAreaName(),
                                       false)
            comingSoon.setModel(kinoAPI.comingSoon, "Coming soon", true)
        }

        onLoading: {
            if(yesno) {
                holder = false
                loading = true;
            } else {
                loading = false;
            }
        }
    }
}



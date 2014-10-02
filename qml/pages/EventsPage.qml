import QtQuick 2.0
import Sailfish.Silica 1.0
import CPPIntegrate 1.0

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
        onCurrentIndexChanged: infoText()

        anchors {
            top: parent.top;
            left: parent.left;
            right: parent.right
        }

        model: VisualItemModel {
                id: movieViews
                Events { id: inSelectedTheatre }
                Events { id: inTheatres }
                Events { id: comingSoon }
        }
    }

    DockedPanel {

        id: panel
        open: true
        width: parent.width
        height: Theme.itemSizeExtraSmall
        dock: Dock.Bottom

        Rectangle {
            anchors.fill: background
            color: Theme.rgba(Theme.highlightBackgroundColor, 0.7)
        }

        BackgroundItem {
            id: background
            height: parent.height
            width: parent.width
        }

        Label {
            id: bottomLabel
            width: parent.width
            height: parent.height
            text: info
            font.pixelSize: Theme.fontSizeSmall
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        BusyIndicator {
            anchors.centerIn: parent
            running: loading
            size: BusyIndicatorSize.Small
        }

    }

    property var date: new Date();
    property bool loading: true;
    property string info

    function infoText() {
        if (menuView.currentItem !== null) {
            info = menuView.currentItem.pgheader + " - " + date.toLocaleDateString()
        } else {
            info = ""
        }
    }

    Connections {
        target: kinoAPI
        onReady: {
            inSelectedTheatre.setModel(kinoAPI.inTheatres, "In Selected Theatre")
            inTheatres.setModel(kinoAPI.inTheatres, "Now in Theatres")
            comingSoon.setModel(kinoAPI.comingSoon, "Coming Soon")
            infoText()
        }
        onLoading: {
            if(yesno) {
                loading = true;
            } else {
                loading = false;
            }
        }
    }
}



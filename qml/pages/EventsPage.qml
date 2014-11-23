import QtQuick 2.0
import Sailfish.Silica 1.0
import CPPIntegrate 1.0

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

    DockedPanel {

        id: panel
        open: false
        width: parent.width
        height: Theme.itemSizeExtraSmall
        dock: Dock.Bottom

        BackgroundItem {
            id: background1
            anchors.left: parent.left;
            width: parent.width / 2
            height: parent.height
            onClicked:  {
                console.log()
                if (menuView.currentIndex !== 0) {
                    menuView.decrementCurrentIndex()
                }
            }

        }

        Rectangle {
            anchors.fill: background1
            color: Theme.rgba(0, 0, 0, 1)
        }

        Label {
            id: bottomLabelLeft
            anchors.fill: background1
            textFormat: Text.RichText;
            font.pixelSize: 20
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter

        }

        BackgroundItem {
            id: background2
            anchors.right: parent.right;
            width: parent.width / 2
            height: parent.height
            onClicked:  {
                if (menuView.currentIndex !== 1) {
                    menuView.incrementCurrentIndex()
                }
            }
        }

        Rectangle {
            anchors.fill: background2
            color: Theme.rgba(0, 0, 0, 1)
        }

        Label {
            id: bottomLabelRight
            anchors.fill: background2
            textFormat: Text.RichText;
            text: "Coming Soon"
            font.pixelSize: Theme.fontSizeSmall
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
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
            inSelectedTheatre.setModel(kinoAPI.inTheatres, kinoAPI.getAreaName(), false)
            comingSoon.setModel(kinoAPI.comingSoon, "Coming soon", true)
            bottomLabelLeft.text = kinoAPI.getAreaName() + "<br />" + kinoAPI.getDate().toLocaleDateString();
            panel.open = true;
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



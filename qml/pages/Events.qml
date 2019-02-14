import QtQuick 2.0
import Sailfish.Silica 1.0

Item {

    height: menuView.height;
    width: menuView.width

    function setModel(model, title, comingsoon) {
        eventsModel.clear();

        if(comingsoon) {
            comingsoonmodel = comingsoon;
        }

        if(kinoAPI.getFilterState() === true && !comingsoon) {
            filter(model);
        } else {
            listview.model = model;
        }
        if(!comingsoon) {
            dt = kinoAPI.getDate().toLocaleDateString();
        }
        pgheader = title;
    }

    function filter(model) {

        for(var i = 0; i < model.count() ; ++i) {
            kinoAPI.setID(model.get(i).id);
            var count = 0;

            if(kinoAPI.getDate().toLocaleDateString() === (new Date()).toLocaleDateString()) {
                count = kinoAPI.getEvent.filteredHasShows();
            } else {
                count = kinoAPI.getEvent.hasShows();
            }

            if(count > 0) {
                eventsModel.append(model.get(i));
            }
        }
        listview.model = eventsModel;
    }

    ListModel {
        id: eventsModel
    }

    property string pgheader: "";
    property string dt: "";

    SilicaListView {

        id: listview

        header: PageHeader {
            title: pgheader
            description: dt
        }

        PullDownMenu {
            id: menu

            MenuItem {
                text: qsTr("Settings")
                onClicked: {
                    pgheader = "";
                    filterdatetimer.stop();
                    pageStack.push("Settings.qml");
                }
            }

            MenuItem {
                enabled: listview.count > 0
                visible: listview.count > 0
                text: qsTr("Search")
                onClicked: {
                    pageStack.push("SearchPage.qml");
                }
            }
        }

        ViewPlaceholder {
            id: pholder
            enabled: listview.count === 0 && !loading
            text: qsTr("No events available for selected date.")
        }

        anchors.fill: parent

        delegate: ListItem {

            width: parent.width
            height: row.height + spacer.height
            contentHeight: height

            BackgroundItem {

                id: background
                anchors {
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    bottom: row.bottom
                }
                height: row.height// + Theme.paddingMedium;
                onClicked: pageStack.push(Qt.resolvedUrl("SingleEvent.qml"), { id: id, comingsoonmodel: comingsoonmodel })

            }

            Row {
                id: row
                anchors.left: parent.left;
                anchors.right: parent.right;

                Column {
                    id: column1
                    width: previewimage.width
                    Image {
                        id: previewimage
                        source: {
                            if (mediumimageportrait !== "") {
                                return mediumimageportrait;
                            } else {
                                return "../images/default-cover.png"
                            }
                        }
                        fillMode: Image.PreserveAspectCrop
                        width: row.width / 3.0
                        height: row.width / 2.0
                        sourceSize.width: width
                        sourceSize.height: height

                    }
                }

                Column {
                    id: column2
                    width: background.width - previewimage.width
                    height: column1.height

                    Label {
                        id: header
                        width: parent.width
                        anchors.left: parent.left;
                        anchors.right: parent.right;
                        text: title
                        anchors.leftMargin: Theme.paddingMedium
                        anchors.rightMargin: Theme.paddingSmall
                        font.pixelSize: Theme.fontSizeMedium
                        wrapMode: Text.NoWrap
                        maximumLineCount: 1
                        truncationMode: TruncationMode.Fade
                    }
                    Label {
                        id: genres
                        width: parent.width
                        anchors.left: parent.left;
                        anchors.right: parent.right;
                        text: genre
                        anchors.leftMargin: Theme.paddingMedium
                        anchors.rightMargin: Theme.paddingSmall
                        font.pixelSize: Theme.fontSizeExtraSmall
                        opacity: 0.5
                        wrapMode: Text.NoWrap
                        maximumLineCount: 1
                        truncationMode: TruncationMode.Fade
                    }
                    Label {
                        id: moviecontent
                        anchors.left: parent.left;
                        anchors.right: parent.right;
                        height: previewimage.height - header.height - genres.height
                        anchors.leftMargin: Theme.paddingMedium
                        anchors.rightMargin: Theme.paddingSmall
                        text: shortsynopsis
                        wrapMode: Text.Wrap
                        font.pixelSize: Theme.fontSizeExtraSmall
                        elide: Text.ElideRight
                        clip: true
                        maximumLineCount: Math.floor((previewimage.height - header.height - genres.height) / genres.height)
                    }

                }
            }
            Rectangle {
                id: spacer
                anchors.top: row.bottom
                width: parent.width
                height: Theme.paddingSmall
                color: Theme.rgba(Theme.highlightBackgroundColor, 0.3)
            }
        }

        contentHeight: page.height
        contentWidth: page.width

        VerticalScrollDecorator { flickable: listview }
    }

    property bool loading: true;
    property bool comingsoonmodel: false;

    Connections {
        target: kinoAPI
        onClear: {
            if(listview.model !== undefined) {
                listview.model.clear();
            }
        }
        onLoading: {
            if (yesno === false) {
                loading = false;
                listview.visible = true;
            } else {
                loading = true;
            }
        }
    }
}


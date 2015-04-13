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

        header: Component {

            PageHeader {
                title: pgheader
                height: 140

                SectionHeader {
                    id: section
                    text: dt
                    anchors.top: parent.top
                    anchors.topMargin: 60
                }
            }
        }

        PullDownMenu {
            id: menu

            MenuItem {
                text: "Settings"
                onClicked: {
                    pgheader = "";
                    filterdatetimer.stop();
                    pageStack.push("Settings.qml");
                }
            }

            MenuItem {
                text: "Search"
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
            height: background.height + Theme.paddingMedium
            anchors.topMargin: Theme.paddingMedium
            anchors.bottomMargin: Theme.paddingMedium

            Rectangle {
                anchors.fill: background
                radius: 5;
                color: Theme.rgba(Theme.highlightBackgroundColor, 0.3)
            }

            BackgroundItem {

                id: background
                anchors.left: parent.left;
                anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right;
                anchors.rightMargin: Theme.paddingMedium
                height: row.height + row2.height + 20;
                onClicked: pageStack.push(Qt.resolvedUrl("SingleEvent.qml"), { id: id, comingsoonmodel: comingsoonmodel })

            }

            Row {

                id: row
                anchors.left: parent.left;
                anchors.leftMargin: Theme.paddingMedium
                anchors.right: background.right;

                Column {
                        id: column
                        width: background.width - previewimage.width
                        height: previewimage.height + 10

                        Label {
                            id: header
                            height: 110
                            width: parent.width
                            anchors.left: parent.left;
                            anchors.right: parent.right;
                            textFormat: Text.RichText
                            text: title
                            anchors.leftMargin: 15
                            font.pixelSize: 36
                            wrapMode: Text.WordWrap
                            verticalAlignment: Text.AlignVCenter
                        }
                        Label {
                            id: genres
                            height: 20
                            width: parent.width
                            anchors.left: parent.left;
                            anchors.right: parent.right;
                            textFormat: Text.RichText
                            text: genre
                            anchors.leftMargin: 15
                            font.pixelSize: 25
                            color: Theme.rgba(Theme.secondaryColor, 0.5)
                            wrapMode: Text.WordWrap
                        }

                    }

                Column {
                    id: column2

                    Image {
                       id: previewimage
                       source: {
                                   if (smallimageportrait !== "") {
                                      return smallimageportrait;
                                   } else {
                                      return "../images/default-cover.png"
                                   }
                               }
                       sourceSize.width: 99
                       sourceSize.height: 146
                    }
                }
            }

            Row {

                id: row2
                anchors.left: parent.left;
                anchors.top: row.bottom;
                anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right;
                anchors.rightMargin: Theme.paddingMedium
                anchors.topMargin: Theme.paddingMedium
                spacing: Theme.paddingMedium


                Column {
                        id: column3
                        width: parent.width

                    Label {
                        id: moviecontent
                        anchors.topMargin: 24
                        anchors.left: parent.left;
                        anchors.right: parent.right;
                        anchors.leftMargin: 15
                        anchors.rightMargin: 5
                        textFormat: Text.RichText
                        text: shortsynopsis
                        wrapMode: Text.Wrap
                        font.pixelSize: Theme.fontSizeSmall
                    }
                }
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


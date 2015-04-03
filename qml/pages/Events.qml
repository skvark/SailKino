import QtQuick 2.0
import Sailfish.Silica 1.0

Item {

    height: menuView.height;
    width: menuView.width

    function setModel(model, title, comingsoon) {
        eventsModel.clear();
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
            if(kinoAPI.getEvent.hasShows()) {
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
                    pageStack.push("Settings.qml");
                }
            }

            MenuItem {
                text: "Search"
                onClicked: {
                    pageStack.push("SearchPage.qml");
                }
            }

            MenuItem {
                text: "Change Date"

                onClicked: {
                    var dialog = pageStack.push(pickerComponent, {
                        date: kinoAPI.getDate()
                    })
                    dialog.accepted.connect(function() {
                        listview.visible = false;
                        dateChanged();
                        kinoAPI.setDate(dialog.date);
                    })
                }

                Component {
                    id: pickerComponent
                    DatePickerDialog {}
                }
            }

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
                onClicked: pageStack.push(Qt.resolvedUrl("SingleEvent.qml"), { id: id })

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

    Connections {
        target: kinoAPI
        onClear: {
            if(listview.model !== undefined) {
                listview.model.clear();
            }
        }
        onLoading: {
            if (yesno === false) {
                listview.visible = true;
            }
        }
    }
}


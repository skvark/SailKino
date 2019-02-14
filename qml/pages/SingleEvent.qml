import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailkino.events 1.0
import QtMultimedia 5.0
import harbour.sailkino.showtimemodel 1.0
import harbour.sailkino.schedulefiltermodel 1.0

Page {
    id: singleevent
    property string id;
    property bool comingsoonmodel;
    property string currentdate: kinoAPI.getDate().toLocaleDateString();

    property var event: {
        kinoAPI.setID(id);
        return kinoAPI.getEvent;
    }

    SilicaFlickable {

        id: info
        anchors.fill: parent
        contentHeight: column.height + listview.height + column2.height + Theme.paddingLarge

        PullDownMenu {
            id: menu

            MenuItem {
                text: qsTr("Change Date")

                onClicked: {
                    var dialog = pageStack.push(pickerComponent, {
                        date: kinoAPI.getDate()
                    })
                    dialog.accepted.connect(function() {
                        kinoAPI.setDate(dialog.date);
                        currentdate = kinoAPI.getDate().toLocaleDateString();
                    })
                }

                Component {
                    id: pickerComponent
                    DatePickerDialog {}
                }
            }
        }

        Column {
            id: column

            PageHeader {
                id: head

                Label {
                    text: event.getTitle() + " (" + event.getProductionYear() + ")"
                    x: Theme.paddingLarge
                    wrapMode: Text.Wrap
                    color: Theme.highlightColor
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignRight
                    height: head.height
                    width: head.width - Theme.paddingLarge*2
                }
            }

            Image {
               id: previewimage
               width: Screen.width
               height: (250 * Screen.width) / 670

               source: {
                   if (event.getLargeImageLandscape() !== "") {
                              return event.getLargeImageLandscape();
                           } else {
                              return "../images/default-cover.png"
                           }
                       }

               fillMode: Image.PreserveAspectFit

               BackgroundItem {
                   id: video
                   anchors.fill: parent
                   onClicked: {
                       onClicked: Qt.openUrlExternally(event.getTrailer())
                   }
               }

               Image {
                   anchors.fill: parent
                   source: "../images/play.png"
               }

            }

            Label {
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingLarge
                width: parent.width - Theme.paddingLarge * 2
                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                textFormat: Text.RichText;
                onLinkActivated: Qt.openUrlExternally(link)
                text: qsTr("Genre: ") + event.getGenres()
            }

            Label {
                id: agelabel
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingLarge
                width: parent.width - Theme.paddingLarge * 2
                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                textFormat: Text.RichText;
                onLinkActivated: Qt.openUrlExternally(link)
                text: qsTr("Age limit: ") + event.getRating()
            }

            Label {
                id: lengthlabel
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingLarge
                width: parent.width - Theme.paddingLarge * 2
                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                textFormat: Text.RichText;
                onLinkActivated: Qt.openUrlExternally(link)
                text: qsTr("Length: ") + event.getLengthInMinutes() + " min"
            }

            BackgroundItem {

                height: Theme.fontSizeSmall*2

                Label {
                    id: showlistheader
                    anchors.fill: parent
                    anchors.leftMargin: Theme.paddingLarge
                    height: Theme.fontSizeSmall*2
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width - Theme.paddingLarge * 2
                    wrapMode: Text.Wrap
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.highlightColor
                    textFormat: Text.RichText;
                    text: currentdate
                }

                onClicked: {

                    var dialog = pageStack.push(pickerComponent, {
                        date: kinoAPI.getDate()
                    })
                    dialog.accepted.connect(function() {
                        kinoAPI.setDate(dialog.date);
                        currentdate = kinoAPI.getDate().toLocaleDateString();
                    })
                }

            }
        }

        SilicaListView {

            id: listview
            model: {
                if(kinoAPI.getDate().toLocaleDateString() === (new Date()).toLocaleDateString() && !comingsoonmodel) {
                    event.reFilter();
                    event.getFilteredModel;
                } else {
                    event.getModel;
                }
            }

            width: parent.width
            interactive: false;
            anchors.top: column.bottom
            anchors.leftMargin: Theme.paddingLarge
            anchors.topMargin: Theme.paddingSmall

            height: {
                if(listview.count !== 0) {
                    return listview.count * Theme.fontSizeExtraSmall*3 + Theme.fontSizeExtraSmall
                } else {
                    return pholder.height
                }
            }

            spacing: 5

            ViewPlaceholder {
                id: pholder
                height: Theme.fontSizeExtraSmall*3
                anchors.top: parent.top
                enabled: listview.count == 0 && !loading
                text: qsTr("No shows.")
                scale: 0.5
            }

            BusyIndicator {
                anchors.centerIn: parent
                size: BusyIndicatorSize.Small
                running: loading
            }

            delegate: ListItem {

                id: item
                contentHeight: Theme.fontSizeExtraSmall*3
                width: parent.width

                Label {
                    id: time
                    anchors.left: parent.left
                    width: parent.width - Theme.paddingLarge * 2
                    height: Theme.fontSizeExtraSmall*3
                    anchors.leftMargin: Theme.paddingLarge
                    anchors.bottomMargin: Theme.paddingLarge
                    wrapMode: Text.Wrap
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.primaryColor
                    textFormat: Text.RichText;
                    verticalAlignment: Text.AlignVCenter
                    text: start + " - " + end + "<br />" + theatre + ", " + auditorium
                }

                Rectangle {
                    anchors.left: time.left
                    anchors.leftMargin: -Theme.paddingLarge
                    color: Theme.rgba(Theme.highlightBackgroundColor, 0.3)
                    height: parent.height
                    width: Theme.paddingLarge / 2
                }

                BackgroundItem {
                    height: Theme.fontSizeExtraSmall*3
                    width: parent.width
                    onClicked: {
                        Qt.openUrlExternally(showurl)
                    }
                }
            }
        }

        Column {
            id: column2
            height: synopsis.height + Theme.paddingLarger
            anchors.top: listview.bottom;
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge
            anchors.topMargin: Theme.paddingSmall

            Label {
                id: synopsis
                width: parent.width
                wrapMode: Text.Wrap
                anchors.topMargin: Theme.paddingLarge
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                textFormat: Text.RichText;
                onLinkActivated: Qt.openUrlExternally(link)
                text: event.getSynopsis()
            }
        }
        VerticalScrollDecorator { flickable: info }
    }

    property bool loading: false;

    Connections {
        target: kinoAPI
        onSchedulesLoading: {
            if(yesno) {
                loading = true;
            } else {
                loading = false;
            }
        }
    }
}

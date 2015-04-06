import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailkino.events 1.0
import QtMultimedia 5.0
import harbour.sailkino.showtimemodel 1.0
import harbour.sailkino.schedulefiltermodel 1.0

Page {
    id: openinghours
    property string id;
    property string currentdate: kinoAPI.getDate().toLocaleDateString();
    property var event: {
        kinoAPI.setID(id);
        return kinoAPI.getEvent;
    }

    SilicaFlickable {

        id: info
        anchors.fill: parent
        contentHeight: column.height + listview.height + column2.height + 20

        PullDownMenu {
            id: menu

            MenuItem {
                text: "Change Date"

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
                    wrapMode: Text.Wrap
                    color: Theme.highlightColor
                    anchors.leftMargin: 110
                    anchors.topMargin: 15
                    height: 80
                    verticalAlignment: Text.AlignVCenter
                    textFormat: Text.RichText;
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
            }

            Image {
               id: previewimage
               width: 540;
               height: 211;
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
                       onClicked: pageStack.push(Qt.resolvedUrl("TrailerPlayer.qml"), { videourl: event.getTrailer() })
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
                width: parent.width
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
                width: parent.width
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
                width: parent.width
                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                textFormat: Text.RichText;
                onLinkActivated: Qt.openUrlExternally(link)
                text: qsTr("Length: ") + event.getLengthInMinutes() + " min"
            }

            Label {
                id: showlistheader
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingLarge
                anchors.topMargin: Theme.paddingLarge
                height: 50
                verticalAlignment: Text.AlignVCenter
                width: parent.width
                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.highlightColor
                textFormat: Text.RichText;
                text: currentdate
            }

        }

        SilicaListView {

            id: listview
            model: {
                if(kinoAPI.getDate().toLocaleDateString() === (new Date()).toLocaleDateString()) {
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
                    return listview.count * 65
                } else {
                    return pholder.height
                }
            }
            spacing: 5

            ViewPlaceholder {
                id: pholder
                height: 60
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
                contentHeight: 60
                width: parent.width

                Label {
                    id: time
                    anchors.left: parent.left
                    width: parent.width
                    height: 60
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
                    anchors.topMargin: 10
                    color: Theme.rgba(Theme.highlightBackgroundColor, 0.3)
                    height: 60
                    width: 15
                }

                BackgroundItem {
                    height: 60
                    width: parent.width
                    onClicked: {
                        Qt.openUrlExternally(showurl)
                    }
                }
            }
        }

        Column {
            id: column2
            height: synopsis.height + 20
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

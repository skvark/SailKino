import QtQuick 2.0
import Sailfish.Silica 1.0
import sailkino.events 1.0
import QtMultimedia 5.0
import ShowIntegrate 1.0

Page {
    id: openinghours
    property string id
    property var event: {
        kinoAPI.setID(id);
        return kinoAPI.getEvent;
    }

    function scaler() {
        // this scales the header so that it fits :D
        // does not work correctly, just a workaround
        return 1.0 - (head.title.split(" ").length * 0.02 * head.title.length * 0.06)
    }

    SilicaFlickable {

        id: info
        anchors.fill: parent
        contentHeight: column.height + listview.height + column2.height + 20

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right

            PageHeader {
                id: head
                title: event.getTitle() + " (" + event.getProductionYear() + ")"
                wrapMode: Text.Wrap
                scale: scaler()
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
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingLarge
                width: parent.width
                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeSmall
                color: Theme.primaryColor
                textFormat: Text.RichText;
                onLinkActivated: Qt.openUrlExternally(link)
                text: qsTr("Ikäraja: ") +
                      event.getRating() +
                      qsTr(", Kesto: ") +
                      event.getLengthInMinutes() +
                      qsTr(" minuuttia")
            }

        }

        SilicaListView {

            id: listview
            model: event.getModel;
            width: parent.width
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
                enabled: listview.count == 0
                text: qsTr("Ei näytöksiä.")
                scale: 0.5
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
}

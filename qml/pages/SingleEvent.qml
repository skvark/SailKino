import QtQuick 2.0
import Sailfish.Silica 1.0
import sailkino.events 1.0
import QtMultimedia 5.0

Page {
    id: openinghours
    property string id
    property var event: kinoAPI.getEvent(id)

    SilicaFlickable {

        id: info
        anchors.fill: parent
        contentHeight: column.height + listview.height + column2.height + 20

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right

            PageHeader {
                title: event.getTitle() + " (" + event.getProductionYear() + ")"
                wrapMode: Text.Wrap
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
                text: qsTr("Ikäraja: ") + event.getRating() + qsTr(" Kesto: ") + event.getLengthInMinutes() + qsTr(" minuuttia")
            }

        }

        SilicaListView {

            id: listview
            model: event.getModel();
            width: parent.width
            anchors.top: column.bottom
            anchors.leftMargin: Theme.paddingLarge
            anchors.topMargin: Theme.paddingSmall
            height: listview.count * 65
            spacing: 5

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
                    text: theatre + ", " + auditorium + "<br />" + start + " - " + end
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
                        console.log('asdsad' + showurl)
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

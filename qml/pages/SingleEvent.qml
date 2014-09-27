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
        contentHeight: column.height + column2.height

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right

            PageHeader {
                title: qsTr(event.getTitle())
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
                text: event.getGenres()
            }

        }

        Column {
            id: column2
            anchors.top: column.bottom;
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge
            anchors.topMargin: Theme.paddingLarge

            Label {
                width: parent.width
                wrapMode: Text.Wrap
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

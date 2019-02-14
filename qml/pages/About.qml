import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: page

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height + Theme.paddingLarge

        Column {
            id: column
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.leftMargin: Theme.paddingLarge
            anchors.rightMargin: Theme.paddingLarge

            PageHeader { title: qsTr("About") }

            Label {
                width: parent.width
                wrapMode: Text.Wrap
                font.pixelSize: Theme.fontSizeSmall
                horizontalAlignment: Text.AlignHCenter
                color: Theme.primaryColor
                textFormat: Text.RichText;
                onLinkActivated: Qt.openUrlExternally(link)
                text: "<h1>SailKino</h1>v"+APP_VERSION+"<br /><br />" +

                      "<style>" +
                      ".legend { font-size: " + Theme.fontSizeExtraSmall + "px;  }" +
                      "a:link { color: " + Theme.highlightColor + "; }" +
                      "</style>" +

                      "<p><span class=\"legend\">Created by</span><br />Olli-Pekka Heinisuo<br /><br />" +
                      "<span class=\"legend\">Icon and cover image by</span><br />Juho Heinisuo</p>" +

                      "<p>SailKino is an unofficial Finnkino and Forum Cinemas client application.<br /><br />" +
                      "The content of the application is provided via Finnkino and Forum Cinemas XML API." +
                      "Authors of SailKino are not responsible for any invalid content.</p>" +

                      "<p>This software is released under MIT license.<br />" +
                      "You can get the code and contribute at:\n" +
                      "<style></style><br />" +
                      "<a href='http://github.com/skvark/SailKino'>GitHub \\ SailKino</a></p>";
            }
        }
    }
}

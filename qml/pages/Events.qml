import QtQuick 2.0
import Sailfish.Silica 1.0

Item {

    height: menuView.height;
    width: menuView.width

    function setModel(model, title) {
        listview.model = model
        pgheader = title
    }

    property string pgheader: "";

    SilicaListView {

        id: listview

        header: Component {
            PageHeader {
                title: pgheader
            }
        }


        PullDownMenu {
            id: menu
            MenuItem {
                text: "About"
            }
            MenuItem {
                text: "Area"
                onClicked: {
                    pageStack.push("SelectArea.qml");
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
                            height: parent.height
                            width: parent.width
                            anchors.left: parent.left;
                            anchors.right: parent.right;
                            textFormat: Text.RichText
                            text: title
                            anchors.leftMargin: 15
                            font.pixelSize: Theme.fontSizeMedium
                            wrapMode: Text.WordWrap
                            verticalAlignment: Text.AlignVCenter
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
                        anchors.topMargin: 20
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
}


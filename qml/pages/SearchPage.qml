import QtQuick 2.0
import Sailfish.Silica 1.0

Page {

    id: searchPage
    property string searchString
    property string searchType

    onSearchStringChanged: getSortedItems()
    Component.onCompleted: {
        searchField.placeholderText = qsTr("Start typing title...");
        searchType = "title";
        getSortedItems();
    }

    SilicaFlickable {

        id: flick
        anchors.fill: parent

        PullDownMenu {
            id: menu
            MenuItem {
                text: (searchType === "title" ? qsTr("Search by Genre") : qsTr("Search by Title"))
                onClicked: {
                    searchField.placeholderText = (searchType === "title" ? qsTr("Start typing genre...") : qsTr("Start typing title..."))
                    searchField.text = "";
                    searchType = (searchType === "title" ? "genre" : "title");
                }
            }
        }

        Column {
            id: headerContainer
            width: parent.width

            PageHeader {
                title: qsTr("Events")
            }

            SearchField {
                id: searchField
                width: parent.width

                Binding {
                    target: searchPage
                    property: "searchString"
                    value: searchField.text.toLowerCase().trim()
                }
            }
        }

        SilicaListView {

            id: listview
            currentIndex: -1
            model: filteredModel
            width: parent.width
            anchors.top: headerContainer.bottom
            anchors.bottom: parent.bottom
            clip: true

            delegate: ListItem {
                height: titlelabel.height + genreLabel.height + 2*Theme.paddingSmall
                contentHeight: height
                Label {
                    id: titlelabel
                    anchors {
                        top: parent.top
                        left: parent.left
                        right: parent.right
                        topMargin: Theme.paddingSmall
                        leftMargin: Theme.paddingLarge
                        rightMargin: Theme.paddingLarge
                    }
                    text: title
                    font.pixelSize: Theme.fontSizeSmall
                    maximumLineCount: 1
                    truncationMode: TruncationMode.Fade
                }

                Label {
                    id: genreLabel
                    anchors {
                        top: titlelabel.bottom
                        left: parent.left
                        right: parent.right
                        leftMargin: Theme.paddingLarge
                        rightMargin: Theme.paddingLarge
                        bottomMargin: Theme.paddingSmall
                    }

                    font.pixelSize: Theme.fontSizeSmall
                    textFormat: Text.RichText
                    text: {
                        kinoAPI.setID(id);
                        return kinoAPI.getEvent.getGenres();
                    }
                    opacity: 0.5
                    maximumLineCount: 1
                    truncationMode: TruncationMode.Fade
                }

                BackgroundItem {
                    id: eventitem
                    anchors.fill: parent
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("SingleEvent.qml"), { id: id, comingsoonmodel: false })
                    }
                }
            }
            VerticalScrollDecorator { flickable: listview }
        }
    }

    ListModel { id: filteredModel }

    property var events: kinoAPI.inTheatres

    function getSortedItems() {

        // Clear the auxiliary model
        filteredModel.clear();

        // Add items to the auxiliary model
        for (var i = 0; i < events.count(); i++) {

            var compare;
            var event = events.get(i);

            if (searchType === "title") {
                compare = event.title.toLowerCase();
            } else {
                kinoAPI.setID(event.id);
                compare = kinoAPI.getEvent.getGenres().toLowerCase();
            }

            if (searchString == "" || compare.indexOf(searchString, 0) >= 0) {
                filteredModel.append(event);
            }
        }

    }
}

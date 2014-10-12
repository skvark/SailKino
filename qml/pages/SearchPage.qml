import QtQuick 2.0
import Sailfish.Silica 1.0

Page {

    id: searchPage
    property string searchString
    property string searchType

    onSearchStringChanged: getSortedItems()
    Component.onCompleted: {
        searchField.placeholderText = "Start typing title...";
        searchType = "title";
        getSortedItems();
    }

    SilicaFlickable {

        id: flick
        width: parent.width
        height: headerContainer.height

        PullDownMenu {
            id: menu
            MenuItem {
                text: "Search by Genre"
                onClicked: {
                    searchType = "genre";
                    searchField.placeholderText = "Start typing genre..."
                }
            }
            MenuItem {
                text: "Search by Title"
                onClicked: {
                    searchType = "title";
                    searchField.placeholderText = "Start typing title..."
                }
            }
        }

        Column {
            id: headerContainer
            width: searchPage.width

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
            anchors.topMargin: 70
            height: 600

            delegate: ListItem {

                Label {
                    id: titlelabel
                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: searchField.textLeftMargin
                        topMargin: 15
                    }
                    textFormat: Text.RichText
                    text: title
                }

                Label {
                    anchors {
                        left: parent.left
                        right: parent.right
                        top: titlelabel.bottom
                        leftMargin: searchField.textLeftMargin
                    }

                    font.pixelSize: 20
                    textFormat: Text.RichText
                    text: {
                        kinoAPI.setID(id);
                        return kinoAPI.getEvent.getGenres();
                    }
                    color: Theme.rgba(Theme.secondaryColor, 0.5)
                }

                BackgroundItem {
                    id: eventitem
                    anchors.fill: parent
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("SingleEvent.qml"), { id: id })
                    }
                }
            }
        }
        VerticalScrollDecorator { flickable: listview }
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

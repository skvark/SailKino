import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailkino.eventsmodel 1.0

Page {

    id: page

    Component.onCompleted: {
        if(kinoAPI.getAreas().length !== 0 ){
            loading = false
            listView.model = kinoAPI.getAreas()
            listView.currentIndex = listView.model.indexOf(kinoAPI.getAreaName())
        } else {
            loading = false
            holder = true
            listView.currentIndex = -1
        }
    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating) {
            if (_navigation === PageNavigation.Back) {
                kinoAPI.clearModels()
                kinoAPI.init()
            }
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        running: loading
    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: childrenRect.height

        PullDownMenu {
            id: menu

            MenuItem {
                text: "About"
                onClicked: pageStack.push("About.qml")
            }

            MenuItem {
                text: "Country & Language"
                onClicked: {
                    var dialog = pageStack.push("SelectLocationLanguage.qml")
                    dialog.accepted.connect(function() {
                        areaModel.clear()
                    })
                }
            }
        }

        Column {
            id: headerContainer
            width: page.width

            PageHeader {
                title: qsTr("Settings")
            }
        }

        SectionHeader {
            id: generalsection
            text: qsTr("General")
            anchors.top: headerContainer.bottom
        }

        TextSwitch {
            id: filterSwitch
            anchors.top: generalsection.bottom
            text: "Filter events"
            description: "Filters events which have no shows for selected date."
            automaticCheck: true
            checked: kinoAPI.getFilterState()
            onCheckedChanged: {
                kinoAPI.setFilterState(checked)
            }
        }

        SectionHeader {
            id: areasection
            text: qsTr("Area")
            anchors.top: filterSwitch.bottom
        }

        SilicaListView {

            id: listView
            height: Screen.height - areasection.y - areasection.height
            width: parent.width
            anchors.top: areasection.bottom
            anchors.leftMargin: Theme.paddingMedium
            currentIndex: -1
            clip: true

            ViewPlaceholder {
                verticalOffset: -300
                id: pholder
                enabled: loading === false && holder === true
                text: qsTr("You haven't selected country and language yet. Please select them via the pulley menu.")
            }

            delegate: ListItem {

                id: delegate
                height: Theme.ItemSizeMedium
                width: parent.width
                highlighted: listView.currentIndex == index

                Label {
                    id: label2
                    anchors.fill: parent
                    text: modelData
                    anchors.leftMargin: Theme.paddingLarge
                    verticalAlignment: Text.AlignVCenter
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                onClicked: {
                    listView.currentIndex = index
                    current = modelData
                    save()
                }

            }
            VerticalScrollDecorator { flickable: listView }
        }

    }

    ListModel {
        id: areaModel
    }

    property bool loading: true
    property bool holder: false
    property string current

    function save() {
        kinoAPI.saveArea(current)
        kinoAPI.clearModels()
    }

    Connections {
        target: kinoAPI
        onAreas: {
            loading = false
            holder = false
            listView.model = kinoAPI.getAreas()
            listView.currentIndex = listView.model.indexOf(kinoAPI.getAreaName())
        }
    }
}

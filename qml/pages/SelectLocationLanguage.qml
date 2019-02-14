import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailkino.eventsmodel 1.0

Dialog {

    id: page

    onAccepted: {
        save();
    }

    onRejected: {
        kinoAPI.resetLanguage();
        kinoAPI.saveLocation(old_country);
        kinoAPI.saveLanguage(old_lang);
    }

    Component.onCompleted: {
        old_country = kinoAPI.getCountryName();
        old_lang = kinoAPI.getLanguageName();
        kinoAPI.resetLanguage();
        for(var i = 0; i < kinoAPI.getLocations().length; ++i) {
            if(old_country === kinoAPI.getLocations()[i]) {
                combo.currentIndex = i;
                kinoAPI.resetLanguage();
                kinoAPI.saveLocation(old_country);
                break;
            }
        }
        listView.currentIndex = -1;
    }

    SilicaFlickable {
       id: flickable
       anchors.fill: parent
       contentWidth: flickable.width

        Column {
            id: locations
            anchors {
               left: parent.left
               right: parent.right
            }

            DialogHeader {
               id: header
               acceptText: qsTr("Select")
            }

            ComboBox {
               id: combo
               label: qsTr("Country")
               description: qsTr("Select country to see available languages.")
               currentIndex: 0
               width: parent.width

               menu: ContextMenu {
                    id: combocontent
                    Repeater {
                        model: kinoAPI.getLocations()
                        delegate: MenuItem {
                            text: model.modelData
                            onClicked: {
                                current_country = modelData;
                                kinoAPI.resetLanguage();
                                kinoAPI.saveLocation(current_country);
                                listView.currentIndex = -1;
                            }
                        }
                    }
                }

            }
        }

        SilicaListView {

            id: listView
            anchors {
               top: locations.bottom
               left: parent.left
               right: parent.right
               bottom: parent.bottom
               leftMargin: Theme.paddingMedium
               topMargin: Theme.paddingMedium
            }

            delegate: ListItem {

                id: delegate
                height: Theme.ItemSizeMedium
                width: parent.width
                highlighted: listView.currentIndex == index;
                visible: !loading

                Label {
                    id: label
                    anchors.fill: parent
                    text: modelData
                    anchors.leftMargin: Theme.paddingMedium
                    verticalAlignment: Text.AlignVCenter
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                onClicked: {
                    listView.currentIndex = index;
                    current_lang = modelData
                }
            }

            VerticalScrollDecorator { flickable: listView }

            BusyIndicator {
                anchors.centerIn: parent
                size: BusyIndicatorSize.Large
                running: loading
            }
        }

    }

    property bool loading: false;
    property string current_lang;
    property string current_country;
    property string old_lang;
    property string old_country;

    function save() {
        kinoAPI.saveLanguage(current_lang);
    }

    Connections {
        target: kinoAPI
        onLanguagesLoading: {
            loading = true;
        }
        onLanguagesReady: {
            loading = false;
            listView.model = langs;
            if(langs.length > 0) {
                current_lang = langs[0];
                listView.currentIndex = 0;
            }
        }
    }
}

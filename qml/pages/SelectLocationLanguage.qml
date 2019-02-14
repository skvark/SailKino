import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailkino.eventsmodel 1.0

Dialog {

    id: page

    canAccept: listView.currentIndex > -1

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
                combo1.currentIndex = i;
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
               id: combo1
               label: qsTr("Country")
               description: qsTr("Select country to see available languages.")
               currentIndex: 0
               width: parent.width

               menu: ContextMenu {
                    id: combocontent
                    Repeater {
                        model: kinoAPI.getLocations()
                        delegate: MenuItem {
                            text: countryTr[model.modelData]
                            onClicked: {
                                current_country = modelData;
                                kinoAPI.resetLanguage();
                                kinoAPI.saveLocation(current_country);
                                listView.currentIndex = -1;
                            }
                            Component.onCompleted: console.log("Country:", model.modelData, countryTr[model.modelData])
                        }
                    }
                }

            }
            ComboBox {
                id: combo2
                label: qsTr("Language")
                description: qsTr("Select language from the list below.")
                visible: !loading

                MouseArea {
                    anchors.fill: parent
                }
            }
        }

        SilicaListView {

            id: listView
            onCurrentIndexChanged: console.log("CurrInd",currentIndex)
            anchors {
               top: locations.bottom
               left: parent.left
               right: parent.right
               bottom: parent.bottom
            }

            BusyIndicator {
                anchors.centerIn: parent
                size: BusyIndicatorSize.Large
                running: loading
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
                    text: languageTr[modelData]
                    anchors {
                        leftMargin: Theme.horizontalPageMargin
                    }
                    verticalAlignment: Text.AlignVCenter
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                onClicked: {
                    listView.currentIndex = index;
                    current_lang = modelData
                }
                Component.onCompleted: console.log("Language:", model.modelData, languageTr[modelData])
            }

            VerticalScrollDecorator { flickable: listView }

        }

    }



    property bool loading: false;
    property string current_lang;
    property string current_country;
    property string old_lang;
    property string old_country;

    property variant countryTr: {
        "Finland": qsTr("Finland", "Country"),
                "Estonia": qsTr("Estonia", "Country"),
                "Latvia": qsTr("Latvia", "Country"),
                "Lithuania": qsTr("Lithuania", "Country")
    }

    property variant languageTr: {
        "English": qsTr("English", "Language"),
                "Finnish": qsTr("Finnish", "Language"),
                "Estonian": qsTr("Estonian", "Language"),
                "Latvian": qsTr("Latvian", "Language"),
                "Lithuanian": qsTr("Lithuanian", "Language"),
                "Russian": qsTr("Russian", "Language")
    }

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
            listView.currentIndex = -1;
        }
    }
}

import QtQuick 2.0
import Sailfish.Silica 1.0
import CPPIntegrate 1.0

Dialog {

    id: page
    width: 540
    onAccepted: { save(); }
    onOpened: {
        if(kinoAPI.getAreas().length !== 0 ){
            loading = false;
            listView.model = kinoAPI.getAreas();
            listView.currentIndex = listView.model.indexOf(kinoAPI.getAreaName());
        } else {
            listView.currentIndex = -1;
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

        DialogHeader {
            id: header
            acceptText: "Tallenna"
        }

        SilicaListView {

            id: listView
            anchors.top: header.bottom
            height: 800
            width: page.width;
            anchors.leftMargin: Theme.paddingMedium
            anchors.topMargin: Theme.paddingMedium
            model: areaModel

            delegate: ListItem {

                id: delegate
                height: Theme.ItemSizeMedium
                width: parent.width
                highlighted: listView.currentIndex == index;

                Label {
                    id: label
                    height: Theme.ItemSizeMedium
                    width: parent.width
                    anchors.left: parent.left
                    text: modelData
                    anchors.leftMargin: Theme.paddingMedium
                    verticalAlignment: Text.AlignVCenter
                    color: delegate.highlighted ? Theme.highlightColor : Theme.primaryColor
                }

                onClicked: {
                    listView.currentIndex = index;
                    current = modelData
                }

            }
            VerticalScrollDecorator { flickable: listView }
        }

    }

    ListModel {
        id: areaModel
    }

    property bool loading: true;
    property string current;

    function save() {
        kinoAPI.saveArea(current);
        kinoAPI.clearModels();
        kinoAPI.init()
    }

    Connections {
        target: kinoAPI
        onAreas: {
            loading = false;
            areaModel = kinoAPI.getAreas();
        }
    }
}

import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailkino.eventsmodel 1.0

Page {
    id: page
    Component.onCompleted: init()

    BusyIndicator {
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        running: loading
    }

    SilicaFlickable {
        enabled: pholder.enabled
        visible: enabled

        width: parent.width

        ViewPlaceholder {
            id: pholder
            enabled: loading == false && holder == true
            text: qsTr("Please select country, language and area in the settings to see events.")
        }
    }

    SlideshowView {

        id: menuView
        itemWidth: width
        itemHeight: height
        height: page.height - footer.visibleHeight
        clip: true

        anchors {
            top: parent.top;
            left: parent.left;
            right: parent.right
        }

        model: VisualItemModel {
                id: movieViews
                Events { id: inSelectedTheatre }
                Events { id: comingSoon }
        }

        onCurrentIndexChanged: {
            if (currentIndex === 0) {
                dockrectangle1.state = "visible";
                dockrectangle2.state = "hidden";
            } else {
                dockrectangle1.state = "hidden";
                dockrectangle2.state = "visible";
            }
        }

    }

    Item {
        id: footer
        property int visibleHeight: footercontent.contentY + height
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        height: 3*Theme.fontSizeSmall + 1.25*Theme.fontSizeExtraSmall + 3*Theme.paddingSmall

        SilicaFlickable {

            enabled: !pholder.enabled

            id: footercontent
            anchors.fill: parent
            contentHeight: parent.height

            Image {
               id: background
               fillMode: Image.PreserveAspectCrop
               anchors.fill: parent
               source: "image://theme/graphic-header"
            }

            Row {
                id: nav
                height: 3*Theme.fontSizeSmall
                width: parent.width

                BackgroundItem {

                    id: dockbackground1
                    width: footercontent.width / 2
                    height: parent.height

                    Label {
                        id: inTeathersLabel
                        anchors.centerIn: parent
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.highlightColor
                        text: qsTr("In Theatres")
                    }

                    onClicked: if (menuView.currentIndex !== 0 ) {
                                    menuView.incrementCurrentIndex()
                               }

                    Rectangle {
                        id: dockrectangle1
                        anchors.centerIn: parent
                        height: 1.5*inTeathersLabel.height
                        width: Math.max(inTeathersLabel.width, inTeathersLabel.width) * 1.5
                        color: Theme.rgba(Theme.highlightColor, 0.0)
                        radius: Theme.paddingSmall
                        anchors.margins: Theme.paddingMedium
                        visible: !pholder.enabled

                        states: [
                            State { name: "visible";
                                PropertyChanges {
                                    target: dockrectangle1;
                                    color: Theme.rgba(Theme.highlightColor, 0.3)
                                }
                            },
                            State { name: "hidden";
                                PropertyChanges {
                                    target: dockrectangle1;
                                    color: Theme.rgba(Theme.highlightColor, 0.0)
                                }
                            }
                        ]
                        transitions: Transition {
                            ColorAnimation { property: "color"; duration: 200}
                        }
                    }
                }

                BackgroundItem {

                    id: dockbackground2
                    width: parent.width / 2
                    height: parent.height

                    Label {
                        id: comingSoonLabel
                        anchors.centerIn: parent
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.highlightColor
                        text: qsTr("Coming Soon")
                    }

                    onClicked: if (menuView.currentIndex !== 1 ) {
                                    menuView.incrementCurrentIndex()
                               }

                    Rectangle {
                        id: dockrectangle2
                        anchors.centerIn: parent
                        height: comingSoonLabel.height * 2
                        width: Math.max(inTeathersLabel.width, inTeathersLabel.width) * 1.5
                        radius: Theme.paddingSmall;
                        color: Theme.rgba(Theme.highlightColor, 0.0)
                        anchors.margins: Theme.paddingMedium
                        visible: !pholder.enabled

                        states: [
                            State {
                                name: "visible";
                                PropertyChanges {
                                    target: dockrectangle2;
                                    color: Theme.rgba(Theme.highlightColor, 0.4)
                                }
                            },
                            State {
                                name: "hidden";
                                PropertyChanges {
                                    target: dockrectangle2;
                                    color: Theme.rgba(Theme.highlightColor, 0.0)
                                }
                            }
                        ]
                        transitions: Transition {
                            ColorAnimation { property: "color"; duration: 200}
                        }
                    }
                }
            }

            Row {
                anchors.top: nav.bottom
                width: parent.width
                height: 1.25*Theme.fontSizeExtraSmall + 3*Theme.paddingSmall
                Label {
                    id: locationdate
                    width: parent.width
                    height: 1.25*Theme.fontSizeExtraSmall
                    font.pixelSize: Theme.fontSizeExtraSmall
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    text: {
                        if (kinoAPI.getAreaName().length !== 0) {
                            return kinoAPI.getAreaName() + " — " + kinoAPI.getDate().toDateString();
                        } else {
                            return kinoAPI.getDate().toDateString();
                        }
                    }
                    wrapMode: Text.NoWrap
                    elide: Text.ElideMiddle
                }
            }

            PushUpMenu {
                id: menu

                visible: !pholder.enabled
                enabled: !pholder.enabled

                MenuItem {
                    text: qsTr("Change Date")

                    onClicked: {
                        var dialog = pageStack.push(pickerComponent, {
                            date: kinoAPI.getDate()
                        })
                        dialog.accepted.connect(function() {
                            inSelectedTheatre.visible = false;
                            comingSoon.visible = false;
                            kinoAPI.setDate(dialog.date);
                        })
                    }

                    Component {
                        id: pickerComponent
                        DatePickerDialog {}
                    }
                }

                MenuItem {
                    text: qsTr("Search")
                    onClicked: {
                        pageStack.push("SearchPage.qml");
                    }
                }

                MenuItem {
                    text: qsTr("Settings")
                    onClicked: {
                        filterdatetimer.stop();
                        pageStack.push("Settings.qml");
                    }
                }
            }
        }
    }

    property bool loading: false;
    property bool holder: false;
    property string info;
    property date initialDate: new Date();

    function init() {

        dockrectangle1.state = "visible";
        dockrectangle2.state = "hidden";

        if (kinoAPI.areaSelectedEarlier()) {
            holder = false
            kinoAPI.init();
        } else {
            holder = true
        }
    }

    Timer {
        id: filterdatetimer
        // 15 min interval
        interval: 900000;
        running: false;
        repeat: true;
        onTriggered: {

            // Update if date changes
            var curDate = new Date().toDateString();
            if (initialDate.toDateString() < curDate) {
                initialDate = new Date();
                kinoAPI.setDate(initialDate);
            }

            // filter inTheatres if date is today
            if(initialDate.toDateString() === kinoAPI.getDate().toDateString() && kinoAPI.getFilterState()) {
                kinoAPI.reFilter();
                inSelectedTheatre.setModel(kinoAPI.inTheatres,
                                           kinoAPI.getAreaName(),
                                           false);
            }
        }
    }

    Connections {
        target: kinoAPI

        onReady: {
            inSelectedTheatre.setModel(kinoAPI.inTheatres,
                                       kinoAPI.getAreaName(),
                                       false);
            comingSoon.setModel(kinoAPI.comingSoon, qsTr("Coming soon"), true);
            if (kinoAPI.getAreaName().length !== 0) {
                locationdate.text = kinoAPI.getAreaName() + " — " + kinoAPI.getDate().toDateString()
            } else {
                locationdate.text = kinoAPI.getDate().toDateString()
            }
            filterdatetimer.start();
        }

        onLoading: {
            if(yesno) {
                holder = false
                loading = true;
            } else {
                loading = false;
                inSelectedTheatre.visible = true;
                comingSoon.visible = true;
            }
        }

        onDateChanged: {
            if (kinoAPI.getAreaName().length !== 0) {
                locationdate.text = kinoAPI.getAreaName() + " — " + kinoAPI.getDate().toDateString()
            } else {
                locationdate.text = kinoAPI.getDate().toDateString()
            }
        }
    }
    onLoadingChanged: {
        if(loading)
            locationdate.text = kinoAPI.getDate().toDateString()
    }
}



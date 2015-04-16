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
        height: app.height - footer.visibleHeight
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
        anchors { bottom: parent.bottom; left: parent.left; right: parent.right }
        height: 125

        SilicaFlickable {

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
                height: 65

                BackgroundItem {

                    id: dockbackground1
                    width: footercontent.width / 2
                    height: parent.height

                    Label {
                        anchors.fill: parent
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.highlightColor
                        text: "In Theatres"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: if (menuView.currentIndex !== 0 ) {
                                    menuView.incrementCurrentIndex()
                               }

                    Rectangle {
                        id: dockrectangle1
                        anchors.fill: parent
                        color: Theme.rgba(Theme.highlightColor, 0.0)
                        radius: 5;
                        anchors.margins: Theme.paddingMedium

                        states: [
                            State { name: "visible";
                                PropertyChanges {
                                    target: dockrectangle1;
                                    color: Theme.rgba(Theme.highlightColor, 0.4)
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
                    width: footercontent.width / 2
                    height: parent.height

                    Label {
                        anchors.fill: parent
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.highlightColor
                        text: "Coming Soon"
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }

                    onClicked: if (menuView.currentIndex !== 1 ) {
                                    menuView.incrementCurrentIndex()
                               }

                    Rectangle {
                        id: dockrectangle2
                        anchors.fill: parent
                        radius: 5;
                        color: Theme.rgba(Theme.highlightColor, 0.0)
                        anchors.margins: Theme.paddingMedium

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
                height: 60
                anchors.top: nav.bottom

                Item {
                    anchors.margins: Theme.paddingMedium
                    width: footercontent.width
                    height: parent.height

                    Label {
                        id: locationdate
                        anchors.fill: parent
                        horizontalAlignment: Text.AlignHCenter
                        anchors.topMargin: 5
                        textFormat: Text.RichText
                        text: {
                            if (kinoAPI.getAreaName().length !== 0) {
                                return kinoAPI.getAreaName() + " — " + kinoAPI.getDate().toDateString();
                            } else {
                                return kinoAPI.getDate().toDateString();
                            }
                        }
                        wrapMode: Text.Wrap
                        font.pixelSize: 22
                    }
                }
            }

            PushUpMenu {
                id: menu

                MenuItem {
                    text: "Change Date"

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
                    text: "Search"
                    onClicked: {
                        pageStack.push("SearchPage.qml");
                    }
                }

                MenuItem {
                    text: "Settings"
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
            comingSoon.setModel(kinoAPI.comingSoon, "Coming soon", true);
            locationdate.text = kinoAPI.getAreaName() + " — " + kinoAPI.getDate().toDateString();
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
            locationdate.text = kinoAPI.getAreaName() + " — " + kinoAPI.getDate().toDateString();
        }
    }
}



import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0

Page {

    id: videopage
    allowedOrientations: Orientation.All
    property string videourl;

    Component.onCompleted: {
        playVideo.source = videourl;
        playVideo.play();
    }

    SilicaFlickable {

        id: player
        anchors.fill: parent

        ViewPlaceholder {
            id: pholder
            enabled: videourl.length === 0
            text: qsTr("There's no trailer available for this movie.")
        }

        MediaPlayer {
            id: playVideo
        }

        VideoOutput {
            anchors.fill: parent
            source: playVideo

            BusyIndicator {
                anchors.centerIn: parent
                size: BusyIndicatorSize.Large
                running: playVideo.status === MediaPlayer.Loading || playVideo.status === MediaPlayer.Buffering
            }

        }

        BackgroundItem {
            id: video
            anchors.fill: parent
            onClicked: {
                if(playVideo.playbackState === MediaPlayer.PlayingState) {
                    playVideo.pause()
                } else if (playVideo.playbackState === MediaPlayer.PausedState) {
                    playVideo.play()
                }
            }
        }

    }

    Connections {
        target: Qt.application
        onActiveChanged:
            if(!Qt.application.active) {
                playVideo.pause();
            }
    }
}

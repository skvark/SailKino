import QtQuick 2.0
import Sailfish.Silica 1.0
import QtMultimedia 5.0

Page {

    id: videopage
    allowedOrientations: Orientation.All
    property string videourl;
    property string videoerror: ""; 

    Component.onCompleted: {
        playVideo.source = videourl;
        playVideo.play();
    }

    ScreenBlank {
        enabled: playVideo.playbackState === MediaPlayer.PlayingState
    }

    onStatusChanged: {
        if (status === PageStatus.Deactivating) {
            if (_navigation === PageNavigation.Back) {
                playVideo.stop();
            }
        }
    }

    SilicaFlickable {

        id: player
        anchors.fill: parent

        ViewPlaceholder {
            id: pholder
            enabled: videourl.length === 0 || videoerror !== ""
            text: qsTr("There's no trailer available for this movie or the video stream type is not supported.\n\n" + videoerror)
        }

        MediaPlayer {
            id: playVideo
            autoLoad: true

            onError: {
                videoerror = playVideo.errorString;
                playVideo.stop();
            }

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

// Default empty project template
import bb.cascades 1.0
import views 1.0
import com.example.views 1.0

Page {
    Container {
        layout: StackLayout {}
        ViewControl {
            id: playVC
            objectName: "playVC"
            visible: true
            horizontalAlignment: HorizontalAlignment.Fill
            
            attachedObjects: [
            	VideoOverlay {
            		id: videoOverlay
            		
					windowID: "videoOverlay1"
					createWindowGroup: true
					z: -1
					x: 0
					y: 0
					width: 0
					height: 0
					mediaURL: _app.fileURL("app/native/assets/media/VID_00000007.mp4")
					topPadding: 50
					bottomPadding: 50
					enabled: false
            	},
	           	VideoOverlay {
            		id: videoOverlayHDMI
            		
					windowID: "videoOverlayHDMI"
					windowGroup: "play-HDMI"
					createFullWindow: true
					createWindowGroup: true
					display: 4
					z: 0
					x: 0
					y: 0
					width: 0
					height: 0
					mediaURL: _app.fileURL("app/native/assets/media/VID_00000007.mp4")
					topPadding: 50
					bottomPadding: 50
					enabled: false
            	}
            ]
        }
/*        

        Container {
            layout: StackLayout {
                orientation: LayoutOrientation.LeftToRight

            }
        	TextField {
	            id:  videoURL
	            objectName: "videoURL"
	            text: ""
	            hintText: qsTr("Type in a URL")
	            textStyle.base: SystemDefaults.TextStyles.BigText
	            //verticalAlignment: VerticalAlignment.Center
	            horizontalAlignment: HorizontalAlignment.Fill
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 2 
                }
	        }
	        
	        Button {
                text: qsTr("play")
                onClicked: {
	                _video.mediaURL = videoURL.text;
	                _video.play();
	            }
                layoutProperties: StackLayoutProperties {
                    spaceQuota: 1 
                }
            }
	    }
*/
    }
    actions: [
        ActionItem {
            title: "Connect to MiraCast"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
//              _app.miracastShowOn();
                _app.miracastPlayOn();
            }
        }
    ]
    actionBarVisibility: ChromeVisibility.Visible

}


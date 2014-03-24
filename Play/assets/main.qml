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
            
            onTouch: {
            	videoOverlay.play();
            }
            
            attachedObjects: [
            	VideoOverlay {
            		id: videoOverlay
            		
					windowID: "videoOverlay1"
					createWindowGroup: true
					z: -2
					x: 0
					y: 0
					width: 0
					height: 0
					//mediaURL: _app.fileURL("app/native/assets/media/VID_00000007.mp4")
					mediaURL: _app.fileURL("app/native/assets/media/Demo_1080i_CC.ts")
					//mediaURL: _app.fileURL("app/native/assets/media/waterfallVideo_CC.mov")
					//captionFileURL: _app.fileURL("app/native/assets/media/waterfall_sample.ttml")
					showCaptions: true
					
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
					//mediaURL: _app.fileURL("app/native/assets/media/VID_00000007.mp4")
					mediaURL: _app.fileURL("app/native/assets/media/Demo_1080i_CC.ts")
					//mediaURL: _app.fileURL("app/native/assets/media/waterfallVideo_CC.mov")
					//captionFileURL: _app.fileURL("app/native/assets/media/waterfall_sample.ttml")
					topPadding: 50
					bottomPadding: 50
					enabled: false
            	}
/*        
				    	Canvas {
				    		id: workCanvas
				           	objectName: "workCanvas"
				
				            visible: true
				            horizontalAlignment: HorizontalAlignment.Fill
				
							property real x1: 0.0
							property real y1: 0.0
							property real length: 400.0
							property real angle:  45.0
							property real red:  1.0
							property real green:  0.0
							property real blue:  0.0
							property real alpha:  1.0
							property int defaultStrokeID:  0
							property bool clockwise: true
										
							onUpdated: {
								if (reset()) {
									if (defaultStrokeID == 0) {
										defaultStrokeID = getStroke();
									}
									
									setColor(Color.create(red, green, blue, alpha));
									drawLine(x1, y1, x1 + length * Math.cos(angle * Math.PI / 180.0), y1 + length * Math.sin(angle * Math.PI / 180.0));
									//console.log("angle: " + angle + " : " + (x1 + length * Math.cos(angle * Math.PI / 180.0)) + " : " + (y1 + length * Math.sin(angle * Math.PI / 180.0)));
								
									if (clockwise) {
										angle -= 0.25;
										if (angle < 0.0) {
											angle = 0.0;
											clockwise = false;
										}
									} else {
										angle += 0.25;
										if (angle > 90.0) {
											angle = 90.0;
											clockwise = true;
										}
									}
									
									done();
								}
							}
							
							onRegenerated: {
								if (reset()) {
									if (defaultStrokeID == 0) {
										defaultStrokeID = getStroke();
									}
									
									var dashArray = new Array();
									dashArray[0] = 10.0;
									dashArray[1] = 10.0;
									//var dashedStrokeID = createStroke(5.0, StrokeCap.CAP_NONE, StrokeJoin.JOIN_NONE, 0.0, dashArray, 2, 0.0); 
									var dashedStrokeID = createStroke(10.0, 0, 0, 0.0, dashArray, 2, 0.0); 
								
									setColor(Color.Red);
									drawLine(0, 0, 300, 300);
								
									setColor(Color.Blue);
									setStroke(dashedStrokeID);
									drawOval(400.0, 300.0, 200.0, 100.0);
								
									done();
								}
							}
						
						}
*/
            	
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
        	imageSource: "asset:///images/actions/ic_play.png"
            title: "Play"
            ActionBar.placement: ActionBarPlacement.OnBar
            onTriggered: {
	                //videoOverlay.seek(0);
	                //videoOverlay.play();
            }
        },
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
		        
		onCreationCompleted: {
	    	OrientationSupport.supportedDisplayOrientation =
	        SupportedDisplayOrientation.DisplayLandscape; 
	    }

}


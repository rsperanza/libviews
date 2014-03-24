import bb.cascades 1.0
import views 1.0
import views.graphs 1.0

TabbedPane {
    id: tabbedPane
    showTabsOnActionBar: true

	property variant colorRed:          Color.create( 1.0, 0.0, 0.0, 1.0 )
	property variant colorRedShaded:    Color.create( 0.75, 0.0, 0.0, 1.0 ) 
	property variant colorGreen:        Color.create( 0.0, 1.0, 0.0, 1.0 )
	property variant colorGreenShaded:  Color.create( 0.0, 0.75, 0.0, 1.0 )
	property variant colorBlue:         Color.create( 0.0, 0.0, 1.0, 1.0 )
	property variant colorBlueShaded:   Color.create( 0.0, 0.0, 0.75, 1.0 )
	property variant colorYellow:       Color.create( 1.0, 1.0, 0.0, 1.0 )
	property variant colorYellowShaded: Color.create( 1.0, 0.75, 0.0, 1.0 )
	property variant colorViolet:       Color.create( 1.0, 0.0, 1.0, 1.0 )
	property variant colorVioletShaded: Color.create( 0.75, 0.0, 0.75, 1.0 )
	property variant colorCyan:         Color.create( 0.0, 1.0, 1.0, 1.0 )
	property variant colorCyanShaded:   Color.create( 0.0, 0.75, 0.75, 1.0 )				            		

    
    Tab {
		title: "Graphs and Charts"
          
        content: 
			Page {
				id: graphsCharts
				
			    Container {
			        layout: StackLayout {}
				        ViewControl {
				            id: workVC
				            objectName: "workVC"
				            visible: true
				            horizontalAlignment: HorizontalAlignment.Fill
				            
				            attachedObjects: [
				            	PieChart {
				            		id: pieChart
				            		
									windowID: "pieChart1"
									z: -1
									x: 0
									y: 0
									width: 0
									height: 0
									enabled: false
									
									leftPadding: 10
									rightPadding: 10
									topPadding: 10
									bottomPadding: 10
									
									showCaption: true
									caption: "Sample Pie Chart"
									captionFont: "/usr/fonts/font_repository/monotype/georgiab.ttf"
									captionFontSize: 13
									subCaption: "for today"
									subCaptionFont: "/usr/fonts/font_repository/monotype/ariali.ttf"
									subCaptionFontSize: 10
									captionPlacement: "top"
									captionAlignment: "center"
				            			
				            		showLegend: true
				            		showLegendTitle: true
									legendTitleFont: "/usr/fonts/font_repository/monotype/SlatePro.ttf"
									legendTitleFontSize: 8
									legendFont: "/usr/fonts/font_repository/monotype/SlatePro-Light.ttf"
									legendFontSize: 8
				            		legendPlacement: "bottom"
				            		
 				            		showSliceLabel: true
									sliceLabelFont: "/usr/fonts/font_repository/monotype/SlatePro-Light.ttf"
									sliceLabelFontSize: 8
				            		sliceLabelPlacement: "vertical"
				            						            		
 				            		itemLabels: [ "item 1", "item 2", "item 3", "item 4", "item 5", "item 6" ]
				            		itemValues: [ 16.666, 13.888, 8.333, 19.444, 5.555, 36.144 ]
				            		itemColors: [ 
				            				colorRed, 
				            				colorRedShaded, 
				            				colorGreen, 
				            				colorGreenShaded, 
				            				colorBlue, 
				            				colorBlueShaded, 
				            				colorYellow, 
				            				colorYellowShaded, 
				            				colorViolet, 
				            				colorVioletShaded, 
				            				colorCyan,
				            				colorCyanShaded
				            			]
				            	},
				            	LineGraph {
				            		id: lineGraph
				            		
				            		display: 4
									windowID: "lineGraph1"
									z: -1
									x: 0
									y: 0
									width: 0
									height: 0
									enabled: false
				            	}
				            	
				            ]
				        }
				    }
				    actions: [
				        ActionItem {
				            title: "Connect to MiraCast"
				            ActionBar.placement: ActionBarPlacement.OnBar
				            onTriggered: {
				              	//_app.miracastShowOn();
				                _app.miracastPlayOn();
				            }
				        }
				    ]
				
				}
    } // end of first Tab
       
    Tab {
		title: "Forms"
          
        content: 
		Page {
		    id: form
		    objectName: "form"
		    
		    titleBar: 
		    TitleBar {
		        title: qsTr("New Message")
		    }
		        
		    Container {
		        id: container
		        objectName: "container"
		        
		        layout: StackLayout {
		            
		        }
		        Container {
		            id: fromContainer
		            objectName: "fromContainer"
		            
		            leftPadding: 10
		            rightPadding: 10
		            topPadding: 10
		            bottomPadding: 10
		            
		            layout: StackLayout {
		                orientation: LayoutOrientation.LeftToRight
		            }
		            
		            Container {
		                id: fromLabelContainer
		                objectName: "fromLabelContainer"
		                
		                leftPadding: 10
		                rightPadding: 10
		                topPadding: 10
		                bottomPadding: 10
		                
		                layout: DockLayout {
		                }
		                
			            Label {
			                id: fromLabel
			                objectName: "fromLabel"
			                
		                    leftMargin: 5
		                    rightMargin: 5
		                    
		                    horizontalAlignment: HorizontalAlignment.Right
		                    verticalAlignment: verticalAlignment.Center
		                    
			                // Localized text with the dynamic translation and locale updates support
			                text: qsTr("From: ") + Retranslate.onLocaleOrLanguageChanged
			                textStyle.base: SystemDefaults.TextStyles.NormalText
			                textStyle.fontWeight: FontWeight.Bold
			            }
			        }
		            Label {
		                id: fromName
		                objectName: "fromName"
		                
		                // Localized text with the dynamic translation and locale updates support
		                text: qsTr("test@example.com") + Retranslate.onLocaleOrLanguageChanged
		                textStyle.base: SystemDefaults.TextStyles.NormalText
		            }
		        }
		        Container {
		            id: subjectContainer
		            objectName: "subjectContainer"
		            
		            leftPadding: 10
		            rightPadding: 10
		            topPadding: 10
		            bottomPadding: 10
		            
		            layout: StackLayout {
		                orientation: LayoutOrientation.LeftToRight
		            }
		            
		            Container {
		                id: subjectLabelContainer
		                objectName: "subjectLabelContainer"
		                
		                leftPadding: 10
		                rightPadding: 10
		                topPadding: 10
		                bottomPadding: 10
		                
		                layout: DockLayout {
		                }
		                
			           Label {
			                id: subjectLabel
			                objectName: "subjectLabel"
			                                                
			                leftMargin: 5
			                rightMargin: 5
			                
		                    horizontalAlignment: HorizontalAlignment.Right
			                verticalAlignment: verticalAlignment.Center
			                
			                // Localized text with the dynamic translation and locale updates support
			                text: qsTr("Subject: ") + Retranslate.onLocaleOrLanguageChanged
			                textStyle.base: SystemDefaults.TextStyles.NormalText
			                textStyle.fontWeight: FontWeight.Bold
			            }
			        }
		        	TextArea {
		                id: subject
		                objectName: "subject"
		                enabled: false
		                
		                // Localized text with the dynamic translation and locale updates support
		                text: qsTr("") + Retranslate.onLocaleOrLanguageChanged
		                textStyle.base: SystemDefaults.TextStyles.NormalText
		            }
		        }
		        Container {
		            id: bodyContainer
		            objectName: "bodyContainer"
		            
		            leftPadding: 10
		            rightPadding: 10
		            topPadding: 10
		            bottomPadding: 10
		            
		            preferredHeight: 400
		            
		            layout: StackLayout {
		                orientation: LayoutOrientation.TopToBottom
		            }
		            
		            TextArea {
		                id: body
		                objectName: "body"
		                enabled: false
		                
		                horizontalAlignment: HorizontalAlignment.Fill
		                verticalAlignment: VerticalAlignment.Fill
		                
		                // Localized text with the dynamic translation and locale updates support
		                text: qsTr("") + Retranslate.onLocaleOrLanguageChanged
		                textStyle.base: SystemDefaults.TextStyles.NormalText
		            }
		        }
		        Container {
		            id: signatureContainer
		            objectName: "signatureContainer"
		            
		            leftPadding: 10
		            rightPadding: 10
		            topPadding: 10
		            bottomPadding: 10
		            		            
		            layout: StackLayout {
		                orientation: LayoutOrientation.TopToBottom
		            }

                    horizontalAlignment: HorizontalAlignment.Fill
                    
	            	property variant signSheet
	            	property bool signSheetOpen: false
		            
		            Label {
		            	
		                id: clickLabel
		                objectName: "clickLabel"
		                multiline: true
		                
	                    horizontalAlignment: HorizontalAlignment.Center
	                    
		                // Localized text with the dynamic translation and locale updates support
		                text: qsTr("Click Here To Sign") + Retranslate.onLocaleOrLanguageChanged
		                textStyle.base: SystemDefaults.TextStyles.BigText
		                textStyle.fontWeight: FontWeight.Bold
		                
		                onTouch: {
		                	if (event.isDown) {
			                	if (!signatureContainer.signSheetOpen) {
					                signatureContainer.signSheet = signSheetDef.createObject();
					                signatureContainer.signSheet.signatureImage.connect(onSignatureImage);
					                signatureContainer.signSheet.signatureImageFileURL = "";
					                signatureContainer.signSheet.closed.connect(signSheetClosed);
					                signatureContainer.signSheet.open();
					                signatureContainer.signSheetOpen = true;
					                signatureContainer.signSheet.readyForFocus = true;
			                		
			                	}
		                	}
		                }
		            }
		            
		            ImageView {
		                id: signature
		                objectName: "signature"
		                //imageSource: "asset:///../../data/signature.png"
		                preferredHeight: 300
		                
		                onTouch: {
		                	if (event.isDown) {
			                	if (!signatureContainer.signSheetOpen) {
					                signatureContainer.signSheet = signSheetDef.createObject();
					                signatureContainer.signSheet.signatureImage.connect(onSignatureImage);
					                signatureContainer.signSheet.signatureImageFileURL = "";
					                signatureContainer.signSheet.closed.connect(signSheetClosed);
					                signatureContainer.signSheet.open();
					                signatureContainer.signSheetOpen = true;
					                signatureContainer.signSheet.readyForFocus = true;
			                		
			                	}
		                	}
		                }
		                
					    attachedObjects: [
					        ImageTracker {
					            id: tracker
					       
					            onStateChanged: {
					                if (state == ResourceState.Loaded)
					                {
        								console.log("tracker loaded imageSource: " + tracker.imageSource);
					                    signature.image = tracker.image
					                }
					            }
					        }
					    ]
		            }
		            
		            
		            attachedObjects: [
		                ComponentDefinition {
		                    // Component definition for the Sheet that will be shown when pressing the Edit action below
		                    id: signSheetDef
		                    source: "signSheet.qml"
		                }
		            ]
		            
		        }
		    }
     }
    } // end of second Tab
     
 	onActivePaneChanged: {
 		if (activePane == graphsCharts) {
 			console.log("enabling graphs ...");
 			workVC.visible = true;
 		} else if (activePane == form) {
 			workVC.visible = false;
 			console.log("disabling graphs ...");
 		}
 	}
		
    function signSheetClosed() {
        signatureContainer.signSheet.destroy();
        signatureContainer.signSheetOpen = false;
        
    	OrientationSupport.supportedDisplayOrientation =
        SupportedDisplayOrientation.DisplayPortrait; 
    }
		
    function onSignatureImage(imageUrl) {
        console.log("signature Url: " + imageUrl);
        clickLabel.visible = false;
        signature.visible = true;
        signature.resetImage();
        signature.imageSource = imageUrl;
        //tracker.imageSource = imageUrl;
    }
		            
} // end of TabbedPane

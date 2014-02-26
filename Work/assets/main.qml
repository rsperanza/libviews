import bb.cascades 1.0
import views 1.0
import com.example.views 1.0
   
TabbedPane {
    id: tabbedPane
    showTabsOnActionBar: true
    
    Tab {
		title: "Graphs and Charts"
          
        content: 
			Page {
				id: graphsCharts
				
			    Container {
			        layout: StackLayout {}
			
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
									legendTitleFont: "/usr/fonts/font_repository/monotype/SlatePro.ttf"
									legendTitleFontSize: 8
									legendFont: "/usr/fonts/font_repository/monotype/SlatePro-Light.ttf"
									legendFontSize: 8
				            		legendPlacement: "top"
				            		
 				            		showSliceLabel: true
									sliceLabelFont: "/usr/fonts/font_repository/monotype/SlatePro-Light.ttf"
									sliceLabelFontSize: 8
				            		sliceLabelPlacement: "vertical"
				            		
 				            		itemLabels: [ "item 1", "item 2", "item 3", "item 4", "item 5", "item 6" ]
				            		itemValues: [ 0.16666, 0.13888, 0.08333, 0.19444, 0.05555, 0.36144 ]
				            		itemColors: [ 
				            				Color.create( 1.0, 0.0, 0.0, 1.0 ), 
				            				Color.create( 0.75, 0.0, 0.0, 1.0 ), 
				            				Color.create( 0.0, 1.0, 0.0, 1.0 ), 
				            				Color.create( 0.0, 0.75, 0.0, 1.0 ), 
				            				Color.create( 0.0, 0.0, 1.0, 1.0 ), 
				            				Color.create( 0.0, 0.0, 0.75, 1.0 ), 
				            				Color.create( 1.0, 1.0, 0.0, 1.0 ), 
				            				Color.create( 1.0, 0.75, 0.0, 1.0 ), 
				            				Color.create( 1.0, 0.0, 1.0, 1.0 ), 
				            				Color.create( 0.75, 0.0, 0.75, 1.0 ), 
				            				Color.create( 0.0, 1.0, 1.0, 1.0 ),
				            				Color.create( 0.0, 0.75, 0.75, 1.0 )
				            			]
/*
				            				Color.create( 1.0, 0.0, 0.0, 1.0 ), 
				            				Color.create( 0.0, 1.0, 0.0, 1.0 ), 
				            				Color.create( 0.0, 0.0, 1.0, 1.0 ), 
				            				Color.create( 1.0, 1.0, 0.0, 1.0 ), 
				            				Color.create( 1.0, 0.0, 1.0, 1.0 ), 
				            				Color.create( 0.0, 1.0, 1.0, 1.0 )
*/
				            	}
/*            
				            	LineGraph {
				            		id: lineGraph
				            		
									windowID: "lineGraph1"
									z: -1
									x: 0
									y: 0
									width: 0
									height: 0
									enabled: false
				            	}
*/            	
				            ]
				        }
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
					                //signatureContainer.signSheet.editPhotoData = photosPage.selectedData;
					                //console.log("editPhotoData: " + photosPage.editSheet.editPhotoData);
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
					                //signatureContainer.signSheet.editPhotoData = photosPage.selectedData;
					                //console.log("editPhotoData: " + photosPage.editSheet.editPhotoData);
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
    }
		
    function onSignatureImage(imageUrl) {
        console.log("signature Url: " + imageUrl);
        clickLabel.visible = false;
        signature.visible = true;
        signature.resetImage();
        tracker.imageSource = imageUrl;
    }
		            
} // end of TabbedPane

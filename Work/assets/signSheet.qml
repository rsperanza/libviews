/* Copyright (c) 2013 SurF-X Interactive
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import bb.cascades 1.0
import views.controls 1.0

Sheet {
    id: signatureSheet

    property variant readyForFocus: false    
	property string signatureImageFileURL: ""		            

    signal signatureImage(string signatureImageURL)

	peekEnabled: false
	
    content: 
        Page {
        
	        id: signPage
	
	        titleBar: TitleBar {
			    id: editBar
			    title: qsTr("Sign The Message")
			    visibility: ChromeVisibility.Visible
			         
			    dismissAction: ActionItem {
			        title: qsTr("Cancel")
			        onTriggered: {
	                    signatureSheet.close();
			        }
			    }
			         
			    acceptAction: ActionItem {
			        title: qsTr("Save")
			        onTriggered: {
		                signature.saveImage(_app.getAbsoluteDataFilename("signature.png"));
	                }
	            }
	        }

		content:
		        Container {
	            	background: Color.LightYellow
		            	
		            id: signContainer
		            objectName: "signContainer"
		            
		            leftPadding: 10
		            rightPadding: 10
		            topPadding: 10
		            bottomPadding: 10
		            		            
		            layout: StackLayout {
		                orientation: LayoutOrientation.TopToBottom
		            }

                    horizontalAlignment: HorizontalAlignment.Fill
                    
		            Label {
		                id: clickLabel
		                objectName: "clickLabel"
		                multiline: true
		                
	                    horizontalAlignment: HorizontalAlignment.Center
	                    verticalAlignment: VerticalAlignment.Fill
	                    
		                // Localized text with the dynamic translation and locale updates support
		                text: qsTr("Sign in the Space Below") + Retranslate.onLocaleOrLanguageChanged
		                textStyle.base: SystemDefaults.TextStyles.NormalText
		                textStyle.fontWeight: FontWeight.Bold
		            }
		            
		            Divider {}
		            
			        SignaturePad {
			            id: signature
			            objectName: "signature"
			            //visible: false
			            
			            horizontalAlignment: HorizontalAlignment.Fill
			            verticalAlignment: VerticalAlignment.Fill
			            
			            //preferredHeight: 300
			            
			            onSaved: {
			            	console.log("onSaved: " + signatureSheet.signatureImageFileURL);
			            	if (signatureSheet.signatureImageFileURL == "") {
			            		console.log("onSaved: " + _app.getAbsoluteDataFilename("signature.png"));
			            		console.log("onSaved: " + _app.getFileURL(_app.getAbsoluteDataFilename("signature.png")));
			            		signatureSheet.signatureImageFileURL = _app.getFileURL(_app.getAbsoluteDataFilename("signature.png"));
				            	signatureImage(signatureSheet.signatureImageFileURL);
		                    	signatureSheet.close();
		                    }
			            }       
			        }
		            
		        }
		        
		onCreationCompleted: {
	    	OrientationSupport.supportedDisplayOrientation =
	        SupportedDisplayOrientation.DisplayLandscape; 
	    }
	}

    onReadyForFocusChanged: {
        signature.visible = true;
    }

} // end of Sheet

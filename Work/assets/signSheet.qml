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
import com.example.controls 1.0

Sheet {
    id: signSheet

    property variant readyForFocus: false    
	property string signatureImageFileURL: ""		            

    signal signatureImage(string signatureImageURL)

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
	                    signSheet.close();
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
                    
	            	property variant signSheet
	            	property bool signSheetOpen
		            
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
			            visible: false
			            
			            horizontalAlignment: HorizontalAlignment.Fill
			            
			            preferredHeight: 300
			            
			            onSaved: {
			            	console.log("onSaved: " + signSheet.signatureImageFileURL);
			            	if (signSheet.signatureImageFileURL == "") {
			            		console.log("onSaved: " + _app.getAbsoluteDataFilename("signature.png"));
			            		console.log("onSaved: " + _app.getFileURL(_app.getAbsoluteDataFilename("signature.png")));
			            		signSheet.signatureImageFileURL = _app.getFileURL(_app.getAbsoluteDataFilename("signature.png"));
				            	signatureImage(signSheet.signatureImageFileURL);
		                    	signSheet.close();
		                    }
			            }       
			        }
		            
		        }
	}

    onReadyForFocusChanged: {
        signature.visible = true;
    }

} // end of Sheet
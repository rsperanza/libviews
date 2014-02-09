// Default empty project template
import bb.cascades 1.0
import views 1.0
import com.example.views 1.0

Page {
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
/*            
            	PieChart {
            		id: pieChart
            		
					windowID: "pieChart1"
					z: -1
					x: 0
					y: 0
					width: 0
					height: 0
					enabled: false
            	}
*/            	
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
    actionBarVisibility: ChromeVisibility.Visible

}


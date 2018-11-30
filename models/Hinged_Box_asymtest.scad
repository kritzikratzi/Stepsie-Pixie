$fn = 100;
width = 54.5;
height = 7;
asym=1;
depth = 74.5;
wallThickness = 1.5;
hingeOuter = 6.5;
hingeInner = 3.5;
hingeInnerSlop = .4;
hingeFingerSlop = .4;
fingerLength = hingeOuter/1.65;
fingerSize = 12;
topFingerSize = fingerSize;
pos = -depth/2;
latchWidth = 8;
lift=0;
z = 0;

top_final_ani();
bottom_final();
//top();
//pcb();
//bottom(); 

module top_final_ani(){
    translate([0,0,height]){
        r = max(0, min(1, ($t*1.4-0.2)));
        rotate([0,-r*180,0]){
            translate([0,0,-height]){
                top_final(); 
            }
        }
    }
}
module top_final(){
    difference(){
        top(); 
        translate([0,0,height+0*asym]){
            rotate([0,180,0]){
                translate([0,0,-height]){
                    pcb(); 
                }
            }
        }
   }
} 

module bottom_final(){
    difference(){
        bottom(); 
        pcb();
    }
}

module pcb(){
    translate([-31,-0.85,5+asym]){
        rotate([0,0,-90]){
            scale([10,10,10]){
                color([1,0,0])
                import("hinge-pcb.stl", convexity=5);
            }
        }
    }
}


module bottom() {
    h2=height+asym;
	union() {
		// main box and cutout
		difference() {
			translate([-width - fingerLength, -depth/2, 0]) {
				cube([width,depth,h2]);
			}
            
/*			translate([(-width - fingerLength) + wallThickness, -depth/2 + wallThickness, wallThickness]) {
				cube([width - (wallThickness * 2), depth - (wallThickness * 2), height]);
			}*/
            
            translate([(-width - fingerLength) + wallThickness*2, -depth/2 + wallThickness*2, wallThickness]) {
				cube([width - (wallThickness * 4), depth - (wallThickness * 4), height]);
			}


             for(i = [-1 : 2: 1]){
                 dy = i*depth*0.3;
                 translate([0,dy,0]){

                    // latch cutout
                    translate([-width - fingerLength - 0.05, (-latchWidth/2) - (hingeFingerSlop/2), wallThickness]) {
                        cube([wallThickness/2 + .1, latchWidth + hingeFingerSlop, height]);
                    }
                }
            }
			
		}

         for(i = [-1 : 2: 1]){
             dy = i*depth*0.3;
             translate([0,dy,0]){

                //latch cylinder
                difference() {
                    translate([-width - fingerLength + (wallThickness/1.5), -latchWidth/2, h2 - 1]) {
                        rotate([-90,0,0]) {
                            color([0,1,0]) cylinder(r = 1, h = latchWidth);
                        }
                    }
                    // front wall wipe
                    translate([-width - fingerLength+1, -depth/2,0]) {
                        color([1,0,0]) cube([5,depth,h2]);
                    }
                }
            }
        }
        
		difference() {
			hull() {
				translate([0,-depth/2,height]) {
					rotate([-90,0,0]) {
						cylinder(r = hingeOuter/2, h = depth);
					}
				}
				translate([-fingerLength - .1, -depth/2,height - hingeOuter]){
					cube([.1,depth,hingeOuter]);
				}
				translate([-fingerLength, -depth/2,height-.1]){
					cube([fingerLength,depth,.1]);
				}
				translate([0, -depth/2,height]){
					rotate([0,45,0]) {
						cube([hingeOuter/2,depth,.01]);
					}
				}
			}
			// finger cutouts

			for  (i = [-depth/2 + fingerSize:fingerSize*2:depth/2]) {
				translate([-fingerLength,i - (fingerSize/2) - (hingeFingerSlop/2),0]) {
					cube([fingerLength*2,fingerSize + hingeFingerSlop,height*2]);
				}
			}
		}

		// center rod
		translate([0, -depth/2, height]) {
			rotate([-90,0,0]) {
				cylinder(r = hingeInner /2, h = depth);
			}
		}
	}
}

module top() {
    h2=height-asym;
	union() {
		difference() {
			translate([fingerLength, -depth/2, 0]) {
				cube([width,depth,h2 - .5]);
			}
	
			translate([fingerLength + wallThickness, -depth/2 + wallThickness, wallThickness]) {
				cube([width - (wallThickness * 2), depth - (wallThickness * 2), h2]);
			}

			
		}

		//latch
         for(i = [-1 : 2: 1]){
             dy = i*depth*0.3;
             translate([0,dy,0]){
                translate([width + fingerLength - 0*wallThickness - 0*1.5, (-latchWidth/2), 0]) {
                    color([0,1,0])
                    cube([1.5, latchWidth, h2 - .5 + 4]);
                }
                translate([width + fingerLength - 0*wallThickness, -latchWidth/2, h2 - .5 + 3]) {
                    rotate([-90,0,0]) {
                        cylinder(r = 1, h = latchWidth);
                    }
                }
            }
        }

		difference() {
			hull() {
				translate([0,-depth/2,height]) {
					rotate([-90,0,0]) {
						cylinder(r = hingeOuter/2, h = depth);
					}
				}
				translate([fingerLength, -depth/2,height - hingeOuter - .5]){
					cube([.1,depth,hingeOuter - .5]);
				}
				translate([-fingerLength/2, -depth/2,height-.1]){
					cube([fingerLength,depth,.1]);
				}
				translate([0, -depth/2,height]){
					rotate([0,45,0]) {
						cube([hingeOuter/2,depth,.01]);
					}
				}
			}
			// finger cutouts
			for  (i = [-depth/2:fingerSize*2:depth/2 + fingerSize]) {
				translate([-fingerLength,i - (fingerSize/2) - (hingeFingerSlop/2),0]) {
					cube([fingerLength*2,fingerSize + hingeFingerSlop,height*2]);
				}
				if (depth/2 - i < (fingerSize * 1.5)) {
					translate([-fingerLength,i - (fingerSize/2) - (hingeFingerSlop/2),0]) {
						cube([fingerLength*2,depth,height*2]);
					}
				}
			}

			// center cutout
			translate([0, -depth/2, height]) {
				rotate([-90,0,0]) {
					cylinder(r = hingeInner /2 + hingeInnerSlop, h = depth);
				}
			}
		}
	}
    
    dh=h2-0.5-wallThickness;
    translate([width/2,0,wallThickness]){
        color([1,0,0]) translate([24,-31.5,0]) cylinder(h=dh,r=3);
        color([1,0,1]) translate([24, 31.5,0]) cylinder(h=dh,r=3);
        color([0,1,0]) translate([-14,-19,0]) cylinder(h=dh,r=3);
        color([0,0,1]) translate([-14, 19,0]) cylinder(h=dh,r=3);
    }
}
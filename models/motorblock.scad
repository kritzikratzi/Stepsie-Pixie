use <LEGO.scad>;

le=8;
lh=9.6;

rotate([0, 0, 180]) union() {
    scale([le,le,lh]){
        translate([-3+1/6,-0.015,3+1/6]) difference(){
            cube([1/3,6+0.03,4+1/3],true);
            scale([1/le,1/le,1/lh]) union(){
                rotate([0,90,0]) cylinder(h=10, d=22, center=true, $fn=32);
                rotate([0,90,0]) translate([ 41/2-5,-41/2+5,0]) cylinder(h=10, d=3, center=true, $fn=32);
                rotate([0,90,0]) translate([-41/2+5, 41/2-5,0]) cylinder(h=10, d=3, center=true, $fn=32);
                rotate([0,90,0]) translate([-41/2+5,-41/2+5,0]) cylinder(h=10, d=3, center=true, $fn=32);
                rotate([0,90,0]) translate([ 41/2-5, 41/2-5,0]) cylinder(h=10, d=3, center=true, $fn=32);
            }
        }
        
         translate([-3,3.5-0.025,3+1/3]) rotate([-90,180,90]) color("orange")  prism(1-0.025, 1, 1);
         translate([-3,-2.54+0.015,3+1/3]) rotate([-90,180,90]) color("orange")  prism(1-0.025, 1, 1);
    };
    
    delta=3*le-42/2;
    length=(6-1/3)*le-0.2;
    translate([(-3+1/3)*le,42/2,lh]) color("black") cube([length,delta,(4+1/3)*lh], center=false);
    translate([(-3+1/3)*le,-42/2-delta-0.2,lh]) color("black") cube([length,delta,(4+1/3)*lh], center=false);
    
    color( "blue" ) place(0, -0.5, 2/3) uncenter(0, 0) block(
      width=8,
      length=7,
      roadway_width=8,
      roadway_length=7,
      height=1/3,
      reinforcement="yes"
     );

    color( "red" ) place(3, -3.5+0.02, 1) uncenter(0, 0)  rotate([0,0,90]) block(
      width=1,
      length=1,
      height=1,
      horizontal_holes=true,
      reinforcement="yes"
);
    color( "red" ) place(3, -3.5+0.02, 4+1/3) uncenter(0, 0)  rotate([0,0,90]) block(
      width=1,
      length=1,
      height=1,
      horizontal_holes=true,
      reinforcement="yes",
      roadway_width=1, 
      roadway_length=1
     );
    color( "red" ) place(-3, -3.5+0.02, 1) uncenter(0, 0) rotate([0,0,90]) block(
      width=1,
      length=1,
      height=1,
      horizontal_holes=true,
      reinforcement="yes"
     );
    color( "red" ) place(-3, -3.5+0.02, +4+1/3) uncenter(0, 0) rotate([0,0,90]) block(
      width=1,
      length=1,
      height=1,
      horizontal_holes=true,
      reinforcement="yes",
      roadway_width=1, 
      roadway_length=1
     );

    
    for(b=[0,1]){
        place(-7*b,0) union(){
            for(x=[0:2]){
                color( "green") place(3.5, 0, 1+x*5/3) uncenter(0, 0) block(
                    width=1,
                    length=6,
                    height=1,
                    horizontal_holes=true
                );
            }
            for(y=[0:1]){
                color( "blue" ) place(3.5, 0, 2+5/3*y) uncenter(0, 0) block(
                    width=1,
                    length=6,
                    height=2/3,
                    horizontal_holes=true
                );
            }
        }
    };
   // }
}

   module prism(l, w, h){
       polyhedron(
               points=[[0,0,0], [l,0,0], [l,w,0], [0,w,0], [0,w,h], [l,w,h]],
               faces=[[0,1,2,3],[5,4,3,2],[0,4,5,1],[0,3,4],[5,2,1]]
               );
       }
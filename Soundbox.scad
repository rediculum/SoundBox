Render = "lid"; // [case:Case,lid:Lid]

wall_thickness=5;
lid_thickness=2;
floor_thickness=10;
wall_height=70;
wall_width=90;
wall_length=160;
button_diameter=27.5;
buttons_per_row=4;
button_rows=2;

buttonSpace=wall_length/buttons_per_row;

module case() {
    translate([0,0,0])
        cube([wall_width,wall_thickness,wall_height]);
        cube([wall_thickness,wall_length,wall_height]);
        cube([wall_width,wall_length,floor_thickness]); // floor
    translate([0,wall_length,0])
        cube([wall_width,wall_thickness,wall_height]);
    translate([wall_width,0,0])
        cube([wall_thickness,wall_length+wall_thickness,wall_height]);
};

module lid() {
    difference() {
        translate([0,0,0]) cube([wall_width+wall_thickness,wall_length+wall_thickness,lid_thickness]); // lid
        for ( r = [1:2:3] ) {        
            for ( h = [0:buttons_per_row-1] ) {
               translate ([wall_width/4*r+wall_thickness/2,buttonSpace*h+wall_thickness/2+buttonSpace/2,0]) cylinder(lid_thickness*2, button_diameter/2,button_diameter/2);
            };
        };
    };
};
 
if (Render=="case") {
    case();
} else if (Render=="lid") {
    lid();
}

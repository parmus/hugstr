/*
 * Neopixel 16 dimensions
 * - 44.5mm / 1.75" outer diameter
 * - 31.75mm / 1.25" inner diameter
 * - 2.5mm / 0.1" thick.
 *
 * Gemma dimensions
 * - 27.98mm / 1.1" round x 7.14mm / 0.28" high
 */

neopixel_outer = 44.5;
neopixel_inner = 31.75;
neopixel_height = 2.5;

gemma_diameter = 27.98;
gemma_height = 7.14;
gemma_h2h = 22.61;

$fn = 200;

halo_adjust=1;
cut_size = (neopixel_outer/2)-3;
rim = gemma_diameter - gemma_h2h;

module gemma_platform(){
difference(){
  union(){
    cylinder(h=4, d=gemma_diameter+halo_adjust);
    translate([0, 0, -2]){
      cylinder(h=2.5, d=gemma_diameter-(2*rim));
      intersection(){
        cylinder(h=2.5, d=gemma_diameter+halo_adjust);
        cube([9, gemma_diameter+(halo_adjust), 2*2.5], center=true);
      };
    };

    rotate([0, 0, -45]) translate([gemma_h2h/2, 0, -2]) cylinder(d=3, h=2.5, $fn=$fn);
    rotate([0, 0, -135]) translate([gemma_h2h/2, 0, -2]) cylinder(d=3, h=2.5, $fn=$fn);
  }

  rotate([0, 0, 22.5]) translate([gemma_h2h/2, 0, -0.5]) cylinder(d=3, h=1.5, $fn=$fn);
  rotate([0, 0, -22.5]) translate([gemma_h2h/2, 0, -0.5]) cylinder(d=3, h=1.5, $fn=$fn);
  rotate([0, 0, 22.5]) translate([-gemma_h2h/2, 0, -0.5]) cylinder(d=3, h=1.5, $fn=$fn);
  rotate([0, 0, -22.5]) translate([-gemma_h2h/2, 0, -0.5]) cylinder(d=3, h=1.5, $fn=$fn);
};
};

difference(){
  union(){
    translate([0, 0, 1]) cube([neopixel_outer+1, neopixel_outer+1, 2], center=true);
    translate([0, 0, 2]) cylinder(h=2.5, d=neopixel_inner-halo_adjust);
  }

  // Wire cut-outs
  for(n=[0:2]){
    rotate([0,0,n*90])
    translate([0,(neopixel_outer-cut_size)/2,(4/2)+1]) cube([2, cut_size, 4], center=true);
  };
  
  translate([0, 0, 1]) gemma_platform();
}
// Gemma + Neopixel ring for debugging 
/*
% translate([0,0,1]){
  difference(){
    cylinder(h=neopixel_height, d=neopixel_outer, $fn=$fn);
    translate([0,0,-0.5]) cylinder(h=neopixel_height+1, d=neopixel_inner, $fn=$fn);
  };

  cylinder(h=gemma_height, d=gemma_diameter, $fn=$fn);
};
*/

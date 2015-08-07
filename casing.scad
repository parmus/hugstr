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

halo_adjust=0.2;

module halo(od, id, h){
  difference(){
    cylinder(h=h, d=od, $fn=$fn);
    translate([0,0,-0.05]) cylinder(h=h+0.1, d=id, $fn=$fn);
  };
};

module plate(length, height, thickness, d){
  difference(){
    translate([0, 0, thickness/2]) cube([length, height, thickness], center=true);
    translate([0,0,-0.05]) cylinder(h=thickness+0.1, d=d, $fn=$fn);
  }
}

cut_size = (neopixel_outer/2)-3;

difference(){
  union() {
    plate(length=neopixel_outer+1, height=neopixel_outer+1, thickness=2, d=gemma_diameter-5.33);
    translate([0,0,1.5]) halo(h=2.5, od=neopixel_inner-halo_adjust, id=gemma_diameter+halo_adjust, $fn=$fn);
  };

  // Wire cut-outs
  for(n=[0:2]){
    rotate([0,0,n*90])
    translate([0,(neopixel_outer-cut_size)/2,(4/2)+1]) cube([2, cut_size, 4], center=true);
  };

  // Socket cut-outs
  # translate([0,0,1]) cube([9, gemma_diameter+(halo_adjust*2), 7], center=true);

};


// Gemma + Neopixel ring for debugging 

/*% translate([0,0,1]){
  difference(){
    cylinder(h=neopixel_height, d=neopixel_outer, $fn=$fn);
    translate([0,0,-0.5]) cylinder(h=neopixel_height+1, d=neopixel_inner, $fn=$fn);
  };

  cylinder(h=gemma_height, d=gemma_diameter, $fn=$fn);
};
*/
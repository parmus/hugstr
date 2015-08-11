/***********************************************
 * Casing for Hugstr
 *
 * Version 3
 *
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
gemma_rim = gemma_diameter - gemma_h2h;

$fn = 200;
halo_adjust=0.6;


module gemma_platform(){
  
  difference(){
    union(){
      cylinder(h=10, d=gemma_diameter+halo_adjust);
      translate([0, 0, -6]){
        cylinder(h=6.5, d=gemma_diameter-(2*gemma_rim));
        intersection(){
          cylinder(h=6.5, d=gemma_diameter+halo_adjust);
          cube([9, gemma_diameter+halo_adjust, 2*6.5], center=true);
        };
      };

      rotate([0, 0, -45]) translate([gemma_h2h/2, 0, -6]) cylinder(d=3, h=6.5);
      rotate([0, 0, -135]) translate([gemma_h2h/2, 0, -6]) cylinder(d=3, h=6.5);
    };

    rotate([0, 0, 22.5]) translate([gemma_h2h/2, 0, -0.5]) cylinder(d=3, h=1.5);
    rotate([0, 0, -22.5]) translate([gemma_h2h/2, 0, -0.5]) cylinder(d=3, h=1.5);
    rotate([0, 0, 22.5]) translate([-gemma_h2h/2, 0, -0.5]) cylinder(d=3, h=1.5);
    rotate([0, 0, -22.5]) translate([-gemma_h2h/2, 0, -0.5]) cylinder(d=3, h=1.5);
  };
};

module triangle(xy, z){
  rotate([0,-90,0])
  translate([0,0,-z/2])
  polyhedron(points=[[0,0,0], [0,0,z],
                     [xy, 0, 0], [xy, 0, z],
                     [0, xy, 0], [0, xy, z]],
             faces=[[0,1,2], [1,3,2],
                    [0,4,1], [1,4,5],
                    [2,3,4], [3,5,4],
                    [0,2,4],[1,5,3]]);
};

module casing(){
  cut_size = (neopixel_outer/2)-3;
  platform_size = neopixel_outer+1;

  difference(){
    union(){
      cube([platform_size, platform_size, 6], center=true);
      translate([0, 0, 3]) cylinder(h=2.5, d=neopixel_inner-halo_adjust);
    };

    // Neopixel wire rims
    for(n=[0:2]){
      rotate([0,0,n*90])
      translate([0,(neopixel_outer-cut_size)/2,(6/2)+1]) cube([2, cut_size, 6], center=true);
    };

    // Power wire rim
    union(){
      translate([0, neopixel_outer/2, -2]) cube([4, neopixel_outer/2, 3], center=true);
      translate([0, (gemma_diameter+halo_adjust)/2-1, -1]) triangle(xy=4, z=9);
      translate([0, (gemma_diameter+halo_adjust)/2, -2]) cube([9, 4, 3], center=true);
    }

    // Sensor wire rims
    translate([0, -neopixel_outer/2, -2]) cube([4, 10, 3], center=true);

    // Bunch out the Gemma platform
    translate([0, 0, 1]) gemma_platform();
  };
};

casing();
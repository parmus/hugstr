/***********************************************
 * Casing for Hugstr
 *
 * Version 4
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

halo_adjust=0.6;
wire_size = 2;

// Rendering parameters
$fa=2;
$fs=0.2;


module gemma_platform(){
  difference(){
    union(){
      cylinder(h=10, d=gemma_diameter+halo_adjust);
      translate([0, 0, -6]){
        cylinder(h=6.5, d=gemma_diameter-(2*gemma_rim));

        // Cut out for reset switch
        translate([-(gemma_diameter/2-gemma_rim), 0, 6-1])
        cube([4, 5, 2], center=true);

        // Center cut out for USB- and power connectors
        intersection(){
          cylinder(h=6.5, d=gemma_diameter+halo_adjust);
          cube([9, gemma_diameter+halo_adjust, 2*6.5], center=true);
        };
      };

      // Holes for sensor wires
      for(d = [1, -1]){
        rotate([0, 0, 270+(d*45)])
        translate([gemma_h2h/2, 0, -6]) {
          cylinder(d=3, h=6.5);
          rotate([0, 0, -270-(d*45)])
          translate([-d*2, 0, 5])
          cube([4, wire_size, 10], center=true);
        };
      };
    };

    // Support pins
    for(r = [22.5, -22.5, 180+22.5, 180-22.5]) {
      rotate([0, 0, r])
      translate([gemma_h2h/2, 0, -0.5])
      cylinder(d=3, h=1.5);
    };

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

module rounded_cube(size, r){
  x = size[0];
  y = size[1];
  z = size[2];
  union(){
    cube([x-2*r,y-2*r,z], center=true);
    cube([x-2*r,y,z-2*r], center=true);
    cube([x,y-2*r,z-2*r], center=true);

    for(xd = [1, -1]) for(yd = [1, -1]) for(zd = [1, -1])
    translate([(x/2-r)*xd,(y/2-r)*yd,(z/2-r)*zd])
    sphere(r=r);

    for(xd = [1, -1]) for(yd = [1, -1])
    translate([(x/2-r)*xd, (y/2-r)*yd, 0])
    cylinder(r=r, h=z-2*r, center=true);


    for(xd = [1, -1]) for(zd = [1, -1])
    translate([(x/2-r)*xd, 0, (z/2-r)*zd])
    rotate([90,0,0])
    cylinder(r=r, h=y-2*r, center=true);

    for(yd = [1, -1]) for(zd = [1, -1])
    translate([0, (y/2-r)*yd, (z/2-r)*zd])
    rotate([0,90,0])
    cylinder(r=r, h=x-2*r, center=true);    
  };
};

module double_wirecut(l){
  h = 2.5;
  difference(){
    cube([wire_size*2, l, h+0.001], center=true);
    for(d=[1, -1]){
      rotate([0,0,d*90])
      translate([0,-(wire_size+0.1),-((h/2)+0.001)])
      triangle(xy=1.2, z=l+0.2);
    };
  };
};

module casing(){
  cut_size = (neopixel_outer/2)-3;
  platform_size = neopixel_outer+1;

  difference(){
    union(){
      rounded_cube([platform_size, platform_size, 6], r=2);
      translate([0, 0, 3]) cylinder(h=2.5, d=neopixel_inner-halo_adjust);
    };

    // Neopixel wire rims
    for(n=[0:2]){
      rotate([0,0,n*90])
      translate([0,(neopixel_outer-cut_size)/2,(6/2)+1]) cube([wire_size, cut_size, 6], center=true);
    };

    // Align with bottom edge
    translate([0,0,-3]){
      // Power wire rim
      union(){
        translate([0, neopixel_outer/2, 2.5/2]) double_wirecut(l=neopixel_outer/2);
        translate([0, (gemma_diameter+halo_adjust)/2-2, 2]) triangle(xy=5, z=9);
        translate([0, (gemma_diameter+halo_adjust)/2, 1]) cube([9, 4, 3], center=true);
      }

      // Sensor wire rims
      translate([0, -neopixel_outer/2, 2.5/2]) {
        double_wirecut(l=10);
        translate([0, 5, 0]){
          cube([17.5, wire_size, 3], center=true);
          for(d = [1, -1]){
            translate([d*(17.5/2-wire_size/2), 10/2, 0])
            cube([wire_size, 10, 3], center=true);
          };
        };
      };
    };

    // Bunch out the Gemma platform
    translate([0, 0, 1]) gemma_platform();
  };
};

casing();

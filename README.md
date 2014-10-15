Hugstr
======


Parts list
----------
* Adafruit FLORA
* Battery for Adafruit FLORA
* NeoPixel Ring - 16 x WS2812 5050 RGB LED with Integrated Drivers
* Pressure-Sensitive Conductive Sheet (Velostat/Linqstat)
* Stainless Medium Conductive Thread - 3 ply - 18 meter/60 ft
* 15k resister
* Battery (e.g. Lithium Ion Polymer Battery - 3.7v 150mAh)

You can replace the Adafruit FLORA with another Arduino board, if you
reconfigure the pin numbers accordingly in the program.
You can also use other NeoPixel components and configurations, if you
reconfigure the number of pixel accordingly in the program.

Constructing the touch sensor
-----------------------------
The touch sensor works by building a voltage divider from the pressure-sensitive
conductive sheet (variable resistance) and a 15k resister.

1. Cut out 2 pieces of pressure-sensitive conductive sheet; about 5cm x 10cm.
2. Place a piece of conductive thread on each pad, so it zig-zags across the
   pad and one end of the thread sticks out to the side, at least 3 cm. Keep
   the thread in place with tape.
3. Place the pads on top of each other, so the conductive thread are on the
   outside. The zig-zag patterns should roughly match up and the two ends of
   thread sticking out, should be on the same side, about 2 cm apart.
4. Solder a 15k resister to one of the threads sticking out.
5. Connect your new sensor to your Arduino and tests it.
6. If the sensor is too sensitive then simply add additional layers of
   pressure-sensitive conductive sheet in between the outer layers.
7. Make sure that the zig-zag patterns are aligned, if the sensor is flacky.
   The threads needs to be more or less on top of each other with the
   pressure-sensitive conductive sheet in between.
8. Once your sensor is working as intended, wrap it out in tape, so it stays
   together.

```
              .----------------.
              |                |
  GND o----------------------\ |
              |              | |
   D9 o--+----| /------------/ |
         |    | |              |
        .+.   | \------------\ |
    15k | |   |              | |
        | |   | /------------/ |  
		'+'   | |              |
		 |    | \------------  |
VBATT o--+    |                | 
              '----------------'
```


Electric diagram
----------------

```
VBATT o-----------o-----------.           
                  |           |           
                 .+.          |           
                 | | 15k      |           
                 | |          |           
                 '+'          | V+        
                  |      .----+-----. IN  
   D9 o-----------o      | NeoPixel +----.
                  |      '----+-----'    |
                 .+.          | G        |
                 | | Touch    |          |
                 | | sensor   |          |
                 '+'          |          |
                  |           |          |
  GND o-----------o-----------'          |
                                         |
   D6 o----------------------------------'
```

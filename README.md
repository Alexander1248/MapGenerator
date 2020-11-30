# MapGenerator2D
Map generator based on Perlin noise.

Currently there are 3 types of generated maps:

1)Generating a map with temperature changes depending on the distance from the equator (the temperature is generated in the same way as on Earth).
To activate it, enter the word "Sphere" in the worldType field in the generator.

2)Generating a map with a change in temperature relative to the map center(you can think of it as a flat world with the sun in the middle of the map).
To activate it, enter the word "CoredPlane" in the worldType field in the generator.

3)A simple flat map is generated with the temperature depending on the height(at the height between the water level and the mountain level, the temperature is 0.25 of the maximum).
To activate it, enter the word "Plane" in the worldType field in the generator.

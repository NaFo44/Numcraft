# Numcraft
## A 3D minecraft-like map generator for numworks ( work on Epsilon ) 

<img src="src/WIN_20240616_23_59_28_Pro.jpg" alt="Sample app for Numworks Calculator" width="300">

### You can move in every direction : 
* Directional arrows to change the pitch and yaw angle of the camera. 
* Key 8 to move the camera up
* key 2 to move it down
* key 4 to move it to the right
* Key 6 to move it to the left
* plus to move forward
* minus to move backward.

### V1.0
Rendering speed for a 4x4 map (16 cubes, 192 triangles): 4fps

### V2.0
Rendering speed for a 4x4 map (16 cubes, 192 triangles): smooth render, 10fps

Current issues : 
* Visual bugs for the rendering of a chunk (16x16)
* Rendering too slow to be truly playable
* Bug when crossing cubes that are behind the player (no collision)
* No block selection
  
Possible applications using the 3D engine of this project: 3D platformer (fewer triangles, faster, and therefore playable), 3D model viewer

Thanks to [Quantale](src/WIN_20240616_23_59_28_Pro.jpg) for his explanation.

Soon : a cleaner version of the code, and faster rendering

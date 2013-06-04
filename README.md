Star Faux - Space Shooter in OpenGL
===================================

Created using GLUT and GLEW libraries.

Team:  
Allen Zhang  
Vincent Kyi  
Kevin Nguyen  
Stanley Xu  
William Tam  

1. Introduction
    Our project is based on the Nintendo's StarFox game. It features a main 
player that flies a ship. So far, there are asteroids and the player must shoot
down the asteroids while avoiding crashing into them.

Keys:
    w: fly up
    a: turn left
    s: fly down
    d: turn right
    e: accelerate forward
    r: accelerate backwards
    <SPACE>: shoot
    p: pause 
    q, Q, Esc: quit


2. Results
A. Bump Mapping
    We used bump mapping on the asteroids. We currently have three different
asteroid geometries that are bump mapped with three different normal maps 
(stepstone_normal.tga, cobble_normal.tga, and many_normal.tga). The asteroids
are placed in random locations and given random rotations.
    Bump mapping (aka normal mapping) is mostly done in the fragment shader. 
We researched bump mapping online and found some really good tutorials (see references
below). One implementation calculates the tangents and bitangents in the
application while another calculates only the tangents in the application
and the bitangents in the vertex shader (by cross product normal and tangent). 
The best implementation that we found was through a new method of normal mapping.
An article posted below shows that the previous methods of calculating the TBN
matrix to get to tangent space is misunderstood and only works via tricks to get
the tangent frame to be orthogonal. The new method uses linear algebra, calculus,
and gradients to compute what really should be called cotangent, cobitagent and normals.
It calculates the TBN matrix in the fragment shader and returns a perturbed normal
based on the normal map sampler.

B. Collision Detection

C. Particle System


References:
To compute tangents and bitangents in the application for bump mapping.
http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-13-normal-mapping/

To do bump mapping only in the fragment shader.
http://www.thetenthplanet.de/archives/1180

------------------------ README -------------------------------

My 70% Project was to create a scene in OpenGL. Specifically, to render the moon rotating around the earth in space. Then, to add a Satellite that orbits the Earth. 
The player can of course fly around with full camera controls, and each model has at least a semi-accurate orbit to that of real life.  
A Cubemap is used to create the space skybox, and the planets themselves are loaded into the project using ASSIMP. 

All Textures and Models can be found in the /media folder, and shaders are organised in the /shaders folder. 
If given the opportunity, I would have liked to have added in a sun, with lighting and shadow effects on the earth and moon, maybe even a day, or night cycle. But these are more advanced features and would take me a little more time to learn. 

As required, there is a signature on each of the planets. The signature is my full name on each texture in stylised white text.

To run, Simply open the EarthSim-Coursework2.sln attached via Github, and start the project. Then, to move around, use the WASD Key's, and Mouse to look around the scene.

-------------------  ENVIRONMENT USED  -------------------------

Created in Visual Studio 2022 Version 17.8
Developed in C++ 
Utilised OOP methodologies 

------------------- LIBRARIES NEEDED TO RUN ---------------------

Please make sure these libraries are added to the project in order to load the scene correctly: 
- GLFW
- GLAD
- GLM
- ASSIMP
- LEARNOPENGL

Everything is found in C:\users\public\OpenGL - Except for assimp, which is also found in C:\users\public\Assimp 

------------------- USEFUL LINKS -------------------------------

Github: https://github.com/DanielSteele1/COMP3016_2

YouTube demo vid: https://youtu.be/O0wtXeHL6Xc

------------------ EXTERNAL RESOURCES USED ---------------------

learnOpenGL.com - used to research how to implement skybox into the scene

Skybox textures - Created using a Free open-source .exe called Spacescape. It enables you to generate space and nebula skyboxes. Warning - clicking this link will start downloading the software. 
https://sourceforge.net/projects/spacescape/files/win/spacescape-0.5.1.zip/download?use_mirror=altushost-swe&download=&failedmirror=deac-riga.dl.sourceforge.net

Free Model and Texture assets for Satellite - https://free3d.com/3d-model/satellite-v1--384167.html

Texture for clouds (merged with texture for earth using PhotoPea) - https://upload.wikimedia.org/wikipedia/commons/7/7a/Solarsystemscope_texture_8k_earth_clouds.jpg

Texture for Moon - https://www.google.com/url?sa=i&url=https%3A%2F%2Fsvs.gsfc.nasa.gov%2Fcgi-bin%2Fdetails.cgi%3Faid%3D4720&psig=AOvVaw3E1jr4UAmoPoXbQ53ivDGX&ust=1704631545047000&source=images&cd=vfe&opi=89978449&ved=0CBMQjRxqFwoTCKCvtMPlyIMDFQAAAAAdAAAAABAD

Texture for Earth - https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.solarsystemscope.com%2Ftextures%2F&psig=AOvVaw0E5Xt9DtfOjRIhIr1Xe8fW&ust=1704630810534000&source=images&cd=vfe&opi=89978449&ved=0CBMQjRxqFwoTCICo5-LiyIMDFQAAAAAdAAAAABAw

Models for Earth & Moon were created by myself using the software Blender.
---------------------- CONCLUSION -------------------------------
To conclude, I'm very happy with the outcome of this project, as it's my first project using OpenGL. I have learnt a lot and enjoyed the process, but I think if I could do more on this project, 
I would try to add in the sun, as well as lighting to create a shading effect, and try to implement a day-night cycle. 
I could also try to use PCG to generate a meteor belt around everything or try to generate the rest of the planets in our solar system. 

Of course, these are just ideas but if I had more time and knowledge, I would have looked into these ideas in a lot more detail.

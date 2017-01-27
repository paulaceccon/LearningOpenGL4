# LearningOpenGL4

A repository containing codes showing examples of the features of OpenGL 4.x, using just the core profile.
I tried to use OO to provide some sort of abstraction to commom definitions required to use this version of OpenGL.

Dependencies rely on [GLEW](http://glew.sourceforge.net/), [GLFW](http://www.glfw.org/) and [GLM](http://glm.g-truc.net/). C++11 is also required.

---
#### Topics:
- [x] Skeleton 
- [x] VBO+VAO
- [x] VBO+VAO+IBO 
- [ ] Texture (*refactoring*)
  - From image
  - From file
- [ ] Lighting
- [ ] Instancing (*refactoring*)
- [ ] Deferred Shading (*refactoring*)
- [x] Tesselation
- [x] Tesselation + PNTriangles

---
#### Bonus:
- [ ] Arcball
- [ ] Gizmos
  
---  
  Tesselation controls:
  * W - wireframe mode on*/off
  * I - interpolation mode (linear, quadratic*)
  * ENTER - x2 tesselation level
  * BACKSPACE - /2 tesselation level
  
_*_ *default*

Tips for setting OpenGL depencies on VS can be found [here](http://www.41post.com/5178/programming/opengl-configuring-glfw-and-glew-in-visual-cplusplus-express).

Running in terminal using `g++ -std=c++11 *.cpp ../Common/*.cpp -framework OpenGL -lGLEW -lglfw`.

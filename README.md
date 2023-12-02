# HaGame2

2D/3D batteries included game development library written in c++. It is designed to 
be as decoupled as possible to allow applications of various design. Therefore, it is up
to the game designer to implement game engine features such as a render pipeline, gameplay features, etc. 

Contains the following set of features:

- Core
    - Game runtime able to run headless and in a separate thread
    - Entity Component System
    - Scene system
- Math
  - Custom Vector, Matrix, Quaternion library
  - Collision detection
- Input
  - Keyboard & Mouse support
  - Gamepad support
  - Unified device handler
  - Mappings TODO
- Graphics
  - Flexible render pipeline - defined at the application level
  - Deferred rendering
  - Particle emitter
  - Sprite Sheet support
  - Text Rendering
  - Standard library of common shaders
- Audio
    - 3D Audio supported
- Physics - TODO (currently handled on application level)
- Common - set of common components you can use in projects
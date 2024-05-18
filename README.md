# thirsty

Quick exploration of 2d sprite-based engine in c++ based on a quick tech demo.

And yes, "thirsty" is a "sprite" pun, albeit a lame one.

## Inspirations

Specific constraints shaping the design of this demo engine include:

* The elegance of functional stack rendering from intermediate mode graphics

* The simple power of the rendering pipeline in THREE.js

* The idea of enhancing a basic 2d sprite game with specific shader effects

* Some things Jonathan Blow said that really irked me the wrong way

* A minimal "least time to images on screen" project I threw together the other day just to remind myself this was still a straightforward route to take

## High-Level Vision

Roughly following model somewhere between "Bartelby" and "Christina" from my engines evolution roadmap.

Specific deltas include:

* An emphasis on loading/translating game specifications; 

* Structs over classes (e.g., friendly to C for eventual translation into Zig)

* More explicit "scene graph" management with depth-ordered rendering & blending buffers

* Flat packaging for C++ code with support for static and dynamic library bindings

* Testable would be good but if it isn't too difficult the main test will probably be a "driver" application

## Design Primitives

render pipeline := `renderer->render(scene, camera)`

scene := `2d graph of sprite nodes`

sprite node := `transform` + `geometry (probably just point or poly, rendered from center)` + `material (texture and shader program)`

Application flow:

1. Initialization

1. Threaded game loop

   1. Input/event thread

   1. Rendering thread

   1. Update/logical thread

1. Cleanup/deallocation

To facilitate threading, we will need explicit state models for concurrent access control.

Aside from a message buffer, this means that only the update/logical thread will have "write" access to the scene state.

## Modeling

Based on the above primitives, we need to implement specifications for:

- [x] Renderer

- [ ] Scene

- [x] Camera

- [x] Sprite Node

- [x] Transform

- [x] Geometry

- [x] Material

- [x] Texture

- [ ] Shader

For now, binding these top-level models within an `App` singleton is acceptible.

We will also need metabehaviors for:

* Event pub/sub

* Input device bindings (keyboard only, mouse later)

* Adjudication (if multiple updates are needed)

* Networking? Sound? Probably not right now.

### Renderer

SDL-based OpenGL with support for GLSL programs.

The fundamental definition of the render pivots around the following interface:

`renderer->render(scene, camera)`

This also effectively manages the graphics buffer, window context, GL configuration, "walk" through GL calls, etc.

### Scene

This is a property of the top-level application that is just a pointer to a single sprite node.

Currently this is just a top-level `Node*`; this is probably fine, but we'll leave the task box unchecked just in case.

### Camera

Primarily a point and bindings to specific GL parameters for projection/orthographic specification.

The camera is defined almost exclusively as a means to define the in-world scene pass (as opposed to UI) context.

### Sprite Node

Can be fairly abstract for now as it is strongly defined by decomposition; components can be bound to lifetime for now but eventually it would be useful to reuse references (smart pointers?) to large resources like materials.

### Transform

Will probably reuse as much math as possible from something like GLM. Technically a 3x3 matrix will work here, and it would be nice to avoid decomposition, but while it's inefficient we'll probably just retain a "flat" 4x4 for compatibility.

Just reused `glm::mat4x4`, and it's working fine. Might be nice to have more sophisticated helpers but even those are in `glm::`. On that note, who knew `glm` was column-major!?

### Geometry

Probably need to define a vertex model; this can be fixed (extensibility will be too complex) and will likely only come in two forms: a point, or a rectangle about a point.

### Material

We'll assume for now that a material consists of a texture/surface, shader program, and bindings between the two.

### Texture

Probably reuse SDL_Surface for now.

Yup. Reused `SDL_Surface` and it's fine.

## Shader

Encapsulates file-loaded vertex and fragment shader to compile/link into a specific GLSL program.

Should probably support hard-coded hooked for our vertex model, as well as procedural hooks for the material to bind texture uniforms.

Currently part of `Material`, debating whether it's worth pulling out when we still need to juggle vertex <-> attribute shader bindings. Will leave the task box unchecked.


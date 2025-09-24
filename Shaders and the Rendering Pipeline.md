# Shaders and the Rendering Pipeline

Series of stages that take place in order to render an image to the screen. <br />
It's a part of the GPU. <br />

Five of the stages are programmable thru "Shaders" <br />
- Vertex, Fragment, Geometry, Tessellation, Computaion. <br />

Shaders are pieces of code written in GLSL(OpenGL Shading Language). <br />
- Based on C. <br />

# 9 Stages of the Rendering Pipeline <br />
1. Vertex Specification <br />
2. Vertex Shader (programmable) <br />
3. Tessellation (programmable) <br />
4. Geometry Shader (programmable) <br />
5. Vertex Post-Processing <br />
6. Primitive Assembly <br />
7. Rasterization <br />
8. Fragment Shader (programmable) <br />
9. Pre-Sample Operations <br />

# Vertex Specification <br />
- Point in space, defined with x, y and z co-ordinates. <br />
- A primitive is a simple shape defined using one or more vertices. <br />
- Pionts, Lines, Triangles, Quads <br />
- Vertex specification: Setting up the data of the vertices for the primitives we want to render. <br />
- Uses Vertex Array Object(VAO) and Vertex Buffer Object(VBO). <br />
- VAO defines WHAT data a vertex has(position, color, texture, normals, etc) <br />
- VBO defines thedata itself <br />
- Attribute Pointers define WHERE and HOW shaders can access vertex data. <br />

# Vertex Specification: Creating VAO/VBO
1. Generate a VAO ID. <br />
2. Bind the VAO with that ID. <br />
3. Generate a VBO ID. <br />
4. Bind the VBO with that ID(Attaches to the chosen VAO) <br />
5. Attach the vertex data to that VBO <br />
6. Define the Attribute pointer formatting <br />
7. Enable the Attribute Pointer <br />
8. Unbind the VAO and VBO - ready to bind other objects <br />

# Vertex Specification: Initiating Draw
1. Activate Shader Program you want to use <br />
2. Bind VAO of object you want to draw <br />
3. Call glDrawArrays, which initiates the rest of the pipeline <br />

# Vertex Shader
1. Handles vertices individually <br />
2. Not Optional <br />
3. Store something in gl_Position(built in variable) - used in later stages <br />
4. Can specify additional outputs that can be picked up <br />
and used by user-defined shader later in the pipeline <br />
5. Inputs consist of vertex data itself. <br />

# Geometry Shader
1. Handles primitives(groups of vertices) <br />
2. Takes primitives then "emits/outputs" their vertices to create the given primitive or new primitives <br />
3. Can alter data given to it to modify given primitives, or even create new ones <br />
4. Can even alter the primitive type (points, lines, triangles, etc) <br />

# Primitive Assembly
1. Vertices are converted in to a series of primitives <br />
2. Triangles - 6 vertices will be 2 triangles(3 vertices each) <br />
3. Face culling - removed primitives that can't be seen. Don't draw things that can't be seen! <br />

# Rasterization
1. It's about the data of each pixel <br />
2. Converts primitives in to "Fragments" <br />
3. Fragments are pieces of data for each pixel, obtained from the rasterization process <br />
4. Fragmnt data will be interpolated based on it's position relative to each vertex <br />

# Fragment Shader
1. Handles data for each fragment <br />
2. Optional. <br />
3. Most important output is the color of the pixel that the fragment covers <br />
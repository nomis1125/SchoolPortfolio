#include "Paths.hpp"
#include "UniformLocations.glsl"
#include "assignment.hpp"

struct Quad
{
    // This contains all the required vertex data in the following order:
    // x | y | z | u | v | ...
    // where x, y, z are the coordinates of the vertices and u, v are the two
    // texture coordinates.
    std::vector<float> vertices;

    // This contains the indices to render the triangles. It is similar to the
    // index buffer we used to render meshes.
    std::vector<unsigned int> indices;
};

// This function will generate a quad that fills the window.
Quad generateQuad()
{
   std::vector<float> data
   {
      // Vertex Positions     Texture coordinates
       1.0f,  1.0f, 0.0f,     1.0f, 1.0f,
       1.0f, -1.0f, 0.0f,     1.0f, 0.0f,
      -1.0f, -1.0f, 0.0f,     0.0f, 0.0f,
      -1.0f,  1.0f, 0.0f,     0.0f, 1.0f

   };

   std::vector<unsigned int> indices
   {
      0, 1, 3,
      1, 2, 3
   };

    return {data, indices};
}

int main()
{
    // Your code goes here.
    return 0;
}

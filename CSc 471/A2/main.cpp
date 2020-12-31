<<<<<<< HEAD
#include "Paths.hpp"
#include "UniformLocations.glsl"

#include <atlas/glx/Buffer.hpp>
#include <atlas/glx/Context.hpp>
#include <atlas/glx/ErrorCallback.hpp>
#include <atlas/glx/GLSL.hpp>
#include <atlas/utils/Cameras.hpp>
#include <atlas/utils/LoadObjFile.hpp>

#include <fmt/printf.h>

#include <array>

//
#include <atlas/utils/stb_image.h>
//

using namespace atlas;

void errorCallback(int code, char const* message)
{
    fmt::print("error ({}): {}\n", code, message);
}

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
    std::vector<float> data{// Vertex Positions       Texture coordinates
                            1.0f, 1.0f,  0.0f,        1.0f,  1.0f,  
							1.0f, -1.0f, 0.0f,        1.0f,  0.0f, 
							-1.0f, -1.0f, 0.0f,       0.0f,  0.0f, 
							-1.0f, 1.0f, 0.0f,        0.0f,  1.0f

    };

    std::vector<unsigned int> indices{0, 1, 3, 1, 2, 3};

    return {data, indices};
}

int main()
{
    glx::WindowSettings settings;
    glx::WindowCallbacks callbacks;
    {
        settings.size.width  = 1280;
        settings.size.height = 720;
        settings.title       = "Lab 04";
    }

    // Let's initialize our camera here.
    utils::MayaCamera::CameraData mayaCamera;
    {
        using namespace atlas::utils;
        auto mousePressedCallback = [&mayaCamera](int button, int action, int,
                                                  double xPos, double yPos) {
            if (action == GLFW_PRESS)
            {
                math::Point2 point{xPos, yPos};

                if (button == GLFW_MOUSE_BUTTON_LEFT)
                {
                    mayaCamera.movement = MayaCamera::Movements::Tumble;
                    MayaCamera::mouseDown(mayaCamera, point);
                }
                else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
                {
                    mayaCamera.movement = MayaCamera::Movements::Track;
                    MayaCamera::mouseDown(mayaCamera, point);
                }
                else if (button == GLFW_MOUSE_BUTTON_RIGHT)
                {
                    mayaCamera.movement = MayaCamera::Movements::Dolly;
                    MayaCamera::mouseDown(mayaCamera, point);
                }
            }
            else
            {
                MayaCamera::mouseUp(mayaCamera);
            }
        };

        auto mouseMoveCallback = [&mayaCamera](double xPos, double yPos) {
            MayaCamera::mouseMove(mayaCamera, {xPos, yPos});
        };

        auto mouseScrollCallback = [&mayaCamera](double xOffset,
                                                 double yOffset) {
            MayaCamera::mouseScroll(mayaCamera, {xOffset, yOffset});
        };

        callbacks.mousePressCallback  = mousePressedCallback;
        callbacks.mouseMoveCallback   = mouseMoveCallback;
        callbacks.mouseScrollCallback = mouseScrollCallback;
    }

    GLFWwindow* window;
    {
        glx::initializeGLFW(errorCallback);
        window = glx::createGLFWWindow(settings);

        glx::bindWindowCallbacks(window, callbacks);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        glx::createGLContext(window, settings.version);

        using namespace atlas::core;
        glx::initializeGLCallback(glx::ErrorSource::All, glx::ErrorType::All,
                                  glx::ErrorSeverity::High |
                                      glx::ErrorSeverity::Medium);
    }

    GLuint vertexHandle{0};
    GLuint fragmentHandle{0};
    GLuint programHandle{0};

    glx::ShaderFile vertexSource;
    glx::ShaderFile fragmentSource;
    std::vector<std::string> includeDirs = {ShaderPath};
    {
        programHandle  = glCreateProgram();
        vertexHandle   = glCreateShader(GL_VERTEX_SHADER);
        fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);

        std::string shaderRoot{ShaderPath};
        std::string vertexFilename = shaderRoot + "image.vert";
        vertexSource = glx::readShaderSource(vertexFilename, includeDirs);
        std::string fragmentFilename = shaderRoot + "image.frag";
        fragmentSource = glx::readShaderSource(fragmentFilename, includeDirs);

        if (auto result =
                glx::compileShader(vertexSource.sourceString, vertexHandle);
            result)
        {
            std::string message = glx::parseErrorLog(vertexSource, *result);
            fmt::print("{}\n", message);

            return 0;
        }

        if (auto result =
                glx::compileShader(fragmentSource.sourceString, fragmentHandle);
            result)
        {
            std::string message = glx::parseErrorLog(fragmentSource, *result);
            fmt::print("{}\n", message);
            return 0;
        }

        glAttachShader(programHandle, vertexHandle);
        glAttachShader(programHandle, fragmentHandle);

        if (auto result = glx::linkShaders(programHandle); result)
        {
            fmt::print("error: {}\n", *result);
            return 0;
        }
    }

    GLuint ubo;
    {
        glCreateBuffers(1, &ubo);
        glNamedBufferStorage(ubo, glx::size<math::Matrix4>(4), nullptr,
                             GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_UNIFORM_BUFFER, UNIFORM_MATRIX_LOCATION, ubo);
    }

    GLuint vbo;
    GLuint vao;
    GLuint ibo;
    GLsizei count;
    {		
	    Quad quwad = generateQuad();
        count = static_cast<GLsizei>(quwad.indices.size());

        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, glx::size<float>(quwad.vertices.size()),
                             quwad.vertices.data(), 0);

        glCreateBuffers(1, &ibo);
        glNamedBufferStorage(ibo, glx::size<GLuint>(quwad.indices.size()),
                             quwad.indices.data(), 0);

        glCreateVertexArrays(1, &vao);

        glVertexArrayVertexBuffer(vao, 0, vbo, 0, glx::stride<float>(5));
        glVertexArrayElementBuffer(vao, ibo);

        glEnableVertexArrayAttrib(vao, VERTEX_ATTRIB_LOCATION);
        glEnableVertexArrayAttrib(vao, TEXTURE_ATTRIB_LOCATION);

        glVertexArrayAttribFormat(vao, VERTEX_ATTRIB_LOCATION, 3, GL_FLOAT,
                                  GL_FALSE, glx::relativeOffset<float>(0));
        glVertexArrayAttribFormat(vao, TEXTURE_ATTRIB_LOCATION, 2, GL_FLOAT,
                                  GL_FALSE, glx::relativeOffset<float>(3));

        glVertexArrayAttribBinding(vao, VERTEX_ATTRIB_LOCATION, 0);
        glVertexArrayAttribBinding(vao, TEXTURE_ATTRIB_LOCATION, 0);
    }

    //
    GLuint textureHandle{0};
    GLuint textureBandle{0};//norm
    GLuint textureCandle{0};//inverse
    GLuint textureFlandle{0};//black and white
    {
        std::string dataRoot{DataPath};
        std::string filename = dataRoot + "input.jpg";
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data =
            stbi_load(filename.c_str(), &width, &height, &channels, 0);


        std::vector<unsigned char> victor;//normal
		int B;
        for (B = 0; B < 256; B=B+8)
        {
            int G;
            for (G = 0; G < 256; G=G+8)
            {
                int R;
                for (R = 0; R < 256; R=R+8)
                {
                    victor.push_back((unsigned char) R);
                    victor.push_back((unsigned char) G);
                    victor.push_back((unsigned char) B);
                }
            }
        }

		std::vector<unsigned char> victor1;//inverse
        for (B = 0; B < 256; B = B + 8)
        {
            int G;
            for (G = 0; G < 256; G = G + 8)
            {
                int R;
                for (R = 0; R < 256; R = R + 8)
                {
                    int R1 = 255 - R;
                    int G1 = 255 - G;
                    int B1 = 255 - B;
                    victor1.push_back((unsigned char)R1);
                    victor1.push_back((unsigned char)G1);
                    victor1.push_back((unsigned char)B1);
                }
            }
        }

		std::vector<unsigned char> victor2; // Black and White
        for (B = 0; B < 256; B = B + 8)
        {
            int G;
            for (G = 0; G < 256; G = G + 8)
            {
                int R;
                for (R = 0; R < 256; R = R + 8)
                {
                    float k = 3.0;//brightness

                    float R1 = k*R;
                    float G1 = k*G;
                    float B1 = k*B;
                    if (R1 > 255)
                    {
                        R1 = 255;
                    }
                    if (G1 > 255)
                    {
                        G1 = 255;
					}
                    if (B1 > 255)
                    {
                        B1 = 255;
					}
                    victor2.push_back((unsigned char)R1);
                    victor2.push_back((unsigned char)G1);
                    victor2.push_back((unsigned char)B1);
                }
            }
        }

		if (data)
           
        {
            GLenum format{0};
            GLenum internalFormat{0};
            if (channels == 1)
            {
                format         = GL_RED;
                internalFormat = GL_R8;
            }
            else if (channels == 3)
            {
                format         = GL_RGB;
                internalFormat = GL_RGB8;
            }
            else if (channels == 4)
            {
                format         = GL_RGBA;
                internalFormat = GL_RGBA8;
            }

			glCreateTextures(GL_TEXTURE_2D, 1, &textureHandle);

            glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureParameteri(textureHandle, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR);
            glTextureParameteri(textureHandle, GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR);
            glTextureStorage2D(textureHandle, 1, internalFormat, width, height);
            glTextureSubImage2D(
                textureHandle, 0, 0, 0, width, height, format,
                GL_UNSIGNED_BYTE,
                data);
            glGenerateTextureMipmap(textureHandle);
            stbi_image_free(data);
        }
        else
        {
            stbi_image_free(data);
            return 0;
        }

		//normal
       glCreateTextures(GL_TEXTURE_3D, 1, &textureBandle);

        glTextureParameteri(textureBandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureBandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(textureBandle, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTextureParameteri(textureBandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureBandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage3D(textureBandle, 1, GL_RGB8, 32, 32,
                           32);
        glTextureSubImage3D(
            textureBandle, 0, 0, 0, 0, 32, 32, 32, GL_RGB,
            GL_UNSIGNED_BYTE,
            victor.data());
        glGenerateTextureMipmap(textureBandle);

		//inverse
        glCreateTextures(GL_TEXTURE_3D, 1, &textureCandle);

        glTextureParameteri(textureCandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureCandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(textureCandle, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTextureParameteri(textureCandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureCandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage3D(textureCandle, 1, GL_RGB8, 32, 32, 32);
        glTextureSubImage3D(
            textureCandle, 0, 0, 0, 0, 32, 32, 32, GL_RGB, GL_UNSIGNED_BYTE,
            victor1.data());
        glGenerateTextureMipmap(textureCandle);

		// Black and White
        glCreateTextures(GL_TEXTURE_3D, 1, &textureFlandle);

        glTextureParameteri(textureFlandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureFlandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(textureFlandle, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTextureParameteri(textureFlandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureFlandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage3D(textureFlandle, 1, GL_RGB8, 32, 32, 32);
        glTextureSubImage3D(textureFlandle, 0, 0, 0, 0, 32, 32, 32, GL_RGB,
                            GL_UNSIGNED_BYTE, victor2.data());
        glGenerateTextureMipmap(textureFlandle);
    }
    //

    static constexpr float nearVal{1.0f};
    static constexpr float farVal{10000000000.0f};

    glEnable(GL_DEPTH_TEST);

	int abcx = 0;
    while (!glfwWindowShouldClose(window))
    {
        int width;
        int height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//check input
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            abcx = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            abcx = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            abcx = 2;
        }

        if (glx::shouldShaderBeReloaded(vertexSource))
        {
            glx::reloadShader(programHandle, vertexHandle, vertexSource,
                              includeDirs);
        }

        if (glx::shouldShaderBeReloaded(fragmentSource))
        {
            glx::reloadShader(programHandle, fragmentHandle, fragmentSource,
                              includeDirs);
        }

        // Setup the matrix data here. First, create the projection matrix.
        auto projection = glm::perspective(glm::radians(mayaCamera.fov),
                                           static_cast<float>(width) / height,
                                           nearVal, farVal);
        auto view       = utils::MayaCamera::computeViewMatrix(mayaCamera);
        auto model =
            glm::scale(math::Matrix4{1.0f}, math::Vector{8.0f, 8.0f, 8.0f});

        // Now update the data in the buffer.
        {
            std::vector<math::Matrix4> uniformMatrices(4);
            uniformMatrices[0] = model;
            uniformMatrices[1] = view * model;
            uniformMatrices[2] = projection * view * model;
            uniformMatrices[3] = glm::transpose(glm::inverse(model));
            glNamedBufferSubData(
                ubo, 0, glx::size<math::Matrix4>(uniformMatrices.size()),
                uniformMatrices.data());
        }

        glUseProgram(programHandle);

		glUniform1i(glGetUniformLocation(programHandle, "LUTmode"), abcx);

        glBindVertexArray(vao);
        
		glBindTextureUnit(textureHandle, UNIFORM_TEXTURE_LOCATION);
        

        if (abcx == 2)
        {
            glBindTextureUnit(textureFlandle, SOMETHING_ELSEBW); // Black and White
        }
        else if (abcx == 1)
        {
            glBindTextureUnit(textureCandle, SOMETHING_ELSEInv); // inverse
		}
        else
        {
            glBindTextureUnit(textureBandle, SOMETHING_ELSE); // normal
        }
        

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT,
                       glx::bufferOffset<GLuint>(0));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glx::destroyGLFWWindow(window);
    glx::terminateGLFW();

    return 0;
}
=======
#include "Paths.hpp"
#include "UniformLocations.glsl"

#include <atlas/glx/Buffer.hpp>
#include <atlas/glx/Context.hpp>
#include <atlas/glx/ErrorCallback.hpp>
#include <atlas/glx/GLSL.hpp>
#include <atlas/utils/Cameras.hpp>
#include <atlas/utils/LoadObjFile.hpp>

#include <fmt/printf.h>

#include <array>

//
#include <atlas/utils/stb_image.h>
//

using namespace atlas;

void errorCallback(int code, char const* message)
{
    fmt::print("error ({}): {}\n", code, message);
}

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
    std::vector<float> data{// Vertex Positions       Texture coordinates
                            1.0f, 1.0f,  0.0f,        1.0f,  1.0f,  
							1.0f, -1.0f, 0.0f,        1.0f,  0.0f, 
							-1.0f, -1.0f, 0.0f,       0.0f,  0.0f, 
							-1.0f, 1.0f, 0.0f,        0.0f,  1.0f

    };

    std::vector<unsigned int> indices{0, 1, 3, 1, 2, 3};

    return {data, indices};
}

int main()
{
    glx::WindowSettings settings;
    glx::WindowCallbacks callbacks;
    {
        settings.size.width  = 1280;
        settings.size.height = 720;
        settings.title       = "Lab 04";
    }

    // Let's initialize our camera here.
    utils::MayaCamera::CameraData mayaCamera;
    {
        using namespace atlas::utils;
        auto mousePressedCallback = [&mayaCamera](int button, int action, int,
                                                  double xPos, double yPos) {
            if (action == GLFW_PRESS)
            {
                math::Point2 point{xPos, yPos};

                if (button == GLFW_MOUSE_BUTTON_LEFT)
                {
                    mayaCamera.movement = MayaCamera::Movements::Tumble;
                    MayaCamera::mouseDown(mayaCamera, point);
                }
                else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
                {
                    mayaCamera.movement = MayaCamera::Movements::Track;
                    MayaCamera::mouseDown(mayaCamera, point);
                }
                else if (button == GLFW_MOUSE_BUTTON_RIGHT)
                {
                    mayaCamera.movement = MayaCamera::Movements::Dolly;
                    MayaCamera::mouseDown(mayaCamera, point);
                }
            }
            else
            {
                MayaCamera::mouseUp(mayaCamera);
            }
        };

        auto mouseMoveCallback = [&mayaCamera](double xPos, double yPos) {
            MayaCamera::mouseMove(mayaCamera, {xPos, yPos});
        };

        auto mouseScrollCallback = [&mayaCamera](double xOffset,
                                                 double yOffset) {
            MayaCamera::mouseScroll(mayaCamera, {xOffset, yOffset});
        };

        callbacks.mousePressCallback  = mousePressedCallback;
        callbacks.mouseMoveCallback   = mouseMoveCallback;
        callbacks.mouseScrollCallback = mouseScrollCallback;
    }

    GLFWwindow* window;
    {
        glx::initializeGLFW(errorCallback);
        window = glx::createGLFWWindow(settings);

        glx::bindWindowCallbacks(window, callbacks);
        glfwMakeContextCurrent(window);
        glfwSwapInterval(1);

        glx::createGLContext(window, settings.version);

        using namespace atlas::core;
        glx::initializeGLCallback(glx::ErrorSource::All, glx::ErrorType::All,
                                  glx::ErrorSeverity::High |
                                      glx::ErrorSeverity::Medium);
    }

    GLuint vertexHandle{0};
    GLuint fragmentHandle{0};
    GLuint programHandle{0};

    glx::ShaderFile vertexSource;
    glx::ShaderFile fragmentSource;
    std::vector<std::string> includeDirs = {ShaderPath};
    {
        programHandle  = glCreateProgram();
        vertexHandle   = glCreateShader(GL_VERTEX_SHADER);
        fragmentHandle = glCreateShader(GL_FRAGMENT_SHADER);

        std::string shaderRoot{ShaderPath};
        std::string vertexFilename = shaderRoot + "image.vert";
        vertexSource = glx::readShaderSource(vertexFilename, includeDirs);
        std::string fragmentFilename = shaderRoot + "image.frag";
        fragmentSource = glx::readShaderSource(fragmentFilename, includeDirs);

        if (auto result =
                glx::compileShader(vertexSource.sourceString, vertexHandle);
            result)
        {
            std::string message = glx::parseErrorLog(vertexSource, *result);
            fmt::print("{}\n", message);

            return 0;
        }

        if (auto result =
                glx::compileShader(fragmentSource.sourceString, fragmentHandle);
            result)
        {
            std::string message = glx::parseErrorLog(fragmentSource, *result);
            fmt::print("{}\n", message);
            return 0;
        }

        glAttachShader(programHandle, vertexHandle);
        glAttachShader(programHandle, fragmentHandle);

        if (auto result = glx::linkShaders(programHandle); result)
        {
            fmt::print("error: {}\n", *result);
            return 0;
        }
    }

    GLuint ubo;
    {
        glCreateBuffers(1, &ubo);
        glNamedBufferStorage(ubo, glx::size<math::Matrix4>(4), nullptr,
                             GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_UNIFORM_BUFFER, UNIFORM_MATRIX_LOCATION, ubo);
    }

    GLuint vbo;
    GLuint vao;
    GLuint ibo;
    GLsizei count;
    {		
	    Quad quwad = generateQuad();
        count = static_cast<GLsizei>(quwad.indices.size());

        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, glx::size<float>(quwad.vertices.size()),
                             quwad.vertices.data(), 0);

        glCreateBuffers(1, &ibo);
        glNamedBufferStorage(ibo, glx::size<GLuint>(quwad.indices.size()),
                             quwad.indices.data(), 0);

        glCreateVertexArrays(1, &vao);

        glVertexArrayVertexBuffer(vao, 0, vbo, 0, glx::stride<float>(5));
        glVertexArrayElementBuffer(vao, ibo);

        glEnableVertexArrayAttrib(vao, VERTEX_ATTRIB_LOCATION);
        glEnableVertexArrayAttrib(vao, TEXTURE_ATTRIB_LOCATION);

        glVertexArrayAttribFormat(vao, VERTEX_ATTRIB_LOCATION, 3, GL_FLOAT,
                                  GL_FALSE, glx::relativeOffset<float>(0));
        glVertexArrayAttribFormat(vao, TEXTURE_ATTRIB_LOCATION, 2, GL_FLOAT,
                                  GL_FALSE, glx::relativeOffset<float>(3));

        glVertexArrayAttribBinding(vao, VERTEX_ATTRIB_LOCATION, 0);
        glVertexArrayAttribBinding(vao, TEXTURE_ATTRIB_LOCATION, 0);
    }

    //
    GLuint textureHandle{0};
    GLuint textureBandle{0};//norm
    GLuint textureCandle{0};//inverse
    GLuint textureFlandle{0};//black and white
    {
        std::string dataRoot{DataPath};
        std::string filename = dataRoot + "input.jpg";
        int width, height, channels;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data =
            stbi_load(filename.c_str(), &width, &height, &channels, 0);


        std::vector<unsigned char> victor;//normal
		int B;
        for (B = 0; B < 256; B=B+8)
        {
            int G;
            for (G = 0; G < 256; G=G+8)
            {
                int R;
                for (R = 0; R < 256; R=R+8)
                {
                    victor.push_back((unsigned char) R);
                    victor.push_back((unsigned char) G);
                    victor.push_back((unsigned char) B);
                }
            }
        }

		std::vector<unsigned char> victor1;//inverse
        for (B = 0; B < 256; B = B + 8)
        {
            int G;
            for (G = 0; G < 256; G = G + 8)
            {
                int R;
                for (R = 0; R < 256; R = R + 8)
                {
                    int R1 = 255 - R;
                    int G1 = 255 - G;
                    int B1 = 255 - B;
                    victor1.push_back((unsigned char)R1);
                    victor1.push_back((unsigned char)G1);
                    victor1.push_back((unsigned char)B1);
                }
            }
        }

		std::vector<unsigned char> victor2; // Black and White
        for (B = 0; B < 256; B = B + 8)
        {
            int G;
            for (G = 0; G < 256; G = G + 8)
            {
                int R;
                for (R = 0; R < 256; R = R + 8)
                {
                    float k = 3.0;//brightness

                    float R1 = k*R;
                    float G1 = k*G;
                    float B1 = k*B;
                    if (R1 > 255)
                    {
                        R1 = 255;
                    }
                    if (G1 > 255)
                    {
                        G1 = 255;
					}
                    if (B1 > 255)
                    {
                        B1 = 255;
					}
                    victor2.push_back((unsigned char)R1);
                    victor2.push_back((unsigned char)G1);
                    victor2.push_back((unsigned char)B1);
                }
            }
        }

		if (data)
           
        {
            GLenum format{0};
            GLenum internalFormat{0};
            if (channels == 1)
            {
                format         = GL_RED;
                internalFormat = GL_R8;
            }
            else if (channels == 3)
            {
                format         = GL_RGB;
                internalFormat = GL_RGB8;
            }
            else if (channels == 4)
            {
                format         = GL_RGBA;
                internalFormat = GL_RGBA8;
            }

			glCreateTextures(GL_TEXTURE_2D, 1, &textureHandle);

            glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTextureParameteri(textureHandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTextureParameteri(textureHandle, GL_TEXTURE_MIN_FILTER,
                                GL_LINEAR);
            glTextureParameteri(textureHandle, GL_TEXTURE_MAG_FILTER,
                                GL_LINEAR);
            glTextureStorage2D(textureHandle, 1, internalFormat, width, height);
            glTextureSubImage2D(
                textureHandle, 0, 0, 0, width, height, format,
                GL_UNSIGNED_BYTE,
                data);
            glGenerateTextureMipmap(textureHandle);
            stbi_image_free(data);
        }
        else
        {
            stbi_image_free(data);
            return 0;
        }

		//normal
       glCreateTextures(GL_TEXTURE_3D, 1, &textureBandle);

        glTextureParameteri(textureBandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureBandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(textureBandle, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTextureParameteri(textureBandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureBandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage3D(textureBandle, 1, GL_RGB8, 32, 32,
                           32);
        glTextureSubImage3D(
            textureBandle, 0, 0, 0, 0, 32, 32, 32, GL_RGB,
            GL_UNSIGNED_BYTE,
            victor.data());
        glGenerateTextureMipmap(textureBandle);

		//inverse
        glCreateTextures(GL_TEXTURE_3D, 1, &textureCandle);

        glTextureParameteri(textureCandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureCandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(textureCandle, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTextureParameteri(textureCandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureCandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage3D(textureCandle, 1, GL_RGB8, 32, 32, 32);
        glTextureSubImage3D(
            textureCandle, 0, 0, 0, 0, 32, 32, 32, GL_RGB, GL_UNSIGNED_BYTE,
            victor1.data());
        glGenerateTextureMipmap(textureCandle);

		// Black and White
        glCreateTextures(GL_TEXTURE_3D, 1, &textureFlandle);

        glTextureParameteri(textureFlandle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureFlandle, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTextureParameteri(textureFlandle, GL_TEXTURE_WRAP_R, GL_REPEAT);
        glTextureParameteri(textureFlandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureFlandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage3D(textureFlandle, 1, GL_RGB8, 32, 32, 32);
        glTextureSubImage3D(textureFlandle, 0, 0, 0, 0, 32, 32, 32, GL_RGB,
                            GL_UNSIGNED_BYTE, victor2.data());
        glGenerateTextureMipmap(textureFlandle);
    }
    //

    static constexpr float nearVal{1.0f};
    static constexpr float farVal{10000000000.0f};

    glEnable(GL_DEPTH_TEST);

	int abcx = 0;
    while (!glfwWindowShouldClose(window))
    {
        int width;
        int height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//check input
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        {
            abcx = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        {
            abcx = 1;
        }
        if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        {
            abcx = 2;
        }

        if (glx::shouldShaderBeReloaded(vertexSource))
        {
            glx::reloadShader(programHandle, vertexHandle, vertexSource,
                              includeDirs);
        }

        if (glx::shouldShaderBeReloaded(fragmentSource))
        {
            glx::reloadShader(programHandle, fragmentHandle, fragmentSource,
                              includeDirs);
        }

        // Setup the matrix data here. First, create the projection matrix.
        auto projection = glm::perspective(glm::radians(mayaCamera.fov),
                                           static_cast<float>(width) / height,
                                           nearVal, farVal);
        auto view       = utils::MayaCamera::computeViewMatrix(mayaCamera);
        auto model =
            glm::scale(math::Matrix4{1.0f}, math::Vector{8.0f, 8.0f, 8.0f});

        // Now update the data in the buffer.
        {
            std::vector<math::Matrix4> uniformMatrices(4);
            uniformMatrices[0] = model;
            uniformMatrices[1] = view * model;
            uniformMatrices[2] = projection * view * model;
            uniformMatrices[3] = glm::transpose(glm::inverse(model));
            glNamedBufferSubData(
                ubo, 0, glx::size<math::Matrix4>(uniformMatrices.size()),
                uniformMatrices.data());
        }

        glUseProgram(programHandle);

		glUniform1i(glGetUniformLocation(programHandle, "LUTmode"), abcx);

        glBindVertexArray(vao);
        
		glBindTextureUnit(textureHandle, UNIFORM_TEXTURE_LOCATION);
        

        if (abcx == 2)
        {
            glBindTextureUnit(textureFlandle, SOMETHING_ELSEBW); // Black and White
        }
        else if (abcx == 1)
        {
            glBindTextureUnit(textureCandle, SOMETHING_ELSEInv); // inverse
		}
        else
        {
            glBindTextureUnit(textureBandle, SOMETHING_ELSE); // normal
        }
        

        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT,
                       glx::bufferOffset<GLuint>(0));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glx::destroyGLFWWindow(window);
    glx::terminateGLFW();

    return 0;
}
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae

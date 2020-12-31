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

using namespace atlas;

void errorCallback(int code, char const* message)
{
    fmt::print("error ({}): {}\n", code, message);
}

int main()
{
    int abcx = 0; //determins wheather to use Phong 0; Gouraud 1
    glx::WindowSettings settings;
    glx::WindowCallbacks callbacks;
    {
        settings.size.width  = 1280;
        settings.size.height = 720;
        settings.title       = "Lab 03";
    }

    // Let's initialize our camera here.
    utils::MayaCamera::CameraData mayaCamera;
    {
        using namespace atlas::utils;
        auto mousePressedCallback = [&mayaCamera](int button, int action,
                                                         int, double xPos,
                                                         double yPos) {
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

					fmt::print("{}\n", xPos);
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

    // Phong Shader
    GLuint vertexHandlePhong{0};
    GLuint fragmentHandlePhong{0};
    GLuint programHandlePhong{0};

    glx::ShaderFile vertexSource;
    glx::ShaderFile fragmentSource;
    std::vector<std::string> includeDirs = {ShaderPath};
    {
        programHandlePhong  = glCreateProgram();
        vertexHandlePhong   = glCreateShader(GL_VERTEX_SHADER);
        fragmentHandlePhong = glCreateShader(GL_FRAGMENT_SHADER);

        std::string shaderRoot{ShaderPath};
        std::string vertexFilename = shaderRoot + "suzanne.vert";
        vertexSource = glx::readShaderSource(vertexFilename, includeDirs);
        std::string fragmentFilename = shaderRoot + "suzanne.frag";
        fragmentSource = glx::readShaderSource(fragmentFilename, includeDirs);

        if (auto result = glx::compileShader(vertexSource.sourceString,
                                             vertexHandlePhong);
            result)
        {
            std::string message = glx::parseErrorLog(vertexSource, *result);
            fmt::print("{}\n", message);

            return 0;
        }

        if (auto result = glx::compileShader(fragmentSource.sourceString,
                                             fragmentHandlePhong);
            result)
        {
            std::string message = glx::parseErrorLog(fragmentSource, *result);
            fmt::print("{}\n", message);
            return 0;
        }

        glAttachShader(programHandlePhong, vertexHandlePhong);
        glAttachShader(programHandlePhong, fragmentHandlePhong);

        if (auto result = glx::linkShaders(programHandlePhong); result)
        {
            fmt::print("error: {}\n", *result);
            return 0;
        }
    }

    // Gouraud
    GLuint vertexHandleGouraud{0}; //-copy from here for each frag and vert
    GLuint fragmentHandleGouraud{0};
    GLuint programHandleGouraud{0};

    glx::ShaderFile vertexSourceGouraud;
    glx::ShaderFile fragmentSourceGouraud;
    std::vector<std::string> includeDirsGouraud = {ShaderPath};
    {
        programHandleGouraud  = glCreateProgram();
        vertexHandleGouraud   = glCreateShader(GL_VERTEX_SHADER);
        fragmentHandleGouraud = glCreateShader(GL_FRAGMENT_SHADER);

        std::string shaderRoot{ShaderPath};
        std::string vertexFilenameGouraud = shaderRoot + "suzanne1.vert";
        vertexSourceGouraud =
            glx::readShaderSource(vertexFilenameGouraud, includeDirsGouraud);
        std::string fragmentFilenameGouraud = shaderRoot + "suzanne1.frag";
        fragmentSourceGouraud =
            glx::readShaderSource(fragmentFilenameGouraud, includeDirsGouraud);

        if (auto result = glx::compileShader(vertexSourceGouraud.sourceString,
                                             vertexHandleGouraud);
            result)
        {
            std::string message =
                glx::parseErrorLog(vertexSourceGouraud, *result);
            fmt::print("{}\n", message);

            return 0;
        }

        if (auto result = glx::compileShader(fragmentSourceGouraud.sourceString,
                                             fragmentHandleGouraud);
            result)
        {
            std::string message =
                glx::parseErrorLog(fragmentSourceGouraud, *result);
            fmt::print("{}\n", message);
            return 0;
        }

        glAttachShader(programHandleGouraud, vertexHandleGouraud);
        glAttachShader(programHandleGouraud, fragmentHandleGouraud);

        if (auto result = glx::linkShaders(programHandleGouraud); result)
        {
            fmt::print("error: {}\n", *result);
            return 0;
        }
    }
    //- doubles for each P and G
    //

    GLuint ubo;
    {
        glCreateBuffers(1, &ubo);
        glNamedBufferStorage(ubo, glx::size<math::Matrix4>(3), nullptr,
                             GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_UNIFORM_BUFFER, UNIFORM_MATRIX_LOCATION, ubo);
    }

    GLuint vbo;
    GLuint vao;
    GLuint ibo;
    GLsizei count;
    {
        std::string dataRoot{DataPath};
        std::string suzanneFilename = dataRoot + "suzanne.obj";

        auto result = utils::loadObjMesh(suzanneFilename);
        if (!result)
        {
            return 0;
        }

        utils::ObjMesh mesh = result.value();

        count = static_cast<GLsizei>(mesh.shapes[0].indices.size());
        std::vector<GLuint> indices(count);
        std::transform(
            mesh.shapes[0].indices.begin(), mesh.shapes[0].indices.end(),
            indices.begin(),
            [](std::size_t i) -> GLuint { return static_cast<GLuint>(i); });

        std::vector<float> vertices;
        for (auto& vertex : mesh.shapes[0].vertices)
        {
            vertices.push_back(vertex.position.x);
            vertices.push_back(vertex.position.y);
            vertices.push_back(vertex.position.z);

            vertices.push_back(vertex.normal.x);
            vertices.push_back(vertex.normal.y);
            vertices.push_back(vertex.normal.z);
        }

        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, glx::size<float>(vertices.size()),
                             vertices.data(), 0);

        glCreateBuffers(1, &ibo);
        glNamedBufferStorage(ibo, glx::size<GLuint>(indices.size()),
                             indices.data(), 0);

        glCreateVertexArrays(1, &vao);

        glVertexArrayVertexBuffer(vao, 0, vbo, 0, glx::stride<float>(6));
        glVertexArrayElementBuffer(vao, ibo);

        glEnableVertexArrayAttrib(vao, VERTEX_ATTRIB_LOCATION);
        glEnableVertexArrayAttrib(vao, NORMAL_ATTRIB_LOCATION);

        glVertexArrayAttribFormat(vao, VERTEX_ATTRIB_LOCATION, 3, GL_FLOAT,
                                  GL_FALSE, glx::relativeOffset<float>(0));
        glVertexArrayAttribFormat(vao, NORMAL_ATTRIB_LOCATION, 3, GL_FLOAT,
                                  GL_FALSE, glx::relativeOffset<float>(3));

        glVertexArrayAttribBinding(vao, VERTEX_ATTRIB_LOCATION, 0);
        glVertexArrayAttribBinding(vao, NORMAL_ATTRIB_LOCATION, 0);
    }

    static constexpr float nearVal{1.0f};
    static constexpr float farVal{10000000000.0f};

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        int width;
        int height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            abcx = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        {
            abcx = 1;
		}

        if (abcx == 0)
        {
            // Phong Shader
            if (glx::shouldShaderBeReloaded(vertexSource))
            {
                glx::reloadShader(programHandlePhong, vertexHandlePhong,
                                  vertexSource, includeDirs);
            }

            if (glx::shouldShaderBeReloaded(fragmentSource))
            {
                glx::reloadShader(programHandlePhong, fragmentHandlePhong,
                                  fragmentSource, includeDirs);
            }
        }

        else
        {
            // Gouraud
            if (glx::shouldShaderBeReloaded(vertexSourceGouraud))
            {
                glx::reloadShader(programHandleGouraud, vertexHandleGouraud,
                                  vertexSourceGouraud, includeDirsGouraud);
            }

            if (glx::shouldShaderBeReloaded(fragmentSourceGouraud))
            {
                glx::reloadShader(programHandleGouraud, fragmentHandleGouraud,
                                  fragmentSourceGouraud, includeDirsGouraud);
            }
        }
        //

        // Setup the matrix data here. First, create the projection matrix.

        auto projection = glm::perspective(glm::radians(mayaCamera.fov),
                                           static_cast<float>(width) / height,
                                           nearVal, farVal);
        auto view       = utils::MayaCamera::computeViewMatrix(mayaCamera);
        auto model =
            glm::scale(math::Matrix4{1.0f}, math::Vector{8.0f, 8.0f, 8.0f});

		

        // Now update the data in the buffer.
        {
            std::vector<math::Matrix4> uniformMatrices{projection, view, model};
            glNamedBufferSubData(
                ubo, 0, glx::size<math::Matrix4>(uniformMatrices.size()),
                uniformMatrices.data());
        }

        if (abcx == 0)
        {
            // Phong Shader
            glUseProgram(programHandlePhong);
        }
        else
        {
            // Gouraud
            glUseProgram(programHandleGouraud);
        }

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT,
                       glx::bufferOffset<GLuint>(0));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glx::destroyGLFWWindow(window);
    glx::terminateGLFW();

    return 0;
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

using namespace atlas;

void errorCallback(int code, char const* message)
{
    fmt::print("error ({}): {}\n", code, message);
}

int main()
{
    int abcx = 0; //determins wheather to use Phong 0; Gouraud 1
    glx::WindowSettings settings;
    glx::WindowCallbacks callbacks;
    {
        settings.size.width  = 1280;
        settings.size.height = 720;
        settings.title       = "Lab 03";
    }

    // Let's initialize our camera here.
    utils::MayaCamera::CameraData mayaCamera;
    {
        using namespace atlas::utils;
        auto mousePressedCallback = [&mayaCamera](int button, int action,
                                                         int, double xPos,
                                                         double yPos) {
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

					fmt::print("{}\n", xPos);
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

    // Phong Shader
    GLuint vertexHandlePhong{0};
    GLuint fragmentHandlePhong{0};
    GLuint programHandlePhong{0};

    glx::ShaderFile vertexSource;
    glx::ShaderFile fragmentSource;
    std::vector<std::string> includeDirs = {ShaderPath};
    {
        programHandlePhong  = glCreateProgram();
        vertexHandlePhong   = glCreateShader(GL_VERTEX_SHADER);
        fragmentHandlePhong = glCreateShader(GL_FRAGMENT_SHADER);

        std::string shaderRoot{ShaderPath};
        std::string vertexFilename = shaderRoot + "suzanne.vert";
        vertexSource = glx::readShaderSource(vertexFilename, includeDirs);
        std::string fragmentFilename = shaderRoot + "suzanne.frag";
        fragmentSource = glx::readShaderSource(fragmentFilename, includeDirs);

        if (auto result = glx::compileShader(vertexSource.sourceString,
                                             vertexHandlePhong);
            result)
        {
            std::string message = glx::parseErrorLog(vertexSource, *result);
            fmt::print("{}\n", message);

            return 0;
        }

        if (auto result = glx::compileShader(fragmentSource.sourceString,
                                             fragmentHandlePhong);
            result)
        {
            std::string message = glx::parseErrorLog(fragmentSource, *result);
            fmt::print("{}\n", message);
            return 0;
        }

        glAttachShader(programHandlePhong, vertexHandlePhong);
        glAttachShader(programHandlePhong, fragmentHandlePhong);

        if (auto result = glx::linkShaders(programHandlePhong); result)
        {
            fmt::print("error: {}\n", *result);
            return 0;
        }
    }

    // Gouraud
    GLuint vertexHandleGouraud{0}; //-copy from here for each frag and vert
    GLuint fragmentHandleGouraud{0};
    GLuint programHandleGouraud{0};

    glx::ShaderFile vertexSourceGouraud;
    glx::ShaderFile fragmentSourceGouraud;
    std::vector<std::string> includeDirsGouraud = {ShaderPath};
    {
        programHandleGouraud  = glCreateProgram();
        vertexHandleGouraud   = glCreateShader(GL_VERTEX_SHADER);
        fragmentHandleGouraud = glCreateShader(GL_FRAGMENT_SHADER);

        std::string shaderRoot{ShaderPath};
        std::string vertexFilenameGouraud = shaderRoot + "suzanne1.vert";
        vertexSourceGouraud =
            glx::readShaderSource(vertexFilenameGouraud, includeDirsGouraud);
        std::string fragmentFilenameGouraud = shaderRoot + "suzanne1.frag";
        fragmentSourceGouraud =
            glx::readShaderSource(fragmentFilenameGouraud, includeDirsGouraud);

        if (auto result = glx::compileShader(vertexSourceGouraud.sourceString,
                                             vertexHandleGouraud);
            result)
        {
            std::string message =
                glx::parseErrorLog(vertexSourceGouraud, *result);
            fmt::print("{}\n", message);

            return 0;
        }

        if (auto result = glx::compileShader(fragmentSourceGouraud.sourceString,
                                             fragmentHandleGouraud);
            result)
        {
            std::string message =
                glx::parseErrorLog(fragmentSourceGouraud, *result);
            fmt::print("{}\n", message);
            return 0;
        }

        glAttachShader(programHandleGouraud, vertexHandleGouraud);
        glAttachShader(programHandleGouraud, fragmentHandleGouraud);

        if (auto result = glx::linkShaders(programHandleGouraud); result)
        {
            fmt::print("error: {}\n", *result);
            return 0;
        }
    }
    //- doubles for each P and G
    //

    GLuint ubo;
    {
        glCreateBuffers(1, &ubo);
        glNamedBufferStorage(ubo, glx::size<math::Matrix4>(3), nullptr,
                             GL_DYNAMIC_STORAGE_BIT);
        glBindBufferBase(GL_UNIFORM_BUFFER, UNIFORM_MATRIX_LOCATION, ubo);
    }

    GLuint vbo;
    GLuint vao;
    GLuint ibo;
    GLsizei count;
    {
        std::string dataRoot{DataPath};
        std::string suzanneFilename = dataRoot + "suzanne.obj";

        auto result = utils::loadObjMesh(suzanneFilename);
        if (!result)
        {
            return 0;
        }

        utils::ObjMesh mesh = result.value();

        count = static_cast<GLsizei>(mesh.shapes[0].indices.size());
        std::vector<GLuint> indices(count);
        std::transform(
            mesh.shapes[0].indices.begin(), mesh.shapes[0].indices.end(),
            indices.begin(),
            [](std::size_t i) -> GLuint { return static_cast<GLuint>(i); });

        std::vector<float> vertices;
        for (auto& vertex : mesh.shapes[0].vertices)
        {
            vertices.push_back(vertex.position.x);
            vertices.push_back(vertex.position.y);
            vertices.push_back(vertex.position.z);

            vertices.push_back(vertex.normal.x);
            vertices.push_back(vertex.normal.y);
            vertices.push_back(vertex.normal.z);
        }

        glCreateBuffers(1, &vbo);
        glNamedBufferStorage(vbo, glx::size<float>(vertices.size()),
                             vertices.data(), 0);

        glCreateBuffers(1, &ibo);
        glNamedBufferStorage(ibo, glx::size<GLuint>(indices.size()),
                             indices.data(), 0);

        glCreateVertexArrays(1, &vao);

        glVertexArrayVertexBuffer(vao, 0, vbo, 0, glx::stride<float>(6));
        glVertexArrayElementBuffer(vao, ibo);

        glEnableVertexArrayAttrib(vao, VERTEX_ATTRIB_LOCATION);
        glEnableVertexArrayAttrib(vao, NORMAL_ATTRIB_LOCATION);

        glVertexArrayAttribFormat(vao, VERTEX_ATTRIB_LOCATION, 3, GL_FLOAT,
                                  GL_FALSE, glx::relativeOffset<float>(0));
        glVertexArrayAttribFormat(vao, NORMAL_ATTRIB_LOCATION, 3, GL_FLOAT,
                                  GL_FALSE, glx::relativeOffset<float>(3));

        glVertexArrayAttribBinding(vao, VERTEX_ATTRIB_LOCATION, 0);
        glVertexArrayAttribBinding(vao, NORMAL_ATTRIB_LOCATION, 0);
    }

    static constexpr float nearVal{1.0f};
    static constexpr float farVal{10000000000.0f};

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        int width;
        int height;
        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        {
            abcx = 0;
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        {
            abcx = 1;
		}

        if (abcx == 0)
        {
            // Phong Shader
            if (glx::shouldShaderBeReloaded(vertexSource))
            {
                glx::reloadShader(programHandlePhong, vertexHandlePhong,
                                  vertexSource, includeDirs);
            }

            if (glx::shouldShaderBeReloaded(fragmentSource))
            {
                glx::reloadShader(programHandlePhong, fragmentHandlePhong,
                                  fragmentSource, includeDirs);
            }
        }

        else
        {
            // Gouraud
            if (glx::shouldShaderBeReloaded(vertexSourceGouraud))
            {
                glx::reloadShader(programHandleGouraud, vertexHandleGouraud,
                                  vertexSourceGouraud, includeDirsGouraud);
            }

            if (glx::shouldShaderBeReloaded(fragmentSourceGouraud))
            {
                glx::reloadShader(programHandleGouraud, fragmentHandleGouraud,
                                  fragmentSourceGouraud, includeDirsGouraud);
            }
        }
        //

        // Setup the matrix data here. First, create the projection matrix.

        auto projection = glm::perspective(glm::radians(mayaCamera.fov),
                                           static_cast<float>(width) / height,
                                           nearVal, farVal);
        auto view       = utils::MayaCamera::computeViewMatrix(mayaCamera);
        auto model =
            glm::scale(math::Matrix4{1.0f}, math::Vector{8.0f, 8.0f, 8.0f});

		

        // Now update the data in the buffer.
        {
            std::vector<math::Matrix4> uniformMatrices{projection, view, model};
            glNamedBufferSubData(
                ubo, 0, glx::size<math::Matrix4>(uniformMatrices.size()),
                uniformMatrices.data());
        }

        if (abcx == 0)
        {
            // Phong Shader
            glUseProgram(programHandlePhong);
        }
        else
        {
            // Gouraud
            glUseProgram(programHandleGouraud);
        }

        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT,
                       glx::bufferOffset<GLuint>(0));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glx::destroyGLFWWindow(window);
    glx::terminateGLFW();

    return 0;
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}
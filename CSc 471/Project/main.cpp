#include "Paths.hpp"
#include "UniformLocations.glsl"

<<<<<<< HEAD
#include <atlas/glx/Buffer.hpp>
#include <atlas/glx/Context.hpp>
#include <atlas/glx/ErrorCallback.hpp>
#include <atlas/glx/GLSL.hpp>
#include <atlas/utils/Cameras.hpp>
#include <atlas/utils/LoadObjFile.hpp>

#include <fmt/printf.h>
#include <atlas/utils/stb_image.h>

=======
#include <atlas/glx/Buffer.hpp>
#include <atlas/glx/Context.hpp>
#include <atlas/glx/ErrorCallback.hpp>
#include <atlas/glx/GLSL.hpp>
#include <atlas/utils/Cameras.hpp>
#include <atlas/utils/LoadObjFile.hpp>

#include <fmt/printf.h>
#include <atlas/utils/stb_image.h>

>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
#include <array>



int main()
{
    ProgramData programData;
    init(programData);

	int renderMode{0};

    auto earth  = loadPipeline("earth");
    auto atmos  = loadPipeline("atmos");
    auto atmosPlan  = loadPipeline("atmosPlan");
    auto sphere = loadMeshFromFile("sphere.obj");
    auto uniformMatrices = createUniformMatrices();

   //GLuint textureHandle = handleMake();

	//Trying to get the earth texture

	GLuint textureHandle{0};
    {
        std::string dataRoot{DataPath};
        std::string filename = dataRoot + "earth.jpg";
        int width2, height2, channels2;
        stbi_set_flip_vertically_on_load(1);
        unsigned char* data =
            stbi_load(filename.c_str(), &width2, &height2, &channels2, 0);
        if (data)
        {
            GLenum format{0};
            GLenum internalFormat{0};
            if (channels2 == 1)
            {
                format         = GL_RED;
                internalFormat = GL_R8;
            }
            else if (channels2 == 3)
            {
                format         = GL_RGB;
                internalFormat = GL_RGB8;
            }
            else if (channels2 == 4)
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
            glTextureStorage2D(textureHandle, 1, internalFormat, width2,
                               height2);
            glTextureSubImage2D(textureHandle, 0, 0, 0, width2, height2, format,
                                GL_UNSIGNED_BYTE, data);
            glGenerateTextureMipmap(textureHandle);
            stbi_image_free(data);
        }
        else
        {
            stbi_image_free(data);
            return 0;
        }
    }

	//

    glEnable(GL_DEPTH_TEST);
    //bool showDemoWindow{true};
    while (!glfwWindowShouldClose(programData.window))
    {
        int width, height;
        glfwGetFramebufferSize(programData.window, &width, &height);
        glViewport(0, 0, width, height);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Shaders for the atmosphere from space, atmosphere from the planet and earth texture(not working)

		if (renderMode == 0)
        {
            reloadPipeline(earth);
            updateUniformMatrices(uniformMatrices, width, height, programData);
            glUseProgram(earth.pipelineHandle);
            glBindVertexArray(sphere.vao);
            glBindTextureUnit(textureHandle, UNIFORM_TEXTURE_LOCATION);

            glDrawElements(GL_TRIANGLES, sphere.count, GL_UNSIGNED_INT,
                           glx::bufferOffset<GLuint>(0));
		}
		else if (renderMode == 1)
		{
            reloadPipeline(atmosPlan);
            updateUniformMatrices(uniformMatrices, width, height, programData);
            glUseProgram(atmosPlan.pipelineHandle);
            glBindVertexArray(sphere.vao);
            glBindTextureUnit(textureHandle, UNIFORM_TEXTURE_LOCATION);

            glDrawElements(GL_TRIANGLES, sphere.count, GL_UNSIGNED_INT,
                           glx::bufferOffset<GLuint>(0));
		}
		else
		{
            reloadPipeline(atmos);
            updateUniformMatrices(uniformMatrices, width, height, programData);
            glUseProgram(atmos.pipelineHandle);

            glBindVertexArray(sphere.vao);
            glBindTextureUnit(textureHandle, UNIFORM_TEXTURE_LOCATION);

            glDrawElements(GL_TRIANGLES, sphere.count, GL_UNSIGNED_INT,
                           glx::bufferOffset<GLuint>(0));
		}
        
        

        //
        /*
        gui::newFrame(programData.windowData);
        
        // This demo window showcases all the controls that ImGui has
        // available and is here for reference only. If you wish to
        // explore the code to use some of these controls, you can find
        // the source file under:
        // {build-dir}/_deps/atlas-src/external/imgui/imgui_demo.cpp
        //
        // It is recommended that you read the documentation there so
        // you can create a custom UI for your project.
        if (showDemoWindow)
        {
            ImGui::ShowDemoWindow(&showDemoWindow);
        }

        // If you want to create your own window, you must wrap it between
        // ImGui::Begin() and ImGui::End() calls.
        ImGui::Begin("Demo Controls");
        ImGui::Checkbox("Show demo window", &showDemoWindow);

        // Make sure that this FPS counter remains in your application so you
        // can keep track of performance! Note: This counter is tied to the
        // refresh rate of your screen and not to the actual speed of your
        // program. That said, the lab computers refresh at 60Hz, so as long as
        // your program remains at 60Hz it will be fine.
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    1000.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::End();
		*/

		gui::newFrame(programData.windowData);
        ImGui::Begin("Filter Controls");
        if (ImGui::Button("Reset Camera"))
        {
            utils::MayaCamera::resetCamera(programData.camera);
        }

        std::vector<const char*> names{
            "Earth",
            "EarthToSky",
            "SkyFromSpace",
        };
        ImGui::Combo("Render mode", &renderMode, names.data(),
                     static_cast<int>(names.size()));
        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                    100.0f / ImGui::GetIO().Framerate,
                    ImGui::GetIO().Framerate);
        ImGui::End();

        ImGui::Render();
        gui::endFrame(programData.windowData, programData.renderData);

        glfwSwapBuffers(programData.window);
        glfwPollEvents();
    }

    gui::destroyGuiRenderData(programData.renderData);
    gui::destroyGuiWindow(programData.windowData);
    ImGui::DestroyContext();

    glx::destroyGLFWWindow(programData.window);
    glx::terminateGLFW();

    return 0;
}

void onError(int code, char const* message)
{
    fmt::print("error ({}): {}\n", code, message);
}

void onMouseDown(ProgramData& data, int button, int action, int mode,
                 double xPos, double yPos)
{
    using namespace atlas::utils;

    gui::mousePressedCallback(data.windowData, button, action, mode);
    if (action == GLFW_PRESS)
    {
        math::Point2 point{xPos, yPos};

        if (button == GLFW_MOUSE_BUTTON_LEFT)
        {
            data.camera.movement = utils::MayaCamera::Movements::Tumble;
            MayaCamera::mouseDown(data.camera, point);
        }
        else if (button == GLFW_MOUSE_BUTTON_MIDDLE)
        {
            data.camera.movement = utils::MayaCamera::Movements::Track;
            MayaCamera::mouseDown(data.camera, point);
        }
        else if (button == GLFW_MOUSE_BUTTON_RIGHT)
        {
            data.camera.movement = utils::MayaCamera::Movements::Dolly;
            MayaCamera::mouseDown(data.camera, point);
        }
    }
    else
    {
        MayaCamera::mouseUp(data.camera);
    }
}

void onMouseMove(ProgramData& data, double xPos, double yPos)
{
    utils::MayaCamera::mouseMove(data.camera, {xPos, yPos});
}

void onMouseScroll(ProgramData& data, double xOffset, double yOffset)
{
    gui::mouseScrollCallback(xOffset, yOffset);
    utils::MayaCamera::mouseScroll(data.camera, {xOffset, yOffset});
}

void onKeyPress(int key, int scancode, int action, int mods)
{
    gui::keyPressCallback(key, scancode, action, mods);
}

void onChar(unsigned int codepoint)
{
    gui::charCallback(codepoint);
}

GLuint handleMake()
{

	GLuint textureHandle{0};

    std::string dataRoot{DataPath};
    std::string filename = "earth.jpg";
    int width, height, channels;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* data =
        stbi_load(filename.c_str(), &width, &height, &channels, 0);
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
        glTextureParameteri(textureHandle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureHandle, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTextureStorage2D(textureHandle, 1, internalFormat, width, height);
        glTextureSubImage2D(textureHandle, 0, 0, 0, width, height, format,
                            GL_UNSIGNED_BYTE, data);
        glGenerateTextureMipmap(textureHandle);
        stbi_image_free(data);
        return textureHandle;
    }
    else
    {
        stbi_image_free(data);
        return 0;
    }
}

UniformMatrices createUniformMatrices()
{
    UniformMatrices mat;
    glCreateBuffers(1, &mat.ubo);
    glNamedBufferStorage(mat.ubo, glx::size<math::Matrix4>(mat.count), nullptr,
                         GL_DYNAMIC_STORAGE_BIT);
    glBindBufferBase(GL_UNIFORM_BUFFER, MATRIX_BUFFER_BINDING, mat.ubo);
    return mat;
}

Mesh loadMeshFromFile(std::string const& name)
{
    std::string path{DataPath};
    std::string sphereFilename = path + name;
    auto result = utils::loadObjMesh(sphereFilename);

	if (!result)
    {
            std::abort();
    }

	utils::ObjMesh mesh = result.value();

    std::vector<float> data;
    for (auto& vertData : mesh.shapes[0].vertices)
    {
        data.push_back(vertData.position.x);
        data.push_back(vertData.position.y);
        data.push_back(vertData.position.z);

        data.push_back(vertData.normal.x);
        data.push_back(vertData.normal.y);
        data.push_back(vertData.normal.z);

        data.push_back(vertData.texCoord.x);
        data.push_back(vertData.texCoord.y);
    }

    std::vector<GLuint> indices(mesh.shapes[0].indices.size());
    std::transform(mesh.shapes[0].indices.begin(), mesh.shapes[0].indices.end(),
                   indices.begin(), [](std::size_t i) -> GLuint {
                       return static_cast<GLuint>(i);
                   });

    GLuint vbo, ibo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, glx::size<float>(data.size()), data.data(), 0);

    glCreateBuffers(1, &ibo);
    glNamedBufferStorage(ibo, glx::size<GLuint>(indices.size()), indices.data(),
                         0);

    GLuint vao;
    glCreateVertexArrays(1, &vao);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, glx::stride<float>(8));
    glVertexArrayElementBuffer(vao, ibo);

    glEnableVertexArrayAttrib(vao, VERTEX_ATTRIB_LOCATION);
    glEnableVertexArrayAttrib(vao, NORMAL_ATTRIB_LOCATION);
    glEnableVertexArrayAttrib(vao, TEXTURE_ATTRIB_LOCATION);

    glVertexArrayAttribFormat(vao, VERTEX_ATTRIB_LOCATION, 3, GL_FLOAT,
                              GL_FALSE, glx::relativeOffset<float>(0));
    glVertexArrayAttribFormat(vao, NORMAL_ATTRIB_LOCATION, 3, GL_FLOAT,
                              GL_FALSE, glx::relativeOffset<float>(3));
    glVertexArrayAttribFormat(vao, TEXTURE_ATTRIB_LOCATION, 2, GL_FLOAT,
                              GL_FALSE, glx::relativeOffset<float>(6));

    glVertexArrayAttribBinding(vao, VERTEX_ATTRIB_LOCATION, 0);
    glVertexArrayAttribBinding(vao, NORMAL_ATTRIB_LOCATION, 0);
    glVertexArrayAttribBinding(vao, TEXTURE_ATTRIB_LOCATION, 0);

    GLsizei count = static_cast<GLsizei>(mesh.shapes[0].indices.size());

    return {vao, vbo, ibo, count};
}

void updateUniformMatrices(UniformMatrices const& matrices, int width,
                           int height, ProgramData const& data)
{
    static constexpr float nearVal{1.0f};
    static constexpr float farVal{10000000000.0f};
    auto projection =
        glm::perspective(glm::radians(data.camera.fov),
                         static_cast<float>(width) / height, nearVal, farVal);
    auto view = utils::MayaCamera::computeViewMatrix(data.camera);
    auto model =
        glm::scale(math::Matrix4{1.0f}, math::Vector{1.0f, 1.0f, 1.0f});

    std::vector<math::Matrix4> uniformMatrices(matrices.count);
    uniformMatrices[0] = model;
    uniformMatrices[1] = view; // * model;
    uniformMatrices[2] = projection * view * model;
    uniformMatrices[3] = glm::transpose(glm::inverse(model));

    glNamedBufferSubData(matrices.ubo, 0,
                         glx::size<math::Matrix4>(matrices.count),
                         uniformMatrices.data());
}

void init(ProgramData& data)
{
    glx::WindowSettings settings;
    settings.size  = {1280, 720};
    settings.title = "Assignment 4";

    glx::WindowCallbacks callbacks;
    callbacks.mousePressCallback = [&data](int button, int action, int mode,
                                           double xPos, double yPos) {
        onMouseDown(data, button, action, mode, xPos, yPos);
    };
    callbacks.mouseMoveCallback = [&data](double xPos, double yPos) {
        onMouseMove(data, xPos, yPos);
    };
    callbacks.mouseScrollCallback = [&data](double xOffset, double yOffset) {
        onMouseScroll(data, xOffset, yOffset);
    };
    callbacks.keyPressCallback = onKeyPress;
    callbacks.charCallback     = onChar;

    glx::initializeGLFW(onError);
    data.window = glx::createGLFWWindow(settings);

    glx::bindWindowCallbacks(data.window, callbacks);
    glfwMakeContextCurrent(data.window);
    glfwSwapInterval(1);

    glx::createGLContext(data.window, settings.version);
    using namespace atlas::core;
    glx::initializeGLCallback(glx::ErrorSource::All, glx::ErrorType::All,
                              glx::ErrorSeverity::High |
                                  glx::ErrorSeverity::Medium);

    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    gui::initializeGuiWindowData(data.windowData);
    gui::initializeGuiRenderData(data.renderData);
    gui::setGuiWindow(data.windowData, data.window);
}

Pipeline loadPipeline(std::string const& name)
{
    // Feel free to modify this to allow for more (or less) shaders in the
    // pipeline as you require
    Pipeline p;
    p.vertHandle     = glCreateShader(GL_VERTEX_SHADER);
    p.fragHandle     = glCreateShader(GL_FRAGMENT_SHADER);
    p.pipelineHandle = glCreateProgram();

    std::string root{ShaderPath};
    std::string vertFilename{root + name + ".vert"};
    std::string fragFilename{root + name + ".frag"};

    p.vertSource = glx::readShaderSource(vertFilename, p.includeDirs);
    p.fragSource = glx::readShaderSource(fragFilename, p.includeDirs);

    auto compileAndCheck = [](std::string const& source, GLuint handle,
                              glx::ShaderFile const& file) {
        if (auto res = glx::compileShader(source, handle); res)
        {
            fmt::print("{}\n", glx::parseErrorLog(file, *res));
            std::abort();
        }
    };

    compileAndCheck(p.vertSource.sourceString, p.vertHandle, p.vertSource);
    compileAndCheck(p.fragSource.sourceString, p.fragHandle, p.fragSource);

    glAttachShader(p.pipelineHandle, p.vertHandle);
    glAttachShader(p.pipelineHandle, p.fragHandle);

    if (auto res = glx::linkShaders(p.pipelineHandle); res)
    {
        fmt::print("{}\n", *res);
        std::abort();
    }

    return p;
}

void reloadPipeline(Pipeline& p)
{
    if (glx::shouldShaderBeReloaded(p.vertSource))
    {
        glx::reloadShader(p.pipelineHandle, p.vertHandle, p.vertSource,
                          p.includeDirs);
    }

    if (glx::shouldShaderBeReloaded(p.fragSource))
    {
        glx::reloadShader(p.pipelineHandle, p.fragHandle, p.fragSource,
                          p.includeDirs);
    }
}

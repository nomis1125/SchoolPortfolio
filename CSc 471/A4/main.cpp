#include "UniformLocations.glsl"
#include "assignment.hpp"

#include <fmt/printf.h>

int main()
{
    ProgramData programData;
    init(programData);

	auto cIco = createIcosahedron();
	auto icosa = loadPipeline("icosa");
	auto uniformMatrices = createUniformMatrices();

    int tessLevelInner{1};
    int tessLevelOuter{1};
    int maxTessLevel;
    glGetIntegerv(GL_MAX_TESS_GEN_LEVEL, &maxTessLevel);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    while (!glfwWindowShouldClose(programData.window))
    {
        int width, height;
        glfwGetFramebufferSize(programData.window, &width, &height);
        glViewport(0, 0, width, height);
        const float grey{92.0f / 255.0f};
        glClearColor(grey, grey, grey, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		reloadPipeline(icosa);

		updateUniformMatrices(uniformMatrices, width, height, programData);

		//4.1
		glUseProgram(icosa.pipelineHandle);
		glBindVertexArray(cIco.vao);
		//glDrawElements(GL_TRIANGLES, cIco.count, GL_UNSIGNED_INT, glx::bufferOffset<GLuint>(0));

		glUniform1i(glGetUniformLocation(icosa.pipelineHandle, "inner"), tessLevelInner);
		glUniform1i(glGetUniformLocation(icosa.pipelineHandle, "outer"), tessLevelOuter);


		glPatchParameteri(GL_PATCH_VERTICES, 3);
		glDrawElements(GL_PATCHES, cIco.count, GL_UNSIGNED_INT, glx::bufferOffset<GLuint>(0));

        gui::newFrame(programData.windowData);
        ImGui::Begin("Program Controls");
        if (ImGui::Button("Reset Camera"))
        {
            utils::MayaCamera::resetCamera(programData.camera);
        }
        ImGui::Text("Tesselation Level Controls");
        ImGui::SliderInt("Inner", &tessLevelInner, 1, maxTessLevel);
        ImGui::SliderInt("Outer", &tessLevelOuter, 1, maxTessLevel);
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

Mesh createIcosahedron()
{
    // clang-format off
    std::vector<float> data
    {
         0.000f,  0.000f,  1.000f,
         0.894f,  0.000f,  0.447f,
         0.276f,  0.851f,  0.447f,
        -0.724f,  0.526f,  0.447f,
        -0.724f, -0.526f,  0.447f,
         0.276f, -0.851f,  0.447f,
         0.724f,  0.526f, -0.447f,
        -0.276f,  0.851f, -0.447f,
        -0.894f,  0.000f, -0.447f,
        -0.276f, -0.851f, -0.447f,
         0.724f, -0.526f, -0.447f,
         0.000f,  0.000f, -1.000f
    };

    std::vector<GLuint> indices
    {
         2,  1,  0,
         3,  2,  0,
         4,  3,  0,
         5,  4,  0,
         1,  5,  0,
        11,  6,  7,
        11,  7,  8,
        11,  8,  9,
        11,  9, 10,
        11, 10,  6,
         1,  2,  6,
         2,  3,  7,
         3,  4,  8,
         4,  5,  9,
         5,  1, 10,
         2,  7,  6,
         3,  8,  7, 
         4,  9,  8,
         5, 10,  9,
         1,  6, 10
    };
    // clang-format on

    GLuint vbo, ibo;
    glCreateBuffers(1, &vbo);
    glNamedBufferStorage(vbo, glx::size<float>(data.size()), data.data(), 0);

    glCreateBuffers(1, &ibo);
    glNamedBufferStorage(ibo, glx::size<GLuint>(indices.size()), indices.data(),
                         0);

    GLuint vao;
    glCreateVertexArrays(1, &vao);

    glVertexArrayVertexBuffer(vao, 0, vbo, 0, glx::stride<float>(3));
    glVertexArrayElementBuffer(vao, ibo);

    glEnableVertexArrayAttrib(vao, VERTEX_ATTRIB_LOCATION);

    glVertexArrayAttribFormat(vao, VERTEX_ATTRIB_LOCATION, 3, GL_FLOAT,
                              GL_FALSE, glx::relativeOffset<float>(0));

    glVertexArrayAttribBinding(vao, VERTEX_ATTRIB_LOCATION, 0);

    GLsizei count = static_cast<GLsizei>(indices.size());

    return {vao, vbo, ibo, count};
}

Pipeline loadPipeline(std::string const& name)
{
    Pipeline p;
    p.vertHandle     = glCreateShader(GL_VERTEX_SHADER);
    p.tescHandle     = glCreateShader(GL_TESS_CONTROL_SHADER);
    p.teseHandle     = glCreateShader(GL_TESS_EVALUATION_SHADER);
    p.geomHandle     = glCreateShader(GL_GEOMETRY_SHADER);
    p.fragHandle     = glCreateShader(GL_FRAGMENT_SHADER);
    p.pipelineHandle = glCreateProgram();

    std::string root{ShaderPath};
    std::string vertFilename{root + name + ".vert"};
    std::string tescFilename{root + name + ".tesc"};
    std::string teseFilename{root + name + ".tese"};
    std::string geomFilename{root + name + ".geom"};
    std::string fragFilename{root + name + ".frag"};

    p.vertSource = glx::readShaderSource(vertFilename, p.includeDirs);
    p.tescSource = glx::readShaderSource(tescFilename, p.includeDirs);
    p.teseSource = glx::readShaderSource(teseFilename, p.includeDirs);
    p.geomSource = glx::readShaderSource(geomFilename, p.includeDirs);
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
    compileAndCheck(p.tescSource.sourceString, p.tescHandle, p.tescSource);
    compileAndCheck(p.teseSource.sourceString, p.teseHandle, p.teseSource);
    compileAndCheck(p.geomSource.sourceString, p.geomHandle, p.geomSource);
    compileAndCheck(p.fragSource.sourceString, p.fragHandle, p.fragSource);

    glAttachShader(p.pipelineHandle, p.vertHandle);
    glAttachShader(p.pipelineHandle, p.tescHandle);
    glAttachShader(p.pipelineHandle, p.teseHandle);
    glAttachShader(p.pipelineHandle, p.geomHandle);
    glAttachShader(p.pipelineHandle, p.fragHandle);

    if (auto res = glx::linkShaders(p.pipelineHandle); res)
    {
        fmt::print("{}\n", *res);
        std::abort();
    }

    return p;
}

<<<<<<< HEAD
UniformMatrices createUniformMatrices()
{
	UniformMatrices mat;
	glCreateBuffers(1, &mat.ubo);
	glNamedBufferStorage(mat.ubo, glx::size<math::Matrix4>(mat.count), nullptr,
		GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, MATRIX_BUFFER_BINDING, mat.ubo);
	return mat;
=======
UniformMatrices createUniformMatrices()
{
	UniformMatrices mat;
	glCreateBuffers(1, &mat.ubo);
	glNamedBufferStorage(mat.ubo, glx::size<math::Matrix4>(mat.count), nullptr,
		GL_DYNAMIC_STORAGE_BIT);
	glBindBufferBase(GL_UNIFORM_BUFFER, MATRIX_BUFFER_BINDING, mat.ubo);
	return mat;
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}

void reloadPipeline(Pipeline& p)
{
    if (glx::shouldShaderBeReloaded(p.vertSource))
    {
        glx::reloadShader(p.pipelineHandle, p.vertHandle, p.vertSource,
                          p.includeDirs);
    }

    if (glx::shouldShaderBeReloaded(p.tescSource))
    {
        glx::reloadShader(p.pipelineHandle, p.tescHandle, p.tescSource,
                          p.includeDirs);
    }

    if (glx::shouldShaderBeReloaded(p.teseSource))
    {
        glx::reloadShader(p.pipelineHandle, p.teseHandle, p.teseSource,
                          p.includeDirs);
    }

    if (glx::shouldShaderBeReloaded(p.geomSource))
    {
        glx::reloadShader(p.pipelineHandle, p.geomHandle, p.geomSource,
                          p.includeDirs);
    }

    if (glx::shouldShaderBeReloaded(p.fragSource))
    {
        glx::reloadShader(p.pipelineHandle, p.fragHandle, p.fragSource,
                          p.includeDirs);
    }
}

<<<<<<< HEAD
void updateUniformMatrices(UniformMatrices const& matrices, int width,
	int height, ProgramData const& data)
{
	static constexpr float nearVal{ 1.0f };
	static constexpr float farVal{ 10000000000.0f };
	auto projection =
		glm::perspective(glm::radians(data.camera.fov),
			static_cast<float>(width) / height, nearVal, farVal);
	auto view = utils::MayaCamera::computeViewMatrix(data.camera);
	auto model =
		glm::scale(math::Matrix4{ 1.0f }, math::Vector{ 1.0f, 1.0f, 1.0f });

	std::vector<math::Matrix4> uniformMatrices(matrices.count);
	uniformMatrices[0] = model;
	uniformMatrices[1] = view * model;
	uniformMatrices[2] = projection * view * model;
	uniformMatrices[3] = glm::transpose(glm::inverse(model));

	glNamedBufferSubData(matrices.ubo, 0,
		glx::size<math::Matrix4>(matrices.count),
		uniformMatrices.data());
=======
void updateUniformMatrices(UniformMatrices const& matrices, int width,
	int height, ProgramData const& data)
{
	static constexpr float nearVal{ 1.0f };
	static constexpr float farVal{ 10000000000.0f };
	auto projection =
		glm::perspective(glm::radians(data.camera.fov),
			static_cast<float>(width) / height, nearVal, farVal);
	auto view = utils::MayaCamera::computeViewMatrix(data.camera);
	auto model =
		glm::scale(math::Matrix4{ 1.0f }, math::Vector{ 1.0f, 1.0f, 1.0f });

	std::vector<math::Matrix4> uniformMatrices(matrices.count);
	uniformMatrices[0] = model;
	uniformMatrices[1] = view * model;
	uniformMatrices[2] = projection * view * model;
	uniformMatrices[3] = glm::transpose(glm::inverse(model));

	glNamedBufferSubData(matrices.ubo, 0,
		glx::size<math::Matrix4>(matrices.count),
		uniformMatrices.data());
>>>>>>> 17e2c9f0015b84508c0980fb762a587738dc55ae
}
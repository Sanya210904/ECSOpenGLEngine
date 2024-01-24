
#include <iostream>

#include <input/Input.hpp>
#include <input/InputManager.hpp>

#include <graphics/IndexBuffer.hpp>
#include <graphics/Mesh.hpp>
#include <graphics/Shader.hpp>
#include <graphics/Texture.hpp>
#include <graphics/VertexArrayBuffer.hpp>
#include <graphics/VertexBuffer.hpp>
#include <graphics/VertexBufferLayout.hpp>

#include <components/CameraComponent.hpp>
#include <components/IDComponent.hpp>
#include <components/MeshComponent.hpp>
#include <components/PrimitiveComponent.hpp>
#include <components/SpriteComponent.hpp>
#include <components/TagComponent.hpp>
#include <components/TransformComponent.hpp>

#include <scene/Entity.hpp>
#include <scene/Scene.hpp>

#include <componentsFactory/PrimitiveFactory.hpp>
#include <componentsFactory/SpriteFactory.hpp>

#include <utils/Debug.hpp>
#include <utils/StringUtils.hpp>

#include <core/ResourceManager.hpp>

#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

class App {
public:
    App();
    ~App();
    void run();

private:
    void initWindow();
    void initImGui();
    void initCallbacks();
    void initInputDevices();
    void initInputMappings();
    void mainLoop();

    Input m_input;
    InputManager m_inputManager;
    GLFWwindow* m_window;

    unsigned int m_scrWidth{1600};
    unsigned int m_scrHeight{900};

    enum ActiveWindowFlags { NO_WINDOWS = 0, ENTITIES_PANEL = 1 << 0 };

    unsigned int m_activeWindowFlags = NO_WINDOWS;
};

App::App() {}

App::~App() { glfwTerminate(); }

void App::run() {
    initWindow();
    initCallbacks();
    initImGui();
    initInputDevices();
    initInputMappings();
    mainLoop();
}

void App::initWindow() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif // DEBUG

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for MAC OS X
#endif                                                   // __APPLE__

    m_window = glfwCreateWindow(m_scrWidth, m_scrHeight, "Test", NULL, NULL);

    if (m_window == NULL) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(m_window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        throw std::runtime_error("Failed to load GLAD");

#ifdef _DEBUG
    ENABLE_DEBUG
#endif // _DEBUG

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST);

    glfwSetWindowUserPointer(m_window, this);
}

void App::initImGui() {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto& io = ImGui::GetIO();
    (void)io;

    ImGui_ImplGlfw_InitForOpenGL(m_window, true);
    ImGui_ImplOpenGL3_Init();
}

void App::initCallbacks() {
    glfwSetKeyCallback(m_window,
                       [](GLFWwindow* window, int key, int scancode, int action, int mods) {
                           auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));

                           if (!app)
                               return;

                           app->m_input.updateKeyboardState(
                               key, action == GLFW_PRESS || action == GLFW_REPEAT ? 1.0f : 0.0f);
                       });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));

        if (!app)
            return;

        app->m_input.updateMouseState(button, action == GLFW_PRESS ? 1.0f : 0.0f);
    });
}

void App::initInputDevices() {
    m_inputManager.regDevice(
        {InputDeviceType::KEYBOARD, 0,
         std::bind(&Input::getKeyboardState, &m_input, std::placeholders::_1)});

    m_inputManager.regDevice({InputDeviceType::MOUSE, 0,
                              std::bind(&Input::getMouseState, &m_input, std::placeholders::_1)});
}

void App::initInputMappings() {
    m_inputManager.mapInputToAction(InputKey::KEY_W, "move_forward");
    m_inputManager.mapInputToAction(InputKey::KEY_S, "move_backward");
    m_inputManager.mapInputToAction(InputKey::KEY_A, "move_left");
    m_inputManager.mapInputToAction(InputKey::KEY_D, "move_right");
    m_inputManager.mapInputToAction(InputKey::KEY_SPACE, "move_up");
    m_inputManager.mapInputToAction(InputKey::KEY_LEFT_SHIFT, "move_down");
    m_inputManager.mapInputToAction(InputKey::KEY_LEFT_CONTROL, "move_faster");
    m_inputManager.mapInputToAction(InputKey::KEY_LEFT_ALT, "move_slower");

    m_inputManager.mapInputToAction(InputKey::KEY_F1, "open_entities_win");

    m_inputManager.mapInputToAction(InputKey::MOUSE_OFFSET_X, "look_x");
    m_inputManager.mapInputToAction(InputKey::MOUSE_OFFSET_Y, "look_y");
}

void App::mainLoop() {
    float lastTime = 0.0f;
    float time = static_cast<float>(glfwGetTime());
    float delta = time - lastTime;

    Scene scene;
    Entity _;
    glm::mat4 localView, localProj;

    scene.createEntity("x+").addComponent<PrimitiveComponent>(
        PrimitiveFactory::createRect({1.0f, 0.0f, 0.0f, 1.0f}));
    scene.createEntity("y+").addComponent<PrimitiveComponent>(
        PrimitiveFactory::createRect({0.0f, 1.0f, 0.0f, 1.0f}));
    scene.createEntity("z+").addComponent<PrimitiveComponent>(
        PrimitiveFactory::createRect({0.0f, 0.0f, 1.0f, 1.0f}));
    scene.createEntity("0").addComponent<PrimitiveComponent>(
        PrimitiveFactory::createRect({0.0f, 0.0f, 0.0f, 1.0f}));

    scene.createEntity("Polygon1")
        .addComponent<PrimitiveComponent>(
            PrimitiveFactory::createPolygon({0.6f, 0.0f, 1.0f, 0.5f}, 6));

    scene.createEntity("Sprite1").addComponent<SpriteComponent>(
        SpriteFactory::createSprite("res/images/textures/maxresdefault.jpg", true));

    _ = scene.createEntity("Model1");
    _.addComponent<MeshComponent>(
        std::make_shared<Mesh>("res/models/hl2gordon_freeman/gordon.obj"));
    {
        auto& transform = _.getComponent<TransformComponent>();
        transform.translation = {-5.0f, 0.0f, 0.0f};
        // transform.rotation = { 0.0f, -1.57f, 1.57f };
        transform.eulerRotate({0.0f, 1.57f, 1.57f});
        transform.scale = {0.05f, 0.05f, 0.05f};
    };

    _ = scene.createEntity("Model2");
    _.addComponent<MeshComponent>(std::make_shared<Mesh>("res/models/backpack/backpack.obj"));
    {
        auto& transform = _.getComponent<TransformComponent>();
        transform.translation = {-5.0f, 0.0f, 0.0f};
        transform.scale = {0.4f, 0.4f, 0.4f};
    };

    _ = scene.createEntity("Model3");
    _.addComponent<MeshComponent>(std::make_shared<Mesh>("res/models/mushrooms/Mushrooms1.obj"));
    {
        auto& transform = _.getComponent<TransformComponent>();
        transform.translation = {-1.0f, -1.0f, -2.0f};
        transform.scale = {0.01f, 0.01f, 0.01f};
    };

    auto camera = scene.createEntity("Camera");
    camera.addComponent<CameraComponent>();

    m_inputManager.registerActionCallback(
        "look_x", {"Scene", [&camera](InputSource, int, float value) {
                       auto& transform = camera.getComponent<TransformComponent>();

                       glm::quat yaw =
                           glm::angleAxis(glm::radians(-value) * 0.3f, glm::vec3(0.0f, 1.0f, 0.0f));
                       transform.orientation = glm::normalize(yaw * transform.orientation);

                       return true;
                   }});

    m_inputManager.registerActionCallback(
        "look_y", {"Scene", [&camera](InputSource, int, float value) {
                       auto& transform = camera.getComponent<TransformComponent>();
                       auto& cameraComp = camera.getComponent<CameraComponent>();

                       glm::quat pitch =
                           glm::angleAxis(glm::radians(-value) * 0.3f, glm::vec3(1.0f, 0.0f, 0.0f));
                       transform.orientation = glm::normalize(transform.orientation * pitch);

                       // transform.orientation.x = glm::clamp(transform.orientation.x,
                       // -0.5f, 0.5f); transform.orientation.z =
                       // glm::clamp(transform.orientation.z, -0.5f, 0.5f);

                       glm::vec3 eulerR = transform.getEulerRotation();
                       eulerR.y = glm::clamp(eulerR.y, -45.0f, 45.0f);
                       transform.eulerRotate(eulerR);

                       // transform.rotation.y -= glm::radians(value * 0.3f);
                       // transform.rotation.y = glm::clamp(
                       //     transform.rotation.y,
                       //     glm::radians(-89.0f),
                       //     glm::radians(89.0f)
                       // );
                       return true;
                   }});

    m_inputManager.registerActionCallback("open_entities_win",
                                          {"Scene", [this](InputSource, int, float value) {
                                               if (value > 0.0f)
                                                   this->m_activeWindowFlags ^= ENTITIES_PANEL;
                                               return true;
                                           }});

    const float camSpeed = 2.0f;

    while (!glfwWindowShouldClose(m_window)) {

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //| GL_STENCIL_BUFFER_BIT);

        time = static_cast<float>(glfwGetTime());
        delta = time - lastTime;
        lastTime = time;

        auto& cameraComp = camera.getComponent<CameraComponent>();
        auto& cameraTrans = camera.getComponent<TransformComponent>();

        if (m_inputManager.getActionIsCalled("move_forward")) {
            cameraTrans.translation +=
                cameraComp.getForward(cameraTrans.getTransform()) * camSpeed * delta;
        }
        if (m_inputManager.getActionIsCalled("move_backward")) {
            cameraTrans.translation -=
                cameraComp.getForward(cameraTrans.getTransform()) * camSpeed * delta;
        }
        if (m_inputManager.getActionIsCalled("move_right")) {
            cameraTrans.translation +=
                cameraComp.getRight(cameraTrans.getTransform()) * camSpeed * delta;
        }
        if (m_inputManager.getActionIsCalled("move_left")) {
            cameraTrans.translation -=
                cameraComp.getRight(cameraTrans.getTransform()) * camSpeed * delta;
        }
        if (m_inputManager.getActionIsCalled("move_up")) {
            cameraTrans.translation += glm::vec3(0.0f, 1.0f, 0.0f) * camSpeed * delta;
        }
        if (m_inputManager.getActionIsCalled("move_down")) {
            cameraTrans.translation -= glm::vec3(0.0f, 1.0f, 0.0f) * camSpeed * delta;
        }

        localProj =
            cameraComp.getProj(static_cast<float>(m_scrWidth) / static_cast<float>(m_scrHeight));
        localView = CameraComponent::getView(cameraTrans.getTransform());

        scene.getAllEntitiesWith<TransformComponent, PrimitiveComponent>().each(
            [time, localProj, localView](TransformComponent& transformComp,
                                         PrimitiveComponent& primitiveComp) {
                transformComp.translation = primitiveComp.color;
                // transformComp.rotation = { sin(time), -cos(time), time };

                primitiveComp.shader->bind();
                primitiveComp.vao->bind();

                primitiveComp.shader->setUniform("model", transformComp.getTransform());
                primitiveComp.shader->setUniform("view", localView);
                primitiveComp.shader->setUniform("proj", localProj);
                glDrawArrays(primitiveComp.type, 0, primitiveComp.vertexCount);

                primitiveComp.vao->unbind();
                primitiveComp.shader->unbind();
            });

        scene.getAllEntitiesWith<TransformComponent, SpriteComponent>().each(
            [time, localProj, localView](TransformComponent& transformComp,
                                         SpriteComponent& spriteComp) {
                transformComp.translation = {3.0f, 0.0f, 0.0f};
                transformComp.eulerRotate({-sin(time), cos(time), time});
                // transformComp.rotation = { cos(time), -sin(time), time };

                spriteComp.shader->bind();
                spriteComp.vao->bind();

                spriteComp.shader->setUniform("model", transformComp.getTransform());
                spriteComp.shader->setUniform("view", localView);
                spriteComp.shader->setUniform("proj", localProj);
                spriteComp.texture->bind();
                glDrawArrays(spriteComp.type, 0, spriteComp.vertexCount);

                spriteComp.texture->unbind();
                spriteComp.vao->unbind();
                spriteComp.shader->unbind();
            });

        scene.getAllEntitiesWith<TransformComponent, MeshComponent>().each(
            [time, localProj, localView](TransformComponent& transformComp,
                                         MeshComponent& meshComp) {
                meshComp.meshGeometry->shader->bind();
                meshComp.meshGeometry->shader->setUniform("model", transformComp.getTransform());
                meshComp.meshGeometry->shader->setUniform("view", localView);
                meshComp.meshGeometry->shader->setUniform("proj", localProj);

                glEnable(GL_CULL_FACE);
                meshComp.meshGeometry->draw();
                glDisable(GL_CULL_FACE);

                meshComp.meshGeometry->shader->unbind();
            });

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (m_activeWindowFlags == NO_WINDOWS) {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            m_inputManager.turnOnInputSource(InputSource::MOUSE);
        }

        if (m_activeWindowFlags & ENTITIES_PANEL) {
            glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            m_inputManager.turnOffInputSource(InputSource::MOUSE);

            if (ImGui::Begin("Entities Panel")) {
                //     ImGui::SetWindowFontScale(1.3f);
                //     // ImGui::InputFloat3("RotCam",
                //     glm::value_ptr(cameraTrans.rotation));
                //
                //     auto forw =
                //     cameraComp.getForward(cameraTrans.getTransform());
                //     ImGui::Text("Forward vector - x:[%f], y:[%f], z:[%f]",
                //     forw.x, forw.y, forw.z);
                //
                //     auto orientation = cameraTrans.orientation;
                //     ImGui::Text("Orientation quat - w:[%f], i:[%f], j:[%f],
                //     k:[%f]", orientation.w, orientation.x, orientation.y,
                //     orientation.z); ImGui::DragFloat4("Orientation quat:",
                //     glm::value_ptr(orientation), 0.05f);
                ImGui::End();
            }
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwPollEvents();
        m_input.updateCursorState(m_window);
        m_inputManager.processInput();
        glfwSwapBuffers(m_window);
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

int main() {
    App app;
    try {
        app.run();
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

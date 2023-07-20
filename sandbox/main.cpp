
#include <iostream>

#include <input/Input.hpp>
#include <input/InputManager.hpp>

#include <graphics/VertexBuffer.hpp>
#include <graphics/IndexBuffer.hpp>
#include <graphics/VertexArrayBuffer.hpp>
#include <graphics/VertexBufferLayout.hpp>
#include <graphics/Shader.hpp>
#include <graphics/Texture.hpp>
#include <graphics/Model.hpp>

#include <components/IDComponent.hpp>
#include <components/TagComponent.hpp>
#include <components/TransformComponent.hpp>
#include <components/PrimitiveComponent.hpp>
#include <components/CameraComponent.hpp>
#include <components/SpriteComponent.hpp>
#include <components/ModelComponent.hpp>

#include <scene/Scene.hpp>
#include <scene/Entity.hpp>

#include <componentsFactory/PrimitiveFactory.hpp>
#include <componentsFactory/CameraFactory.hpp>
#include <componentsFactory/SpriteFactory.hpp>

#include <utils/Debug.hpp>
#include <utils/StringUtils.hpp>

#include <core/ResourceManager.hpp>

#include <GLFW/glfw3.h>

class App
{
public:
	App();
	~App();
	void run();

private:
	void initWindow();
	void initCallbacks();
	void initInputDevices();
	void initInputMappings();
	void mainLoop();

	Input m_input;
	InputManager m_inputManager;
	GLFWwindow* m_window;

	unsigned int m_scrWidth {1600};
	unsigned int m_scrHeight {900};
};

App::App()
{ }

App::~App()
{
	glfwTerminate();
}

void App::run()
{
	initWindow();
	initCallbacks();
	initInputDevices();
	initInputMappings();
	mainLoop();
}

void App::initWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, true);
#endif // DEBUG


#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // for MAC OS X
#endif // __APPLE__

	m_window = glfwCreateWindow(m_scrWidth, m_scrHeight, "Test", NULL, NULL);

	if (m_window == NULL)
	{
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

void App::initCallbacks()
{
	glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));

		if (!app) return;

		app->m_input.updateKeyboardState(key, action == GLFW_PRESS || action == GLFW_REPEAT
			? 1.0f
			: 0.0f);
	});

	glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
	{
		auto* app = static_cast<App*>(glfwGetWindowUserPointer(window));

		if (!app) return;

		app->m_input.updateMouseState(button, action == GLFW_PRESS
			? 1.0f
			: 0.0f);
	});
}

void App::initInputDevices()
{
	m_inputManager.regDevice([&](){
		InputDevice _;
		_.type = InputDeviceType::KEYBOARD;
		_.index = 0;
		_.stateFunc = std::bind(&Input::getKeyboardState, &m_input, std::placeholders::_1);
		return _;
	}());

	m_inputManager.regDevice([&](){
		InputDevice _;
		_.type = InputDeviceType::MOUSE;
		_.index = 0;
		_.stateFunc = std::bind(&Input::getMouseState, &m_input, std::placeholders::_1);
		return _;
	}());
}

void App::initInputMappings()
{
	m_inputManager.mapInputToAction(InputKey::KEY_W, "move_forward");
	m_inputManager.mapInputToAction(InputKey::KEY_S, "move_backwards");
	m_inputManager.mapInputToAction(InputKey::KEY_A, "move_left");
	m_inputManager.mapInputToAction(InputKey::KEY_D, "move_right");
	m_inputManager.mapInputToAction(InputKey::KEY_SPACE, "move_up");
	m_inputManager.mapInputToAction(InputKey::KEY_LEFT_SHIFT, "move_down");
	m_inputManager.mapInputToAction(InputKey::KEY_LEFT_CONTROL, "move_faster");
	m_inputManager.mapInputToAction(InputKey::KEY_LEFT_ALT, "move_slower");

	m_inputManager.mapInputToAction(InputKey::MOUSE_OFFSET_X, "move_look_x");
	m_inputManager.mapInputToAction(InputKey::MOUSE_OFFSET_Y, "move_look_y");
}

void App::mainLoop()
{
	float lastTime = 0.0f;
	float time = static_cast<float>(glfwGetTime());
	float delta = time - lastTime;

	Scene scene;
	Entity _;
	glm::mat4 localView, localProj;

	scene.createEntity("Polygon1").addComponent<PrimitiveComponent>(
		PrimitiveFactory::createPolygon({ 0.6f, 0.0f, 1.0f, 0.5f }, 6)
	);

	scene.createEntity("Sprite1").addComponent<SpriteComponent>(
		SpriteFactory::createSprite("res/images/textures/maxresdefault.jpg", true)
	);

	_ = scene.createEntity("Model1");
	_.addComponent<ModelComponent>(
		Model("res/models/hl2gordon_freeman/gordon.obj")
	);
	{		
		auto& transform = _.getComponent<TransformComponent>();
		transform.translation = { -5.0f, 0.0f, 0.0f };
		transform.rotation = { 0.0f, 1.57f, 1.57f };
		transform.scale = { 0.05f, 0.05f, 0.05f };
	};

	_ = scene.createEntity("Model2");
	_.addComponent<ModelComponent>(
		Model("res/models/backpack/backpack.obj")
	);
	{
		auto& transform = _.getComponent<TransformComponent>();
		transform.translation = { -5.0f, 0.0f, 0.0f };
		transform.scale = { 0.4f, 0.4f, 0.4f };
	};

	_ = scene.createEntity("Model3");
	_.addComponent<ModelComponent>(
		Model("res/models/mushrooms/Mushrooms1.obj")
	);
	{
		auto& transform = _.getComponent<TransformComponent>();
		transform.translation = { -1.0f, -1.0f, -2.0f };
		transform.scale = { 0.01f, 0.01f, 0.01f };
	};

	scene.createEntity("Camera").addComponent<CameraComponent>(
		CameraFactory::createPerspectiveCamera(m_scrWidth, m_scrHeight, 45.0f, 0.01f, 100.0f)
	);

	scene.getAllEntitiesWith<CameraComponent>().each(
		[&](CameraComponent& camera)
		{
			std::unordered_map<std::string, Camera::Action> moveSet {
				{"move_forward", Camera::Action::FORWARD}, 
				{"move_backwards", Camera::Action::BACKWARDS},
				{"move_left", Camera::Action::LEFT},
				{"move_right", Camera::Action::RIGHT},
				{"move_up", Camera::Action::UP},
				{"move_down", Camera::Action::DOWN}
			};

			for (const auto& move : moveSet)
			{
				m_inputManager.registerActionCallback(move.first, {
					"Scene",
					[&camera, move](InputSource, int, float value)
					{
						camera.camera.move(value, 2.0f, 2.0f, move.second);
						return true;
					}
				});
			}

			m_inputManager.registerActionCallback("move_faster", {
				"Scene",
				[&camera](InputSource, int, float value)
				{
					if (value >= 0.5f)
						camera.camera.setSpeedMode(Camera::SpeedMode::FAST);
					else
						camera.camera.setSpeedMode(Camera::SpeedMode::NORMAL);
					return true;
				}
			});

			m_inputManager.registerActionCallback("move_slower", {
				"Scene",
				[&camera](InputSource, int, float value)
				{
					if (value >= 0.5f)
						camera.camera.setSpeedMode(Camera::SpeedMode::SLOW);
					else
						camera.camera.setSpeedMode(Camera::SpeedMode::NORMAL);
					return true;
				}
			});

			m_inputManager.registerActionCallback("move_look_x", {
				"Scene",
				[&camera](InputSource, int, float value)
				{
					camera.camera.transforms.eulerAngles.x += glm::radians(value * 0.3f);
					return true;
				}
			});

			m_inputManager.registerActionCallback("move_look_y", {
				"Scene",
				[&camera](InputSource, int, float value)
				{
					camera.camera.transforms.eulerAngles.y -= glm::radians(value * 0.3f);
					camera.camera.transforms.eulerAngles.y = glm::clamp(camera.camera.transforms.eulerAngles.y, glm::radians(-89.0f), glm::radians(89.0f));
					return true;
				}
			});
		}
	);

	while (!glfwWindowShouldClose(m_window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //| GL_STENCIL_BUFFER_BIT);	

		time = static_cast<float>(glfwGetTime());
		delta = time - lastTime;
		lastTime = time;

		glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		scene.getAllEntitiesWith<CameraComponent>().each(
			[&](CameraComponent& camera)
			{
				localProj = camera.camera.getProjection();
				localView = camera.camera.getView();

				camera.camera.onUpdate(delta);
			});

		scene.getAllEntitiesWith<TransformComponent, PrimitiveComponent>().each(
			[time, localProj, localView](TransformComponent& transform, PrimitiveComponent& primitive)
			{
				transform.rotation = { sin(time), -cos(time), time };

				primitive.shader->bind();
				primitive.vao->bind();

				primitive.shader->setUniform("model", transform.getTransform());
				primitive.shader->setUniform("view", localView);
				primitive.shader->setUniform("proj", localProj);
				glDrawArrays(primitive.type, 0, primitive.vertexCount);

				primitive.vao->unbind();
				primitive.shader->unbind();
			});

		scene.getAllEntitiesWith<TransformComponent, SpriteComponent>().each(
			[time, localProj, localView](TransformComponent& transform, SpriteComponent& sprite)
			{
				transform.translation = { 3.0f, 0.0f, 0.0f};
				transform.rotation = { cos(time), -sin(time), time };

				sprite.shader->bind();
				sprite.vao->bind();

				sprite.shader->setUniform("model", transform.getTransform());
				sprite.shader->setUniform("view", localView);
				sprite.shader->setUniform("proj", localProj);
				sprite.texture->bind();
				glDrawArrays(sprite.type, 0, sprite.vertexCount);

				sprite.texture->unbind();
				sprite.vao->unbind();
				sprite.shader->unbind();
			});
		
		scene.getAllEntitiesWith<TransformComponent, ModelComponent>().each(
			[time, localProj, localView](TransformComponent& transform, ModelComponent& model)
			{				
				model.model.shader->bind();
				model.model.shader->setUniform("model", transform.getTransform());
				model.model.shader->setUniform("view", localView);
				model.model.shader->setUniform("proj", localProj);

				glEnable(GL_CULL_FACE);
				model.model.draw();
				glDisable(GL_CULL_FACE);

				model.model.shader->unbind();
			}
		);
		
		glfwPollEvents();
		m_input.updateCursorState(m_window);
		m_inputManager.processInput();
		glfwSwapBuffers(m_window);
	}
}

int main()
{
	App app;
	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#include <shader.h>
#include <camera.h>
#include <mesh.h>
#include <room.h>
#include <model.h>
#include <levelGeneration.h>
#include <light.h>
#include <skybox.h>

#include <time.h>

#include <iostream>
#include <vector>
#include <array>
#include <algorithm>
#include <thread>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void LoadLevelFromFile();
string SelectTexture(int texNumber);
string SelectModel(int modelNumber);

void GetFPS();
void GetUserCommands();
void DepthMapFBO();

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600; 

// camera
Camera camera(glm::vec3(0.0f, 0.5f, 0.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int initial_time = time(NULL), final_time, frame_count = 0;

bool showFPS = false;

// lighting
Light directionalLight = Light({ 0,0,0 }, { 0.0f, -1.0f, 0.0f }, 2);

vector<Room> rooms = {};
vector<Light> pointLights = {};
vector<Light> spotLights = {};
vector<Mesh> pointMeshes = {};
vector<Model> modelObjs = {};

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	///DepthMapFBO();

	// build and compile shaders
	// -------------------------
	Shader lightObjShader("light.vert", "light.frag");
	Shader normalMapShader("normalShader.vert", "normalShader.frag");
	Shader skyboxShader("skyboxShader.vert", "skyboxShader.frag");

	Skybox skybox = Skybox("craterlake");

	// load models
	// -----------
	LoadLevelFromFile();

	string playerText = "";

	while (!playerText._Equal("Start"))
	{
		std::cout << "Enter Commands" << endl;
		cin >> playerText;

		if (playerText._Equal("LoadModel"))
		{
			int x, y, z;
			string modelPath = "";

			cout << "Input Name: ";
			cin >> modelPath;

			cout << "Input Position: " << endl;
			cout << "X Position: ";
			cin >> x;
			cout << "Y Position: ";
			cin >> y;
			cout << "Z Position: ";
			cin >> z;

			cout << "Loading..." << endl;

			string fullPath = ("Models/" + modelPath + "/");

			modelObjs.push_back(Model(fullPath.c_str(),modelPath, glm::vec3(x, y, z)));
		}
		else if (playerText._Equal("FPS"))
		{
			showFPS = !showFPS;

			cout << "Show FPS - " << showFPS << endl;
		}
		else if (playerText._Equal("NumTris"))
		{
			cout << triCount << " - Triangles " << endl;
		}
		else if (playerText._Equal("Help"))
		{
			cout << "Commands Are: Start, LoadModel, FPS, NumTris" << endl;
		}
		else if (playerText._Equal("Start"))
		{
			cout << "Starting Level..." << endl;
		}
		else
			cout << "Invalid Command: " << playerText << endl;
	}

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);

	//lightingShader.use();
	//lightingShader.setInt("material.diffuse", 0);
	//lightingShader.setInt("material.specular", 1);

	normalMapShader.use();
	normalMapShader.setInt("material.diffuse", 0);
	normalMapShader.setInt("material.specular", 1);
	normalMapShader.setInt("material.normal", 2);

	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// don't forget to enable shader before setting uniforms
		//lightingShader.use();
		//lightingShader.setVec3("viewPos", camera.Position);
		//lightingShader.setFloat("material.shininess", 15.0f);
		normalMapShader.use();
		normalMapShader.setVec3("viewPos", camera.Position);
		normalMapShader.setFloat("material.shininess", 15.0f);

		// view/projection tranansformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		//lightingShader.setMat4("projection", projection);
		//lightingShader.setMat4("view", view);

		normalMapShader.setMat4("projection", projection);
		normalMapShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		//lightingShader.setMat4("model", model);
		normalMapShader.setMat4("model", model);

		for (unsigned int i = 0; i < rooms.size(); ++i)
		{			
			rooms[i].Draw(normalMapShader);
		}

		for (unsigned int i = 0; i < modelObjs.size(); ++i)
		{
			vec3 pos = modelObjs[i].position;

			model = glm::translate(model, pos);
			normalMapShader.setMat4("model", model);

			modelObjs[i].Draw(normalMapShader);

			model = glm::mat4(1.0f);
		}
		normalMapShader.setMat4("model", model);

		directionalLight.RenderLight(0, normalMapShader);

		for (int i = 0; i < pointLights.size(); i++)
		{
			pointLights[i].RenderLight(i , normalMapShader);

			spotLights[i].RenderLight(i , normalMapShader);
		}

		lightObjShader.use();
		lightObjShader.setMat4("projection", projection);
		lightObjShader.setMat4("view", view);
		lightObjShader.setMat4("model", model);

		for (int i = 0; i < pointMeshes.size(); i++)
		{
			pointMeshes[i].Draw(lightObjShader);
		}

		model = glm::mat4(1.0f);
		view = camera.GetViewMatrix();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

		// draw skybox as last
		glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.use();
		view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
		skyboxShader.setMat4("view", view);
		skyboxShader.setMat4("projection", projection);
		skybox.drawSkybox(skyboxShader);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
		GetFPS();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void LoadLevelFromFile()
{
	bool validLevel = false;
	string levelName = "";
	int numRooms = 0;
	string jsonFileString = "";
	string line = "";
	json jsonObj;

	while (!validLevel)
	{
		std::cout << "Enter Level Name" << endl;
		cin >> levelName;

		ifstream jsonFile(levelName + ".json");

		while (jsonFile.is_open())
		{
			while (getline(jsonFile, line))
			{
				jsonFileString += line;
			}
			jsonFile.close();
		}

		if (jsonFileString != "")
		{
			validLevel = true;
			jsonObj = json::parse(jsonFileString); // works its a visual studio bug
		}
		else
		{
			cout << "Invalid File - " << levelName << endl;
		}
	}

	if (!jsonObj["Rooms"].is_null())
	{
		numRooms = jsonObj["Rooms"].size(); // works its a visual studio bug

		cout << numRooms << " Number of Rooms" << endl;

		for (int i = 0; i < numRooms; ++i)
		{
			json room = jsonObj["Rooms"][i];

			float roomX = 0;
			float roomY = 0;
			float roomZ = 0;

			int length = 1;
			int width = 2;
			int height = 1;

			int floorTex = 0;
			int wallTex = 0;
			int ceilingTex = 0;

			if (!room["Position"][0].is_null())
				roomX = room["Position"][0];
			if (!room["Position"][1].is_null())
				roomY = room["Position"][1];
			if (!room["Position"][2].is_null())
				roomZ = room["Position"][2];

			if (!room["Length"].is_null())
				length = room["Length"];
			if (!room["Width"].is_null())
				width = room["Width"];
			if (!room["Height"].is_null())
				height = room["Height"];

			if (!room["FloorTexture"].is_null())
				floorTex = room["FloorTexture"];
			if (!room["WallTexture"].is_null())
				wallTex = room["WallTexture"];
			if (!room["CeilingTexture"].is_null())
				ceilingTex = room["CeilingTexture"];

			int doors[5] = { 1,1,1,1,1 };

			if (!room["Doors"][0].is_null())
				doors[0] = room["Doors"][0];

			if (!room["Doors"][1].is_null())
				doors[1] = room["Doors"][1];

			if (!room["Doors"][2].is_null())
				doors[2] = room["Doors"][2];

			if (!room["Doors"][3].is_null())
				doors[3] = room["Doors"][3];

			if (!room["Doors"][4].is_null())
				doors[4] = room["Doors"][4];

			string floorPath = SelectTexture(floorTex);
			string wallPath = SelectTexture(wallTex);
			string ceilingPath = SelectTexture(ceilingTex);

			rooms.push_back(GenerateRoom(roomX, roomY, length, width, height, doors, floorPath, wallPath, ceilingPath));
		}
	}

	if (!jsonObj["Models"].is_null())
	{
		numRooms = jsonObj["Models"].size(); // works its a visual studio bug

		for (int i = 0; i < numRooms; ++i)
		{
			json model = jsonObj["Models"][i];

			int modelID = 0;
			int xPos = 0;
			int yPos = 0;
			int zPos = 0;

			if (!model["ObjectID"].is_null())
				modelID = model["ObjectID"];

			if (!model["Position"][0].is_null())
				xPos = model["Position"][0];
			if (!model["Position"][1].is_null())
				yPos = model["Position"][1];
			if (!model["Position"][2].is_null())
				zPos = model["Position"][2];


			string modelPath = SelectModel(modelID);

			cout << modelPath << " Path" << endl;

			modelObjs.push_back(Model(modelPath.c_str(),"model_" + std::to_string(modelID), glm::vec3(xPos,yPos,zPos)));
		}
	}

	for (int i = 0; i < 50; ++i)
	{
		pointLights.push_back(Light());

		spotLights.push_back(Light(1));
	}

	if (!jsonObj["Lights"].is_null())
	{
		numRooms = jsonObj["Lights"].size(); // works its a visual studio bug

		numRooms = std::min(numRooms, 50);

		for (int i = 0; i < numRooms; ++i)
		{
			json light = jsonObj["Lights"][i];

			float lightX = 0;
			float lightY = 0;
			float lightZ = 0;

			int lightType = 0;

			if (!light["Type"].is_null())
				lightType = light["Type"];

			if (lightType == 2)
			{
				float dirX = 0;
				float dirY = 0;
				float dirZ = 0;

				if (!light["Direction"][0].is_null())
				{
					dirX = light["Direction"][0];
				}
				if (!light["Direction"][1].is_null())
				{
					dirY = light["Direction"][1];
				}
				if (!light["Direction"][2].is_null())
				{
					dirZ = light["Direction"][2];
				}

				directionalLight = Light({0,0,0}, {dirX,dirY,dirZ}, 2);
			}
			else
			{

				float dirX = 0;
				float dirY = 0;
				float dirZ = 0;

				if (!light["Direction"][0].is_null())
				{
					dirX = light["Direction"][0];
				}
				if (!light["Direction"][1].is_null())
				{
					dirY = light["Direction"][1];
				}
				if (!light["Direction"][2].is_null())
				{
					dirZ = light["Direction"][2];
				}

				if (!light["Position"][0].is_null())
					lightX = light["Position"][0];
				if (!light["Position"][1].is_null())
					lightY = light["Position"][1];
				if (!light["Position"][2].is_null())
					lightZ = light["Position"][2];

				if (lightType == 0)
					pointLights[i] = Light({ lightX,lightY,lightZ }, { 0,0,0 });
				else if (lightType == 1)
					spotLights[i] = Light({ lightX,lightY,lightZ }, { dirX,dirY,dirZ }, 1);

				pointMeshes.push_back(GenerateCube(lightX, lightY, lightZ));
			}
		}
	}

	if (!jsonObj["Skybox"].is_null())
	{
		string skyboxName = jsonObj["Skybox"]["Name"];

		//skybox = Skybox(skyboxName);
	}
}

string SelectTexture(int texNumber)
{
	string texturePath = std::string("Textures/img_") + std::to_string(texNumber) + std::string(".jpg");

	return texturePath;
}

string SelectModel(int modelNumber)
{
	string modelPath = std::string("Models/") + std::string("model_") + std::to_string(modelNumber) + "/";

	return modelPath;
}

void GetFPS()
{
	frame_count++;
	final_time = time(NULL);

	if (final_time - initial_time > 0)
	{
		if (showFPS)
			cout << "FPS: " << (frame_count / (final_time - initial_time)) << endl;
		frame_count = 0;
		initial_time = final_time;
	}
}

void GetUserCommands()
{
	string text;
	cin >> text;
}

void DepthMapFBO()
{
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

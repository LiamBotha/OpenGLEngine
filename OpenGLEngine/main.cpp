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

#include <iostream>
#include <vector>
#include <array>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);

void LoadLevelFromFile(string levelPath);
string SelectTexture(int texNumber);

Mesh GenerateMesh(float x, float y, float z, string texturePath);
Mesh GenerateRoomMesh(float x, float y, float length, float width, float height, bool doors[], string texturePath);
Room GenerateRoom(float x, float y, float length, float width, float height, bool doors[], string floorPath, string wallPath, string ceilingPath);
void BuildWalls(float x, float y, float length, float width, float height, bool doors[4], int doorCount, vector<Vertex> &vertices);
void AssignIndices(bool doors[4], int doorCount, vector<unsigned int> &indices, int vertSize);
void DrawQuadIndices(int val, int xWidth, vector<unsigned int> &indices);
void DrawDoorIndices(int val, int yWidth, int xWidth, int doorCount, int doorNum, vector<unsigned int> &indices);
void GenerateVert(float x, float y, float z, float u, float v, vector<Vertex> &vertices);

void CalculateNormals(vector<unsigned int>& indices, vector<Vertex>& vertices);
glm::vec3 CrossProduct(glm::vec3 a, glm::vec3 b);

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

// lighting
glm::vec3 lightPos(0.0f, 0.5f, 0.0f);

vector<Room> rooms = {};

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

	// build and compile shaders
	// -------------------------
	Shader ourShader("shader.vs", "shader.fs");
	Shader lightingShader("lightShader.vs", "lightShader.fs");

	lightingShader.use();
	lightingShader.setInt("material.diffuse", 0);

	// load models
	// -----------
	LoadLevelFromFile("level.json");

	//bool doorA[] = { true,false,false,false };
	//bool doorB[] = { false,false,true,false };
	//rooms.push_back(GenerateRoom(0, 0, 5, 2, 2, doorA, "Textures/awesomeface.png","Textures/container.jpg", "Textures/awesomeface.png"));
	//rooms.push_back(GenerateRoom(0, 6, 5, 2, 2, doorB, "Textures/awesomeface.png","Textures/container.jpg", "Textures/awesomeface.png"));

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

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
		//ourShader.use();
		lightingShader.use();
		lightingShader.setVec3("light.position", lightPos);
		lightingShader.setVec3("viewPos", camera.Position);

		// light properties
		lightingShader.setVec3("light.ambient", 1.0f, 1.0f, 1.0f);
		lightingShader.setVec3("light.diffuse", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		// material properties
		lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		lightingShader.setFloat("material.shininess", 40.0f);

		// view/projection tranansformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("projection", projection);
		lightingShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		lightingShader.setMat4("model", model);

		for (int i = 0; i < rooms.size(); ++i)
		{
			rooms[i].Draw(lightingShader);
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
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

void LoadLevelFromFile(string levelPath)
{
	int numRooms = 0;
	string jsonFileString = "";
	string line = "";
	json jsonObj;

	ifstream jsonFile(levelPath);

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
		jsonObj = json::parse(jsonFileString);
	}

	if (!jsonObj["Rooms"].is_null())
	{
		numRooms = jsonObj["Rooms"].size();

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

			bool doors[4] = { false, false, false, false };

			if (!room["Doors"][0].is_null())
				doors[0] = room["Doors"][0];

			if (!room["Doors"][1].is_null())
				doors[1] = room["Doors"][1];

			if (!room["Doors"][2].is_null())
				doors[2] = room["Doors"][2];

			if (!room["Doors"][3].is_null())
				doors[3] = room["Doors"][3];

			string floorPath = SelectTexture(floorTex);
			string wallPath = SelectTexture(wallTex);
			string ceilingPath = SelectTexture(ceilingTex);

			cout << roomX << ", " << roomY << ", " << roomZ << endl;

			rooms.push_back(GenerateRoom(roomX, roomY, length, width, height, doors, floorPath, wallPath, ceilingPath));
		}
	}

	if (!jsonObj["Models"].is_null())
	{
		numRooms = jsonObj["Models"].size();

		for (int i = 0; i < numRooms; ++i)
		{

		}
	}

	if (!jsonObj["Lights"].is_null())
	{
		numRooms = jsonObj["Lights"].size();

		for (int i = 0; i < numRooms; ++i)
		{

		}
	}
}

string SelectTexture(int texNumber)
{
	string texturePath = std::string("Textures/img_") + std::to_string(texNumber) + std::string(".jpg");

	return texturePath;
}

Mesh GenerateMesh(float x, float y, float z, string texturePath)
{
	bool doors[4] = { false,false,false,false };
	vector<Vertex> vertices = {};
	vector<unsigned int> indices = {};
	vector<Texture> textures = {};

	Vertex vertA = {};
	Vertex vertB = {};
	Vertex vertC = {};
	Vertex vertD = {};

	vertA.Position = { x - 1.0, y - 1.0, z + 1.0 };
	vertB.Position = { x + 1.0, y - 1.0, z + 1.0 };
	vertC.Position = { x + 1.0, y + 1.0, z + 1.0 };
	vertD.Position = { x - 1.0, y + 1.0, z + 1.0 };

	vertA.TexCoords = { 0.0f,0.0f };
	vertB.TexCoords = { 1.0f,0.0f };
	vertC.TexCoords = { 1.0f,1.0f };
	vertD.TexCoords = { 0.0f,1.0f };

	vertices.push_back(vertA);
	vertices.push_back(vertB);
	vertices.push_back(vertC);
	vertices.push_back(vertD);

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(0);

	//AssignIndices(doors, 0, indices, vertices.size());

	Texture tex = LoadTexture(texturePath.c_str(), "texture_diffuse");

	textures.push_back(tex);

	return Mesh(vertices, indices, textures);
}

Mesh GenerateRoomMesh(float x, float y, float length, float width, float height,bool doors[], string texturePath)
{
	int doorCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (doors[i] == true)
		{
			++doorCount;
		}
	}

	vector<Vertex> vertices = {};
	vector<unsigned int> indices = {};
	vector<Texture> textures = {};

	Texture tex = LoadTexture(texturePath.c_str(), "texture_diffuse");
	Texture tex2 = LoadTexture("Textures/awesomeface.png", "texture_diffuse");

	textures.push_back(tex);
	textures.push_back(tex2);

	BuildWalls(x, y, length, width, height, doors, doorCount, vertices); // minus 0.001f

	AssignIndices(doors, doorCount, indices, vertices.size() - 4);

	return Mesh(vertices, indices, textures);
}

Room GenerateRoom(float x, float y, float length, float width, float height, bool doors[],string floorPath, string wallPath,string ceilingPath)
{
	int doorCount = 0;
	for (int i = 0; i < 4; ++i)
	{
		if (doors[i] == true)
		{
			++doorCount;
		}
	}

	vector<Vertex> floorVertices = {};
	vector<unsigned int> floorIndices = {};
	vector<Texture> floorTextures = {};

	vector<Vertex> wallVertices = {};
	vector<unsigned int> wallIndices = {};
	vector<Texture> wallTextures = {};

	vector<Vertex> ceilingVertices = {};
	vector<unsigned int> ceilingIndices = {};
	vector<Texture> ceilingTextures = {};

	Texture floorTex = LoadTexture(floorPath.c_str(), "texture_diffuse");
	floorTextures.push_back(floorTex);

	Texture wallTex = LoadTexture(wallPath.c_str(), "texture_diffuse");
	wallTextures.push_back(wallTex);

	Texture ceilingTex = LoadTexture(ceilingPath.c_str(), "texture_diffuse");
	ceilingTextures.push_back(ceilingTex);

	GenerateVert(x - (width / 2), y + (length / 2), 0, 0.0, 1.0, floorVertices); // 0
	GenerateVert(x + (width / 2), y + (length / 2), 0, 1.0, 1.0, floorVertices); // 1
	GenerateVert(x + (width / 2), y - (length / 2), 0, 1.0, 0.0, floorVertices); // 2
	GenerateVert(x - (width / 2), y - (length / 2), 0, 0.0, 0.0, floorVertices); // 3

	GenerateVert(x - (width / 2), y + (length / 2), height, 0.0, 1.0, ceilingVertices); // 0
	GenerateVert(x + (width / 2), y + (length / 2), height, 1.0, 1.0, ceilingVertices); // 1
	GenerateVert(x + (width / 2), y - (length / 2), height, 1.0, 0.0, ceilingVertices); // 2
	GenerateVert(x - (width / 2), y - (length / 2), height, 0.0, 0.0, ceilingVertices); // 3

	floorIndices.push_back(0);
	floorIndices.push_back(3);
	floorIndices.push_back(2);
	floorIndices.push_back(2);
	floorIndices.push_back(1);
	floorIndices.push_back(0);

	ceilingIndices.push_back(0);
	ceilingIndices.push_back(1);
	ceilingIndices.push_back(2);
	ceilingIndices.push_back(2);
	ceilingIndices.push_back(3);
	ceilingIndices.push_back(0);

	BuildWalls(x, y, length, width, height, doors, doorCount, wallVertices);

	AssignIndices(doors, doorCount, wallIndices, wallVertices.size());

	CalculateNormals(wallIndices, wallVertices);
	CalculateNormals(floorIndices, floorVertices);
	CalculateNormals(ceilingIndices, ceilingVertices);

	return Room(Mesh(floorVertices,floorIndices,floorTextures), Mesh(wallVertices, wallIndices, wallTextures), Mesh(ceilingVertices, ceilingIndices, ceilingTextures));
}

void BuildWalls(float x, float y, float length, float width, float height, bool doors[4], int doorCount, vector<Vertex> &vertices)
{
	int downTex = 0;

	//Bottom Wall Verts
	GenerateVert(x - (width / 2), y + (length / 2), 0, downTex, 1.0, vertices); // 4
	downTex += 1.0 * width / 2;

	if (doors[0] == true)
	{
		cout << "door 0" << endl;
		GenerateVert(x - (width / 4), y + (length / 2), 0, downTex, 1.0, vertices); // 5.1
		GenerateVert(x + (width / 4), y + (length / 2), 0, downTex + 1, 1.0, vertices); // 5.2
		downTex += 2.0;
	}

	GenerateVert(x + (width / 2), y + (length / 2), 0, downTex, 1.0, vertices); cout << downTex * width << " - down tex, " << width << endl;
	downTex += 1.0 * length / 2;

	if (doors[1] == true)
	{
		cout << "door 1" << endl;
		GenerateVert(x + (width / 2), y + (length / 4), 0, downTex, 1.0, vertices); // 6.1
		GenerateVert(x + (width / 2), y - (length / 4), 0, downTex + 1, 1.0, vertices); // 6.1
		downTex += 2.0;
	}

	GenerateVert(x + (width / 2), y - (length / 2), 0, downTex, 1.0, vertices); //
	downTex += 1.0 * width / 2;

	if (doors[2] == true)
	{
		cout << "door 2" << endl;
		GenerateVert(x + (width / 4), y - (length / 2), 0, downTex, 1.0, vertices); // 7.1
		GenerateVert(x - (width / 4), y - (length / 2), 0, downTex + 1, 1.0, vertices); // 7.2
		downTex += 2.0;
	}

	GenerateVert(x - (width / 2), y - (length / 2), 0, downTex, 1.0, vertices); // 7
	downTex += 1.0 * length / 2;

	if (doors[3] == true)
	{
		cout << "door 3" << endl;
		GenerateVert(x - (width / 2), y - (length / 4), 0, downTex, 1.0, vertices); // 8.1
		GenerateVert(x - (width / 2), y + (length / 4), 0, downTex + 1, 1.0, vertices); // 8.2
		downTex += 2.0;
	}

	GenerateVert(x - (width / 2), y + (length / 2), 0, downTex, 1.0, vertices); cout << downTex * width << " - down tex, " << width << endl;

	downTex = 1.0;
	//Middle Verts
	if (doors[0] == true)
	{
		GenerateVert(x - (width / 4), y + (length / 2), (height / 1.5), downTex, 1.0, vertices); // 5.1
		GenerateVert(x + (width / 4), y + (length / 2), (height / 1.5), downTex + 1, 1.0, vertices); // 5.2
		downTex += 3;
	}
	else downTex++;
	if (doors[1] == true)
	{
		GenerateVert(x + (width / 2), y + (length / 4), (height / 1.5), downTex, 1.0, vertices); // 6.1
		GenerateVert(x + (width / 2), y - (length / 4), (height / 1.5), downTex + 1, 1.0, vertices); // 6.1
		downTex += 3;
	}
	else downTex++;
	if (doors[2] == true)
	{
		GenerateVert(x + (width / 4), y - (length / 2), (height / 1.5), downTex, 1.0, vertices); // 7.1
		GenerateVert(x - (width / 4), y - (length / 2), (height / 1.5), downTex + 1, 1.0, vertices); // 7.2
		downTex += 3;
	}
	else downTex++;
	if (doors[3] == true)
	{
		GenerateVert(x - (width / 2), y - (length / 4), (height / 1.5), downTex, 1.0, vertices); // 8.1
		GenerateVert(x - (width / 2), y + (length / 4), (height / 1.5), downTex + 1, 1.0, vertices); // 8.2
		downTex += 3;
	}

	downTex = 0.0;
	//nextTex = 0.0;

	// Top Wall Verts
	GenerateVert(x - (width / 2), y + (length / 2), height, downTex, 0.0, vertices); // 9
	downTex += 1.0 * width / 2;

	if (doors[0] == true)
	{

		GenerateVert(x - (width / 4), y + (length / 2), height, downTex, 0.0, vertices); // 8.1
		GenerateVert(x + (width / 4), y + (length / 2), height, downTex + 1, 0.0, vertices); // 8.2
		downTex += 2.0;
	}

	GenerateVert(x + (width / 2), y + (length / 2), height, downTex, 0.0, vertices); // 10
	downTex += 1.0 * length /2 ;

	if (doors[1] == true)
	{
		GenerateVert(x + (width / 2), y + (length / 4), height, downTex, 0.0, vertices); // 8.1
		GenerateVert(x + (width / 2), y - (length / 4), height, downTex + 1, 0.0, vertices); // 8.2
		downTex += 2.0;
	}

	GenerateVert(x + (width / 2), y - (length / 2), height, downTex, 0.0, vertices); // 11
	downTex += 1.0 * width / 2;

	if (doors[2] == true)
	{
		GenerateVert(x + (width / 4), y - (length / 2), height, downTex, 0.0, vertices); // 8.1
		GenerateVert(x - (width / 4), y - (length / 2), height, downTex + 1, 0.0, vertices); // 8.2
		downTex += 2.0;
	}

	GenerateVert(x - (width / 2), y - (length / 2), height, downTex, 0.0, vertices); // 12
	downTex += 1.0 * length / 2;

	if (doors[3] == true)
	{
		GenerateVert(x - (width / 2), y - (length / 4), height, downTex, 0.0, vertices); // 8.1
		GenerateVert(x - (width / 2), y + (length / 4), height, downTex + 1, 0.0, vertices); // 8.2
		downTex += 2.0;
	}

	GenerateVert(x - (width / 2), y + (length / 2), height, downTex, 0.0, vertices); // 13
}

void AssignIndices(bool doors[4], int doorCount, vector<unsigned int>& indices, int vertSize)
{
	//std::cout << vertSize << " - verts" << std::endl;

	//indices.push_back(vertSize - 11 - (doorCount * 6));
	//indices.push_back(vertSize - 12 - (doorCount * 6));
	//indices.push_back(vertSize - 13 - (doorCount * 6));
	//indices.push_back(vertSize - 13 - (doorCount * 6));
	//indices.push_back(vertSize - 14 - (doorCount * 6));
	//indices.push_back(vertSize - 11 - (doorCount * 6));

	int yWidth = 5 + (4 * doorCount);
	int xWidth = (5 + (2 * doorCount));

	int val2 = (vertSize - (xWidth * 2 + (doorCount * 2)));

	for (int i = 0, j = 0; j < 4; ++i, ++j) // increment j in code
	{

		if (doors != nullptr && doors[j] == true)
		{
			DrawDoorIndices(i, yWidth, xWidth, doorCount, j + 1, indices);
			i += 2;
		}
		else
		{
			DrawQuadIndices(i, yWidth, indices); 
		}
	}

	//indices.push_back(vertSize + 0);
	//indices.push_back(vertSize + 1);
	//indices.push_back(vertSize + 2);
	//indices.push_back(vertSize + 2);
	//indices.push_back(vertSize + 3);
	//indices.push_back(vertSize + 0);
}

void CalculateNormals(vector<unsigned int> &indices, vector<Vertex> &vertices)
{
	for (int i = 0; i < indices.size(); i += 3)
	{
		Vertex vert1 = vertices[indices[i]];
		Vertex vert2 = vertices[indices[i + 1]];
		Vertex vert3 = vertices[indices[i + 2]];

		//cout << vert1.Position.x << "," << vert2.Position.x << "," << vert3.Position.x << " X pos" << endl;

		glm::vec3 edgeA = vert1.Position - vert2.Position;
		glm::vec3 edgeB = vert1.Position - vert3.Position;

		glm::vec3 normal = CrossProduct(edgeA, edgeB);

		vertices[indices[i]].Normal = normal;
		vertices[indices[i + 1]].Normal = normal;
		vertices[indices[i + 2]].Normal = normal;

		//cout << edgeA.x << "," << edgeA.y << "," << edgeA.z << " - Edge A" << endl;
		//cout << edgeB.x << "," << edgeB.y << "," << edgeB.z << " - Edge B" << endl;
	}

	for (int i = 0; i < vertices.size(); ++i)
	{
		vertices[i].Normal = glm::normalize(vertices[i].Normal);
	}
}

glm::vec3 CrossProduct(glm::vec3 a, glm::vec3 b)
{
	int crossX = (a.y * b.z) - (a.z * b.y);
	int crossY = (a.x * b.z) - (a.z * b.x);
	int crossZ = (a.x * b.y) - (a.y * b.x);

	//cout << crossX;
	//cout << crossY;
	//cout << crossZ;
	//cout << endl;;

	return glm::vec3(crossX, crossY, crossZ);
}

void DrawQuadIndices(int val, int xWidth, vector<unsigned int> &indices)
{
	indices.push_back(val);
	indices.push_back(val + 1);
	indices.push_back(val + xWidth + 1);

	indices.push_back(val + xWidth + 1);
	indices.push_back(val + xWidth);
	indices.push_back(val);
}

void DrawDoorIndices(int val, int yWidth, int xWidth, int doorCount, int doorNum, vector<unsigned int> &indices)
{
	int doorOffset = (2 + (2 * doorCount));

	indices.push_back(val);
	indices.push_back(val + 1);
	indices.push_back(val + yWidth + 1);

	indices.push_back(val + yWidth + 1);
	indices.push_back(val + yWidth);
	indices.push_back(val);

	++val;

	//Door
	//indices.push_back(val);
	//indices.push_back(val + 1);
	//indices.push_back(val + xWidth - (doorNum - 1));

	//indices.push_back(val + xWidth - (doorNum - 1));
	//indices.push_back(val + xWidth - doorNum);
	//indices.push_back(val);

	indices.push_back(val + xWidth - doorNum);
	indices.push_back(val + xWidth - (doorNum - 1));
	indices.push_back(val + xWidth + doorOffset - 1);

	indices.push_back(val + xWidth + doorOffset - 1);
	indices.push_back(val + xWidth + doorOffset - 2);
	indices.push_back(val + xWidth - doorNum);

	++val;

	indices.push_back(val);
	indices.push_back(val + 1);
	indices.push_back(val + yWidth + 1);

	indices.push_back(val + yWidth + 1);
	indices.push_back(val + yWidth);
	indices.push_back(val);
}

void GenerateVert(float x, float y, float z, float u, float v, vector<Vertex> &vertices)
{
	Vertex vert = {};

	vert.Position = { x,z,y };
	vert.TexCoords = { u,v };

	vertices.push_back(vert);
}
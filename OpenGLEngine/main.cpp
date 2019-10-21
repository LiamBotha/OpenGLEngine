#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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

Mesh GenerateMesh(float x, float y, float z, string texturePath);
Mesh GenerateRoomMesh(float x, float y, float length, float width, float height, bool doors[], string texturePath);
Room GenerateRoom(float x, float y, float length, float width, float height, bool doors[], string floorPath, string wallPath, string ceilingPath);
void BuildWalls(float x, float y, float length, float width, float height, bool doors[4], int doorCount, vector<Vertex> &vertices);
void AssignIndices(bool doors[4], int doorCount, vector<unsigned int> &indices, int vertSize);
void DrawQuad(int val, int xWidth, vector<unsigned int> &indices);
void DrawDoor(int val, int yWidth, int xWidth, int doorCount, int doorNum, vector<unsigned int> &indices);
void GenerateVert(float x, float y, float z, float u, float v, vector<Vertex> &vertices);

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

	// load models
	// -----------
	//Model ourModel("Objects/nanosuit/nanosuit.obj");

	bool doorA[] = { true,false,false,false };
	bool doorB[] = { false,false,true,false };

	vector<Room> rooms = {};

	rooms.push_back(GenerateRoom(0, 0, 5, 2, 2, doorA, "Textures/awesomeface.png","Textures/container.jpg", "Textures/awesomeface.png"));
	rooms.push_back(GenerateRoom(0, 6, 5, 2, 2, doorB, "Textures/awesomeface.png","Textures/container.jpg", "Textures/awesomeface.png"));

	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		//courModel.Draw(ourShader);

		for (int i = 0; i < rooms.size(); ++i)
		{
			rooms[i].Draw(ourShader);
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

	BuildWalls(x, y, length, width, height, doors, doorCount, vertices);

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
	floorIndices.push_back(1);
	floorIndices.push_back(2);
	floorIndices.push_back(2);
	floorIndices.push_back(3);
	floorIndices.push_back(0);

	ceilingIndices.push_back(0);
	ceilingIndices.push_back(1);
	ceilingIndices.push_back(2);
	ceilingIndices.push_back(2);
	ceilingIndices.push_back(3);
	ceilingIndices.push_back(0);

	BuildWalls(x, y, length, width, height, doors, doorCount, wallVertices);

	AssignIndices(doors, doorCount, wallIndices, wallVertices.size());

	return Room(Mesh(floorVertices,floorIndices,floorTextures), Mesh(wallVertices, wallIndices, wallTextures), Mesh(ceilingVertices, ceilingIndices, ceilingTextures));
}

void BuildWalls(float x, float y, float length, float width, float height, bool doors[4], int doorCount, vector<Vertex> &vertices)
{
	float downTex = 0.0;
	//Bottom Wall Verts
	GenerateVert(x - (width / 2), y + (length / 2), 0, downTex, 1.0, vertices); // 4
	downTex += 1.0;

	if (doors[0] == true)
	{
		GenerateVert(x - (width / 4), y + (length / 2), 0, downTex, 1.0, vertices); // 5.1
		GenerateVert(x + (width / 4), y + (length / 2), 0, downTex + 1, 1.0, vertices); // 5.2
		downTex += 2.0;
	}

	GenerateVert(x + (width / 2), y + (length / 2), 0, downTex, 1.0, vertices); // 5
	downTex += 1.0;

	if (doors[1] == true)
	{
		GenerateVert(x + (width / 2), y + (length / 4), 0, downTex, 1.0, vertices); // 6.1
		GenerateVert(x + (width / 2), y - (length / 4), 0, downTex + 1, 1.0, vertices); // 6.1
		downTex += 2.0;
	}

	GenerateVert(x + (width / 2), y - (length / 2), 0, downTex, 1.0, vertices); // 6
	downTex += 1.0;

	if (doors[2] == true)
	{
		GenerateVert(x + (width / 4), y - (length / 2), 0, downTex, 1.0, vertices); // 7.1
		GenerateVert(x - (width / 4), y - (length / 2), 0, downTex + 1, 1.0, vertices); // 7.2
		downTex += 2.0;
	}

	GenerateVert(x - (width / 2), y - (length / 2), 0, downTex, 1.0, vertices); // 7
	downTex += 1.0;
	if (doors[3] == true)
	{
		GenerateVert(x - (width / 2), y - (length / 4), 0, downTex, 1.0, vertices); // 8.1
		GenerateVert(x - (width / 2), y + (length / 4), 0, downTex + 1, 1.0, vertices); // 8.2
		downTex += 2.0;
	}

	GenerateVert(x - (width / 2), y + (length / 2), 0, downTex, 1.0, vertices); // 8

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

	// Top Wall Verts
	GenerateVert(x - (width / 2), y + (length / 2), height, downTex, 0.0, vertices); // 9
	downTex += 1.0;

	if (doors[0] == true)
	{
		GenerateVert(x - (width / 4), y + (length / 2), height, downTex, 0.0, vertices); // 8.1
		GenerateVert(x + (width / 4), y + (length / 2), height, downTex + 1, 0.0, vertices); // 8.2
		downTex += 2.0;
	}

	GenerateVert(x + (width / 2), y + (length / 2), height, downTex, 0.0, vertices); // 10
	downTex += 1.0;

	if (doors[1] == true)
	{
		GenerateVert(x + (width / 2), y + (length / 4), height, downTex, 0.0, vertices); // 8.1
		GenerateVert(x + (width / 2), y - (length / 4), height, downTex + 1, 0.0, vertices); // 8.2
		downTex += 2.0;
	}

	GenerateVert(x + (width / 2), y - (length / 2), height, downTex, 0.0, vertices); // 11
	downTex += 1.0;

	if (doors[2] == true)
	{
		GenerateVert(x + (width / 4), y - (length / 2), height, downTex, 0.0, vertices); // 8.1
		GenerateVert(x - (width / 4), y - (length / 2), height, downTex + 1, 0.0, vertices); // 8.2
		downTex += 2.0;
	}

	GenerateVert(x - (width / 2), y - (length / 2), height, downTex, 0.0, vertices); // 12
	downTex += 1.0;

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
	std::cout << vertSize << " - verts" << std::endl;

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
			DrawDoor(i, yWidth, xWidth, doorCount, j + 1, indices);
			i += 2;
		}
		else
		{
			DrawQuad(i, yWidth, indices); 
		}
	}

	//indices.push_back(vertSize + 0);
	//indices.push_back(vertSize + 1);
	//indices.push_back(vertSize + 2);
	//indices.push_back(vertSize + 2);
	//indices.push_back(vertSize + 3);
	//indices.push_back(vertSize + 0);
}

void DrawQuad(int val, int xWidth, vector<unsigned int> &indices)
{
	indices.push_back(val);
	indices.push_back(val + 1);
	indices.push_back(val + xWidth + 1);

	indices.push_back(val + xWidth + 1);
	indices.push_back(val + xWidth);
	indices.push_back(val);
}

void DrawDoor(int val, int yWidth, int xWidth, int doorCount, int doorNum, vector<unsigned int> &indices)
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
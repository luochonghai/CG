#include <iostream>
#include <cmath>
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <learnopengl/shader.h>
#include <stb_image.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "learnopengl/Camera.h"
#define WIN_WIDTH   600
#define WIN_HEIGHT  600
using namespace std;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);
void loadTexture(unsigned int *texture, const char *path);
unsigned int loadTextureReal(char const *path);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

//相机位置
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
//面对的方向,以这个为尺度更改观看的，其实就是单位速度
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
//头顶的方向
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

//当前帧与上一帧的时间差
float deltaTime = 0.0f;
//上一帧的时间
float lastFrameTime = 0.0f;

float lastX = 300;
float lastY = 300;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow *window = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, "LightOnCubes", NULL, NULL);
	if (window == NULL) 
	{
		std::cout << "Fail to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
	{
		std::cout << "File to initialize GLAD" << std::endl;
		return -1;
	}
	//使窗口隐藏光标并且捕捉它
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//使用回调
	glfwSetCursorPosCallback(window, mouse_callback);

	//指定可视窗口大小
	glViewport(0, 0, WIN_WIDTH, WIN_HEIGHT);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//加载纹理
	unsigned int diffuseMap, diffuse_specular_Map, matrixMap;
	const char *path1 = "E:/opengl_lib/LearnOpenGL-master/resources/textures/toy_box_diffuse.png";
	const char *path2 = "E:/opengl_lib/LearnOpenGL-master/resources/textures/toy_box_disp.png";
	const char *path3 = "E:/opengl_lib/LearnOpenGL-master/resources/textures/toy_box_normal.png";
	diffuseMap = loadTextureReal(path1);
	diffuse_specular_Map = loadTextureReal(path2);
	matrixMap = loadTextureReal(path3);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseMap);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuse_specular_Map);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, matrixMap);

	//载入物体及光源
	Shader lightingShader("C:\\Users\\tjqqd\\Documents\\Visual Studio 2017\\Projects\\CG_PJ1\\CG_PJ1\\vx.glsl",
		"C:\\Users\\tjqqd\\Documents\\Visual Studio 2017\\Projects\\CG_PJ1\\CG_PJ1\\fx.glsl");
	Shader lampShader("C:\\Users\\tjqqd\\Documents\\Visual Studio 2017\\Projects\\CG_PJ1\\CG_PJ1\\vy.glsl",
		"C:\\Users\\tjqqd\\Documents\\Visual Studio 2017\\Projects\\CG_PJ1\\CG_PJ1\\fy.glsl");


	//立方体
	float vertices[] = 
	{
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 1.0f, -1.0f),
		glm::vec3(1.0f, -1.0f, -1.0f),
		glm::vec3(-1.0f, -1.0f, -1.0f),
		glm::vec3(-2.0f, -2.0f, -2.0f),
		glm::vec3(2.0f, 2.0f, -2.0f),
		glm::vec3(-2.0f, 2.0f, -2.0f),
		glm::vec3(3.0f, -3.0f, -3.0f),
		glm::vec3(-3.0f,3.0f, -3.0f),
		glm::vec3(3.0f, 3.0f, -3.0f),
	};

	unsigned int cubeVAO,VBO,lightVAO;
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	glGenBuffers(1, &VBO); 
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(0));
	glEnableVertexAttribArray(0);

	//循环绘制
	while (!glfwWindowShouldClose(window)) 
	{
		//判断界面是否关闭，没关闭就循环绘制
		processInput(window);
		//记录deltaTime
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrameTime;
		lastFrameTime = currentFrame;
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//如果开启了深度测试，这里就要把GL_DEPTH_BUFFER_BIT也清空
		
		//让光源随着时间的变化而改变位置
		//lightPos = glm::vec3(1.2f * sin(glfwGetTime()), 1.0f * sin(glfwGetTime()), 2.0f);

		glm::vec3 lightColor;
		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);//降低影响
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);//环境光，更低
																//开启深度测试
		glEnable(GL_DEPTH_TEST);

		//lighting其实是物体。。。
		lightingShader.use();
		lightingShader.setVec3("viewPos", camera.Position);//观察位置

		//设置各个光照的强度
		//lightingShader.setVec3("light.ambient", ambientColor);
		//lightingShader.setVec3("light.diffuse", diffuseColor);
		
		lightingShader.setVec3("light.ambient", 0.1f, 0.1f, 0.1f);
		lightingShader.setVec3("light.diffuse", 0.2f, 0.2f, 0.2f);
		lightingShader.setVec3("light.specular", 0.7f, 0.7f, 0.7f);

		//镜面反射
		lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);//定向光
		lightingShader.setVec3("light.position", lightPos);//点光源位置
		
		//聚光灯
		lightingShader.setVec3("light.position", camera.Position);//摄像机位置就是光源的来源
		lightingShader.setVec3("light.direction", camera.Front);//聚光灯正前方
		lightingShader.setFloat("light.cutoff", glm::cos(glm::radians(12.5f)));//聚光灯的切光角
		lightingShader.setFloat("light.outerCutOff", glm::cos(glm::radians(17.5f)));//聚光灯的切光角

		//光的衰减参数
		lightingShader.setFloat("light.constant", 1.0f);
		lightingShader.setFloat("light.linear", 0.1f);
		lightingShader.setFloat("light.quadratic", 0.9f);

		//绑定箱子的纹理(0--漫反射光照下物体的颜色；1--镜面反射对物体颜色的影响；2--反射光的贴图)
		lightingShader.setInt("material.diffuse", 0);
		lightingShader.setInt("material.specular", 1);
		lightingShader.setInt("material.emission", 2);
		//调节箱子的反光性
		lightingShader.setFloat("material.shininess", 0.7f);

		glm::mat4 model = glm::mat4();
		//设置投影矩阵的视野，就是这个光线能照多远,设为10看过去就像手电筒了，设为100就像自然光了
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), 1.0f, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		lightingShader.setMat4("view", view);
		lightingShader.setMat4("projection", projection);

		glBindVertexArray(cubeVAO);
		for (unsigned int i = 0; i < 18; i++) 
		{
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			lightingShader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		lampShader.use();
		lampShader.setMat4("view", view);
		lampShader.setMat4("projection", projection);
		model = glm::mat4();
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		lampShader.setMat4("model", model);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//双缓冲机制，前缓冲保存着最终输出的图像，后缓冲则进行绘制，绘制完成以后与前缓冲交换，就会立即显示
		//单缓冲会存在闪烁问题
		glfwSwapBuffers(window);//交换颜色缓冲
		glfwPollEvents();//检查有没有什么触发事件，鼠标键盘等，并调用相关的回调

	}
	//回收数据
	glDeleteVertexArrays(1, &cubeVAO);
	glDeleteVertexArrays(1, &lightVAO);
	glDeleteBuffers(1, &VBO);

	glfwTerminate();//结束绘制
	return 0;
}

//窗口尺寸改变的回调
void framebuffer_size_callback(GLFWwindow *window, int width, int height) 
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window) 
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

void loadTexture(unsigned int *texture, const char *path) 
{
	glGenTextures(1, texture);
	//对*p赋值，从而改变p所指的地址上说保存的值
	//*textrue就能表示这个内存地址上表示的值
	glBindTexture(GL_TEXTURE_2D, *texture);//GL_TEXTURE_2D同样，它也是一个目标
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//设置环绕和过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//加载纹理，第三个参数是颜色通道的个数
	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(true);
	//图片
	unsigned char *data = stbi_load(path, &width,&height, &nrChannels, 0);

	//第二个参数为多级渐远纹理的级别，0为基本级别，第三个参数为指定纹理存储为何种格式
	//第六个总是设置为0，第七第八定义源图的格式和数据类型
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);
	//释放图像内存
	stbi_image_free(data);
}


unsigned int loadTextureReal(char const *path) 
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data) 
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else 
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}

	return textureID;
}

bool firstMouse = true;

void mouse_callback(GLFWwindow *window, double xpos, double ypos) 
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

//滚轮回调
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) 
{
	camera.ProcessMouseScroll(yoffset);
}

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<windows.h>
#include<random>
#include <iostream>
#include<ctime>
#include <vector>
#include<string>
#include <cmath>
using namespace std;
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;
//用于存储pcm文件解析出的数据
vector<float> vertices;
int wstart = 0;
//记录pcm文件中数据个数
int n;

//控制图像输出的时间
void MSleep(long lTime, bool bProcessMsg)
{
	LARGE_INTEGER litmp;
	long long QPart1,QPart2;
	double dfMinus, dfFreq,dfTim, dfSpec;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;
	dfSpec = 0.000001*lTime;
		do
		{
			if (bProcessMsg == true)
			{
				MSG msg;
					PeekMessage(&msg, NULL, 0, 0, PM_REMOVE);
					TranslateMessage(&msg);
					DispatchMessage(&msg);
			}
			QueryPerformanceCounter(&litmp);
			QPart2 = litmp.QuadPart;
			dfMinus = (double)(QPart2 - QPart1);
			dfTim = dfMinus / dfFreq;
		} while (dfTim < dfSpec);
}
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void drawWave();

//PCM文件数据解码保存到数组中
void fileOutput(char* path_name)
{
	short pcm_In = 0;
	int size = 0;
	FILE *fp;
	fopen_s(&fp,path_name, "rb+");     
	int i = 0;
	while (!feof(fp))
	{
		size = fread(&pcm_In, 2, 1, fp);     
		if (size>0)
			vertices.push_back((float)pcm_In / 30000);
		i++;
	}
	n = i;
	fclose(fp);
}

int main()
{
	char path_name[1000];
	scanf_s("%s",path_name,1000);
	srand((int)time(NULL));
	fileOutput(path_name);
	
	// glfw: 初始化并配置
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw 创建窗口
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Music Circular Spectrum", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: 家在所有OpenGL函数指针
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	float* arr2 = new float[3 * n];
	int i = 0;
	float xstart = -1.0;

	//波形频谱图数据存储
	for (vector<float>::iterator it = vertices.begin(); it != vertices.end(); it += 2)    //用迭代器的方式输出容器对象的值
	{
		arr2[i++] = xstart;
		arr2[i++] = *it;
		arr2[i++] = 0.0f;

		xstart = xstart + 0.001;
		if (xstart>1.0) 
			xstart = -1.0;
	}

	//波形
	unsigned int waveVAO, waveVBO;
	glGenVertexArrays(1, &waveVAO);
	glGenBuffers(1, &waveVBO);
	glBindVertexArray(waveVAO);
	glBindBuffer(GL_ARRAY_BUFFER, waveVBO);
	glBufferData(GL_ARRAY_BUFFER, 12 * n, arr2, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// 循环渲染
	while (!glfwWindowShouldClose(window))
	{
		processInput(window);
		// 渲染
		glClearColor(rand()%(100)/100.0, rand() % (100) / 100.0, rand() % (100) / 100.0, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(waveVAO); // 激活VAO表示的顶点缓存
		if (wstart<3 * n) 
		{   //到达终点之前每次绘制一帧的频谱图
			drawWave();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &waveVAO);
	glDeleteBuffers(1, &waveVBO);
	glfwTerminate();
	return 0;
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: 每当窗口大小改变时，调用该回调函数
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//绘制波形频谱
void drawWave()
{
	MSleep(44100,1);
	float redValue = 0.5f, blueValue = 0.5f;
	for (int i = wstart; i<2000 + wstart; i++) 
	{
		//float redValue = rand()%(100)/100.0, blueValue = rand() % (100) / 100.0;
		glUniform4f(0, redValue, blueValue, 1.0f, 1.0f);
		if (i <= 500 + wstart) 
		{
			redValue = redValue + 0.002;
			blueValue = blueValue - 0.002;
		}
		else 
		{
			redValue = redValue - 0.002;
			blueValue = blueValue + 0.002;
		}
		glPointSize(4);
		glDrawArrays(GL_POINTS, i, 1);
	}
	wstart += 2000;
}

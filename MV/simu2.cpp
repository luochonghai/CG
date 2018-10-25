#define FILE_PATH "D:\\FDU\\Template\\CS\\Computer Graphics\\PJ\\test.pcm"     
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#define Freq 1411
#define Ter  1.0/Freq
using namespace std;

vector<float> vertices;
vector<float>::iterator istart;
vector<float>::iterator iend;

void fileOutput()
{
	short pcm_In = 0;
	int size = 0;
	FILE *fp;
	fopen_s(&fp,FILE_PATH, "rb+");     
	while (!feof(fp))
	{
		size = fread(&pcm_In, 2, 1, fp);     
		if (size>0)
		{
			vertices.push_back((float)pcm_In / 30000);
		}
	}

	fclose(fp);
}

void drawLint()
{
	glClearColor(0.8, 0.8, 0.8, 0.8);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(2);
	glBegin(GL_LINES);
	glColor3f(0.6, 0.6, 0.6);

	float temp = 0.0;
	float xstart = -1.0;
	float xend = -1.0;

	//绘制波形图,用迭代器的方式输出容器对象的值
	for (vector<float>::iterator it = istart; it != iend; it++)    
	{
		xstart = xstart + Ter;
		xend = xstart + Ter;
		glVertex2f(xstart, temp);
		glVertex2f(xend, *it);
		temp = *it;
	}
	//进行下一次绘制的起点和终点
	istart += 4.47*Freq;
	iend += 4.47*Freq;
	glEnd();
}

int main(void)
{
	fileOutput();
	istart = vertices.begin();
	iend = vertices.begin() + 60000;
	GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 800, "Visualize Music ", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/*your draw*/
		if (iend < vertices.end()) {
			drawLint();
		}
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

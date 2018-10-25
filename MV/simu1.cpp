//PCM源文件
#define TEST_FILE_PATH "D:\\FDU\\Template\\CS\\Computer Graphics\\PJ\\16k.pcm"     
#include <GLFW/glfw3.h>
#include <iostream>
using namespace std;

void drawLint()
{
	//设置背景颜色（分为R,G,B三个通道）
	glClearColor(0.1, 0.1, 0.5, 0.99);
	glClear(GL_COLOR_BUFFER_BIT);

	//设置显示图像的线段的宽度
	glLineWidth(0.5);
	glBegin(GL_LINES);
	//设置显示的图像的颜色
	glColor3f(1.0, 0.5, 0.0);

	short pcm_In = 0;
	int size = 0;
	float xstart = -1.0,ystart = 0.0,
	xend = 0.0,yend = 0.0,temp = 0.0;

	FILE *fp;
	fopen_s(&fp,TEST_FILE_PATH, "rb+");     

	while (!feof(fp))
	{
		//pcm中每个数据大小为2字节，每次读取1个数据
		size = fread(&pcm_In, 2, 1, fp);     
		if (size>0)
		{
			//确定绘制波形的折线两点坐标
			xstart = xstart + 0.00003;
			ystart = temp;
			xend = xstart + 0.00003;

			//short类型除以一个数要么是0要么是大于一的整数，所以坐标变换要注意先转为float
			//yend = (float)pcm_In / 10000;       
			yend = (float)pcm_In / 10000;
			//绘制折线									
			glVertex2f(xstart, ystart);
			glVertex2f(xend, yend);
			//终点作为下一次的起点
			temp = yend;    
		}
	}
	fclose(fp);
	glEnd();
}

int main(void)
{
	GLFWwindow* window;
	/* Initialize the library */
	if (!glfwInit())
		return -1;
	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(800, 800, "Music_Visualizer Window", NULL, NULL);
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
		drawLint();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}

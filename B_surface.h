#pragma once
#include <stdlib.h>
#include <time.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <GL/freeglut_ext.h>
#include <iomanip>
#include <string>
#include <cassert>
#include <vector>
using namespace std;

GLUnurbsObj* B_sur;
GLfloat sur3[4][4][3];
GLfloat knots32[8];
bool mouseLeftDown32;
bool mouseRightDown32;
bool mouseMiddleDown32;
float mouseX32, mouseY32;
float cameraDistanceX32;
float cameraDistanceY32;
float cameraAngleX32;
float cameraAngleY32;
float times32;

class B_surface {
public:
	B_surface(const char* cpfile, const char* knotsfile) {
		ifstream is(cpfile);
		int row = 0, col = 0;
		double x_temp, y_temp, z_temp;
		string line;
		while (std::getline(is, line))
		{
			line_process(line);//把行首和行尾的多个空格, tab去掉，把注释文字也去掉
			if (line.empty()) continue;//line为空则继续
			//根据实际需求处理
			std::istringstream iss(line);
			iss >> x_temp >> y_temp >> z_temp;
			sur3[row][col][0] = x_temp;
			sur3[row][col][1] = y_temp;
			sur3[row][col][2] = z_temp;
			col++;
			if (col == 4) {
				col = 0;
				row++;
			}
		}
		ifstream is1(knotsfile);
		row = 0;
		while (std::getline(is1, line))
		{
			line_process(line);//把行首和行尾的多个空格, tab去掉，把注释文字也去掉
			if (line.empty()) continue;//line为空则继续
			//根据实际需求处理
			std::istringstream iss(line);
			double x_temp;
			iss >> x_temp;
			knots32[row] = x_temp;
			row++;
		}
	}

	void init(void)
	{
		//背景色
		glClearColor(0.0, 0.0, 0.0, 1.0);
		//代码开关3：设置材质与光源
		GLfloat ambient[] = { 0.4, 0.6, 0.2, 1.0 };
		GLfloat position[] = { 1.0, 1.0, 3.0, 1.0 };
		GLfloat mat_diffuse[] = { 0.8, 0.6, 0.3, 1.0 };
		GLfloat mat_specular[] = { 0.8, 0.6, 0.3, 1.0 };
		GLfloat mat_shininess[] = { 45.0 };

		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT0);
		glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
		glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

		//允许深度测试
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		//代码开关4：取消下面两行代码，查看曲面显示效果差异
		//打开自动法矢量开关
		glEnable(GL_AUTO_NORMAL);
		//允许正则化法矢量
		glEnable(GL_NORMALIZE);
		B_sur = gluNewNurbsRenderer(); // 创建一个B样条曲面对象  
		//修改B样条曲面对象的属性——glu库函数
		////采样sampling容错torerance
		gluNurbsProperty(B_sur, GLU_SAMPLING_TOLERANCE, 5.0);
		gluNurbsProperty(B_sur, GLU_DISPLAY_MODE, GLU_FILL);
	}

	static void ControlPoint()//画点
	{
		glPointSize(5);
		glBegin(GL_POINTS);
		for (unsigned int i = 0; i < 4; i++) {
			for (unsigned int j = 0; j < 4; j++) {
				glVertex3fv(sur3[i][j]);
			}
		}
		glEnd();
	}
	/*绘制曲线*/
	static void Display(void)
	{
		glRotatef(cameraAngleX32, 1, 0, 0);
		glRotatef(cameraAngleY32, 0, 1, 0);
		glTranslatef(cameraDistanceX32, cameraDistanceY32, 0);
		glScalef(times32, times32, times32);//缩放
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0, 0.0, 0.0);
		ControlPoint();
		glColor3f(0.0, 1.0, 0.0);
		gluBeginSurface(B_sur); // 开始曲面绘制 
		gluNurbsSurface(B_sur, 8, knots32, 8, knots32, 4 * 3, 3, &sur3[0][0][0], 4, 4, GL_MAP2_VERTEX_3); // 定义曲面的数学模型，确定其形状  
		gluEndSurface(B_sur); // 结束曲面绘制  
		glutSwapBuffers();
	}
	static void Reshape(GLsizei w, GLsizei h)
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (w <= h)
			glOrtho(-5.0, 5.0, -5.0 * (GLfloat)h / (GLfloat)w, 5.0 * (GLfloat)h / (GLfloat)w, -5.0, 5.0);
		else
			glOrtho(-5.0 * (GLfloat)w / (GLfloat)h, 5.0 * (GLfloat)w / (GLfloat)h, -5.0, 5.0, -5.0, 5.0);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	static void keyboard(unsigned char key, int x, int y) //key 为键盘按键的ASCII码
	{
		switch (key)
		{
		case 27:   //ESC键
			exit(0);
			break;
		default:
			break;
		}
	}

	static void mouseCB(int button, int state, int x, int y)
	{
		mouseX32 = x;
		mouseY32 = y;
		times32 = 1;

		if (button == GLUT_LEFT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				mouseLeftDown32 = true;
			}
			else if (state == GLUT_UP)
				mouseLeftDown32 = false;
		}

		else if (button == GLUT_RIGHT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				mouseRightDown32 = true;
			}
			else if (state == GLUT_UP)
				mouseRightDown32 = false;
		}
	}

	static void mouseMotionCB(int x, int y)
	{
		cameraAngleX32 = cameraAngleY32 = 0;
		cameraDistanceX32 = cameraDistanceY32 = 0;

		if (mouseLeftDown32)
		{
			cameraAngleY32 += (x - mouseX32) * 0.003f;
			cameraAngleX32 += (y - mouseY32) * 0.003f;
			mouseX32 = x;
			mouseY32 = y;
		}
		if (mouseRightDown32)
		{
			cameraDistanceX32 = (x - mouseX32) * 0.001f;
			cameraDistanceY32 = -(y - mouseY32) * 0.001f;
			mouseY32 = y;
			mouseX32 = x;
		}
		glutPostRedisplay();
	}

	static void mouseWheel(int button, int state, int x, int y)
	{
		if (button == 0 && state == GLUT_UP) // 往上滚滚轮，放大
		{
			times32 = 1 + 0.008f;
			glutPostRedisplay();
		}
		else {  // 往下滚滚轮，缩小
			times32 = 1 - 0.008f;
			glutPostRedisplay();
		}
	}

	void render()
	{
		int a = NULL;
		glutInit(&a, NULL);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(600, 400);
		glutInitWindowPosition(200, 200);
		glutCreateWindow("三次B样条曲线");
		/*绘制与显示*/
		init();
		glutDisplayFunc(Display);
		glutReshapeFunc(Reshape);
		glutKeyboardFunc(keyboard);
		glutIdleFunc(Display);//设置空闲时调用的函数
		glutMouseFunc(mouseCB);
		glutMotionFunc(mouseMotionCB);
		glutMouseWheelFunc(mouseWheel);
		glutMainLoop();
	}

	void line_process(std::string& line, const std::string comment_str = "#")
	{
		for (char& c : line)//C++11以上版本的语法
		{
			//制表符 tab，逗号，分号都当作有效的分隔符，统一转成空格
			//为了避免错误，回车符和换行符也转为空格（否则无法处理空行）
			if (c == '\t' || c == ',' || c == ';' || c == '\r' || c == '\n')
				c = ' ';
		}

		line.erase(0, line.find_first_not_of(" "));//删除行首空格
		line.erase(line.find_last_not_of(" ") + 1);//删除行末空格
		//调用的string& erase (size_t pos = 0, size_t len = npos);
		//len为默认参数


		//查找注释符所在位置，如果不存在，则得到string::npos
		int n_comment_start = line.find_first_of(comment_str);
		if (n_comment_start != std::string::npos)//这一句必须的
			line.erase(n_comment_start);         //删除注释

		//处理完毕。如果这一行只有空格，制表符 tab，注释，那么处理后line为空；
		//如果行首有多个空格(或者空格和tab交错)，行尾为注释，那么处理后字符串line的
		//行首多个空格(和tab)和行尾注释被删掉，只保留有意义的内容。
	}
};

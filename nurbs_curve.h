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

GLUnurbsObj* theNurb11;
GLfloat cur2[11][3];
GLfloat knots21[15];

class Nurbs_curve {
public:
	Nurbs_curve(const char* cpfile, const char* knotsfile) {
		ifstream is(cpfile);
		int row = 0;
		double x_temp, y_temp, z_temp;
		string line;
		while (std::getline(is, line))
		{
			line_process(line);//把行首和行尾的多个空格, tab去掉，把注释文字也去掉
			if (line.empty()) continue;//line为空则继续
			//根据实际需求处理
			std::istringstream iss(line);
			iss >> x_temp >> y_temp >> z_temp;
			cur2[row][0] = x_temp;
			cur2[row][1] = y_temp;
			cur2[row][2] = z_temp;
			row++;
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
			knots21[row] = x_temp;
			row++;
		}
	}

	void init(void)
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);//设置背景色
		theNurb11 = gluNewNurbsRenderer();//创建NURBS对象theNurb
		gluNurbsProperty(theNurb11, GLU_SAMPLING_TOLERANCE, 10);
	}

	static void ControlPoint()//画点
	{
		glPointSize(5);
		glBegin(GL_POINTS);
		for (unsigned int i = 0; i < 11; i++) {
			glVertex3fv(cur2[i]);
		}
		glEnd();

	}

	static void Display(void)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		glColor3f(1.0, 0.0, 0.0);
		ControlPoint();
		glColor3f(0.0, 1.0, 0.0);
		glLineWidth(3.0);
		/*绘制曲线*/
		gluBeginCurve(theNurb11);
		gluNurbsCurve(theNurb11, 15, knots21, 3, &cur2[0][0], 4, GL_MAP1_VERTEX_3);
		//gluNurbsCurve(theNurb, 13, knots, 3, &color[0][0], 4, GL_MAP1_COLOR_4);
		gluEndCurve(theNurb11);
		/*绘制控制多边形*/
		glLineWidth(1);//设置栅格化线条的宽度
		glColor3f(0.0, 0.0, 1.0);
		glPopMatrix();
		glutSwapBuffers();
	}
	static void Reshape(GLsizei w, GLsizei h)
	{
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		if (w <= h)
			glOrtho(-3.0, 10.0, -5.0 * (GLfloat)h / (GLfloat)w, 5.0 * (GLfloat)h / (GLfloat)w, -5.0, 5.0);
		else
			glOrtho(-3.0 * (GLfloat)w / (GLfloat)h, 10.0 * (GLfloat)w / (GLfloat)h, -5.0, 5.0, -5.0, 5.0);
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

	void render()
	{
		int a = NULL;
		glutInit(&a, NULL);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(600, 400);
		glutInitWindowPosition(200, 200);
		glutCreateWindow("三次Nurbs曲线");
		/*绘制与显示*/
		init();
		glutReshapeFunc(Reshape);
		glutKeyboardFunc(keyboard);
		glutDisplayFunc(Display);
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

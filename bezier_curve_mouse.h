#pragma once
#pragma once
//使用glmap
#include <stdlib.h>
#include <time.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <sstream>
//#include <gl/freeglut.h>
#include <iomanip>
#include <string>
#include <cassert>
#include <vector>
using namespace std;

struct XPoint { // 定义顶点，为避免与Point名称冲突，改为XPoint
	int x, y;
	XPoint()//构造函数，无参数
	{
		x = 0;
		y = 0;
	}
	XPoint(int ptx, int pty)//构造函数
	{
		x = ptx;
		y = pty;
	}
};

bool mouseLeftDown11;
bool mouseRightDown11;
int newheight;
vector<XPoint> cpt, bzpt, dcpt; // cpt：控制点集, bzpt:Bezier曲线点集
bool bDraw;  // 是否显示Bezier曲线
int nInput;  // 计算当前点击的控制点数量
int totalInput; // 控制点总量
float T; // 参数

class Bezier_curve_mouse {
public:

	static constexpr int f(int i) {  // 阶乘
		return i < 2 ? 1 : i * f(i - 1);
	}


	// 计算Bezier曲线上点的坐标
	static void CalcBZPoints(vector<XPoint> cpt)
	{
		int num = 50; // 曲线分段数量
		float dt = 1 / (float)num;
		bzpt.resize(num + 1); //改变容器容量，扩容元素默认为0
		float t = 0;
		for (unsigned int i = 0; i < num + 1; i++) {
			double tmpX = 0, tmpY = 0;
			for (unsigned int j = 0; j < cpt.size(); j++) {  // 对每个控制点
				unsigned int n = cpt.size() - 1;             // 阶数个数为控制点数量 - 1
				double B = f(n) * pow(t, j) * pow((1 - t), n - j) / (double)f(j) / (double)f(n - j);
				tmpX += B * cpt[j].x;  // tmp是关于t的一个函数，每次随t而变化
				tmpY += B * cpt[j].y;
			}
			bzpt[i].x = tmpX;  // 计算出每个t对应的点的坐标（x, y)
			bzpt[i].y = tmpY;
			t += dt;
		}
		//Evaluate(0.3);
	}

	static void ControlPoint(vector<XPoint> cpt)//画点
	{
		glPointSize(3);
		glBegin(GL_POINTS);
		for (unsigned int i = 0; i < cpt.size(); i++)
			glVertex2i(cpt[i].x, cpt[i].y);
		glEnd();
	}

	static void PolylineGL(vector<XPoint> cpt)//画折线
	{
		glBegin(GL_LINE_STRIP);
		for (unsigned int i = 0; i < cpt.size(); i++)
			glVertex2i(cpt[i].x, cpt[i].y);
		glEnd();
	}

	static void showt() {
		int number = T / 0.02;
		vector<XPoint> tmp;
		tmp.push_back(bzpt[number]);
		ControlPoint(tmp);
		printf("参数为 %g 对应的点为: (%d, %d)\n", T, tmp[0].x, tmp[0].y);
		CalcBZPoints(dcpt);
		tmp.push_back(bzpt[number]);
		printf("参数为 %g 对应的点的导数为: (%d, %d)\n", T, tmp[1].x, tmp[1].y);
		printf("重新选择控制点或按ESC退出。\n");
		printf("\n");
	}

	static void myDisplay()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		//glColor3f(0.0f, 0.0f, 0.0f);

		glPointSize(10);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 0.0f); glVertex2i(0, 0);// 显示坐标原点位置
		glEnd();

		if (cpt.size() > 0) {//cpt.size容器中点的数量
			glColor3f(1.0f, 1.0f, 1.0f);
			ControlPoint(cpt);//画点
			PolylineGL(cpt);//画折线
		}

		if (bDraw)//bDraw为true，画Bezier曲线
		{
			CalcBZPoints(cpt);//生成Bezier曲线上的各个点
			glColor3f(1.0f, 0.0f, 0.0f);
			PolylineGL(bzpt);//画Bezier曲线
			showt();
		}
		glFlush();
	}

	void Init()
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_SMOOTH);//控制绘制指定顶点间颜色的过渡模式,GL_SMOOTH渐变
		printf("使用鼠标左键选择控制点...\n");
		//反走样
		glEnable(GL_BLEND);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	static void Reshape(int w, int h)
	{
		glViewport(0, 0, (GLsizei)w, (GLsizei)h);//视区函数，（0，0）左下角坐标，w宽,h高
		glMatrixMode(GL_PROJECTION);//投影矩阵
		glLoadIdentity();//单位矩阵
		gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);//平行投影，X:0.0~w, Y:0.0~h，y坐标从下到大增大，x坐标从左向右增大
		newheight = h;
	}

	static void keyboard(unsigned char key, int x, int y)
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

	static void mouse(int button, int state, int x, int y)//y坐标从上到下增大，x坐标从左向右增大
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (nInput == 0) {
				cpt.clear();//将cpt容器清空
				dcpt.clear();
				cpt.push_back(XPoint(x, newheight - y));//新入的点加入在后面
				nInput++;//鼠标点击次数加1
				bDraw = false;//是否显示曲线，false，不显示
				glutPostRedisplay();//窗口重新绘制，调用绘制函数
			}
			else if (nInput < totalInput - 1) {
				cpt.push_back(XPoint(x, newheight - y));
				int dx = cpt[nInput].x - cpt[nInput - 1].x;
				int dy = cpt[nInput].y - cpt[nInput - 1].y;
				dcpt.push_back(XPoint(dx, dy));
				nInput++;
				glutPostRedisplay();
			}
			else {
				cpt.push_back(XPoint(x, newheight - y));
				int dx = cpt[nInput].x - cpt[nInput - 1].x;
				int dy = cpt[nInput].y - cpt[nInput - 1].y;
				dcpt.push_back(XPoint(dx, dy));
				nInput = 0;//nInput为0，为新一轮点击作准备
				bDraw = true;//表示显示曲线
				glutPostRedisplay();
			}
		}
		else if (button == GLUT_RIGHT_BUTTON)
		{
			if (state == GLUT_DOWN)
			{
				mouseRightDown11 = true;
				printf("%d %d\n", x, y);
			}
			else if (state == GLUT_UP)
				mouseRightDown11 = false;
		}
	}

	static void mouseMotionCB(int x, int y)
	{
		float min = 99999999;
		int index;
		if (mouseRightDown11)
		{
			for (int i = 0; i < cpt.size(); i++) {  // 找到离当前鼠标最近的控制点
				if (min > ((x - cpt[i].x) * (x - cpt[i].x) + ((480 - y) - cpt[i].y) * ((480 - y) - cpt[i].y))) {
					min = (x - cpt[i].x) * (x - cpt[i].x) + ((480 - y) - cpt[i].y) * ((480 - y) - cpt[i].y);
					index = i;
				}
			}
			cpt[index].x = x;
			cpt[index].y = newheight - y;
		}
		glutPostRedisplay();
	}

	void render()
	{
		int a = NULL;
		glutInit(&a, NULL);
		glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
		float t;
		printf("请输入参数t(0-1): ");
		scanf_s("%f", &T);
		printf("请输入绘制次数: ");
		scanf_s("%d", &totalInput);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(640, 480);
		glutCreateWindow("Bezier Curve");

		Init();
		glutDisplayFunc(myDisplay);
		glutReshapeFunc(Reshape);
		glutMouseFunc(mouse);//响应鼠标
		glutMotionFunc(mouseMotionCB);
		glutKeyboardFunc(keyboard);
		glutMainLoop();
	}
};



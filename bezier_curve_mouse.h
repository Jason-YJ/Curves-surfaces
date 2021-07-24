#pragma once
#pragma once
//ʹ��glmap
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

struct XPoint { // ���嶥�㣬Ϊ������Point���Ƴ�ͻ����ΪXPoint
	int x, y;
	XPoint()//���캯�����޲���
	{
		x = 0;
		y = 0;
	}
	XPoint(int ptx, int pty)//���캯��
	{
		x = ptx;
		y = pty;
	}
};

bool mouseLeftDown11;
bool mouseRightDown11;
int newheight;
vector<XPoint> cpt, bzpt, dcpt; // cpt�����Ƶ㼯, bzpt:Bezier���ߵ㼯
bool bDraw;  // �Ƿ���ʾBezier����
int nInput;  // ���㵱ǰ����Ŀ��Ƶ�����
int totalInput; // ���Ƶ�����
float T; // ����

class Bezier_curve_mouse {
public:

	static constexpr int f(int i) {  // �׳�
		return i < 2 ? 1 : i * f(i - 1);
	}


	// ����Bezier�����ϵ������
	static void CalcBZPoints(vector<XPoint> cpt)
	{
		int num = 50; // ���߷ֶ�����
		float dt = 1 / (float)num;
		bzpt.resize(num + 1); //�ı���������������Ԫ��Ĭ��Ϊ0
		float t = 0;
		for (unsigned int i = 0; i < num + 1; i++) {
			double tmpX = 0, tmpY = 0;
			for (unsigned int j = 0; j < cpt.size(); j++) {  // ��ÿ�����Ƶ�
				unsigned int n = cpt.size() - 1;             // ��������Ϊ���Ƶ����� - 1
				double B = f(n) * pow(t, j) * pow((1 - t), n - j) / (double)f(j) / (double)f(n - j);
				tmpX += B * cpt[j].x;  // tmp�ǹ���t��һ��������ÿ����t���仯
				tmpY += B * cpt[j].y;
			}
			bzpt[i].x = tmpX;  // �����ÿ��t��Ӧ�ĵ�����꣨x, y)
			bzpt[i].y = tmpY;
			t += dt;
		}
		//Evaluate(0.3);
	}

	static void ControlPoint(vector<XPoint> cpt)//����
	{
		glPointSize(3);
		glBegin(GL_POINTS);
		for (unsigned int i = 0; i < cpt.size(); i++)
			glVertex2i(cpt[i].x, cpt[i].y);
		glEnd();
	}

	static void PolylineGL(vector<XPoint> cpt)//������
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
		printf("����Ϊ %g ��Ӧ�ĵ�Ϊ: (%d, %d)\n", T, tmp[0].x, tmp[0].y);
		CalcBZPoints(dcpt);
		tmp.push_back(bzpt[number]);
		printf("����Ϊ %g ��Ӧ�ĵ�ĵ���Ϊ: (%d, %d)\n", T, tmp[1].x, tmp[1].y);
		printf("����ѡ����Ƶ��ESC�˳���\n");
		printf("\n");
	}

	static void myDisplay()
	{
		glClear(GL_COLOR_BUFFER_BIT);
		//glColor3f(0.0f, 0.0f, 0.0f);

		glPointSize(10);
		glBegin(GL_POINTS);
		glColor3f(1.0f, 1.0f, 0.0f); glVertex2i(0, 0);// ��ʾ����ԭ��λ��
		glEnd();

		if (cpt.size() > 0) {//cpt.size�����е������
			glColor3f(1.0f, 1.0f, 1.0f);
			ControlPoint(cpt);//����
			PolylineGL(cpt);//������
		}

		if (bDraw)//bDrawΪtrue����Bezier����
		{
			CalcBZPoints(cpt);//����Bezier�����ϵĸ�����
			glColor3f(1.0f, 0.0f, 0.0f);
			PolylineGL(bzpt);//��Bezier����
			showt();
		}
		glFlush();
	}

	void Init()
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);
		glShadeModel(GL_SMOOTH);//���ƻ���ָ���������ɫ�Ĺ���ģʽ,GL_SMOOTH����
		printf("ʹ��������ѡ����Ƶ�...\n");
		//������
		glEnable(GL_BLEND);
		glEnable(GL_LINE_SMOOTH);
		glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	static void Reshape(int w, int h)
	{
		glViewport(0, 0, (GLsizei)w, (GLsizei)h);//������������0��0�����½����꣬w��,h��
		glMatrixMode(GL_PROJECTION);//ͶӰ����
		glLoadIdentity();//��λ����
		gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);//ƽ��ͶӰ��X:0.0~w, Y:0.0~h��y������µ�������x���������������
		newheight = h;
	}

	static void keyboard(unsigned char key, int x, int y)
	{
		switch (key)
		{
		case 27:   //ESC��
			exit(0);
			break;
		default:
			break;
		}
	}

	static void mouse(int button, int state, int x, int y)//y������ϵ�������x���������������
	{
		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			if (nInput == 0) {
				cpt.clear();//��cpt�������
				dcpt.clear();
				cpt.push_back(XPoint(x, newheight - y));//����ĵ�����ں���
				nInput++;//�����������1
				bDraw = false;//�Ƿ���ʾ���ߣ�false������ʾ
				glutPostRedisplay();//�������»��ƣ����û��ƺ���
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
				nInput = 0;//nInputΪ0��Ϊ��һ�ֵ����׼��
				bDraw = true;//��ʾ��ʾ����
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
			for (int i = 0; i < cpt.size(); i++) {  // �ҵ��뵱ǰ�������Ŀ��Ƶ�
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
		printf("���������t(0-1): ");
		scanf_s("%f", &T);
		printf("��������ƴ���: ");
		scanf_s("%d", &totalInput);
		glutInitWindowPosition(100, 100);
		glutInitWindowSize(640, 480);
		glutCreateWindow("Bezier Curve");

		Init();
		glutDisplayFunc(myDisplay);
		glutReshapeFunc(Reshape);
		glutMouseFunc(mouse);//��Ӧ���
		glutMotionFunc(mouseMotionCB);
		glutKeyboardFunc(keyboard);
		glutMainLoop();
	}
};



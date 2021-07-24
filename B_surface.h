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
			line_process(line);//�����׺���β�Ķ���ո�, tabȥ������ע������Ҳȥ��
			if (line.empty()) continue;//lineΪ�������
			//����ʵ��������
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
			line_process(line);//�����׺���β�Ķ���ո�, tabȥ������ע������Ҳȥ��
			if (line.empty()) continue;//lineΪ�������
			//����ʵ��������
			std::istringstream iss(line);
			double x_temp;
			iss >> x_temp;
			knots32[row] = x_temp;
			row++;
		}
	}

	void init(void)
	{
		//����ɫ
		glClearColor(0.0, 0.0, 0.0, 1.0);
		//���뿪��3�����ò������Դ
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

		//������Ȳ���
		glDepthFunc(GL_LESS);
		glEnable(GL_DEPTH_TEST);
		//���뿪��4��ȡ���������д��룬�鿴������ʾЧ������
		//���Զ���ʸ������
		glEnable(GL_AUTO_NORMAL);
		//�������򻯷�ʸ��
		glEnable(GL_NORMALIZE);
		B_sur = gluNewNurbsRenderer(); // ����һ��B�����������  
		//�޸�B���������������ԡ���glu�⺯��
		////����sampling�ݴ�torerance
		gluNurbsProperty(B_sur, GLU_SAMPLING_TOLERANCE, 5.0);
		gluNurbsProperty(B_sur, GLU_DISPLAY_MODE, GLU_FILL);
	}

	static void ControlPoint()//����
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
	/*��������*/
	static void Display(void)
	{
		glRotatef(cameraAngleX32, 1, 0, 0);
		glRotatef(cameraAngleY32, 0, 1, 0);
		glTranslatef(cameraDistanceX32, cameraDistanceY32, 0);
		glScalef(times32, times32, times32);//����
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glColor3f(1.0, 0.0, 0.0);
		ControlPoint();
		glColor3f(0.0, 1.0, 0.0);
		gluBeginSurface(B_sur); // ��ʼ������� 
		gluNurbsSurface(B_sur, 8, knots32, 8, knots32, 4 * 3, 3, &sur3[0][0][0], 4, 4, GL_MAP2_VERTEX_3); // �����������ѧģ�ͣ�ȷ������״  
		gluEndSurface(B_sur); // �����������  
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

	static void keyboard(unsigned char key, int x, int y) //key Ϊ���̰�����ASCII��
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
		if (button == 0 && state == GLUT_UP) // ���Ϲ����֣��Ŵ�
		{
			times32 = 1 + 0.008f;
			glutPostRedisplay();
		}
		else {  // ���¹����֣���С
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
		glutCreateWindow("����B��������");
		/*��������ʾ*/
		init();
		glutDisplayFunc(Display);
		glutReshapeFunc(Reshape);
		glutKeyboardFunc(keyboard);
		glutIdleFunc(Display);//���ÿ���ʱ���õĺ���
		glutMouseFunc(mouseCB);
		glutMotionFunc(mouseMotionCB);
		glutMouseWheelFunc(mouseWheel);
		glutMainLoop();
	}

	void line_process(std::string& line, const std::string comment_str = "#")
	{
		for (char& c : line)//C++11���ϰ汾���﷨
		{
			//�Ʊ�� tab�����ţ��ֺŶ�������Ч�ķָ�����ͳһת�ɿո�
			//Ϊ�˱�����󣬻س����ͻ��з�ҲתΪ�ո񣨷����޷�������У�
			if (c == '\t' || c == ',' || c == ';' || c == '\r' || c == '\n')
				c = ' ';
		}

		line.erase(0, line.find_first_not_of(" "));//ɾ�����׿ո�
		line.erase(line.find_last_not_of(" ") + 1);//ɾ����ĩ�ո�
		//���õ�string& erase (size_t pos = 0, size_t len = npos);
		//lenΪĬ�ϲ���


		//����ע�ͷ�����λ�ã���������ڣ���õ�string::npos
		int n_comment_start = line.find_first_of(comment_str);
		if (n_comment_start != std::string::npos)//��һ������
			line.erase(n_comment_start);         //ɾ��ע��

		//������ϡ������һ��ֻ�пո��Ʊ�� tab��ע�ͣ���ô�����lineΪ�գ�
		//��������ж���ո�(���߿ո��tab����)����βΪע�ͣ���ô������ַ���line��
		//���׶���ո�(��tab)����βע�ͱ�ɾ����ֻ��������������ݡ�
	}
};

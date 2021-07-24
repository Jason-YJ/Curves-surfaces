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
			line_process(line);//�����׺���β�Ķ���ո�, tabȥ������ע������Ҳȥ��
			if (line.empty()) continue;//lineΪ�������
			//����ʵ��������
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
			line_process(line);//�����׺���β�Ķ���ո�, tabȥ������ע������Ҳȥ��
			if (line.empty()) continue;//lineΪ�������
			//����ʵ��������
			std::istringstream iss(line);
			double x_temp;
			iss >> x_temp;
			knots21[row] = x_temp;
			row++;
		}
	}

	void init(void)
	{
		glClearColor(0.0, 0.0, 0.0, 0.0);//���ñ���ɫ
		theNurb11 = gluNewNurbsRenderer();//����NURBS����theNurb
		gluNurbsProperty(theNurb11, GLU_SAMPLING_TOLERANCE, 10);
	}

	static void ControlPoint()//����
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
		/*��������*/
		gluBeginCurve(theNurb11);
		gluNurbsCurve(theNurb11, 15, knots21, 3, &cur2[0][0], 4, GL_MAP1_VERTEX_3);
		//gluNurbsCurve(theNurb, 13, knots, 3, &color[0][0], 4, GL_MAP1_COLOR_4);
		gluEndCurve(theNurb11);
		/*���ƿ��ƶ����*/
		glLineWidth(1);//����դ�������Ŀ��
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

	void render()
	{
		int a = NULL;
		glutInit(&a, NULL);
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
		glutInitWindowSize(600, 400);
		glutInitWindowPosition(200, 200);
		glutCreateWindow("����Nurbs����");
		/*��������ʾ*/
		init();
		glutReshapeFunc(Reshape);
		glutKeyboardFunc(keyboard);
		glutDisplayFunc(Display);
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

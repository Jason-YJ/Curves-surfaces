#pragma once
//ʹ��glmap
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

GLfloat cur1[4][3];
bool mouseLeftDown1;
bool mouseRightDown1;
bool mouseMiddleDown1;
float mouseX1, mouseY1;
float cameraDistanceX1;
float cameraDistanceY1;
float cameraAngleX1;
float cameraAngleY1;
float times1;

class Bezier_curve {
public:
    Bezier_curve(const char* datafile) {
        ifstream is(datafile);
        int row = 0;
        string line;
        while (std::getline(is, line))
        {
            line_process(line);//�����׺���β�Ķ���ո�, tabȥ������ע������Ҳȥ��
            if (line.empty()) continue;//lineΪ�������
            //����ʵ��������
            std::istringstream iss(line);
            double x_temp, y_temp, z_temp;
            iss >> x_temp >> y_temp >> z_temp;
            cur1[row][0] = x_temp;
            cur1[row][1] = y_temp;
            cur1[row][2] = z_temp;
            row++;
        }
    }

    void init(void)
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glMap1f(GL_MAP1_VERTEX_3, 0, 100, 3, 4, &cur1[0][0]);  // ��ȡ�ļ��еĿ��Ƶ����꣬shape = (4, 3)
        glEnable(GL_MAP1_VERTEX_3);
        //������
        glEnable(GL_BLEND);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    static void ControlPoint()//����
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i < 4; i++) {
            glVertex3fv(cur1[i]);
        }
        glEnd();
    }

    static void display(void)
    {
        glRotatef(cameraAngleX1, 1, 0, 0);
        glRotatef(cameraAngleY1, 0, 1, 0);
        glTranslatef(cameraDistanceX1, cameraDistanceY1, 0);
        glScalef(times1, times1, times1);//����
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1.0, 0.0, 0.0);
        ControlPoint();
        glColor3f(0.0, 1.0, 0.0);
        glMapGrid1f(100, 0.0f, 100.0f); 
        glEvalMesh1(GL_LINE, 0, 100);
        glutSwapBuffers();
    }

    static void reshape(GLsizei w, GLsizei h)
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

    static void mouseCB(int button, int state, int x, int y)
    {
        mouseX1 = x;
        mouseY1 = y;
        times1 = 1;

        if (button == GLUT_LEFT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                mouseLeftDown1 = true;
            }
            else if (state == GLUT_UP)
                mouseLeftDown1 = false;
        }

        else if (button == GLUT_RIGHT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                mouseRightDown1 = true;
            }
            else if (state == GLUT_UP)
                mouseRightDown1 = false;
        }
    }

    static void mouseMotionCB(int x, int y)
    {
        cameraAngleX1 = cameraAngleY1 = 0;
        cameraDistanceX1 = cameraDistanceY1 = 0;

        if (mouseLeftDown1)
        {
            cameraAngleY1 += (x - mouseX1) * 0.001f;
            cameraAngleX1 += (y - mouseY1) * 0.001f;
            mouseX1 = x;
            mouseY1 = y;
        }
        if (mouseRightDown1)
        {
            cameraDistanceX1 = (x - mouseX1) * 0.001f;
            cameraDistanceY1 = -(y - mouseY1) * 0.001f;
            mouseY1 = y;
            mouseX1 = x;
        }
        glutPostRedisplay();
    }

    static void mouseWheel(int button, int state, int x, int y)
    {
        if (button == 0 && state == GLUT_UP) // ���Ϲ����֣��Ŵ�
        {
            times1 = 1 + 0.00008f;
            glutPostRedisplay();
        }
        else {  // ���¹����֣���С
            times1 = 1 - 0.00008f;
            glutPostRedisplay();
        }
    }

    void render()
    {
        int a = 1;
        glutInit(&a, NULL);
        srand((unsigned int)time(0));
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//ʹ��˫����ģʽ����Ȼ���
        glutInitWindowPosition(0, 0);
        glutInitWindowSize(800, 800);
        glutCreateWindow("Bezier�����߿�ģ��");
        init();
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutIdleFunc(display);//���ÿ���ʱ���õĺ���
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



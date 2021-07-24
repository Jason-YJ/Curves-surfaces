#pragma once
#include <stdlib.h>
#include <time.h>
#include <GL/freeglut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <cassert>
#include <GL/freeglut_ext.h>
#include <vector>
using namespace std;

GLfloat csur1[4][4][3]; 
GLUnurbsObj* ctheB1;
GLfloat csur3[4][4][3];
GLfloat cknots3[8];
bool mouseLeftDownc;
bool mouseRightDownc;
bool mouseMiddleDownc;
float mouseXc, mouseYc;
float cameraDistanceXc;
float cameraDistanceYc;
float cameraAngleXc;
float cameraAngleYc;
float timesc;

class Bezier_and_B_surface {
public:
    Bezier_and_B_surface(const char* bazierdata, const char* B_cp, const char* B_knots) {
        ifstream is(bazierdata);
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
            csur1[row][col][0] = x_temp;
            csur1[row][col][1] = y_temp;
            csur1[row][col][2] = z_temp;
            col++;
            if (col == 4) {
                col = 0;
                row++;
            }
        }
        ifstream is1(B_cp);
        row = 0;
        col = 0;
        while (std::getline(is1, line))
        {
            line_process(line);//�����׺���β�Ķ���ո�, tabȥ������ע������Ҳȥ��
            if (line.empty()) continue;//lineΪ�������
            //����ʵ��������
            std::istringstream iss(line);
            iss >> x_temp >> y_temp >> z_temp;
            csur3[row][col][0] = y_temp;
            csur3[row][col][1] = x_temp;
            csur3[row][col][2] = z_temp;
            col++;
            if (col == 4) {
                col = 0;
                row++;
            }
        }
        ifstream is2(B_knots);
        row = 0;
        while (std::getline(is2, line))
        {
            line_process(line);//�����׺���β�Ķ���ո�, tabȥ������ע������Ҳȥ��
            if (line.empty()) continue;//lineΪ�������
            //����ʵ��������
            std::istringstream iss(line);
            iss >> x_temp;
            cknots3[row] = x_temp;
            row++;
        }
    }

    void init(void)
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        //������
        glEnable(GL_BLEND);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //������Ȳ���
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        //���Զ���ʸ������
        glEnable(GL_AUTO_NORMAL);
        //�������򻯷�ʸ��
        glEnable(GL_NORMALIZE);
    }

    static void ControlPoint()//����
    {
        glPointSize(5);
        glBegin(GL_POINTS);
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                //glVertex3fv(csur1[i][j]);
                glVertex3fv(csur1[i][j]);
            }
        }
        glEnd();
        glBegin(GL_LINE_STRIP);
        for (unsigned int i = 0; i < 4; i++) {
            for (unsigned int j = 0; j < 4; j++) {
                //glVertex3fv(csur1[i][j]);
                glVertex3fv(csur1[i][j]);
            }
        }
        glEnd();
    }

    static void drawBezier(void)
    {
        /* draw yellow triangle on LHS of screen */
        glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4, &csur1[0][0][0]);  // ��ȡ�ļ��е�Bezier������Ƶ����꣬shape = (4, 4, 3)
        glEnable(GL_MAP2_VERTEX_3);
        glColor3f(0.0, 0.4, 0.0);
        glMapGrid2f(10, 0.0, 1.0, 10, 0.0, 1.0);
        glEvalMesh2(GL_FILL, 0, 10, 0, 10);
    }

    static void drawB(void)
    {
        ////����sampling�ݴ�torerance
        ctheB1 = gluNewNurbsRenderer(); // ����һ��B�����������  
        gluNurbsProperty(ctheB1, GLU_SAMPLING_TOLERANCE, 5.0);
        gluNurbsProperty(ctheB1, GLU_DISPLAY_MODE, GLU_FILL);
        glColor3f(0.0, 0.0, 0.4);
        gluBeginSurface(ctheB1); // ��ʼ������� 
        gluNurbsSurface(ctheB1, 8, cknots3, 8, cknots3, 4 * 3, 3, &csur3[0][0][0], 4, 4, GL_MAP2_VERTEX_3); // �����������ѧģ�ͣ�ȷ������״  
        gluEndSurface(ctheB1); // �����������  
    }

    static void display(void)
    {
        //glPushMatrix();
        glRotatef(cameraAngleXc, 1, 0, 0);
        glRotatef(cameraAngleYc, 0, 1, 0);
        glTranslatef(cameraDistanceXc, cameraDistanceYc, 0);
        glScalef(timesc, timesc, timesc);//����
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
        drawBezier();
        drawB();
        glutSwapBuffers(); 
        glFlush();
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
        mouseXc = x;
        mouseYc = y;
        timesc = 1;

        if (button == GLUT_LEFT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                mouseLeftDownc = true;
            }
            else if (state == GLUT_UP)
                mouseLeftDownc = false;
        }

        else if (button == GLUT_RIGHT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                mouseRightDownc = true;
            }
            else if (state == GLUT_UP)
                mouseRightDownc = false;
        }
    }

    static void mouseMotionCB(int x, int y)
    {
        cameraAngleXc = cameraAngleYc = 0;
        cameraDistanceXc = cameraDistanceYc = 0;

        if (mouseLeftDownc)
        {
            cameraAngleYc += (x - mouseXc) * 0.003f;
            cameraAngleXc += (y - mouseYc) * 0.003f;
            mouseXc = x;
            mouseYc = y;
        }
        if (mouseRightDownc)
        {
            cameraDistanceXc = (x - mouseXc) * 0.001f;
            cameraDistanceYc = -(y - mouseYc) * 0.001f;
            mouseYc = y;
            mouseXc = x;
        }
        glutPostRedisplay();
    }

    static void mouseWheel(int button, int state, int x, int y)
    {
        if (button == 0 && state == GLUT_UP) // ���Ϲ����֣��Ŵ�
        {
            timesc = 1 + 0.008f;
            glutPostRedisplay();
        }
        else {  // ���¹����֣���С
            timesc = 1 - 0.008f;
            glutPostRedisplay();
        }
    }

    void render()
    {
        int a = 1;
        glutInit(&a, NULL);
        srand((unsigned int)time(0));
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//ʹ��˫����ģʽ����Ȼ���
        glutInitWindowSize(800, 800);
        glutInitWindowPosition(0, 0);
        glutCreateWindow("Bezier������B��������ģ��");
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
    //����ע�ͣ��ո񣬺Ϳ��еĺ���
    //line����ʾһ���ı�����
    //comment_str����ʾע��ǰ���ַ�����Ĭ������Ϊ#��Ҳ������//����%
    //stringɾ���ִ�
    //string�����ִ�
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

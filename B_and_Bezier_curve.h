#pragma once
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

GLfloat ccur11[4][3];
GLUnurbsObj* ctheB11;
GLfloat csur31[4][4][3];
GLfloat cknots31[8];
bool mouseLeftDownc1;
bool mouseRightDownc1;
bool mouseMiddleDownc1;
float mouseXc1, mouseYc1;
float cameraDistanceXc1;
float cameraDistanceYc1;
float cameraAngleXc1;
float cameraAngleYc1;
float timesc1;

class Bezier_and_B_curve {
public:
    Bezier_and_B_curve(const char* bazierdata, const char* B_cp, const char* B_knots) {
        ifstream is(bazierdata);
        int row = 0;
        string line;
        double x_temp, y_temp, z_temp;
        while (std::getline(is, line))
        {
            line_process(line);//�����׺���β�Ķ���ո�, tabȥ������ע������Ҳȥ��
            if (line.empty()) continue;//lineΪ�������
            //����ʵ��������
            std::istringstream iss(line);
            iss >> x_temp >> y_temp >> z_temp;
            ccur11[row][0] = x_temp;
            ccur11[row][1] = y_temp;
            ccur11[row][2] = z_temp;
            row++;
        }
        ifstream is1(B_cp);
        row = 0;
        int col = 0;
        while (std::getline(is1, line))
        {
            line_process(line);//�����׺���β�Ķ���ո�, tabȥ������ע������Ҳȥ��
            if (line.empty()) continue;//lineΪ�������
            //����ʵ��������
            std::istringstream iss(line);
            iss >> x_temp >> y_temp >> z_temp;
            csur31[row][col][0] = y_temp;
            csur31[row][col][1] = x_temp;
            csur31[row][col][2] = z_temp;
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
            cknots31[row] = x_temp;
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
        glColor3f(1.0, 0.0, 0.0);
        for (unsigned int i = 0; i < 4; i++) {
            glVertex3fv(ccur11[i]);
        }
        glEnd();
        for (unsigned int i = 0; i < 4; i++) {
            glBegin(GL_POINTS);
            glColor3f(0.0, 1.0, 0.0);
            for (unsigned int j = 0; j < 4; j++) {
                glVertex3fv(csur31[i][j]);
            }
            glEnd();
        }
    }

    static void drawBezier(void)
    {
        /* draw yellow triangle on LHS of screen */
        glMap1f(GL_MAP1_VERTEX_3, 0, 100, 3, 4, &cur1[0][0]);  // ��ȡ�ļ��еĿ��Ƶ����꣬shape = (4, 3)
        glEnable(GL_MAP1_VERTEX_3);
        glColor3f(0.0, 0.4, 0.0);
        glLineWidth(3.0);
        glMapGrid1f(100, 0.0f, 100.0f);
        glEvalMesh1(GL_LINE, 0, 100);
    }

    static void drawB(void)
    {
        ////����sampling�ݴ�torerance
        ctheB11 = gluNewNurbsRenderer(); // ����һ��B�����������  
        gluNurbsProperty(ctheB11, GLU_SAMPLING_TOLERANCE, 5.0);
        gluNurbsProperty(ctheB11, GLU_DISPLAY_MODE, GLU_FILL);
        glColor3f(0.0, 0.0, 0.4);
        gluBeginSurface(ctheB11); // ��ʼ������� 
        gluNurbsSurface(ctheB11, 8, cknots31, 8, cknots31, 4 * 3, 3, &csur31[0][0][0], 4, 4, GL_MAP2_VERTEX_3); // �����������ѧģ�ͣ�ȷ������״  
        gluEndSurface(ctheB11); // �����������  
    }

    static void display(void)
    {
        //glPushMatrix();
        glRotatef(cameraAngleXc1, 1, 0, 0);
        glRotatef(cameraAngleYc1, 0, 1, 0);
        glTranslatef(cameraDistanceXc1, cameraDistanceYc1, 0);
        glScalef(timesc1, timesc1, timesc1);//����
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        ControlPoint();
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
        mouseXc1 = x;
        mouseYc1 = y;
        timesc1 = 1;

        if (button == GLUT_LEFT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                mouseLeftDownc1 = true;
            }
            else if (state == GLUT_UP)
                mouseLeftDownc1 = false;
        }

        else if (button == GLUT_RIGHT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                mouseRightDownc1 = true;
            }
            else if (state == GLUT_UP)
                mouseRightDownc1 = false;
        }
    }

    static void mouseMotionCB(int x, int y)
    {
        cameraAngleXc1 = cameraAngleYc1 = 0;
        cameraDistanceXc1 = cameraDistanceYc1 = 0;

        if (mouseLeftDownc1)
        {
            cameraAngleYc1 += (x - mouseXc1) * 0.003f;
            cameraAngleXc1 += (y - mouseYc1) * 0.003f;
            mouseXc1 = x;
            mouseYc1 = y;
        }
        if (mouseRightDownc1)
        {
            cameraDistanceXc1 = (x - mouseXc1) * 0.001f;
            cameraDistanceYc1 = -(y - mouseYc1) * 0.001f;
            mouseYc1 = y;
            mouseXc1 = x;
        }
        glutPostRedisplay();
    }

    static void mouseWheel(int button, int state, int x, int y)
    {
        if (button == 0 && state == GLUT_UP) // ���Ϲ����֣��Ŵ�
        {
            timesc1 = 1 + 0.00008f;
            glutPostRedisplay();
        }
        else {  // ���¹����֣���С
            timesc1 = 1 - 0.00008f;
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

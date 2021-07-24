#pragma once
//使用glmap
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
            line_process(line);//把行首和行尾的多个空格, tab去掉，把注释文字也去掉
            if (line.empty()) continue;//line为空则继续
            //根据实际需求处理
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
        glMap1f(GL_MAP1_VERTEX_3, 0, 100, 3, 4, &cur1[0][0]);  // 读取文件中的控制点坐标，shape = (4, 3)
        glEnable(GL_MAP1_VERTEX_3);
        //反走样
        glEnable(GL_BLEND);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    static void ControlPoint()//画点
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
        glScalef(times1, times1, times1);//缩放
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
        case 27:   //ESC键
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
        if (button == 0 && state == GLUT_UP) // 往上滚滚轮，放大
        {
            times1 = 1 + 0.00008f;
            glutPostRedisplay();
        }
        else {  // 往下滚滚轮，缩小
            times1 = 1 - 0.00008f;
            glutPostRedisplay();
        }
    }

    void render()
    {
        int a = 1;
        glutInit(&a, NULL);
        srand((unsigned int)time(0));
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//使用双缓存模式和深度缓存
        glutInitWindowPosition(0, 0);
        glutInitWindowSize(800, 800);
        glutCreateWindow("Bezier曲线线框模型");
        init();
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        glutIdleFunc(display);//设置空闲时调用的函数
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



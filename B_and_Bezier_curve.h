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
            line_process(line);//把行首和行尾的多个空格, tab去掉，把注释文字也去掉
            if (line.empty()) continue;//line为空则继续
            //根据实际需求处理
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
            line_process(line);//把行首和行尾的多个空格, tab去掉，把注释文字也去掉
            if (line.empty()) continue;//line为空则继续
            //根据实际需求处理
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
            line_process(line);//把行首和行尾的多个空格, tab去掉，把注释文字也去掉
            if (line.empty()) continue;//line为空则继续
            //根据实际需求处理
            std::istringstream iss(line);
            iss >> x_temp;
            cknots31[row] = x_temp;
            row++;
        }
    }

    void init(void)
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        //反走样
        glEnable(GL_BLEND);
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        //允许深度测试
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        //打开自动法矢量开关
        glEnable(GL_AUTO_NORMAL);
        //允许正则化法矢量
        glEnable(GL_NORMALIZE);
    }

    static void ControlPoint()//画点
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
        glMap1f(GL_MAP1_VERTEX_3, 0, 100, 3, 4, &cur1[0][0]);  // 读取文件中的控制点坐标，shape = (4, 3)
        glEnable(GL_MAP1_VERTEX_3);
        glColor3f(0.0, 0.4, 0.0);
        glLineWidth(3.0);
        glMapGrid1f(100, 0.0f, 100.0f);
        glEvalMesh1(GL_LINE, 0, 100);
    }

    static void drawB(void)
    {
        ////采样sampling容错torerance
        ctheB11 = gluNewNurbsRenderer(); // 创建一个B样条曲面对象  
        gluNurbsProperty(ctheB11, GLU_SAMPLING_TOLERANCE, 5.0);
        gluNurbsProperty(ctheB11, GLU_DISPLAY_MODE, GLU_FILL);
        glColor3f(0.0, 0.0, 0.4);
        gluBeginSurface(ctheB11); // 开始曲面绘制 
        gluNurbsSurface(ctheB11, 8, cknots31, 8, cknots31, 4 * 3, 3, &csur31[0][0][0], 4, 4, GL_MAP2_VERTEX_3); // 定义曲面的数学模型，确定其形状  
        gluEndSurface(ctheB11); // 结束曲面绘制  
    }

    static void display(void)
    {
        //glPushMatrix();
        glRotatef(cameraAngleXc1, 1, 0, 0);
        glRotatef(cameraAngleYc1, 0, 1, 0);
        glTranslatef(cameraDistanceXc1, cameraDistanceYc1, 0);
        glScalef(timesc1, timesc1, timesc1);//缩放
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
        case 27:   //ESC键
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
        if (button == 0 && state == GLUT_UP) // 往上滚滚轮，放大
        {
            timesc1 = 1 + 0.00008f;
            glutPostRedisplay();
        }
        else {  // 往下滚滚轮，缩小
            timesc1 = 1 - 0.00008f;
            glutPostRedisplay();
        }
    }

    void render()
    {
        int a = 1;
        glutInit(&a, NULL);
        srand((unsigned int)time(0));
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);//使用双缓存模式和深度缓存
        glutInitWindowSize(800, 800);
        glutInitWindowPosition(0, 0);
        glutCreateWindow("Bezier曲线与B样条曲面模型");
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
    //处理注释，空格，和空行的函数
    //line，表示一行文本内容
    //comment_str，表示注释前导字符串，默认设置为#，也可以用//或者%
    //string删除字串
    //string查找字串
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

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
#include <GL/freeglut_ext.h>
#include <vector>
using namespace std;

GLfloat sur[4][4][3];
bool mouseLeftDown2;
bool mouseRightDown2;
bool mouseMiddleDown2;
float mouseX2, mouseY2;
float cameraDistanceX2;
float cameraDistanceY2;
float cameraAngleX2;
float cameraAngleY2;
float times2;

class Bezier_surface {
public:
    Bezier_surface(const char* datafile) {
        ifstream is(datafile);
        int row = 0, col = 0;
        string line;
        while (std::getline(is, line))
        {
            line_process(line);//把行首和行尾的多个空格, tab去掉，把注释文字也去掉
            if (line.empty()) continue;//line为空则继续
            //根据实际需求处理
            std::istringstream iss(line);
            double x_temp, y_temp, z_temp;
            iss >> x_temp >> y_temp >> z_temp;
            sur[row][col][0] = x_temp;
            sur[row][col][1] = y_temp;
            sur[row][col][2] = z_temp;
            col++;
            if (col == 4) {
                col = 0;
                row++;
            }
        }
    }

    void init(void)
    {
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glMap2f(GL_MAP2_VERTEX_3, 0, 100, 3, 4, 0, 100, 12, 4, &sur[0][0][0]);  // 读取文件中的控制点坐标，shape = (4, 4, 3)
        glEnable(GL_MAP2_VERTEX_3);
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
            for (unsigned int j = 0; j < 4; j++) {
                glVertex3fv(sur[i][j]);
            }
        }
        glEnd();
    }

    static void display(void)
    {
        glRotatef(cameraAngleX2, 1, 0, 0);
        glRotatef(cameraAngleY2, 0, 1, 0);
        glTranslatef(cameraDistanceX2, cameraDistanceY2, 0);
        glScalef(times2, times2, times2);//缩放
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1.0, 0.0, 0.0);
        glPushMatrix();
        ControlPoint();
        glColor3f(0.0, 1.0, 0.0);
        glMapGrid2f(20, 0.0, 100.0, 20, 0.0, 100.0);
        glEvalMesh2(GL_LINE, 0, 20, 0, 20);
        //生成2D网格坐标，以从控制点参数插值确定网格点所对应的点集所对应的坐标,也可以用下面的方式:
        //int i, j;
        //for (j = 0; j <= 20; j++) {
        //    glBegin(GL_LINE_STRIP);
        //    for (i = 0; i <= 30; i++)
        //        glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)j / 20.0);  //固定y坐标时x方向的网格坐标
        //    glEnd();
        //    glBegin(GL_LINE_STRIP);
        //    for (i = 0; i <= 30; i++)
        //        glEvalCoord2f((GLfloat)j / 20.0, (GLfloat)i / 30.0);  //固定x坐标时y方向的网格坐标     
        //    glEnd();
        //}
        glPopMatrix();
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
        mouseX2 = x;
        mouseY2 = y;
        times2 = 1;

        if (button == GLUT_LEFT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                mouseLeftDown2 = true;
            }
            else if (state == GLUT_UP)
                mouseLeftDown2 = false;
        }

        else if (button == GLUT_RIGHT_BUTTON)
        {
            if (state == GLUT_DOWN)
            {
                mouseRightDown2 = true;
            }
            else if (state == GLUT_UP)
                mouseRightDown2 = false;
        }
    }

    static void mouseMotionCB(int x, int y)
    {
        cameraAngleX2 = cameraAngleY2 = 0;
        cameraDistanceX2 = cameraDistanceY2 = 0;

        if (mouseLeftDown2)
        {
            cameraAngleY2 += (x - mouseX2) * 0.05f;
            cameraAngleX2 += (y - mouseY2) * 0.05f;
            mouseX2 = x;
            mouseY2 = y;
        }
        if (mouseRightDown2)
        {
            cameraDistanceX2 = (x - mouseX2) * 0.01f;
            cameraDistanceY2 = -(y - mouseY2) * 0.01f;
            mouseY2 = y;
            mouseX2 = x;
        }
        glutPostRedisplay();
    }

    static void mouseWheel(int button, int state, int x, int y)
    {
        if (button == 0 && state == GLUT_UP) // 往上滚滚轮，放大
        {
            times2 = 1 + 0.0008f;
            glutPostRedisplay();
        }
        else {  // 往下滚滚轮，缩小
            times2 = 1 - 0.0008f;
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
        glutCreateWindow("Bezier曲面线框模型");
        init();
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        //glutIdleFunc(display);//设置空闲时调用的函数
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

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
            line_process(line);//�����׺���β�Ķ���ո�, tabȥ������ע������Ҳȥ��
            if (line.empty()) continue;//lineΪ�������
            //����ʵ��������
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
        glMap2f(GL_MAP2_VERTEX_3, 0, 100, 3, 4, 0, 100, 12, 4, &sur[0][0][0]);  // ��ȡ�ļ��еĿ��Ƶ����꣬shape = (4, 4, 3)
        glEnable(GL_MAP2_VERTEX_3);
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
        glScalef(times2, times2, times2);//����
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glColor3f(1.0, 0.0, 0.0);
        glPushMatrix();
        ControlPoint();
        glColor3f(0.0, 1.0, 0.0);
        glMapGrid2f(20, 0.0, 100.0, 20, 0.0, 100.0);
        glEvalMesh2(GL_LINE, 0, 20, 0, 20);
        //����2D�������꣬�Դӿ��Ƶ������ֵȷ�����������Ӧ�ĵ㼯����Ӧ������,Ҳ����������ķ�ʽ:
        //int i, j;
        //for (j = 0; j <= 20; j++) {
        //    glBegin(GL_LINE_STRIP);
        //    for (i = 0; i <= 30; i++)
        //        glEvalCoord2f((GLfloat)i / 30.0, (GLfloat)j / 20.0);  //�̶�y����ʱx�������������
        //    glEnd();
        //    glBegin(GL_LINE_STRIP);
        //    for (i = 0; i <= 30; i++)
        //        glEvalCoord2f((GLfloat)j / 20.0, (GLfloat)i / 30.0);  //�̶�x����ʱy�������������     
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
        case 27:   //ESC��
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
        if (button == 0 && state == GLUT_UP) // ���Ϲ����֣��Ŵ�
        {
            times2 = 1 + 0.0008f;
            glutPostRedisplay();
        }
        else {  // ���¹����֣���С
            times2 = 1 - 0.0008f;
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
        glutCreateWindow("Bezier�����߿�ģ��");
        init();
        glutDisplayFunc(display);
        glutReshapeFunc(reshape);
        glutKeyboardFunc(keyboard);
        //glutIdleFunc(display);//���ÿ���ʱ���õĺ���
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

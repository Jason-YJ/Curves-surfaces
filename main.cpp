#include <GL/freeglut.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <gl/glui.h>
#include "bezier_curve_mouse.h"
#include "bezier_curve.h"
#include "bezier_surface.h"
#include "B_surface.h"
#include "nurbs_curve.h"
#include "B_and_Bezier_curve.h"
#include "B_and_Bezier_surface.h"
using namespace std;

int main() {
	Bezier_curve bezier_curve("./data/Bezier_curve.txt");
	Bezier_surface bezier_surface("./data/Bezier_surface.txt");
	Bezier_curve_mouse bezier_curve_mouse;
	Nurbs_curve nurbs_curve("./data/Nurbs_curve_cp.txt", "./data/Nurbs_curve_knots.txt");
	B_surface b_surface("./data/B_surface.txt", "./data/B_surface_knots.txt");
	Bezier_and_B_curve bezier_B_curve("./data/Bezier_curve.txt", "./data/B_surface.txt", "./data/B_surface_knots.txt");
	Bezier_and_B_surface bezier_B_suface("./data/Bezier_surface.txt", "./data/B_surface.txt", "./data/B_surface_knots.txt");
	int choice, way;
	printf("请输入要绘画的曲线/曲面的序号（1-6）：\n");
	printf("1、Bazier曲线\t2、Bazier曲面\t3、B样条曲面\t4、NURBS曲线\t5、Bazier曲线和B样条曲面相交\t6、Bazier曲面和B样条曲面相交\n");
	cin >> choice;
	if (choice == 1) {
		printf("是否选择手动选择控制点（输入1选择手动, 输入0选择文件导入）：\n");
		cin >> way;
	}
	switch (choice) {
	case(1):
		if (way) bezier_curve_mouse.render();
		else bezier_curve.render();
		break;
	case(2):
		bezier_surface.render();
		break;
	case(3):
		b_surface.render();
		break;
	case(4):
		nurbs_curve.render();
		break;
	case(5):
		bezier_B_curve.render();
		break;
	case(6):
		bezier_B_suface.render();
		break;
	default:
		printf("输入不合法，程序结束！");
		break;
	}
	return 0;
}
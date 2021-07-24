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
	printf("������Ҫ�滭������/�������ţ�1-6����\n");
	printf("1��Bazier����\t2��Bazier����\t3��B��������\t4��NURBS����\t5��Bazier���ߺ�B���������ཻ\t6��Bazier�����B���������ཻ\n");
	cin >> choice;
	if (choice == 1) {
		printf("�Ƿ�ѡ���ֶ�ѡ����Ƶ㣨����1ѡ���ֶ�, ����0ѡ���ļ����룩��\n");
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
		printf("���벻�Ϸ������������");
		break;
	}
	return 0;
}
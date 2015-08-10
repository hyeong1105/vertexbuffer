
// glew 선언후 freeglut 선언할것
#include <gl/freeglut.h>

#pragma comment (lib, "freeglut.lib")

#include <glm\glm.hpp>

#include <iostream>
#include <fstream>

#include <vector>

// 에이전트 프로그램 생성

// 프로그램 이름
char *a_ProgramName = "VertexBuffer";

glm::vec2 WindowsSize(1024, 1024);
glm::vec2 GridSize(96, 96);
glm::vec2 c_Length(24, 24);

//테스트 코드
float *test_code;

std::vector<glm::vec3> AxisPos;
std::vector<glm::vec3> AxisColor;
std::vector<glm::vec2> l_Data;

// 데이터 로드 값 저장
double balanceData[4000][2];


float axis[3 * 6];

void loadData(void)
{
	std::ifstream in;
	int c = 0;
	int d = 0;
	int Xcount = 0;
	int Ycount = 0;
	bool readingX = true;
	double w = 0;
	double x = 0;
	double y = 0;
	double z = 0;
	int count = 0;
	in.open("00003.txt");

	int loadCount = 0;

	char str; // 텍스트파일의 길이 만큼의 문자형 배열을 만듬

	while (in.good())
	{	
		loadCount++;

		str = in.get();	// 텍스트 글자별로 읽어드림
		if (str == '+')
		{
			w = 1;
		}
		else if (str == '-')
		{
			w = -1;
		}
		else if (count == 4)
		{
			count = -1;
		}
		else if (str == 'C')
		{
			break;
		}
		else
		{
			if (count == 1){
				x = atoi(&str); // char형을 int형으로 형변환
			}
			else if (count == 2){
				y = atoi(&str); // char형을 int형으로 형변환
			}
			else if (count == 3)
			{
				z = atoi(&str); // char형을 int형으로 형변환
				if (d == 0)
				{
					balanceData[Xcount][d] = (x * 100 + y * 10 + z)*w;
					Xcount++;
					d++;
				}
				else if (d == 1)
				{
					balanceData[Ycount][d] = (x * 100 + y * 10 + z)*w;
					Ycount++;
					d--;
				}
			}
		}
		count++;
	}


	in.close();

	// 스무스 데이터 
	for (int i = 0; i < 4000; i++)
	{

		int p = i - 1;
		int n = i + 1;
		double midX, midY;
		if (p >= 0 && n < 4000)
		{
			midX = (balanceData[p][0] + balanceData[n][0]) / 2.0;
			midY = (balanceData[p][1] + balanceData[n][1]) / 2.0;

			float x = (balanceData[i][0] + midX) * 0.5;
			float y = (balanceData[i][1] + midY) * 0.5;

			l_Data.push_back(glm::vec2(x * 0.05, y* 0.05));

		}

	}
	
	l_Data.size();
}



void LightSet(void) {

	GLfloat mat_specular[] = { 1.0, 1.0f, 1.0f, 1.0f };
	GLfloat mat_diffuse[] = { 1.0, 1.0f, 1.0f, 1.0f };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0f, 1.0f, 0.0f };

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
}


void init(void) {
	
	AxisPos.push_back(glm::vec3(0, 0, 0));
	AxisPos.push_back(glm::vec3(1, 0, 0));
	AxisPos.push_back(glm::vec3(0, 0, 0));
	AxisPos.push_back(glm::vec3(0, 1, 0));
	AxisPos.push_back(glm::vec3(0, 0, 0));
	AxisPos.push_back(glm::vec3(0, 0, 1));

	AxisColor.push_back(glm::vec3(1, 0, 0));
	AxisColor.push_back(glm::vec3(1, 0, 0));

	AxisColor.push_back(glm::vec3(0, 1, 0));
	AxisColor.push_back(glm::vec3(0, 1, 0));

	AxisColor.push_back(glm::vec3(0, 0, 1));
	AxisColor.push_back(glm::vec3(0, 0, 1));


	axis[0] = 0.0f;
	axis[1] = 0.0f;
	axis[2] = 0.0f;

	axis[3] = 1.0f;
	axis[4] = 0.0f;
	axis[5] = 0.0f;

	axis[6] = 0.0f;
	axis[7] = 0.0f;
	axis[8] = 0.0f;

	axis[9]  = 0.0f;
	axis[10] = 1.0f;
	axis[11] = 0.0f;

	axis[12] = 0.0f;
	axis[13] = 0.0f;
	axis[14] = 0.0f;

	axis[15] = 0.0f;
	axis[16] = 0.0f;
	axis[17] = 1.0f;


	glEnable(GL_DEPTH_TEST);

	glClearColor(0.8, 0.9, 0.9, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 1, 1, 1000);

	glEnable(GL_POINT_SMOOTH);

	loadData();

}

void reshape(int w, int h) {
	WindowsSize.x = w;
	WindowsSize.y = h;

	float asp = float(w) / float(h);
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, asp, 1, 1000);
}


void DrawAxis(void) {

	glDisable(GL_LIGHTING);

	glLineWidth(3.0f);

	glBegin(GL_LINES);
	glColor3f(1, 0, 0);
	glVertex3f(0, 0, 0);
	glVertex3f(1, 0, 0);
	glColor3f(0, 1, 0);
	glVertex3f(0, 1, 0);
	glVertex3f(0, 0, 0);
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, 0);
	glVertex3f(0, 0, 1);
	glEnd();

	glEnable(GL_LIGHTING);
}

void Agent_Draw(void)
{


}



void display(void) {

	glClearColor(1.0, 1.0, 1.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, WindowsSize.x, WindowsSize.y);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(60, (float)WindowsSize.x / WindowsSize.y, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

	glColor3f(1, 0, 0);

	//for (int i = 0; i < l_Data.size(); i++)
	//	l_Data[i] *= 0.05;

	glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	glVertexPointer(2, GL_FLOAT, 0, &l_Data.front());

	//glVertexPointer(3, GL_FLOAT, 0, &AxisPos.front());
	//glColorPointer(3, GL_FLOAT, 0, &AxisColor.front());

	//glVertexPointer(3, GL_FLOAT, 0, &axis);

	glDrawArrays(GL_LINE_STRIP, 0, l_Data.size());

	//glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);

	glutSwapBuffers();
}

void processNormalKeys(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 27:
		exit(0); break;
	}

}

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(WindowsSize.x * 1, WindowsSize.y * 1);
	glutInitWindowPosition(0, 0);
	glutCreateWindow(a_ProgramName);

	init(); // 초기화

	// 콜백 함수 등록 
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(processNormalKeys);

	glutMainLoop(); // 무한루프
	return 0;
}


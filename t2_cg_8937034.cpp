#include <GL/glut.h>
#include <cmath>

//Variaveis de movimento
GLfloat tiroNavex = 0.0f;
GLfloat tiroNavey = 0.0f;
GLfloat posNave = 0.0f;

// Funcao callback chamada quando o tamanho da janela alterado
void AlteraTamanhoJanela(GLsizei w, GLsizei h)
{
	GLsizei largura, altura;

	// Evita a divisao por zero
	if(h == 0) h = 1;

	// Atualiza as vari�veis
	largura = w;
	altura = h;

	// Especifica as dimens�es da Viewport
	glViewport(0, 0, largura, altura);

	// Inicializa o sistema de coordenadas
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Estabelece a janela de sele��o (esquerda, direita, inferior,
	// superior) mantendo a propor��o com a janela de visualiza��o
	if (largura <= altura)
	{
		gluOrtho2D (-1.0f, 1.0f, -1.0f*altura/largura, 1.0f*altura/largura);
		//win = 1.0f;
	}
	else
	{
		gluOrtho2D (-1.0f*largura/altura, 1.0f*largura/altura, -1.0f, 1.0f);
	//	win = 1.0f*largura/altura;
	}
}

void init() {
	//Define a cor preta para o fundo
  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	gluOrtho2D(-1.0f,1.0f,-1.0f,1.0f);
	glViewport(0,0,800,600);
}

void desenhaCirculo(GLfloat xcentro, GLfloat ycentro) {
	GLfloat total = 2.0f*3.1415f, N = 100.0f, theta = 0.0f, var = total/N;
	GLfloat raio = 15.0f;
	GLfloat a = xcentro;
	GLfloat b = ycentro;

	for (theta = 0.0f; theta <= total; theta = theta + var) {
		glBegin(GL_TRIANGLES);
			glVertex2f(a, b);
			glVertex2f(a+raio*cos(theta), b+raio*sin(theta));
			glVertex2f(a+raio*cos(theta+var), b+raio*sin(theta+var));
		glEnd();
	}

}

void desenhaAlien1() {

	// Especifica a cor verde
	glColor3f(0.0f, 1.0f, 0.0f);

	GLfloat ya1fix = 585.0f;
	GLfloat yb1fix = 545.0f;
	GLfloat a1, b1, c1;

	for (b1 = 85.0f; b1 < 800.0f; b1=b1+75.0f) {
		a1 = b1 - 60.0f;
		c1 = b1 - 30.0f;
		glBegin(GL_TRIANGLES);
			glVertex2f(a1, ya1fix);
			glVertex2f(b1, ya1fix);
			glVertex2f(c1, yb1fix);
		glEnd();
	}

}

void desenhaAlien2() {

	glColor3f(1.0f, 1.0f, 0.0f);

	GLfloat ya2fix = 530.0f;
	GLfloat yb2fix = 500.0f;
	GLfloat a2, b2;

	for(b2 = 115.0f; b2 < 800.0f; b2 = b2+75.0f) {
		a2 = b2 - 40;
		glBegin(GL_QUADS);
			glVertex2f(a2, ya2fix);
			glVertex2f(b2, ya2fix);
			glVertex2f(a2, yb2fix);
			glVertex2f(b2, yb2fix);
		glEnd();
	}
}

void desenhaAlien3() {

	glColor3f(0.0f, 0.0f, 1.0f);

	GLfloat centro;
	GLfloat yfix = 470.0f;

	for(centro = 45.0f; centro < 800.0; centro = centro + 45.0f) {
		desenhaCirculo(centro, yfix);
	}

}

void desenhaAliens(){
	desenhaAlien1();
	desenhaAlien2();
	desenhaAlien3();
	glFlush();
}

void desenhaNave(){

	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_QUADS);
		glVertex2f(-0.5f,-0.5f);
		glVertex2f(0.5f, -0.5f);
		glVertex2f(0.5f,0.5f);
		glVertex2f(-0.5f,0.5f);
	glEnd();
}

void Desenha() {

	// Muda para o sistema de coordenadas do modelo
	glMatrixMode(GL_MODELVIEW);
	// Inicializa a matriz de transforma��o corrente
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);
	//desenhaAliens();
	glTranslatef(posNave,0.0f,0.0f);
	glTranslatef(0.0f, -0.8f, 0.0f);
	glScalef(0.15f, 0.15f, 0.0f);

	desenhaNave();
	glFlush();

}

void MovimentosNave(int key, int x, int y) {
	if(key == GLUT_KEY_LEFT) {
		posNave-=0.05f;
		if(posNave<-1.5f)
			posNave = -1.5f;
	}
	if(key == GLUT_KEY_RIGHT) {
		posNave+=0.05f;
		if(posNave>1.5f)
			posNave = 1.5f;
	}
	glutPostRedisplay();
}

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitWindowSize(800,600);
	glutInitWindowPosition(250,75);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Aliens");

	glutDisplayFunc(Desenha);
	glutSpecialFunc(MovimentosNave);

	init();

	glutMainLoop();

	return 0;
}

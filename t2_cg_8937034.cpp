#include <GL/glut.h>
#include <cmath>

#define MAX_ALIENS_H 5
#define MAX_ALIENS_V 5

//Variaveis de movimento
int nTiros = 0, countTiros = 0;
GLfloat tiroNavex[] = {0.0f, 0.0f, 0.0f};
GLfloat tiroNavey[] = {0.0f, 0.0f, 0.0f};
GLfloat posNave = 0.0f;

// Matriz aliens
GLfloat aliens[5][5][2];

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

	// inicializa matriz de aliens
	for (GLint i = 0; i < MAX_ALIENS_V; i++) {
		for(GLint j = 0; j < MAX_ALIENS_H; j++) {
			// valor x
			aliens[i][j][0] = 0.4f - (0.2f * i);
			// valor y
			aliens[i][j][1] = 0.9f - (j * 0.2f);
		}
	}
}

void desenhaAlien1() {

	// Especifica a cor verde
	glColor3f(0.0f, 1.0f, 0.0f);

	glBegin(GL_TRIANGLES);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
		glVertex2f(0.0f, -0.5f);
	glEnd();

}

void desenhaAlien2() {

	glColor3f(1.0f, 1.0f, 0.0f);

	glBegin(GL_QUADS);
		glVertex2f(-0.5f, -0.5f);
		glVertex2f(0.5f, -0.5);
		glVertex2f(0.5f, 0.5f);
		glVertex2f(-0.5f, 0.5f);
	glEnd();

}

void desenhaAlien3() {

	glColor3f(0.0f, 0.0f, 1.0f);

	GLfloat total = 2.0f*3.1415f, N = 100.0f, theta = 0.0f, var = total/N;
	GLfloat raio = 0.5f;
	GLfloat a = 0.0f;
	GLfloat b = 0.0f;

	for (theta = 0.0f; theta <= total; theta = theta + var) {
		glBegin(GL_TRIANGLES);
			glVertex2f(a, b);
			glVertex2f(a+raio*cos(theta), b+raio*sin(theta));
			glVertex2f(a+raio*cos(theta+var), b+raio*sin(theta+var));
		glEnd();
	}

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

void desenhaTiro() {

	glColor3f(1.0f, 0.0f, 0.0f);

	glBegin(GL_LINES);
		glVertex2f(0.0f, -0.2f);
		glVertex2f(0.0f, 0.2f);
	glEnd();
}

void moveTiro(int passo) {
	int i;
	for(i = 0; i < nTiros; i++) {
		tiroNavey[i] += (1.0*passo)/100;
	}
	glutPostRedisplay();
	if(passo == countTiros)
		glutTimerFunc(10, moveTiro, passo);

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

	// Desenha aliens
	for (GLint i = 0; i < MAX_ALIENS_V; i++) {
		for (GLint j = 0; j < MAX_ALIENS_H; j++) {
			glLoadIdentity();
			glTranslatef(aliens[i][j][0], aliens[i][j][1], 0.0f);
			glScalef(0.1f, 0.1f, 0.0f);
			if (j < 2)
				desenhaAlien1();
			else if (j < 4)
				desenhaAlien2();
			else
				desenhaAlien3();
		}
	}

	//Aliens 1
	/*int i;
	for(i = 1; i < 8; i++) {
		GLfloat a = 0.8 - 0.2*i;
		glLoadIdentity();
		glTranslatef(a,0.9f,0.0f);
		glScalef(0.1f,0.1f,0.0f);
		desenhaAlien1();
	}

	//Aliens 2
	for(i = 1; i < 8; i++) {
		GLfloat a = 0.8 - 0.2*i;
		glLoadIdentity();
		glTranslatef(a,0.7f,0.0f);
		glScalef(0.1f,0.1f,0.0f);
		desenhaAlien2();
	}

	//Aliens 3
	for(i = 1; i < 8; i++) {
		GLfloat a = 0.8 - 0.2*i;
		glLoadIdentity();
		glTranslatef(a,0.5f,0.0f);
		glScalef(0.1f,0.1f,0.0f);
		desenhaAlien3();
	}*/

	for(i = 0; i < nTiros; i++) {
		glLoadIdentity();
		glTranslatef(tiroNavex[i], tiroNavey[i], 0.0f);
		glScalef(0.1f, 0.1f, 0.0f);
		desenhaTiro();
	}

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
	if(key == GLUT_KEY_UP) {
		if(nTiros < 3){
			nTiros++;
			tiroNavex[nTiros - 1] = posNave;
			tiroNavey[nTiros - 1] = -0.8f;
			glutTimerFunc(10, moveTiro, ++countTiros);
		}
	}

	glutPostRedisplay();
}


int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitWindowSize(600,600);
	glutInitWindowPosition(250,75);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutCreateWindow("Aliens");

	glutDisplayFunc(Desenha);
	glutSpecialFunc(MovimentosNave);

	init();

	glutMainLoop();

	return 0;
}

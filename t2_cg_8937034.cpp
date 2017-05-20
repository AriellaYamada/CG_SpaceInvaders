#include <GL/glut.h>
#include <cstdlib>
#include <ctime>

// declara as constantes
#define MAX_ALIENS_H 5			// numero de aliens na horizontal
#define MAX_ALIENS_V 5			// numero de aliens na vertical
#define MAX_TIROS_NAVE 3		// numero maximo de tiros do jogador
#define MAX_TIROS_ALIENS 5		// numero maximo de tiros dos aliens
#define MOVIMENTO_TIRO 0.05f	// distancia que o tiro se movimenta a cada passo
#define MOVIMENTO_NAVE 0.05f	// distancia que a nave so movimenta a cada passo
#define MOVIMENTO_ALIENS 0.05f	// distancia que os aliens se movem a capa passo
#define SHOT_SPEED 33			// velocidade dos tiros
#define ALIEN_SHOT_SPEED 1000	// velocidade em que cada tiro eh disparado
#define ALIEN_SPEED 2000		// velocidade dos aliens
#define Y_NAVE -0.75f			// posicao da nave na vertical
#define ALIEN_RADIUS 0.08f		// distacia maxima do alien em relação ao seu centro
#define NAVE_RADIUS 0.1f		// distancia maxima da nave em relação ao seu centro

// declara as variaveis globais
GLfloat posNave = 0.0f;								// posicao da nave na horizontal
GLfloat posAliens[MAX_ALIENS_V][MAX_ALIENS_H][2];	// matriz de posicao (X,Y) dos aliens
GLfloat posTirosNave[MAX_TIROS_NAVE][2];			// vetor de posicao (X,Y) dos tiros do jogador
GLfloat posTirosAliens[MAX_TIROS_ALIENS][2];		// vetor de posicao (X,Y) dos tiros dos aliens
GLfloat alienOffsetX = 0;
GLfloat alienOffsetY = 0;
int countTirosNave = 0;								// contador de tiros da nave
int countTirosAliens = 0;							// contador de tiros dos aliens
int countVidas = 3;									// numero de vidas do jogador
int countAliens = MAX_ALIENS_H * MAX_ALIENS_V;		// numero de aliens vivos
bool aliensAtivos[MAX_ALIENS_V][MAX_ALIENS_H];		// controla quais aliens ainda estão vivos
bool tirosAtivosNave[MAX_TIROS_NAVE];				// controla quais tiros da nave estão ativos
bool tirosAtivosAliens[MAX_TIROS_ALIENS];			// controla quais tiros dos aliens estão ativos
bool running = true;


//////////////////////////////////////////////////////////////////////////////////////////////////
//									funcoes de controle											//
//////////////////////////////////////////////////////////////////////////////////////////////////

// funcao que verifica se o tiro acertou algum alien
bool VerificaTiroNave(int tiro_idx) {
	for (int i = 0; i < MAX_ALIENS_V; i++) {
		for (int j = 0; j < MAX_ALIENS_H; j++) {
			if (aliensAtivos[i][j] 
				&& posTirosNave[tiro_idx][0] >= (posAliens[i][j][0] - ALIEN_RADIUS)
				&& posTirosNave[tiro_idx][0] <= (posAliens[i][j][0] + ALIEN_RADIUS)
				&& posTirosNave[tiro_idx][1] >= (posAliens[i][j][1] - ALIEN_RADIUS)
				&& posTirosNave[tiro_idx][1] <= (posAliens[i][j][1] + ALIEN_RADIUS)) {
				// mata o alien atingido pelo tiro
				aliensAtivos[i][j] = false;
				countAliens--;
				return true;
			}
		}
	}
	return false;
}

// funcao que move um tiro até atingir um alvo ou sair da tela
void MoveTiroNave(int tiro_idx) {
	// move o tiro
	posTirosNave[tiro_idx][1] = posTirosNave[tiro_idx][1] + MOVIMENTO_TIRO;

	// verifica se o tiro matou algum alien
	bool acertouTiro = VerificaTiroNave(tiro_idx);

	glutPostRedisplay();

	// se acertou o tiro ou saiu da tela, apaga o tiro
	if (acertouTiro || posTirosNave[tiro_idx][1] > 1) {
		tirosAtivosNave[tiro_idx] = false;
		countTirosNave--;
	}
	// se o tiro não saiu da tela e não acertou nenhum alien chama a funcao de movimentacao outra vez
	else if (running) {
		glutTimerFunc(SHOT_SPEED, MoveTiroNave, tiro_idx);
	}
}

// funcao que incia um novo tiro da nave
void AtiraNave(GLfloat x, GLfloat y) {
	if (countTirosNave < MAX_TIROS_NAVE) {
		countTirosNave++;
		// coloca um novo tiro na posição livre
		for (int i = 0; i < MAX_TIROS_NAVE; i++) {
			if (!tirosAtivosNave[i] && running) {
				tirosAtivosNave[i] = true;
				posTirosNave[i][0] = x;
				posTirosNave[i][1] = y;
				// chama a funcao que vai mover o tiro
				glutTimerFunc(SHOT_SPEED, MoveTiroNave, i);
				return;
			}
		}
	}
}

// funcao que verifica se o tiro acertou a nave do jogador
bool VerificaTiroAlien(int tiro_idx) {
	if (posTirosAliens[tiro_idx][0] >= (posNave - NAVE_RADIUS)
		&& posTirosAliens[tiro_idx][0] <= (posNave + NAVE_RADIUS)
		&& posTirosAliens[tiro_idx][1] >= (Y_NAVE - NAVE_RADIUS)
		&& posTirosAliens[tiro_idx][1] <= (Y_NAVE + NAVE_RADIUS)) {
		// jogador perde uma vida
		countVidas--;
		return true;
	}
		
	return false;
}


// funcao que move um tiro até atingir um alvo ou sair da tela
void MoveTiroAlien(int tiro_idx) {
	// move o tiro
	posTirosAliens[tiro_idx][1] -= MOVIMENTO_TIRO;

	// verifica se o tiro matou algum alien
	bool acertouTiro = VerificaTiroAlien(tiro_idx);

	glutPostRedisplay();

	// se acertou o tiro ou saiu da tela, apaga o tiro
	if (acertouTiro || posTirosAliens[tiro_idx][1] < -1) {
		tirosAtivosAliens[tiro_idx] = false;
		countTirosAliens--;
	}
	// se o tiro não saiu da tela e não acertou nave chama a funcao de movimentacao outra vez
	else if (running) {
		glutTimerFunc(SHOT_SPEED, MoveTiroAlien, tiro_idx);
	}
}

// funcao que faz com que os aliens ataquem aleatoriamente
void AtiraAlien(int count) {
	// escolhe um alien aleatorio
	srand(time(NULL));
	int i = rand() % MAX_ALIENS_V;
	int j = rand() % MAX_ALIENS_H;

	// se o alien estiver vivo atira
	if (aliensAtivos[i][j] && countTirosAliens < MAX_TIROS_ALIENS) {
		countTirosAliens++;
		// coloca um novo tiro na posição livre
		for (int k = 0; k < MAX_TIROS_ALIENS; k++) {
			if (!tirosAtivosAliens[k] && running) {
				tirosAtivosAliens[k] = true;
				posTirosAliens[k][0] = posAliens[i][j][0];
				posTirosAliens[k][1] = posAliens[i][j][1];
				// chama a funcao que vai mover o tiro
				glutTimerFunc(SHOT_SPEED, MoveTiroAlien, k);
				break;
			}
		}
	}

	// chama a funcao pra dar mais um tiro
	if (running) {
		glutTimerFunc(ALIEN_SHOT_SPEED, AtiraAlien, 0);
	}
}

// movimenta os aliens
void MoveAliens(int direcao) {
	alienOffsetX += (direcao * MOVIMENTO_ALIENS);
	alienOffsetY -= MOVIMENTO_ALIENS * 0.2;

	glutPostRedisplay();

	// se os aliens chegaram no limite esquerdo
	if (direcao == -1 && posAliens[0][0][0] <= -0.8 && running) {
		// move para a direita
		glutTimerFunc(ALIEN_SPEED, MoveAliens, 1);
	}
	// se os aliens chegaram no limite direito
	else if (direcao == 1 && posAliens[0][MAX_ALIENS_H -1][0] >= 0.8 && running) {
		// move para a esquerda
		glutTimerFunc(ALIEN_SPEED, MoveAliens, -1);
	}
	else if (running){
		glutTimerFunc(ALIEN_SPEED, MoveAliens, direcao);
	}
}

// verifica se os aliens chegaram na base
bool AliensChegaram() {
	for (int i = 0; i < MAX_ALIENS_V; i++) {
		for (int j = 0; j < MAX_ALIENS_H; j++) {
			if (aliensAtivos[i][j]) {
				if (posAliens[i][j][1] - ALIEN_RADIUS <= Y_NAVE + NAVE_RADIUS) {
					return true;
				}
				else {
					return false;
				}
			}
		}
	}
}

// funcao que trata os comandos do teclado
void MoveKeys(int key, int x, int y) {
	if (key == GLUT_KEY_LEFT) {
		posNave -= MOVIMENTO_NAVE;
		if (posNave < -1.0f)
			posNave = -1.0f;
	}
	else if (key == GLUT_KEY_RIGHT) {
		posNave += MOVIMENTO_NAVE;
		if (posNave > 1.0f)
			posNave = 1.0f;
	}
	else if (key == GLUT_KEY_UP) {
		AtiraNave(posNave, Y_NAVE);
	}

	glutPostRedisplay();
}

// funcao que inicia o jogo novamente
void Restart() {
	// inicializa as variaveis globais
	posNave = 0.0f;
	alienOffsetX = 0;
	alienOffsetY = 0;
	countTirosNave = 0;
	countTirosAliens = 0;
	countVidas = 3;
	running = true;
	countAliens = MAX_ALIENS_H * MAX_ALIENS_V;

	// inicializa as matrizes relativas aos aliens
	for (int i = 0; i < MAX_ALIENS_V; i++) {
		for (int j = 0; j < MAX_ALIENS_H; j++) {
			posAliens[i][j][0] = -0.4f + (j * 0.2f);	// pos x
			posAliens[i][j][1] = 0.1f + (i * 0.2f);		// pos y
			aliensAtivos[i][j] = true;
		}
	}

	// inicializa os vetores tiros ativos dos aliens
	for (int i = 0; i < MAX_TIROS_ALIENS; i++) {
		tirosAtivosAliens[i] = false;
	}

	// inicializa os vetores tiros ativos da nave
	for (int i = 0; i < MAX_TIROS_NAVE; i++) {
		tirosAtivosNave[i] = false;
	}
	// inicializa os timers
	glutTimerFunc(ALIEN_SPEED, MoveAliens, 1);
	glutTimerFunc(ALIEN_SHOT_SPEED, AtiraAlien, 0);

	// atualiza a tela
	glutPostRedisplay();
}

void RestartKey(unsigned char key, int x, int y) {
	if (key == ' ')
		Restart();
}

///////////////////////////////////////////////////////////////////////////////////////////
//								funcoes de desenho										 //
///////////////////////////////////////////////////////////////////////////////////////////

// funcao que desenha a nave
void DesenhaNave() {
	glTranslatef(posNave, Y_NAVE, 0.0f);
	glScalef(NAVE_RADIUS, NAVE_RADIUS, 0.0f);

	glColor3f(1.0f, 1.0f, 1.0f); // braco

	// asas
	glBegin(GL_POLYGON);
	glVertex2f(-1.0f, -0.2f);
	glVertex2f(-1.0f, -0.3f);
	glVertex2f(-0.3f, -0.6f);
	glVertex2f(0.3f, -0.6f);
	glVertex2f(1.0f, -0.3f);
	glVertex2f(1.0f, -0.2f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f); // preto
	glBegin(GL_LINE_LOOP);
	glVertex2f(-1.0f, -0.2f);
	glVertex2f(-1.0f, -0.3f);
	glVertex2f(-0.3f, -0.6f);
	glVertex2f(0.3f, -0.6f);
	glVertex2f(1.0f, -0.3f);
	glVertex2f(1.0f, -0.2f);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f); // braco

	// quadrado central
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, 0.0f);
	glVertex2f(0.3f, 0.0f);
	glVertex2f(0.3f, -0.6f);
	glVertex2f(-0.3f, -0.6f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f); // preto
	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.3f, 0.0f);
	glVertex2f(0.3f, 0.0f);
	glVertex2f(0.3f, -0.6f);
	glVertex2f(-0.3f, -0.6f);
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f); // braco

	// cabine do piloto
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, 0.0f);
	glVertex2f(0.3f, 0.0f);
	glVertex2f(0.1f, 1.0f);
	glVertex2f(-0.1f, 1.0f);
	glEnd();
	
	// haste esquerda
	glBegin(GL_QUADS);
	glVertex2f(-0.95f, -0.2f);
	glVertex2f(-0.95f, 0.5f);
	glVertex2f(-0.85f, 0.5f);
	glVertex2f(-0.85f, -0.2f);
	glEnd();

	// haste direita
	glBegin(GL_QUADS);
	glVertex2f(0.95f, -0.2f);
	glVertex2f(0.95f, 0.5f);
	glVertex2f(0.85f, 0.5f);
	glVertex2f(0.85f, -0.2f);
	glEnd();

	glColor3f(0.48f, 0.6f, 0.65f); // cinza

	// turbuna esquerda
	glBegin(GL_QUADS);
	glVertex2f(-1.0f, -0.2f);
	glVertex2f(-1.0f, -0.5f);
	glVertex2f(-0.8f, -0.5f);
	glVertex2f(-0.8f, -0.2f);
	glEnd();

	// turbina direita
	glBegin(GL_QUADS);
	glVertex2f(1.0f, -0.2f);
	glVertex2f(1.0f, -0.5f);
	glVertex2f(0.8f, -0.5f);
	glVertex2f(0.8f, -0.2f);
	glEnd();

	// turbina esquerda
	glBegin(GL_QUADS);
	glVertex2f(-0.3f, -0.3f);
	glVertex2f(-0.1f, -0.3f);
	glVertex2f(-0.1f, -0.8f);
	glVertex2f(-0.3f, -0.8f);
	glEnd();

	// turbina direita
	glBegin(GL_QUADS);
	glVertex2f(0.3f, -0.3f);
	glVertex2f(0.1f, -0.3f);
	glVertex2f(0.1f, -0.8f);
	glVertex2f(0.3f, -0.8f);
	glEnd();

	glColor3f(0.0f, 0.0f, 0.0f); // preto

	// vidro da cabine
	glBegin(GL_QUADS);
	glVertex2f(-0.2f, 0.0f);
	glVertex2f(0.2f, 0.0f);
	glVertex2f(0.1f, 0.4f);
	glVertex2f(-0.1f, 0.4f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.3f, 0.0f);
	glVertex2f(0.3f, 0.0f);
	glVertex2f(0.1f, 1.0f);
	glVertex2f(-0.1f, 1.0f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(-1.0f, -0.2f);
	glVertex2f(-1.0f, -0.5f);
	glVertex2f(-0.8f, -0.5f);
	glVertex2f(-0.8f, -0.2f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(1.0f, -0.2f);
	glVertex2f(1.0f, -0.5f);
	glVertex2f(0.8f, -0.5f);
	glVertex2f(0.8f, -0.2f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.95f, -0.2f);
	glVertex2f(-0.95f, 0.5f);
	glVertex2f(-0.85f, 0.5f);
	glVertex2f(-0.85f, -0.2f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(0.95f, -0.2f);
	glVertex2f(0.95f, 0.5f);
	glVertex2f(0.85f, 0.5f);
	glVertex2f(0.85f, -0.2f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(-0.3f, -0.3f);
	glVertex2f(-0.1f, -0.3f);
	glVertex2f(-0.1f, -0.8f);
	glVertex2f(-0.3f, -0.8f);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2f(0.3f, -0.3f);
	glVertex2f(0.1f, -0.3f);
	glVertex2f(0.1f, -0.8f);
	glVertex2f(0.3f, -0.8f);
	glEnd();
}

void DesenhaTiroNave(int i) {
	glColor3f(0.12f, 0.72f, 0.98f); // azul

	glTranslatef(posTirosNave[i][0], posTirosNave[i][1], 0.0f);
	glScalef(0.15f, 0.15f, 0.0f);

	glBegin(GL_QUADS);
	glVertex2f(-0.1f, -0.2f);
	glVertex2f(0.1f, -0.2f);
	glVertex2f(0.1f, 0.2f);
	glVertex2f(-0.1f, 0.2f);
	glEnd();
}

void DesenhaTirosNave() {
	for (int i = 0; i < MAX_TIROS_NAVE; i++) {
		if (tirosAtivosNave[i]) {
			glLoadIdentity();
			DesenhaTiroNave(i);
		}
	}
}

void DesenhaAlien1(int i, int j) {
	glColor3f(0.34f, 0.8f, 0.56f); // verde

	glTranslatef(posAliens[i][j][0], posAliens[i][j][1], 0.0f);
	glScalef(ALIEN_RADIUS, ALIEN_RADIUS, 0.0f);

	glBegin(GL_QUADS);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(1.0f, -1.0f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glEnd();
}

void DesenhaAlien2(int i, int j) {
	glColor3f(0.98f, 0.75f, 0.36f); // amarelo

	glTranslatef(posAliens[i][j][0], posAliens[i][j][1], 0.0f);
	glScalef(ALIEN_RADIUS, ALIEN_RADIUS, 0.0f);

	glBegin(GL_QUADS);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(1.0f, -1.0f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glEnd();
}

void DesenhaAlien3(int i, int j) {
	glColor3f(0.75f, 0.6f, 0.71f); // roxo

	glTranslatef(posAliens[i][j][0], posAliens[i][j][1], 0.0f);
	glScalef(ALIEN_RADIUS, ALIEN_RADIUS, 0.0f);

	glBegin(GL_QUADS);
	glVertex2f(-1.0f, -1.0f);
	glVertex2f(1.0f, -1.0f);
	glVertex2f(1.0f, 1.0f);
	glVertex2f(-1.0f, 1.0f);
	glEnd();
}

void DesenhaAliens() {
	for (int i = 0; i < MAX_ALIENS_V; i++) {
		for (int j = 0; j < MAX_ALIENS_H; j++) {
			if (aliensAtivos[i][j]) {
				posAliens[i][j][0] += alienOffsetX;
				posAliens[i][j][1] += alienOffsetY;

				glLoadIdentity();
				if (i < 2)
					DesenhaAlien1(i, j);
				else if (i < 4)
					DesenhaAlien2(i, j);
				else
					DesenhaAlien3(i, j);
			}
		}
	}
	alienOffsetX = 0;
	alienOffsetY = 0;
}

void DesenhaTiroAlien(int i) {
	glColor3f(0.93f, 0.38f, 0.32f); // vermelho

	glTranslatef(posTirosAliens[i][0], posTirosAliens[i][1], 0.0f);
	glScalef(0.15f, 0.15f, 0.0f);

	glBegin(GL_QUADS);
	glVertex2f(-0.1f, -0.2f);
	glVertex2f(0.1f, -0.2f);
	glVertex2f(0.1f, 0.2f);
	glVertex2f(-0.1f, 0.2f);
	glEnd();
}


void DesenhaTirosAliens() {
	for (int i = 0; i < MAX_TIROS_ALIENS; i++) {
		if (tirosAtivosAliens[i]) {
			glLoadIdentity();
			DesenhaTiroAlien(i);
		}
	}
}

void DesenhaVidas() {
	for (int i = 0; i < countVidas; i++) {
		glLoadIdentity();
		glColor3f(0.93f, 0.38f, 0.32f); // vermelho

		glTranslatef((i-1)*0.2, -0.95, 0.0f);
		glScalef(0.02, 0.02, 0.0f);

		// mudar aqui 
		glBegin(GL_QUADS);
		glVertex2f(-1.0f, -1.0f);
		glVertex2f(1.0f, -1.0f);
		glVertex2f(1.0f, 1.0f);
		glVertex2f(-1.0f, 1.0f);
		glEnd();
	}
}

void DesenhaGameOver() {
	// escrever na tela "Derrota - pressione 'espaço' para tentar novamente"
}

void DesenhaVitoria() {
	// escreve na tela "Parabéns! você venceu"
}

void Desenha() {
	glMatrixMode(GL_MODELVIEW);	// muda para o sistema de coordenadas do modelo
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT);

	// se o jogador matar todos os aliens
	if (countAliens == 0) {
		running = false;
		DesenhaVitoria();
	}
	// se o jogador perdeu todas as vidas ou as naves inimigas o alcançaram
	else if (countVidas < 0 || AliensChegaram()) {
		running = false;
		DesenhaGameOver();
	}
	// o jogo continua
	else {
		DesenhaNave();
		DesenhaTirosNave();

		DesenhaAliens();
		DesenhaTirosAliens();

		DesenhaVidas();
	}	
	
	//	carrega o desenho na tela
	glFlush();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									funcao de inicializacao												  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////

//Inicializa a tela e as variaveis
void init() {
	glClearColor(0.05f, 0.17f, 0.32f, 0.0f);// Define a cor azul para o fundo
	gluOrtho2D(-1.0f, 1.0f, -1.0f, 1.0f);	// Define o quadrado normalizado
	glViewport(0, 0, 600, 600);				// Define a view port

	// inicializa as matrizes relativas aos aliens
	for (int i = 0; i < MAX_ALIENS_V; i++) {
		for (int j = 0; j < MAX_ALIENS_H; j++) {
			posAliens[i][j][0] = -0.4f + (j * 0.2f);	// pos x
			posAliens[i][j][1] = 0.1f + (i * 0.2f);		// pos y
			aliensAtivos[i][j] = true;
		}
	}

	// inicializa os vetores tiros ativos dos aliens
	for (int i = 0; i < MAX_TIROS_ALIENS; i++) {
		tirosAtivosAliens[i] = false;
	}

	// inicializa os vetores tiros ativos da nave
	for (int i = 0; i < MAX_TIROS_NAVE; i++) {
		tirosAtivosNave[i] = false;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//									     main															  //
////////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[]) {

	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(250, 75);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutCreateWindow("Space Invaders");

	init();

	glutDisplayFunc(Desenha);
	glutSpecialFunc(MoveKeys);
	glutKeyboardFunc(RestartKey);

	// funcoes de timer
	glutTimerFunc(ALIEN_SPEED, MoveAliens, 1);
	glutTimerFunc(ALIEN_SHOT_SPEED, AtiraAlien, 0);

	glutMainLoop();

	return 0;
}
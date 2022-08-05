/*
Video for this https://www.youtube.com/watch?v=gYRrGTC7GtA

engine 1: https://www.youtube.com/watch?v=3cRI9mHe6co
https://www.youtube.com/watch?v=exQ43PFWJBU
*/

#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#define PI 3.1415926535
#define P1 PI/2
#define P3 3*PI/2

float px, py; // position of the player.
float pdx, pdy, pa; // delta x,y and angle of the player / object
int mapX=8, Mapy=8, mapS=64;

/* -- Ray Caster part -- */
void DrawRays3d()
{
	int r, mx, my, mp, dof;
	float rx, ry, ra, x0, y0;
	
	/* A- Check the Horizontal Lines */
	// 1- Set the Ray Angle to the player angle.
	
	/*
	ra = pa;
	//example 1: just cast one ray
	for(r=0; r<1; r++){
		dof = 0;
		float aTan = -1 / tan(ra); // horizontal line
		// prior to know if it the ray is gonna hit the first horizontal line.
		// we need to know if the ray is looking up or down. By the rays angle.
		if(ra>PI){ // looking up?
			ry = (((int)py>>6)<<6)-0.0001;
			// distance of the player and the ray y position times the inverse of tangent
			rx = (py-ry) * aTan + px;
			yo=-64; x0 = -y0*aTan;
		}
		if(ra<PI){ // looking up?
			ry = (((int)py>>6)<<6)+64;
			rx = (py-ry) * aTan + px;
			yo=64; x0 = -y0*aTan;
		}
		// looking straigh left or right, impossible to hit a horizontal line
		if(ra==0 || ra==PI){
			rx=px;
			ry=py;
			dof=8;
		}
		while(dof<8){
			mx=(int)(rx)>>6;
			my(int)(ry)>>6;
			mp=my*mapX+mx;
			if(mp<mapX*mapY && map[mp]==1){dof= 8;} // hit wall.
			else {rx+=x0; ry+=y0; dof+=1;}
		}
		// finally draw the ray cast based on the player position
		glColor3f(0,1,0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(px,py);
		glVertex2i(rx,ry);
		glEnd();
	}
	*/
	
	ra = pa;
	//example 1: just cast one ray
	for(r=0; r<1; r++){
		dof = 0;
		float nTan = -tan(ra); // horizontal line
		// prior to know if it the ray is gonna hit the first horizontal line.
		// we need to know if the ray is looking up or down. By the rays angle.
		if(ra>P2 && ra<P3){ // looking left
			rx = (((int)px>>6)<<6)-0.0001;
			// distance of the player and the ray y position times the inverse of tangent
			ry = (px-rx) * nTan + px;
			yo=-64;
			x0 = -y0*nTan;
		}
		if(ra<P2 || ra>P3){ // looking right?
			rx = (((int)px>>6)<<6)+64;
			ry = (px-rx) * nTan + py;
			yo=64; 
			x0 = -x0*nTan;
		}
		if(ra==0 || ra==PI){ // looking up or down
			rx=px;
			ry=py;
			dof=8;
		}
		while(dof<8){
			mx=(int)(rx)>>6;
			my(int)(ry)>>6;
			mp=my*mapX+mx;
			if(mp<mapX*mapY && map[mp]==1){dof= 8;} // hit wall.
			else {rx+=x0; ry+=y0; dof+=1;}
		}
		// finally draw the ray cast based on the player position
		glColor3f(0,1,0);
		glLineWidth(1);
		glBegin(GL_LINES);
		glVertex2i(px,py);
		glVertex2i(rx,ry);
		glEnd();
	}
	
	
}

/*************************/

// the world; 1's are walls and 0's are empty spaces.
int map[] = 
{
	1,1,1,1,1,1,1,1,
	1,0,0,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,0,0,1,
	1,0,1,0,0,1,0,1,
	1,0,1,0,0,0,0,1,
	1,0,0,0,0,0,0,1,
	1,1,1,1,1,1,1,1
};

// offset = 64 is the cube size
void DrawMap2d()
{
	int x, y, x0, y0;
	for(y=0; y<mapY; y++){
		for(x=0; x<MapX; x++){
			if(map[y*mapX+x]==1){ // Walls?
				glColor3f(1,1,1);
			}else {
				glColor3f(0,0,0);
			}
			x0 = x*maps; 
			y0 = y*mapS;
			glBegin(GL_QUADS);
			glVertex2i(x0  +1, y0+1);
			glVertex2i(x0  +1, y0+mapS-1);
			glVertex2i(x0+mapS-1, y0+mapS-1);
			glVertex2i(x0+mapS-1, y0+1);
			glEnd();
		}
	}
}

// Move the player, or the Shape.
void buttons(unsigned char key, int x, int y)
{
	if(key=='a'){	
		px -= 0.1;
		if( pa < 0 ) {
			pa += 2*PI; 
		}
		pdx = cos(pa)*5;
		pdy = sin(pa)*5;
	}
	if(key=='d'){
		px += 0.1;
		if( pa > 2*PI ){
			pa -= 2*PI; 
		}
		pdx = cos(pa)*5;
		pdy = sin(pa)*5;
	}
	if(key=='w'){
		py -= 5;
		px += pdx;
		px += pdy;
	}
	if(key=='s'){
		py += 5;
		px -= pdx;
		px -= pdy;
	}
	glutPostRedisplay();
}

void drawPlayer()
{
	glColor3f(1, 1,0);
	glPointSize(8);
	glBegin(GL_POINTS);
	glVertex2i(px, py);
	glEnd();
	
	// See the direction the player is looking at.
	glLineWidth(2);
	glBegin(GL_LINES);
	glVertex2i(px, py);
	glVertex2i(px+pdx*5, py+pdy*5);
	glEnd();
}

void init()
{
	// Set the backgorund color
	glClearColor(0.3, 0.3, 0.3, 0);
	gluOrtho2D(0, 1024, 512, 0);
	px=300; 
	py=300;
	// calcul,ate delta x and y at first execution
	pdx = cos(pa)*5;
	pdy = sin(pa)*5;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// Draw Map Before drawing the player.
	DrawMap2d();
	drawPlayer();
	glutSwapBuffers();
}

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1024, 512);
	glutCreateWindow("3d - Engine - 1");
	init();
	glutDisplayFunc(display);
	glutMainLoop();
	return 0;
}

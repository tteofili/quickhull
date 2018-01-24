#include <windows.h>
#include <gl/glut.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define M_PI 3.14159

GLsizei ww,hh;  //le dimensioni della finestra

/* variabili per la luce LIGHT0 */
GLfloat luce[] = {0.15,0.61,10.1,0};
GLfloat lucepoint[] = {1.003,0.531,0.91,0};
GLfloat luce2[] = {1002.003,30.01,20.31,0};

int lenlista=0; //lunghezza della lista di punti

/* le variabili che rappresentano le display list */
int lampadina = 11;
int light = 0;
int lucespot = 9;
int coppola = 8;
int lampada = 7;
int punti = 6;
int text = 5;
int gambe = 4;
int tavolo = 3;
int gabbia = 2;
int poligono = 1;




/* variabili per la trackball */
float 	angle = 0.0, axis[3], trans[3];
bool 	trackingMouse = false;
bool 	redrawContinue = false;
bool    trackballMove = false;
float lastPos[3] = {0.0, 0.0, 0.0};
int curx, cury;
int startX, startY;


/* materiali */
typedef struct materialStruct {
        GLfloat ambient[4];
        GLfloat diffuse[4];
        GLfloat specular[4];
        GLfloat shininess;
} materialStruct;

materialStruct brassMaterials = {
               {0.33,0.22,0.03,1.0},
               {0.78,0.57,0.11,1.0},
               {0.99,0.91,0.81,1.0},
               27.8
};

materialStruct brassMaterials1 = {
               {0.13,0.72,0.06,0.41},
               {0.8,0.967,0.811,0.8203},
               {0.309,0.191,0.81,0.012},
               17.1
};

materialStruct brassMaterials2 = {
               {0.33,0.22,0.03,0.1},
               {0.78,0.7,0.81,0.1},
               {0.99,0.1,0.81,0.1},
               0.8
};

materialStruct redPlasticMaterials = {
               {0.3,0.0,0.0,1.0},
               {0.6,0.0,0.0,1.0},
               {0.8,0.6,0.6,1.0},
               100.0
};

materialStruct whiteShineyMaterials = {
               {1.0,1.0,1.0,1.0},
               {1.0,1.0,1.0,1.0},
               {1.0,1.0,1.0,1.0},
               100.0
};

materialStruct myMaterials1 = {
               {0.03,0.5,1.0,0.7301},
               {0.23,0.9,1.0,0.801},
               {1.0,1.0,0.3,0.901},
               0.62
};

materialStruct myMaterials2 = {
               {0.123,1.0,1.0,1.0},
               {0.8,0.50,0.8181,0.18},
               {0.51,0.27,1.0,0.2},
               0.62
               
};

materialStruct myMaterials3 = {
               {0.3,0.301,0.932,0.4131},
               {0.505,0.732,0.751,0.971},
               {0.313,0.67,0.9312,0.91},
               14.312
               
};

/*assegna un materiale*/
void materials(materialStruct *materials) {
     glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, materials -> ambient);
     glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, materials -> diffuse);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, materials -> specular);
     glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, materials -> shininess);
}
               


/*struttura punto per la memorizzazione dei punti dati dal mouse*/
struct Punto {
       GLfloat coordx;
       GLfloat coordy;
       GLfloat coordz;
       /* elemento della lista */
       struct Punto *next;
       /* puntatore all'elemento successivo */
};

typedef struct Punto *lista;

struct Punto *listap = NULL;


int ii=0; //contatore per la lista di punti





/*copia dalla lista all'array*/
void esamina(lista p1,GLfloat array[][3]){
      if(p1){                                         

        /* elaborazione nodo */                       
        GLfloat x,y,z;
        array[ii][0] = p1->coordx;
        array[ii][1] = p1->coordy;
        array[ii][2] = p1->coordz;
        
        ii++;
        
        esamina(p1->next,array);                            
      } 
}


/*inserisce un punto nella lista*/
lista nuovopunt2(GLfloat x, GLfloat y, GLfloat z, lista p){
      lista nuovo;
      if (p!=NULL) {
         nuovo=(lista) malloc(sizeof(struct Punto));
         nuovo->coordx=x;
         nuovo->coordy=y;
         nuovo->coordz=z;
         nuovo->next=p;
      }
      else if (p==NULL){
           p = (lista) malloc(sizeof(struct Punto));
           p->coordx=x;
           p->coordy=y;
           p->coordz=z;
           p->next=NULL;
           nuovo=p;
      }
      
      return nuovo;
}

/* disegna un punto sul viewport*/
void disegnapunto(int x, int y) {
     GLfloat xf,yf,zf;
     xf = (((GLfloat) x) /(ww));
     yf = (((GLfloat) y) /(hh));
     if (xf>0.275 && xf<0.7 && yf<0.57 && yf>0.15) {
       lenlista++;
       zf=0.1; //la z è sempre 0.1
       
       /* inserisce il punto nella lista di punti */
       listap = nuovopunt2(xf,yf,zf,listap);
       
       
       GLfloat ar[lenlista][3];
       esamina(listap,ar);
       /* ridefinisce la display list punti*/
       glNewList(punti,GL_COMPILE);
        materials(&myMaterials1);
        for (int icor=0;icor<lenlista;icor++) {
            
            glBegin(GL_POINTS);
              glVertex3fv(ar[icor]);
            glEnd();
            glPushMatrix();
            glTranslatef(ar[icor][0],ar[icor][1],ar[icor][2]);
            glColor3f(0.1,0.11,0.55);
            glutSolidSphere(0.01,20,20);
            glPopMatrix();
        }
      glEndList();
      
      ii = 0; //resetta il contatore
     }
}



/* bubble sort dei punti in base alla x*/
void bsortx(GLfloat lista[][3], int a, int z ) {

    GLfloat scambiox;
    GLfloat scambioy;
    GLfloat scambioz;
    int j;
    int k;

    /* Inizia il ciclo di scansione dell'array.			     */
    for ( j = a; j < z; j++ ) {

	/* Scansione interna dell'array per collocare nella	     */
	/* posizione j l'elemento giusto.			     */
        for ( k = j+1; k <= z; k++ ) {

            if ( lista[k][0] < lista[j][0] ) {

		    /* Scambia i valori.				     */
                scambiox = lista[k][0];
                scambioy = lista[k][1];
                scambioz = lista[k][2];
                lista[k][0] = lista[j][0];
                lista[k][1] = lista[j][1];
                lista[k][2] = lista[j][2];
                lista[j][0] = scambiox;
                lista[j][1] = scambioy;
                lista[j][2] = scambioz;
            }
        }
    }
}

/* bubble sort dei punti in base alla y*/
void bsorty(GLfloat lista[][3], int a, int z ) {

    GLfloat scambiox;
    GLfloat scambioy;
    GLfloat scambioz;
    int j;
    int k;

    /* Inizia il ciclo di scansione dell'array.			     */
    for ( j = a; j < z; j++ ) {

	/* Scansione interna dell'array per collocare nella	     */
	/* posizione j l'elemento giusto.			     */
        for ( k = j+1; k <= z; k++ ) {

            if ( lista[k][1] < lista[j][1] ) {

		    /* Scambia i valori.				     */
                scambiox = lista[k][0];
                scambioy = lista[k][1];
                scambioz = lista[k][2];
                lista[k][0] = lista[j][0];
                lista[k][1] = lista[j][1];
                lista[k][2] = lista[j][2];
                lista[j][0] = scambiox;
                lista[j][1] = scambioy;
                lista[j][2] = scambioz;
            }
        }
    }
}
/*calcola la distanza tra un punto e una retta data da m e q*/
GLfloat dist(GLfloat p[2],GLfloat m,GLfloat q) {
        GLfloat d;
        d = (p[1] - m*p[0] - q)/sqrt(1+pow(m,2));
        if (d<0) d=-d;
        return d;
}

/* bubble sort dei punti in base alla distanza da una retta*/
void bsortdist(GLfloat lista[][3], int a, int z,GLfloat sin[2],GLfloat des[2] ) {
    GLfloat m = (des[1]-sin[1])/(des[0]-sin[0]);
    GLfloat m2 = pow(m,2);
    if (des[0]==sin[0]) sin[0]=sin[0]+0.01;
    GLfloat q = ((des[0]*sin[1]-sin[0]*des[1])/(des[0]-sin[0]));
    GLfloat scambiox;
    GLfloat scambioy;
    GLfloat scambioz;
    int j;
    int k;

    /* Inizia il ciclo di scansione dell'array.			     */
    for ( j = a; j < z; j++ ) {

	/* Scansione interna dell'array per collocare nella	     */
	/* posizione j l'elemento giusto.			     */
        for ( k = j+1; k <= z; k++ ) {

            if ( dist(lista[k],m,q) < dist(lista[j],m,q) ) {

		    /* Scambia i valori.				     */
                scambiox = lista[k][0];
                scambioy = lista[k][1];
                scambioz = lista[k][2];
                lista[k][0] = lista[j][0];
                lista[k][1] = lista[j][1];
                lista[k][2] = lista[j][2];
                lista[j][0] = scambiox;
                lista[j][1] = scambioy;
                lista[j][2] = scambioz;
            }
        }
    }
}


/*disegna il triangolo al di sopra dei punti specificati, se ce n'è uno*/
void uptriangle(GLfloat punti[][3],int len,GLfloat xmin[3],GLfloat xmax[3]) {
     
     bsortdist(punti,0,len,xmin,xmax);
     int j=len;
     bool trovato = false;
     while (!trovato && j>=0) {
           GLfloat corrx=punti[j][0];
           GLfloat corry=punti[j][1];
           GLfloat corrz=punti[j][2];
           if ( (((xmax[1]-xmin[1])/(xmax[0]-xmin[0]) )*(((corry-xmin[1])/(xmax[1]-xmin[1]))-((corrx-xmin[0])/(xmax[0]-xmin[0])))) > 0) {
               trovato = true;
               glBegin(GL_TRIANGLES);
                 glColor3f(j*0.071,j*0.3628,j*0.1751);
                 glVertex3fv(xmin);
                 glVertex3f(corrx,corry,corrz);
                 glVertex3fv(xmax);
               glEnd();
               glFlush();
               
               GLfloat apice[3];
               apice[0] = corrx;
               apice[1] = corry;
               apice[2] = corrz;
               uptriangle(punti,len,xmin,apice);
               uptriangle(punti,len,apice,xmax);
           }
           j--;
           
     }
}


/* disegna il triangolo al di sotto dei punti indicati, se ce n'è uno*/
void downtriangle(GLfloat punti[][3],int len, GLfloat xmin[3],GLfloat xmax[3]) {
     
     bsortdist(punti,0,len,xmin,xmax);
     int j=len;
     bool trovato = false;
     while (!trovato && j>=0) {
           GLfloat corrx=punti[j][0];
           GLfloat corry=punti[j][1];
           GLfloat corrz=punti[j][2];
           if (  (  ((xmax[1]-xmin[1])/(xmax[0]-xmin[0]))  * (((corry-xmin[1])/(xmax[1]-xmin[1]))-((corrx-xmin[0])/(xmax[0]-xmin[0])))) < 0) {
               trovato = true;
               glBegin(GL_TRIANGLES);
                 glColor3f(j*0.041,j*.083,j*0.69);
                 glVertex3fv(xmin);
                 glVertex3f(corrx,corry,corrz);
                 glVertex3fv(xmax);
               glEnd();
               glFlush();
               
               GLfloat apice[3];
               apice[0] = corrx;
               apice[1] = corry;
               apice[2] = corrz;
               downtriangle(punti,len,xmin,apice);
               downtriangle(punti,len,apice,xmax);
           }
           j--;
           
     }
}

/*calcola il guscio convesso dell'insieme di punti dell'array punti tramite l'algoritmo quickhull*/
void convexhull2(GLfloat punti[][3],int len) {
     /*trovo i punti di ascissa minima e massima */
     bsortx(punti,0,len);
     GLfloat minx[3];
     minx[0]=punti[0][0];
     minx[1]=punti[0][1];
     minx[2]=punti[0][2];
     GLfloat maxx[3];
     maxx[0]=punti[len][0];
     maxx[1]=punti[len][1];
     maxx[2]=punti[len][2];
     /*trovo i punti di ordinata massima e minima */
     bsorty(punti,0,len);
     GLfloat miny[3];
     miny[0]=punti[0][0];
     miny[1]=punti[0][1];
     miny[2]=punti[0][2];
     GLfloat maxy[3];
     maxy[0]=punti[len][0];
     maxy[1]=punti[len][1];
     maxy[2]=punti[len][2];
     glNewList(poligono,GL_COMPILE);
     materials(&myMaterials1);
     /*disegno i 2 triangoli */
     glBegin(GL_TRIANGLES);
       glColor3f(0.4,0.1,0.5);
       glVertex3fv(minx);
       glVertex3fv(maxy);
       glVertex3fv(maxx);
     glEnd();
     
     glBegin(GL_TRIANGLES);
       glColor3f(0.1,0.4,0.1);
       glVertex3fv(minx);
       glVertex3fv(miny);
       glVertex3fv(maxx);
     glEnd();
     
     glFlush();
     /* ricorsione per gli altir punti */
     uptriangle(punti,len,minx,maxy);
     uptriangle(punti,len,maxy,maxx);
     downtriangle(punti,len,minx,miny);
     downtriangle(punti,len,miny,maxx);
     glEndList();
}

/* il menu dopo 'vista 3D' */
void menuesci(int n) {
     if (n==0) exit(0);
}

/* funzioni per la trackball */
void trackball_ptov(int x, int y, int width, int height, float v[3]) {
    
    float d, a;
    
    v[0] = (2.0F*x - width) / width;
    v[1] = (height - 2.0F*y) / height;
    d = (float) sqrt(v[0]*v[0] + v[1]*v[1]);
    v[2] = (float) cos((M_PI/2.0F) * ((d < 1.0F) ? d : 1.0F));
    a = 1.0F / (float) sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
    v[0] *= a;
    v[1] *= a;
    v[2] *= a;
    
}


void mouseMotion(int x, int y) {
    float curPos[3], dx, dy, dz;
    
    trackball_ptov(x, y, ww, hh, curPos);
    if(trackingMouse) {
                      dx = curPos[0] - lastPos[0];
                      dy = curPos[1] - lastPos[1];
                      dz = curPos[2] - lastPos[2];

           if (dx || dy || dz) {
             angle = 90.0F * sqrt(dx*dx + dy*dy + dz*dz);
             axis[0] = lastPos[1]*curPos[2] - lastPos[2]*curPos[1];
             axis[1] = lastPos[2]*curPos[0] - lastPos[0]*curPos[2];
             axis[2] = lastPos[0]*curPos[1] - lastPos[1]*curPos[0];
           
             lastPos[0] = curPos[0];
             lastPos[1] = curPos[1];
	         lastPos[2] = curPos[2];
           }
    } 
    
    glutPostRedisplay();
}

void stopMotion(int x, int y) {
    
    trackingMouse = false;

    if (startX != x || startY != y) {
	redrawContinue = true;
    } else {
	angle = 0.0F;
	redrawContinue = false;
	trackballMove = false;
    }
    
}


void startMotion(int x, int y) {
    
    trackingMouse = true;
    redrawContinue = false;
    startX = x; startY = y;
    curx = x; cury = y;
    
    trackball_ptov(x, y, ww, hh, lastPos);
    
	trackballMove=true;
	
}


void mousepostclick(int button,int state,int x,int y) {
	
	if(button==GLUT_LEFT_BUTTON) switch(state) 
	{
    case GLUT_DOWN:
	y=hh-y;
	
	startMotion( x,y);
	
	break;
    case GLUT_UP:
	stopMotion(x,y);
	break;
    }
    
}

/*menu per il mouse dopo il quickhull*/
void postquick(int v) {
     ii=0;
     glOrtho(-4,1,-4,1,-10,10);
     
     if (v==4) exit(0);
     else if (v==5) {
          glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
          
          ii=0;
          
          //gluLookAt(0.3,0.41,-0.3,-0.4,0.5,0.22,0,0,1);
          glPushMatrix();
          
          GLfloat array[lenlista][3];
          esamina(listap,array);
          convexhull2(array,lenlista-1);
          
          glPopMatrix();
          
          glutCreateMenu(menuesci);
          glutAddMenuEntry("Esci",0);
          glutAttachMenu(GLUT_RIGHT_BUTTON);
          glutMotionFunc(mouseMotion);
     
          glutMouseFunc(mousepostclick);
          glScalef(1,1,-1);
          glTranslatef(-0.5,-0.5,0);
          glutPostRedisplay();
     }
     lenlista=0;
}



/*il menu iniziale del mouse*/
void mymenu(int value) {
     if (value==1) {
        
        glNewList(lucespot,GL_COMPILE_AND_EXECUTE);
          glDisable(GL_LIGHT0);
          glPushMatrix();
          
          GLfloat spotdirection[] = {0.5,0.43,0,1};
          GLfloat position[] = {0.32,0.45,0.37,1};
          GLfloat ambient[] = {10.3,20.829,32.51,0.099841};
          GLfloat diffuse[] = {1.5,1.5,30.19,1};
          GLfloat specular[] = {1.6,1.6,1.81,1.4};
          
          glEnable(GL_LIGHT2);
          glLightfv(GL_LIGHT2,GL_POSITION,position);
          glLightfv(GL_LIGHT2,GL_DIFFUSE,diffuse);
          glLightfv(GL_LIGHT2,GL_SPECULAR,specular);
          glLightfv(GL_LIGHT2,GL_AMBIENT,ambient);
          glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION,spotdirection);
          glLightf(GL_LIGHT2,GL_SPOT_CUTOFF,90);
          glLightf(GL_LIGHT2,GL_SPOT_EXPONENT,2220.7);
          glPopMatrix();
        glEndList();
        
        glNewList(light,GL_COMPILE_AND_EXECUTE);
        glEndList();
        glutRemoveMenuItem(1);
        
     }
    
     if (value==2)
        exit(0);
        
     if (value==0) {
        if (lenlista>=3) {

        GLfloat array[lenlista][3];
        esamina(listap,array);
        convexhull2(array,lenlista-1);
        glutMouseFunc(NULL);
        
        glutCreateMenu(postquick);
        glutAddMenuEntry("Vista 3D",5);
        glutAddMenuEntry("Esci",4);
        glutAttachMenu(GLUT_RIGHT_BUTTON);
        glutPostRedisplay();
        }
     }
}



/*la callback di display*/
void display() {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     if (trackballMove) {
	    glRotatef(angle, axis[0], axis[1], axis[2]);
    }
     glCallList(lucespot);
     glCallList(light);
     glCallList(gambe);
     glCallList(tavolo);
     glCallList(punti);
     glFlush();
}



/*callback per input da tastiera*/
void mykey(unsigned char key,int x, int y) {
     if (key == 'q' || key == 'Q' || key == '\27') exit(0);
     if (key == '+') {
             glScalef(2,2,2);
             glutPostRedisplay();
     }
     if (key == '-') {
             glScalef(0.5,0.5,0.5);
             glutPostRedisplay();
     }
     if (key == 'd') {
             glTranslatef(-0.2,0,0);
             glutPostRedisplay();
     }
     if (key == 'a') {
             glTranslatef(0.2,0,0);
             glutPostRedisplay();
     }
     if (key == 'w') {
             glTranslatef(0,-0.2,0);
             glutPostRedisplay();
     }
     if (key == 's') {
             glTranslatef(0,0.2,0);
             glutPostRedisplay();
     }
}

/*callback per input da mouse*/
void mymouse(int button, int state, int x, int y) {
     static int xx,yy;
     xx=x;
     yy=hh-y;
     if (state==GLUT_DOWN && button==GLUT_LEFT_BUTTON) {
         disegnapunto(xx,yy);
         
         glutPostRedisplay();
         
     }
     
}

/*funzione di inizializzazione*/
void init() {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
     
     
     /* abilitazione della luce*/
     glEnable(GL_LIGHTING);
     glLightModeli(GL_LIGHT_MODEL_TWO_SIDE,GL_TRUE);
     glEnable(GL_LIGHT0);
     
     /* normali */
     glEnable(GL_NORMALIZE);
     glNormal3f(0.3,0.1,0.55);
     
     /* luce iniziale */
     GLfloat pos0[3] = {0.5,0.5,0.2};
     glLightfv(GL_LIGHT0, GL_POSITION, pos0);
     GLfloat dif[] = {0.2,0.91,0.75,0.8};
     glLightfv(GL_LIGHT0,GL_DIFFUSE, dif);
     GLfloat amb[] = {0.91,1.1,0.991,1};
     glLightfv(GL_LIGHT0,GL_AMBIENT, amb);
     glLightf(GL_LIGHT0,GL_CONSTANT_ATTENUATION,0.55);
     glLightf(GL_LIGHT0,GL_LINEAR_ATTENUATION,0.2);
     glLightf(GL_LIGHT0,GL_QUADRATIC_ATTENUATION,0.066);
     
     
     glEnable(GL_BLEND);
     glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
     
     
     /* texture */
     GLubyte image[64][64][3];
     
     glEnable(GL_TEXTURE_2D);
     glTexEnvfv(GL_TEXTURE_ENV,GL_TEXTURE_ENV_COLOR,luce);
     glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,64,64,0,GL_RGB,GL_UNSIGNED_BYTE,image);
     glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
     glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
     glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
     glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
     
     int i,j,r,c;
     for (i=0;i<64;i++) {
         c = ((((i&0x8) == 0)^((j&0x8)) == 0))*255;
         image[i][j][0] = (GLubyte) c;
         image[i][j][1] = (GLubyte) c;
         image[i][j][2] = (GLubyte) c;
     }
     
     /*rimozione parti nascoste */
     glEnable(GL_DEPTH_TEST);
     glDepthMask(GL_TRUE);
     glCullFace(GL_BACK); 
     
     /* menu contestuale */
     glutCreateMenu(mymenu);
     glutAddMenuEntry("Lampada ON/OFF",1);
     glutAddMenuEntry("QuickHull",0);
     glutAddMenuEntry("Esci",2);
     glutAttachMenu(GLUT_RIGHT_BUTTON);
     
     /* vista */
     glOrtho(0,1,0,1,-10,10);
     
     /*il complesso poliedrale del tavolo*/
     
     /* le gambe */
     glNewList(gambe,GL_COMPILE);
       glPushMatrix();
       glDepthMask(GL_TRUE);
       materials(&brassMaterials);
       glScalef(1,1,6);
       glTranslatef(0.15,0.12,-0.05);
       glutSolidCube(0.1);
       glPopMatrix();
       glPushMatrix();
       glScalef(1,1,6);
       glTranslatef(0.84,0.12,-0.05);
       glutSolidCube(0.1);
       glPopMatrix();
       glPushMatrix();
       glScalef(1,1,6);
       glTranslatef(0.84,0.6,-0.05);
       glutSolidCube(0.1);
       glPopMatrix();
       glPushMatrix();
       glScalef(1,1,6);
       glTranslatef(0.15,0.6,-0.05);
       glutSolidCube(0.1);
       glPopMatrix();
     glEndList();
     
     /* la tavola del tavolo */
     glNewList(tavolo, GL_COMPILE);
       glDepthMask(GL_TRUE);
       glPushMatrix();
       glScalef(4,3,0.3);
       glTranslatef(0.123,0.123,0);
       materials(&redPlasticMaterials);
       glutSolidCube(0.2);
       glPopMatrix();
       glPushMatrix();
       materials(&myMaterials3);
       glRotatef(90,1,0,0);
       glTranslatef(0.8,0.09,-0.14);
       glutSolidTeapot(0.08);
       glPopMatrix();
       
       glCallList(gabbia);
       glCallList(lampada);
     glEndList();
     
     /* la teca trasparente */
     glNewList(gabbia, GL_COMPILE);
       glPushMatrix();
       glDepthMask(GL_FALSE);
       materials(&brassMaterials2);
       glTranslatef(0.49,0.365,0.123);
       glScalef(1,1,0.7);
       glutSolidCube(0.425);
       glPopMatrix();
       glCallList(punti);
       glCallList(poligono);
     glEndList();
     
     /* il poligono */
     glNewList(poligono,GL_COMPILE);
     glEndList();
     
     /*i punti*/
     glNewList(punti,GL_COMPILE);
     glEndList();
     
     /*le due facce della copertura della lampada */
     glNewList(coppola,GL_COMPILE);
       glDepthMask(GL_TRUE);
       materials(&brassMaterials);
       glBegin(GL_QUADS);
         glTexCoord2f(0,0);
         glVertex3f(0,0,0);
         glTexCoord2f(0,1);
         glVertex3f(1,0,0);
         glTexCoord2f(1,1);
         glVertex3f(0.8,0.2,1);
         glTexCoord2f(1,0);
         glVertex3f(0.2,0.2,1);
         glVertex3f(0.2,0.8,1);
         glVertex3f(0,1,0);
         glVertex3f(1,1,0);
         glVertex3f(0.8,0.8,1);
         glVertex3f(0.8,0.2,1);
         glVertex3f(1,0,0);
       glEnd();
     glEndList();
     
     /* la luce iniziale */
     glNewList(light,GL_COMPILE);
     glEndList();
     
     glNewList(lampadina,GL_COMPILE);
       glPushMatrix();
       glDepthMask(GL_TRUE);
	   materials(&myMaterials2);
       glutSolidSphere(0.03,30,30);
       glPopMatrix();
     glEndList();
     
     /* il braccio più la lampadina della lampada */
     glNewList(lampada,GL_COMPILE);
       glPushMatrix();	
       glDepthMask(GL_TRUE);
       materials(&brassMaterials1);
       glRotatef(-12,0,1,0);
       glScalef(0.1,0.1,1);
       glTranslatef(1.2,4.5,0.12);
	   glutSolidCube(0.3); //primo braccio
	   glScalef(10,10,1);
	   glTranslatef(0.13,0,0.21);
	   glRotatef(55,0,1,0);
	   glScalef(0.1,0.1,1);
	   glutSolidCube(0.3); //secondo braccio
	   glScalef(10,10,1);
	   glTranslatef(0.11,0,0.11);
	   glCallList(lampadina);
       glRotatef(-70,0,1,0);
       glTranslatef(-0.05,-0.05,0);
       glScalef(0.1,0.1,0.1);
       glCallList(coppola);
       glRotatef(90,0,0,1);
       glTranslatef(0,-1,0);
       glCallList(coppola);
       
	   glPopMatrix();
	   
    glEndList();
}

/*la callback di resize*/
void myreshape3(GLsizei w, GLsizei h) {
     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
     glMatrixMode(GL_MODELVIEW);
     glViewport(0,0,w,h);
     ww = w;
     hh = w;
     
}

/*funzione principale*/
int main(int argc, char** argv) {
    glutInit(&argc,argv);
    glutInitWindowSize(500,500);
    glutCreateWindow("quickhull nella teca di vetro - 0.7");
    init();
    glutKeyboardFunc(mykey);
    glutReshapeFunc(myreshape3);
    glutMouseFunc(mymouse);
    glutDisplayFunc(display);
    glutMainLoop();
}

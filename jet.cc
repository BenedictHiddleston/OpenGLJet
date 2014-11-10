// OpenGL Jet program.
// ECE8893, Georgia Tech, Fall 2012

#ifdef LINUX
//Linux Headers
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#endif

#ifdef OSX
// MAC Headers
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>
#endif

#ifdef WINDOWS
//Windows Headers
#include <Windows.h>
#include <gl/GL.h>
#include <gl/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <istream>
#include <stdint.h>

#include "Face.h"
#include "Material.h"


using namespace std;

GLfloat updateRate = 1.0; // Change this later

vector<Vertex> vertices;
vector<Normal> normals ;
vector<Face> faces ;
vector<Material> materials ;

bool x_rotat, y_rotat, z_rotat = false ;
float x_angle, y_angle, z_angle = 0 ;   //initial rotation angle
float scale = 1.0 ;
// 6 positions of lights
GLfloat light_pos0[] = { 1.0, 1.0, 1.0, 0.0 };
GLfloat light_pos1[] = { 1.0, 1.0, 0.0, 0.0 };
GLfloat light_pos2[] = { 1.0, 0.0, 0.0, 0.0 };
GLfloat light_pos3[] = { 0.0, 1.0, 1.0, 1.0 };
GLfloat light_pos4[] = { 0.0, 0.0, 1.0, 1.0 };
GLfloat light_pos5[] = { 0.0, 0.0, 0.0, 1.0 };
int light_count = 1 ;

uint32_t height, width;

// code to read the obj file
void Read()
{
  ifstream ifs("jet.obj");
  if (!ifs) return;
  int mtl = 0;
  while(ifs)
    {
      string header;
      ifs >> header;
      if (header == string("mtllib"))
        {
          string mtllib;
          ifs >> mtllib;
          // cout << header << " " << mtllib << endl;
        }
      else if (header == string("v"))
        {
          float v[3];
          ifs >> v[0] >> v[1] >> v[2];
          vertices.push_back(Vertex(v[0], v[1], v[2])) ;
          // cout << header << " " << v[0] << " " << v[1] << " " << v[2] << endl;
        }
      else if (header == string("vn"))
        {
          float vn[3];
          ifs >> vn[0] >> vn[1] >> vn[2];
          normals.push_back(Normal(vn[0], vn[1], vn[2])) ;
        }
      else if (header == string("usemtl"))
      {
        ifs >> mtl ;
      }
      else if (header == string("f"))
      { 
        // f_v/blank/f_n f_v/blank/f_n f_v/blank/f_n
        string f[3] ;
        int f_v[3] ;
        int blank[3] ;
        int f_n[3] ;
        ifs >> f[0] >> f[1] >> f[2];
        for (int i = 0; i < 3; i++)
        {
          sscanf((f[i].c_str()), "%i/%i/%i", &f_v[i], &blank[i], &f_n[i]);
        }
        faces.push_back(Face(vertices[f_v[0] - 1], vertices[f_v[1] - 1], vertices[f_v[2] - 1],
                             normals[f_n[0] - 1], normals[f_n[1] - 1], normals[f_n[2] - 1],
                             mtl)) ;
        faces[faces.size() - 1].mtl = mtl ;
      }  
    }
}

// Code to read material file
void ReadMtl()
{
  ifstream ifs("jet.mtl");
  if (!ifs) return;
  cout.precision(4);
  int n ;
  while(ifs)
    {
      string header;
      ifs >> header;
      if (header == string("newmtl"))
        {
          ifs >> n;
          // cout << header << " " << n << endl;
          materials.push_back(Material()) ;
          materials[n].mtl_inx = n ;
        }
      else if (header == string("Ns"))
        {
          float Ns;
          ifs >> Ns;
          materials[n].shiny = Ns ;
          //cout << header << " " << Ns << endl;
        }
      else if (header == string("Tr"))
        {
          float Tr;
          ifs >> Tr;
          materials[n].transparency = Tr ;         
        }
       else if (header == string("Ka")) 
       {
          float Ka[3];
          ifs >> Ka[0] >> Ka[1] >> Ka[2];
          materials[n].ar = Ka[0];
          materials[n].ag = Ka[1];
          materials[n].ab = Ka[2];
       }
       else if (header == string("Kd")) 
       {
          float Kd[3];
          ifs >> Kd[0] >> Kd[1] >> Kd[2];
          materials[n].dr = Kd[0];
          materials[n].dg = Kd[1];
          materials[n].db = Kd[2];
       }
       else if (header == string("Ks")) 
       {
          float Ks[3];
          ifs >> Ks[0] >> Ks[1] >> Ks[2];
          materials[n].sr = Ks[0];
          materials[n].sg = Ks[1];
          materials[n].sb = Ks[2];
       }
       else if (header == string("Ke")) 
       {
          float Ke[3];
          ifs >> Ke[0] >> Ke[1] >> Ke[2];
          materials[n].er = Ke[0];
          materials[n].eg = Ke[1];
          materials[n].eb = Ke[2];
       }

    }
  
}

void setMaterial(int materialId)
{
  float specular[4];
  float ambient[4];
  float diffuse[4];
  float emmisive[4];
  float shiny;
  
  // Ugly but works
  
  specular[0] = materials[materialId].sr;
  specular[1] = materials[materialId].sg;
  specular[2] = materials[materialId].sb;
  specular[3] = 1 - materials[materialId].transparency;
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
  
  ambient[0] = materials[materialId].ar;
  ambient[1] = materials[materialId].ag;
  ambient[2] = materials[materialId].ab;
  ambient[3] = 1 - materials[materialId].transparency;
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
  
  diffuse[0] = materials[materialId].dr;
  diffuse[1] = materials[materialId].dg;
  diffuse[2] = materials[materialId].db;
  diffuse[3] = 1 - materials[materialId].transparency;
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
  
  emmisive[0] = materials[materialId].er;
  emmisive[1] = materials[materialId].eg;
  emmisive[2] = materials[materialId].eb;
  emmisive[3] = 1 - materials[materialId].transparency;
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emmisive);
  
  glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &materials[materialId].shiny);
}

void drawModel()
{
  // Your code to draw the model here
  for(unsigned int i = 0; i < faces.size(); i++) {
    glBegin(GL_TRIANGLES) ;
    setMaterial(faces[i].mtl) ;
    glVertex3f(faces[i].x.x, faces[i].x.y, faces[i].x.z) ;
    glVertex3f(faces[i].y.x, faces[i].y.y, faces[i].y.z) ;
    glVertex3f(faces[i].z.x, faces[i].z.y, faces[i].z.z) ;

    glNormal3f(faces[i].x.x, faces[i].x.y, faces[i].x.z);
    glNormal3f(faces[i].x.x, faces[i].x.y, faces[i].x.z);
    glNormal3f(faces[i].x.x, faces[i].x.y, faces[i].x.z);
     // ??
    glEnd() ;
  }
}


void init(void)
{ // Called from main
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}


// double updateRate = 20;

void timer(int)
{
  // Adjust rotation angles as needed here
  if(x_rotat) {
    x_angle += 5 ;
  }
  if(y_rotat) {
    y_angle += 5 ;
  }
  if(z_rotat) {
    z_angle += 5 ;
  }

  // Then tell glut to redisplay
  glutPostRedisplay();
  // And reset tht timer
  glutTimerFunc(1000.0 / updateRate, timer, 0);
}

void display(void)
{
  //glClearColor(0.0, 0.0, 0.0, 1.0); // black background
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0, 15.0, -25.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    // rotations and scaling here

    glPushMatrix(); 
    glRotatef(x_angle, 1, 0, 0);        
    glRotatef(y_angle, 0, 1, 0);        
    glRotatef(z_angle, 0, 0, 1);        

    glScalef(scale, scale, scale);

    // light
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);
    glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);
    glLightfv(GL_LIGHT2, GL_POSITION, light_pos2);
    glLightfv(GL_LIGHT3, GL_POSITION, light_pos3);
    glLightfv(GL_LIGHT4, GL_POSITION, light_pos4);
    glLightfv(GL_LIGHT5, GL_POSITION, light_pos5);
    // Draw th emodel
    drawModel();
    // Swap the double buffers
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    height = h;
    width = w;
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
}



void keyboard (unsigned char key, int x, int y) 
{
  switch(key)
  {
    case 'q' :
      exit(0) ; break ;
    case 'x' :
      x_rotat = !x_rotat ; break ;
    case 'y' :
      y_rotat = !y_rotat ; break ;
    case 'z' :
      z_rotat = !z_rotat ; break ;
    case 's' :
      scale -= 0.1 ; break ;
    case 'S' :
      scale += 0.1 ; break ;
    case 'r' :
      x_angle = 0 ;
      y_angle = 0 ;
      z_angle = 0 ;
      break ;
    case 'l' :
      switch(light_count)
      {
        case 1 :
          break ;
        case 2 :
          glDisable(GL_LIGHT1) ;
          light_count-- ;
          break ;  
        case 3 :
          glDisable(GL_LIGHT2) ;
          light_count-- ;
          break ;  
        case 4 :
          glDisable(GL_LIGHT3) ;
          light_count-- ;
          break ;  
        case 5 :
          glDisable(GL_LIGHT4) ;
          light_count-- ;
          break ;  
        case 6 :
          glDisable(GL_LIGHT5) ;
          light_count-- ;
          break ;  
      }
      break ;
    case 'L' :
      switch(light_count) 
      {
        case 1 :
           glEnable(GL_LIGHT1);
           light_count++ ;
           break ;
        case 2 :
           glEnable(GL_LIGHT2);
           light_count++ ;
           break ;
        case 3 :
           glEnable(GL_LIGHT3);
           light_count++ ;
           break ;
        case 4 :
           glEnable(GL_LIGHT4);
           light_count++ ;
           break ;
        case 5 :
           glEnable(GL_LIGHT5);
           light_count++ ;
           break ;
        case 6 :
           break ;
     }
      break ;
  }
}


int main(int argc, char* argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Jet Display");
    Read() ;
    ReadMtl() ;
    init();
    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutKeyboardFunc (keyboard);
    glutReshapeFunc(reshape);
    // Compute the update rate here...
    glutTimerFunc(1000.0 / updateRate, timer, 0);
    glutMainLoop();
    return 0;
}

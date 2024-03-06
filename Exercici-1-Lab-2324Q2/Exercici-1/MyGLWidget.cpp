#include "MyGLWidget.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  glClearColor (0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffersRectangle();
  creaBuffersCano();
  //creaBuffersRoda();
}


void MyGLWidget::modelTransformQuadrat(glm::vec3 posicio, glm::vec3 escala)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,posicio);  
  TG = glm::scale(TG,escala);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformQuadratCano()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,glm::vec3(tx + 0.125,0.25,0.0));
  TG = glm::rotate(TG, rotacio, glm::vec3(0.0,0.0,1.0));
  TG = glm::translate(TG,glm::vec3(-0.125,-0.25,0.0));
  
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

/*void MyGLWidget::modelTransformQuadratRoda(glm::vec3 posicio, glm::vec3 escala) 
{

}*/

void MyGLWidget::pintaCos() {

  glBindVertexArray(VAORec);
  modelTransformQuadrat(glm::vec3(tx,0.0,0.0), glm::vec3(1.0));      
  glDrawArrays(GL_TRIANGLES, 0, 6);    		                    
  // Desactivem el VAO
  glBindVertexArray(0); 

  glBindVertexArray(VAORec);    
  modelTransformQuadrat(glm::vec3(0.125 + tx,0.25,0.0), glm::vec3(0.5,1.0,1.0));      
  glDrawArrays(GL_TRIANGLES, 0, 6);    		                    
  // Desactivem el VAO
  glBindVertexArray(0); 
}

void MyGLWidget::pintaCano() {
  glBindVertexArray(VAOCano);
  modelTransformQuadratCano();      
  glDrawArrays(GL_TRIANGLES, 0, 6);    		                    
  // Desactivem el VAO
  glBindVertexArray(0);
}

/*void MyGLWidget::pintaRodes() {

  for (int i = 0; i < 6; ++i) {
    glBindVertexArray(VAOGreyWheel);
    modelTransformQuadratRoda(glm::vec3(0.0), glm::vec3(1.0));      
    glDrawArrays(GL_TRIANGLES, 0, 6);    		                    
    // Desactivem el VAO
    glBindVertexArray(0);
  }
}*/

void MyGLWidget::pintaTanc() {
  pintaCos();
  pintaCano();
  //pintaRodes();
}

void MyGLWidget::paintGL ()
{
// En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer

  // Pintem un tanc
  pintaTanc();                           
}

void MyGLWidget::resizeGL (int w, int h)
{
// Aquest codi és necessari únicament per a MACs amb pantalla retina.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif
}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Left:
      if (rotacio < M_PI/2 - 0.01) rotacio += M_PI/18; 
    	break;
    case Qt::Key_Right: 
      if (rotacio > 0) rotacio -= M_PI/18;   
    	break;
    case Qt::Key_A: 
      tx -= 0.01;
    	break;
    case Qt::Key_D: 
      tx += 0.01;
    	break;		
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::creaBuffersRectangle()
{
  glm::vec3 VertRec1[6];  // vèrtexs amb X, Y i Z
  
  // minX = -1.0
  // maxX = 0.0
  // minY = -1.0
  // maxY = 0.0
  
  VertRec1[0] = glm::vec3(-0.5, 0.125, 0);
  VertRec1[1] = glm::vec3( 0.5, -0.125, 0); 
  VertRec1[2] = glm::vec3( 0.5, 0.125, 0);
  VertRec1[3] = glm::vec3( 0.5, -0.125, 0);
  VertRec1[4] = glm::vec3( -0.5, 0.125, 0);
  VertRec1[5] = glm::vec3( -0.5, -0.125, 0);
  
  // Creació del Vertex Array Object (VAO) que usarem per pintar el quadrat
  glGenVertexArrays(1, &VAORec);
  glBindVertexArray(VAORec);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertRec1), VertRec1, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glm::vec3 VertCol1[6];  // vèrtexs amb X, Y i Z

  for (int i = 0; i < 6; ++i) VertCol1[i] = verd;

  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertCol1), VertCol1, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}

void MyGLWidget::creaBuffersCano()
{
  glm::vec3 VertCano[6];
  VertCano[0] = glm::vec3(0.125, 0.3125, 0);
  VertCano[1] = glm::vec3( 0.875, 0.1875, 0); 
  VertCano[2] = glm::vec3( 0.875, 0.3125, 0);
  VertCano[3] = glm::vec3( 0.875, 0.1875, 0);
  VertCano[4] = glm::vec3( 0.125, 0.3125, 0);
  VertCano[5] = glm::vec3( 0.125, 0.1875, 0);

  glGenVertexArrays(1, &VAOCano);
  glBindVertexArray(VAOCano);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertCano), VertCano, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glm::vec3 VertCol[6];  // vèrtexs amb X, Y i Z

  for (int i = 0; i < 6; ++i) VertCol[i] = gris;

  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertCol), VertCol, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);

}

/*void MyGLWidget::creaBuffersRoda()
{
  glm::vec3 VertRoda[6];
  glm::vec3 VertCol[6];
  VertRoda[0] = glm::vec3(-0.4, 0.075, 0);
  VertRoda[1] = glm::vec3( -0.35, -0.025, 0); 
  VertRoda[2] = glm::vec3( -0.35, 0.075, 0);
  VertRoda[3] = glm::vec3( -0.35, -0.025, 0);
  VertRoda[4] = glm::vec3( -0.4, 0.075, 0);
  VertRoda[5] = glm::vec3( -0.4, -0.025, 0);

  glGenVertexArrays(1, VAOGreyWheel);
  glBindVertexArray(&VAOGreyWheel);

  GLuint VBO1;
  glGenBuffers(1, &VBO1);
  glBindBuffer(GL_ARRAY_BUFFER, VBO1);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertRoda), VertRoda, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  for (int j = 0; j < 6; ++j) VertCol[j] = gris; 

  GLuint VBO2;
  glGenBuffers(1, &VBO2);
  glBindBuffer(GL_ARRAY_BUFFER, VBO2);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertCol), VertCol, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  glBindVertexArray(0);
}*/



void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  
  // Obtenim els identificadors dels uniforms
  TGLoc = glGetUniformLocation(program->programId(), "TG"); 
  rotLoc = glGetUniformLocation(program->programId(), "TG");
  colorLoc = glGetAttribLocation(program->programId(), "colors");
}

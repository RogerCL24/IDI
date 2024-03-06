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
}


void MyGLWidget::modelTransformQuadrat(glm::vec3 posicio, glm::vec3 escala)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG,posicio);  
  TG = glm::scale(TG,escala);
  glUniformMatrix4fv(TGLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::pintaCos() {

  glBindVertexArray(VAORec1);
  modelTransformQuadrat(glm::vec3(0.0), glm::vec3(1.0));      //Habra que modificar argumentos a variables del .h
  glDrawArrays(GL_TRIANGLES, 0, 6);    		                    
  // Desactivem el VAO
  glBindVertexArray(0); 

  glBindVertexArray(VAORec2);    
  modelTransformQuadrat(glm::vec3(0.0), glm::vec3(1.0));      //Habra que modificar argumentos a variables del .h
  glDrawArrays(GL_TRIANGLES, 0, 6);    		                    
  // Desactivem el VAO
  glBindVertexArray(0); 
}

void MyGLWidget::pintaCano() {

}

void MyGLWidget::pintaRodes() {

}

void MyGLWidget::pintaTanc() {
  pintaCos();
  //pintaCano();
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
    	break;
    case Qt::Key_Right: 
    	break;
    case Qt::Key_A: 
    	break;
    case Qt::Key_D: 
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
  glGenVertexArrays(1, &VAORec1);
  glBindVertexArray(VAORec1);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO11;
  glGenBuffers(1, &VBO11);
  glBindBuffer(GL_ARRAY_BUFFER, VBO11);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertRec1), VertRec1, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);

  glm::vec3 VertCol1[6];  // vèrtexs amb X, Y i Z

  for (int i = 0; i < 6; ++i) VertCol1[i] = verd;

  GLuint VBO12;
  glGenBuffers(1, &VBO12);
  glBindBuffer(GL_ARRAY_BUFFER, VBO12);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertCol1), VertCol1, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);

  glm::vec3 VertRec2[6];  // vèrtexs amb X, Y i Z
  
  VertRec2[0] = glm::vec3(-0.125, 0.375, 0);
  VertRec2[1] = glm::vec3( 0.375, 0.125, 0);
  VertRec2[2] = glm::vec3( 0.375, 0.375, 0);
  VertRec2[3] = glm::vec3( 0.375, 0.125, 0);
  VertRec2[4] = glm::vec3( -0.125, 0.375, 0);
  VertRec2[5] = glm::vec3( -0.125, 0.125, 0);

  glGenVertexArrays(1, &VAORec2);
  glBindVertexArray(VAORec2);

  // Creació del buffer amb les posicions dels vèrtexs
  GLuint VBO21;
  glGenBuffers(1, &VBO21);
  glBindBuffer(GL_ARRAY_BUFFER, VBO21);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertRec2), VertRec2, GL_STATIC_DRAW);
  glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(vertexLoc);
  
  glGenBuffers(1, &VBO12);
  glBindBuffer(GL_ARRAY_BUFFER, VBO12);
  glBufferData(GL_ARRAY_BUFFER, sizeof(VertCol1), VertCol1, GL_STATIC_DRAW);
  glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
  glEnableVertexAttribArray(colorLoc);

  // Desactivem el VAO
  glBindVertexArray(0);
}


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
  colorLoc = glGetAttribLocation(program->programId(), "colors");
}

// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent) : LL4GLWidget(parent) {

}

int MyGLWidget::printOglError(const char file[], int line, const char func[]) 
{
    GLenum glErr;
    int    retCode = 0;

    glErr = glGetError();
    const char * error = 0;
    switch (glErr)
    {
        case 0x0500:
            error = "GL_INVALID_ENUM";
            break;
        case 0x501:
            error = "GL_INVALID_VALUE";
            break;
        case 0x502: 
            error = "GL_INVALID_OPERATION";
            break;
        case 0x503:
            error = "GL_STACK_OVERFLOW";
            break;
        case 0x504:
            error = "GL_STACK_UNDERFLOW";
            break;
        case 0x505:
            error = "GL_OUT_OF_MEMORY";
            break;
        default:
            error = "unknown error!";
    }
    if (glErr != GL_NO_ERROR)
    {
        printf("glError in file %s @ line %d: %s function: %s\n",
                             file, line, error, func);
        retCode = 1;
    }
    return retCode;
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  
  carregaShaders();
  
  creaBuffersModels();
  creaBuffersTerra();

  iniEscena();
  iniCamera();
  connect(&timer,SIGNAL(timeout()),this, SLOT(animar()));
  clock = shut_down = false;
}

void MyGLWidget::iniMaterialTerra ()
{
  // Donem valors al material del terra
  amb = glm::vec3(0.,0.,0.);
  diff = glm::vec3(0.,0.5,0.5);
  spec = glm::vec3(0.8,0.8,0.8);
  shin = 100;
}

void MyGLWidget::paintGL () 
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  
  // Road
  glBindVertexArray (VAO_models[ROAD]);
  for (int i = 0; i < 4; ++i) {
    modelTransformRoad (glm::vec3(trackX[i], 0.01, trackY[i]), float(M_PI/2.)*i);
    glDrawArrays(GL_TRIANGLES, 0, models[ROAD].faces().size()*3);
  }

  // Car
  glBindVertexArray (VAO_models[CAR]);
  for (int i = 0; i < 2; ++i) {
    paintC = i + 1;
    glUniform1i(paintCarLoc, paintC);
    modelTransformCar (carDist[i], carAngle[i], driveCar[i*2], driveCar[i*2 + 1]);
    glDrawArrays(GL_TRIANGLES, 0, models[CAR].faces().size()*3);
    if (i == 0) modelTransformFocus1(driveLight[i*2], driveLight[i*2 + 1]);
    else modelTransformFocus2(driveLight[i*2], driveLight[i*2 + 1]);   
  }

  paintC = 0;
  glUniform1i(paintCarLoc, paintC);

  // Pipe
  glBindVertexArray (VAO_models[PIPE]);
  modelTransformPipe ();
  glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  
  glBindVertexArray(0);
}

void MyGLWidget::iniCamera(){
  distancia = radiEsc * 2.0;
  fov = 2.0*asin(radiEsc/distancia);
  znear =  distancia - radiEsc;
  zfar  = distancia + radiEsc;

  angleY = 0.0;
  angleX = float(M_PI/5.);

  viewTransform();

  posFocus = glm::vec3(0.,0.,0.);
  colorFocus = glm::vec3(0.8,0.8,0.8);
  posFocusC1 = glm::vec3(0,0,0);
  posFocusC2 = glm::vec3(0,0,0);
  colorFocusC = glm::vec3(0.6,0.6,0.);


  glUniform3fv (posFocusLoc, 1, &posFocus[0]);
  glUniform3fv (colorFocusLoc, 1, &colorFocus[0]);
  glUniform3fv (posFocusC1Loc, 1, &posFocusC1[0]);
  glUniform3fv (posFocusC2Loc, 1, &posFocusC2[0]);
  glUniform3fv (colorFocusCLoc, 1, &colorFocusC[0]);

}

void MyGLWidget::iniEscena ()
{
  paintC = 0;
  minx = -12.5; 
  miny = 0.0; 
  minz = -12.5;
  maxx = 12.5;  
  maxy = 5.0; 
  maxz = 12.5;

  minim = glm::vec3(minx, miny, minz);     
  maxim = glm::vec3(maxx, maxy, maxz);  
  centreEsc = glm::vec3((minx+maxx)/2.f, (miny+maxy)/2.f, (minz+maxz)/2.f);
      
  radiEsc = glm::distance(centreEsc, maxim);
  glUniform1i(paintCarLoc, paintC);

}

void MyGLWidget::modelTransformFocus1(float X, float Z) {
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(6.5 + X, 0.0, Z));
  TG = glm::translate(TG, glm::vec3(2.48,0.4,-3.2));
  TG = glm::translate(TG, -centreBaseModels[CAR]);
  glUniformMatrix4fv (transFocus1Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFocus2(float X, float Z) {
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(9 + X, 0.0, Z));
  TG = glm::translate(TG, glm::vec3(2.48,0.4,-3.2));
  TG = glm::translate(TG, -centreBaseModels[CAR]);
  glUniformMatrix4fv (transFocus2Loc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG,glm::vec3(2.5f,1,2.5f));
  TG = glm::translate(TG, glm::vec3(-5,0,-5));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCar (float radi, float angle, float X, float Z)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, glm::vec3(radi + X,0, Z));
  TG = glm::scale(TG, glm::vec3(escalaModels[CAR]));
  TG = glm::rotate(TG, angle, glm::vec3(0,1,0));  
  TG = glm::translate(TG, -centreBaseModels[CAR]);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformRoad (glm::vec3 pos, float angle)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::scale(TG, glm::vec3(escalaModels[ROAD]*5));
  TG = glm::rotate(TG, angle,glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseModels[ROAD]);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformPipe ()
{
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE]*5));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX -= (e->y() - yClick) * M_PI / alt;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_Up: {
    drive();
    break;
	}
  case Qt::Key_L: {
      if (not shut_down) {
          colorFocus = glm::vec3(0.,0.,0.);
          glUniform3fv (colorFocusLoc, 1, &colorFocus[0]);
          shut_down = true;
          glClearColor(0.3, 0.3, 0.3, 1.0);
      }
      else {
          colorFocus = glm::vec3(0.8,0.8,0.8);
          glUniform3fv (colorFocusLoc, 1, &colorFocus[0]);
          shut_down = false;
          glClearColor(0.5, 0.7, 1.0, 1.0); 
      }

    break;
	}
  case Qt::Key_S: {
      clock = !(clock);
      if (clock) timer.start(50);
      else timer.stop();
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

void MyGLWidget::viewTransform ()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -2*radiEsc));
  View = glm::rotate(View, angleX, glm::vec3(1,0,0));
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  Proj = glm::perspective(fov, ra, znear, zfar);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::carregaShaders()
{
  LL4GLWidget::carregaShaders();
  paintCarLoc = glGetUniformLocation (program->programId(), "paintCar");
  posFocusLoc = glGetUniformLocation (program->programId(), "posFocus");
  colorFocusLoc = glGetUniformLocation (program->programId(), "colorFocusFS");
  posFocusC1Loc = glGetUniformLocation (program->programId(), "posFocusC1");
  posFocusC2Loc = glGetUniformLocation (program->programId(), "posFocusC1");
  colorFocusCLoc = glGetUniformLocation (program->programId(), "colorFocusC");
  transFocus1Loc = glGetUniformLocation (program->programId(), "TGfocus1");
  transFocus2Loc = glGetUniformLocation (program->programId(), "TGfocus2");
}

void MyGLWidget::drive() {
      carAngle[0] += float(M_PI/90.);
      carAngle[1] += float(M_PI/60.);

      driveCar[0] -= sin(carAngle[0])/4.3;
      driveCar[1] -= cos(carAngle[0])/4.3;
      driveCar[2] -= sin(carAngle[1])/2.2;
      driveCar[3] -= cos(carAngle[1])/2.2;

      //Los focos estan avanzados al coche, la trayectoria de la luz es la misma que la del coche pero habiendo girado ya varias veces
      float lightAngle_1 = carAngle[0] + float(M_PI/90.)*6;       // En el caso del coche rojo unas 6 veces (aprox)
      float lightAngle_2 = carAngle[1] + float(M_PI/60.)*3.2;     // En el verde unas 3 (aprox)

      driveLight[0] -= sin(lightAngle_1)/4.3;
      driveLight[1] -= cos(lightAngle_1)/4.3;
      driveLight[2] -= sin(lightAngle_2)/2.2;
      driveLight[3] -= cos(lightAngle_2)/2.2;
}

void MyGLWidget::animar() {
  makeCurrent();
  drive();
  update();
}



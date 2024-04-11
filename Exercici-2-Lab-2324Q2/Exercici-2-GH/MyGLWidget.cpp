#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) 
{
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

MyGLWidget::~MyGLWidget()
{
}

void MyGLWidget::initializeGL () {
  LL2GLWidget::initializeGL();
  connect(&timer,SIGNAL(timeout()),this, SLOT(animar()));
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
  ra = float(ample)/float(alt);
  factorAngleY = M_PI / ample;
  factorAngleX = M_PI / alt;
  if (ra > 1.0) {
    left = -radiEscena*ra;
    right = radiEscena*ra;
  }
  else {
    bottom = -radiEscena/ra;
    top = radiEscena/ra;
  }
  projectTransform();
}


bool MyGLWidget::checkPosition(glm::vec3 pos)
{
	bool valid = true;

  valid = !(pos == tailPos);

  int i = 0;
  while (i < bodyPos.size() and valid) {
    if (pos == bodyPos[i]) valid = false;
    ++i;
  }

  i = 0;
  while (i < 30 and valid) {
    valid = !(pos == glm::vec3(15,0,15 - i) || pos == glm::vec3(-15,0,15 - i) || pos == glm::vec3(-15 + i,0,15) || pos == glm::vec3(-15 + i,0,-15));
    ++i;
  }

	return valid; 
}

void MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // SnakeHead
  glBindVertexArray (VAO_models[SNAKE_HEAD]);
  SnakeHeadTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_HEAD].faces().size()*3);
  
  // SnakeBody
  glBindVertexArray (VAO_models[SNAKE_BODY]);
  for (unsigned int i = 0; i < bodyPos.size(); i++)
  {  
	  SnakeBodyTransform(bodyPos[i]);
	  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_BODY].faces().size()*3);
  }

  // SnakeTail
  glBindVertexArray (VAO_models[SNAKE_TAIL]);
  SnakeTailTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[SNAKE_TAIL].faces().size()*3);
  
  // Marble
  glBindVertexArray (VAO_models[MARBLE]);
  MarbleTransform();
  glDrawArrays(GL_TRIANGLES, 0, models[MARBLE].faces().size()*3);

  // Pipe
  glBindVertexArray (VAO_models[PIPE]);
  for (int i = 0; i <= 30; ++i) {
    PipeTransform(glm::vec3(-15,0,15 - i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }

  for (int i = 0; i <= 30; ++i) {
    PipeTransform(glm::vec3(15,0,15 - i));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }

  for (int i = 0; i <= 30; ++i) {
    PipeTransform(glm::vec3(-15 + i,0,15));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }

  for (int i = 0; i <= 30; ++i) {
    PipeTransform(glm::vec3(-15 + i,0,-15));
    glDrawArrays(GL_TRIANGLES, 0, models[PIPE].faces().size()*3);
  }


  
  // Terra
  glBindVertexArray (VAO_Terra);
  TerraTransform();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray (0);
}

void MyGLWidget::TerraTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(3.0,1.0,3.0));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::PipeTransform (glm::vec3 pos)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos); 
  TG = glm::scale(TG, glm::vec3(1.0,3.0,1.0));
  TG = glm::scale(TG, glm::vec3(escalaModels[PIPE]));
  TG = glm::translate(TG, -centreBaseModels[PIPE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::MarbleTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, marblePos);
  TG = glm::scale(TG, glm::vec3(0.5,0.5,0.5));
  TG = glm::scale(TG, glm::vec3(escalaModels[MARBLE]));
  TG = glm::translate(TG, -centreBaseModels[MARBLE]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::SnakeHeadTransform ()
{
  glm::mat4 TG(1.0f);  
  TG = glm::translate(TG, headPos);
  TG = glm::rotate(TG, float(-M_PI/2.0) + glm::radians(headAngle),glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_HEAD]));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_HEAD]);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::SnakeTailTransform ()
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, tailPos);
  TG = glm::rotate(TG, glm::radians(tailAngle),glm::vec3(0,1,0));
  TG = glm::scale(TG, glm::vec3(escalaModels[SNAKE_TAIL]));
  TG = glm::translate(TG, -centreBaseModels[SNAKE_TAIL]);
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
    glm::mat4 View(1.0);
    View = glm::translate(View, glm::vec3(0,0,-dist));
    View = glm::rotate(View, Theta + factorAngleY, glm::vec3(1,0,0));
    View = glm::rotate(View, -Psi + factorAngleX, glm::vec3(0,1,0));
    View = glm::translate(View, -centreEscena);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  if (cam_perpectiva) Proj = glm::perspective (fov, ra, znear, zfar);
  else Proj = glm::ortho(left, right, bottom, top, znear, zfar);
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::iniCamera(){

  //obs = glm::vec3(0, 8, -30);
  //vrp = glm::vec3(0, -2, 0);
  //up = glm::vec3(0, 1, 0);
  dist = radiEscena * 3.0;
  fov = 2.0*asin(radiEscena/dist);
  znear =  dist - radiEscena;
  zfar  = dist + radiEscena;
  
  if (ra < 1.0) {
    bottom = -radiEscena/ra;
    top = radiEscena/ra;
  }
  else if (ra > 1.0){
    left = -radiEscena*ra;
    right = radiEscena*ra;
  }
  else {
    left = bottom = -radiEscena;
    right = top = radiEscena;
  }

  Theta = float(M_PI/4.0);
  Psi = factorAngleX = factorAngleY = 0.0;

  viewTransform();
}

void MyGLWidget::iniEscena()
{
  minx = -15.0; maxx = -minx;
  miny = 0.0; maxy = 3.0; 
  minz = -15.0; maxz = -minz;    
  centreEscena = glm::vec3((minx+maxx)/2.0, (miny+maxy)/2.0, (minz+maxz)/2.0);

  headPos = glm::vec3(0,0,0);
  headAngle = 0;
  bodyPos.clear();	    
  bodyPos.push_back(glm::vec3(-1,0,0));    	
  tailPos = glm::vec3(-2,0,0);
  tailAngle = 0;

  marblePos = glm::vec3(10, 0, 0);

  direction = glm::vec3(1,0,0);

  max = glm::vec3(maxx, maxy, maxz);
  radiEscena = glm::distance(centreEscena, max);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  if (cam_perpectiva) {
    makeCurrent();

    if (e->y() > yClick) factorAngleY += 0.02;
    else if (e->y() < yClick) factorAngleY -= 0.02;

    if (e->x() > xClick) factorAngleX += 0.02;
    else if (e->x() < xClick) factorAngleX -= 0.02;

    xClick = e->x();
    yClick = e->y();
    
    viewTransform();

    update ();
  }
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: {
      direction = glm::vec3(0,0,-1);
      if (!clock) updateSnakeGame();
      break;
    }
    case Qt::Key_Down: { 
      direction = glm::vec3(0,0,1);
      if (!clock) updateSnakeGame();
      break;
        }
    case Qt::Key_Left: { 
      direction = glm::vec3(-1,0,0);
      if (!clock) updateSnakeGame();
      break;
        }  
    case Qt::Key_Right: { 
      direction = glm::vec3(1,0,0);
      if (!clock) updateSnakeGame();
      break;
        }
    case Qt::Key_C: { 
      if (cam_perpectiva) {
        cam_perpectiva = false;
        Theta = float(M_PI/2.);
        x_anterior = factorAngleX;
        y_anterior = factorAngleY; 
        factorAngleX = factorAngleY = 0;

      }
      else {
        cam_perpectiva = true;
        Theta -= float(M_PI/4.);   
        factorAngleX = x_anterior;
        factorAngleY = y_anterior;
      }
      projectTransform();
      viewTransform();
      break;
        }           
    case Qt::Key_R: {
      cam_perpectiva = true;
      iniEscena();
      iniCamera();
      projectTransform();
      break;
        }  
      case Qt::Key_T: {
        if (!clock) {
          timer.start(200);
          clock = true;
        }
        else {
          timer.stop();
          clock = false;
        }
      }
    default: event->ignore(); break;
  }
  update();
}

void MyGLWidget::animar() {
  makeCurrent();
  updateSnakeGame();
  update();
}






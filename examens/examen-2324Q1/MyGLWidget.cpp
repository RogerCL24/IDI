// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

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

MyGLWidget::MyGLWidget(QWidget *parent) : ExamGLWidget(parent) {

		
}

MyGLWidget::~MyGLWidget() {
}

void MyGLWidget::resizeGL (int w, int h) 
{
#ifdef __APPLE__
  // Aquest codi és necessari únicament per a MACs amb pantalla retina.
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#else
  ample = w;
  alt = h;
#endif

  ra = float(ample)/float(alt);
  if (ra < 1) fov = 2.0 * atan(tan(fovini/2.0)/ra);

  projectTransform();
}

void MyGLWidget::iniEscena ()
{
    creaBuffersModels();

  // inicialitzem tots els possibles objectes (blocs de Lego) com a "buits" --> no es pinten ni tenen valors guardats més que els per defecte
  for (int i = 0; i < NUM_BRICKS; i++)
  {
  	pintarBricks[i] = false;  	
  	brickModelIndex[i] = 1;
  	brickTGs[i] = glm::mat4(1);
  	brickColors[i] = glm::vec3(1,1,1);
  }
  
  // inicialitzem l'objecte actual (el bloc de Lego) a editar
  currentBrickObjectIndex = 0;
  currentBrickModelIndex = 1;

  // inicialitzem el color actual
  currentColor = white;	

  // Inicialitzem els paràmetres de l'escena amb valors arbitraris
  Pmax = glm::vec3(10,9.75,10);
  Pmin = glm::vec3(-10,-1,-10);
  centreEsc = (Pmax + Pmin)/glm::vec3(2.0);
  radiEsc = glm::distance(Pmax,Pmin)/2.0;
  d = 2*radiEsc;
  movX = movY = movZ = angle = 0;
}

void MyGLWidget::iniCamera ()
{
    // Inicialitzem els paràmetres de càmera amb valors arbitraris
  angleX = -float(M_PI) / 12;
  angleY = float(M_PI) / 12;
  ra = float(width())/height();
  fov = 2.0 * asin(radiEsc/d);
  fovini = fov;
  zn = radiEsc;
  zf = 3*radiEsc;
  camera_2 = false;

  left = bottom = -radiEsc;
  right = top = radiEsc;

  projectTransform ();
  viewTransform ();
}

void MyGLWidget::paintGL ()
{
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  
  // Pintem el terra = GROUND_BRICKS
  glm::vec3 red(0.7,0,0);
  glUniform3fv(colorLoc,1,&red[0]);
  glBindVertexArray (VAO_models[GROUND_BRICKS]);
  modelTransformGround();
  glDrawArrays(GL_TRIANGLES, 0, models[GROUND_BRICKS].faces().size()*3);
   
  // Pintem el bloc de Lego actual (currentBrick) del color actual
  glUniform3fv(colorLoc,1,&currentColor[0]);
  glBindVertexArray (VAO_models[currentBrickModelIndex]);
  modelTransformCurrentBrick();
  glDrawArrays(GL_LINES, 0, models[currentBrickModelIndex].faces().size()*3);
  glDrawArrays(GL_TRIANGLES, 0, models[currentBrickModelIndex].faces().size()*3);

  for (int i = 0; i < currentBrickObjectIndex; ++i) {
    glUniform3fv(colorLoc,1,&brickColors[i][0]);
    glBindVertexArray (VAO_models[brickModelIndex[i]]);
    modelTransformBrick(i);
    glDrawArrays(GL_TRIANGLES, 0, models[brickModelIndex[i]].faces().size()*3);
  }
  
  glBindVertexArray(0);
}


void MyGLWidget::modelTransformCurrentBrick()
{
  TG = glm::mat4(1.f);
  if (currentBrickModelIndex != 1) TG = glm::translate(TG, glm::vec3(movX,movY,movZ)); 
  else  TG = glm::translate(TG, glm::vec3(abs(cos(angle)/2) + movX,movY,movZ + abs(sin(angle)/2)));
  TG = glm::rotate(TG, angle,glm::vec3(0,1,0));
  TG = glm::rotate(TG, float(M_PI/2),glm::vec3(1,0,0));
  TG = glm::scale(TG, glm::vec3 (escalaModels[currentBrickModelIndex], escalaModels[currentBrickModelIndex], escalaModels[currentBrickModelIndex]));
  TG = glm::translate(TG, -centreCapsaModels[currentBrickModelIndex]);  
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()
{
  View = glm::translate(glm::mat4(1.f), glm::vec3(0, 0, -d));
  View = glm::rotate(View, -angleX, glm::vec3(1, 0, 0)); 
  View = glm::rotate(View, -angleY, glm::vec3(0, 1, 0));
  View = glm::translate(View, -centreEsc);

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj;  // Matriu de projecció
  if (!camera_2) Proj = glm::perspective(fov, ra, zn, zf);
  else Proj = glm::ortho(left, right, bottom, top, zn, zf);

  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_M: { // al prèmer la tecla M canviem el model del bloc que estem editant
	    	currentBrickModelIndex++;
	    	if (currentBrickModelIndex == NUM_MODELS)
		    	currentBrickModelIndex = 1;
          emit modelChanged(currentBrickModelIndex);
	    	break;
  }
  case Qt::Key_A: {
    if (movX > -10) movX -= 1;
    break;
	}
  case Qt::Key_D: {
    if (movX < 10) movX += 1;
    break;
    }
  case Qt::Key_W: {
    if (movZ < 10) movZ += 1;
    break;
	}
  case Qt::Key_S: {
    if (movZ > -10) movZ -= 1;
    break;  
	}
  case Qt::Key_Up: {
    if (movY < 10) movY += 1.25;
    break;
	}
  case Qt::Key_Down: {
    if (movY > 0) movY -= 1.25  ;
    break;  
	}		
  case Qt::Key_Q: {
    angle += float(M_PI)/2.0;
    break;
	}
  case Qt::Key_C: {
    camera_2 = !(camera_2);
    projectTransform();
    break;
	}
  case Qt::Key_Space: {
    brickModelIndex[currentBrickObjectIndex] = currentBrickModelIndex;
    brickTGs[currentBrickObjectIndex] = TG;
    brickColors[currentBrickObjectIndex] = currentColor;
    pintarBricks[currentBrickObjectIndex] = false;
    if (currentBrickObjectIndex < 50) ++currentBrickObjectIndex;
    break;
    }
  case Qt::Key_R: {
    break;
	}
  default: ExamGLWidget::keyPressEvent(event); break;
  }
  
  update();
}


void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació (només en Y)
    angleY += (e->x() - xClick) * M_PI / ample;
    angleX += (e->y() - yClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::changeModel(int i) {
  makeCurrent();
  currentBrickModelIndex = i;
  update();
}

void MyGLWidget::paintRed(int i) {
  makeCurrent();
  currentColor.x = float(i)/100;
  update();
}

void MyGLWidget::paintGreen(int i) {
  makeCurrent();
  currentColor.y = float(i)/100;
  update();
}

void MyGLWidget::paintBlue(int i) {
  makeCurrent();
  currentColor.z = float(i)/100; 
  update();
}


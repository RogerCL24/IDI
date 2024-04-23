// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

using namespace std;

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

MyGLWidget::~MyGLWidget() {
    lego_pj = false;
}

void MyGLWidget::carregaShaders() { // declarem-lo també en MyGLWidget.h
    BL2GLWidget::carregaShaders(); // cridem primer al mètode de BL2GLWidget
    projLoc = glGetUniformLocation (program->programId(), "proj");
    viewLoc = glGetUniformLocation (program->programId(), "view");
}

void MyGLWidget::modelTransformLego() 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, angle, glm::vec3(0, 1, 0));
  transform = glm::translate(transform, glm::vec3(0.0,-1.0,0.0));
  transform = glm::scale(transform, glm::vec3(escala*scale));
  transform = glm::translate(transform, -centreBase);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformPatricio() 
{
  // Matriu de transformació de model
  glm::mat4 transform (1.0f);
  transform = glm::rotate(transform, angle, glm::vec3(0, 1, 0));
  transform = glm::translate(transform, glm::vec3(0.0,-1.0,0.0));
  transform = glm::scale(transform, glm::vec3(escala*scale));
  transform = glm::translate(transform, -centreBase);
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::modelTransformTerra()
{
  glm::mat4 transform (1.0f);
  transform = glm::scale(transform, glm::vec3(1.0f));
  transform = glm::translate(transform, glm::vec3(0,-1,0));
  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::calcula() {
  escala = scale = 1.0f;
  radi = 0.0f;
  calculaCapsaEscena();
  calculaCapsaModel();
  calculaCentreEscena();
  calculaRadiEscena();

  camera();
}


void MyGLWidget::initializeGL() { // declarem-lo també en MyGLWidget.h
    // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();  

  glClearColor(0.5, 0.7, 1.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();

  glEnable (GL_DEPTH_TEST);

  calcula();
 
}

void MyGLWidget::paintGL() { // declarem-lo també en MyGLWidget.h
    // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
// useu els paràmetres que considereu (els que hi ha són els de per defecte)
//  glViewport (0, 0, ample, alt);
  
  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

if (lego_pj) {
  modelTransformLego();
  glBindVertexArray (VAO_lego);
  glDrawArrays (GL_TRIANGLES, 0, Lego.faces().size() * 3);
}
else {
  modelTransformPatricio();
  glBindVertexArray (VAO_Patricio);
  glDrawArrays (GL_TRIANGLES, 0, Patricio.faces().size() * 3);
}

  glBindVertexArray (0);

  modelTransformTerra();

  glBindVertexArray(VAO_terra);

  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  glBindVertexArray(0);
}

void MyGLWidget::camera() {
    distancia = radi * 2.0;
    FOV = 2.0*asin(radi/distancia);
    FOV_prev = FOV;
    raw = 1.0f;
    Znear = distancia - radi;
    Zfar = distancia + radi;

    left = bottom = -radi;
    right = top = radi;

    //OBS = glm::vec3(0,2.0,distancia);
    //VRP = glm::vec3(centreEscena);
    //UP = glm::vec3(0,1,0);


    girTheta = float(M_PI/8.); //22.5º
    girPsi = 0.0;



    projectTransform();
    viewTransform();
}

void MyGLWidget::projectTransform () {
// glm::perspective (FOV en radians, ra window, znear, zfar)
    glm::mat4 Proj;
    if (perspective) Proj = glm::perspective (FOV, raw, Znear, Zfar);
    else Proj = glm::ortho(left, right, bottom, top, Znear, Zfar);
    glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::viewTransform () {
    //glm::mat4 View = glm::lookAt (OBS, VRP, UP);
    glm::mat4 View(1.0);
    View = glm::translate(View, glm::vec3(0,0,-distancia));
    View = glm::rotate(View, girTheta, glm::vec3(1,0,0));
    View = glm::rotate(View, -girPsi, glm::vec3(0,1,0));
    View = glm::translate(View, -centreEscena);
    glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::creaBuffers ()  {
    Patricio.load("../models/Patricio.obj");

    glGenVertexArrays(1, &VAO_Patricio);
    glBindVertexArray(VAO_Patricio);

    GLuint VBO1;
    glGenBuffers(2, &VBO1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);

    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * Patricio.faces ().size () * 3 * 3,Patricio.VBO_vertices (), GL_STATIC_DRAW);

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);


    GLuint VBO2;
    glGenBuffers(2, &VBO2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);

    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * Patricio.faces ().size () * 3 * 3, Patricio.VBO_matdiff (), GL_STATIC_DRAW); // color

    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);

    Lego.load("../models/legoman.obj");

    glGenVertexArrays(1, &VAO_lego);
    glBindVertexArray(VAO_lego);

    GLuint VBO5;
    glGenBuffers(2, &VBO5);
    glBindBuffer(GL_ARRAY_BUFFER, VBO5);

    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * Lego.faces ().size () * 3 * 3,Lego.VBO_vertices (), GL_STATIC_DRAW);

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);


    GLuint VBO6;
    glGenBuffers(2, &VBO6);
    glBindBuffer(GL_ARRAY_BUFFER, VBO6);

    glBufferData (GL_ARRAY_BUFFER, sizeof(GLfloat) * Lego.faces ().size () * 3 * 3, Lego.VBO_matdiff (), GL_STATIC_DRAW); // color

    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    glBindVertexArray (0);

    glm::vec3 Vertices[4];

    Vertices[0] = glm::vec3(-2.5, 0.0, -2.5); //frontal esquerra
    Vertices[1] = glm::vec3(2.5,0.0,-2.5);
    Vertices[2] = glm::vec3(-2.5,0.0, 2.5);
    Vertices[3] = glm::vec3(2.5,0.0, 2.5);

    glGenVertexArrays(1, &VAO_terra);
    glBindVertexArray(VAO_terra);


    GLuint VBO3;
    glGenBuffers(2, &VBO3);
    glBindBuffer(GL_ARRAY_BUFFER, VBO3);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    glm::vec3 Colors[4];
    Colors[0] = glm::vec3(128./255., 77./255., 46./255.); //color marro
    Colors[1] = glm::vec3(128./255., 77./255., 46./255.); 
    Colors[2] = glm::vec3(128./255., 77./255., 46./255.); 
    Colors[3] = glm::vec3(128./255., 77./255., 46./255.);

    GLuint VBO4;
    glGenBuffers(2, &VBO4);
    glBindBuffer(GL_ARRAY_BUFFER, VBO4);

    glBufferData(GL_ARRAY_BUFFER, sizeof(Colors), Colors, GL_STATIC_DRAW);

    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

}

void MyGLWidget::keyPressEvent(QKeyEvent *event) {

    makeCurrent();

    switch (event -> key()) {

        case Qt::Key_S :  // escalar a més gran
            escala += 0.05;
            break;

        case Qt::Key_D :  // escalar a més petit
            escala -= 0.05;
            break;

        case Qt::Key_R :
            angle += M_PI/4;
            break;
        case Qt::Key_O :
            if (perspective) perspective = false;
            else perspective = true;
            projectTransform();
            break;
        case Qt::Key_Z:
            FOV -= 0.1;
            left /= 1.5;
            right /= 1.5;
            bottom /= 1.5;
            top /= 1.5;
            projectTransform();
            break;
        case Qt::Key_X: 
            FOV += 0.1;
            left *= 1.5;
            right *= 1.5;
            bottom *= 1.5;
            top *= 1.5;
            projectTransform();
            break;
        default: event->ignore(); break;
    }
    update();
}

void MyGLWidget::calculaCapsaModel() {
    float minx, miny, minz, maxx, maxy, maxz;
    if (lego_pj) {
        minx = maxx = Lego.vertices()[0];
        miny = maxy = Lego.vertices()[1];
        minz = maxz = Lego.vertices()[2];

        for (unsigned int i = 3; i < Lego.vertices().size(); i += 3) {
            if (Lego.vertices()[i] < minx) minx = Lego.vertices()[i];
            if (Lego.vertices()[i] > maxx) maxx = Lego.vertices()[i];
            if (Lego.vertices()[i + 1] < miny) miny = Lego.vertices()[i + 1];
            if (Lego.vertices()[i + 1] > maxy) maxy = Lego.vertices()[i + 1];
            if (Lego.vertices()[i + 2] < minz) minz = Lego.vertices()[i + 2];
            if (Lego.vertices()[i + 2] > maxz) maxz = Lego.vertices()[i + 2];
        }
    }
    else {
         minx = maxx = Patricio.vertices()[0];
        miny = maxy = Patricio.vertices()[1];
        minz = maxz = Patricio.vertices()[2];
        for (unsigned int i = 3; i < Patricio.vertices().size(); i += 3) {
            if (Patricio.vertices()[i] < minx) minx = Patricio.vertices()[i];
            if (Patricio.vertices()[i] > maxx) maxx = Patricio.vertices()[i];
            if (Patricio.vertices()[i + 1] < miny) miny = Patricio.vertices()[i + 1];
            if (Patricio.vertices()[i + 1] > maxy) maxy = Patricio.vertices()[i + 1];
            if (Patricio.vertices()[i + 2] < minz) minz = Patricio.vertices()[i + 2];
            if (Patricio.vertices()[i + 2] > maxz) maxz = Patricio.vertices()[i + 2];
        }
    }
    


  //cout << "MIN(x,y,z) = (" << minx << "," << miny << "," << minz << ")" << endl;            
  //cout << "MAX(x,y,z) = (" << maxx << "," << maxy << "," << maxz << ")" << endl;

  scale = 4.0/(maxy - miny);
  centreBase = glm::vec3((minx+maxx)/2.0, miny, (minz+maxz)/2.0);
  //cout << "Centre Base: (" << centreBase.x << " " << centreBase.y << " " << centreBase.z << ")" << endl;
  maximEscena.y = 4.0;    
  
}

void MyGLWidget::calculaCapsaEscena() {
    maximEscena.x = 2.5;
    maximEscena.z = 2.5;

    minimEscena = glm::vec3(-2.5,0.0,-2.5);
}

void MyGLWidget::calculaCentreEscena() {
    centreEscena = (maximEscena + minimEscena);
    centreEscena = centreEscena / glm::vec3(2.0);

    //cout << "Centre escena: (" << centreEscena[0] << ", " << centreEscena[1] << ", " << centreEscena[2] << ")" << endl;
}

void MyGLWidget::calculaRadiEscena() {
    radi = distance(minimEscena, maximEscena)/2;

    //cout << "Radi: " << radi << endl;
}

void MyGLWidget::resizeGL(int w, int h) {
    float rav = float(w) / float(h);
    raw = rav;
    if (perspective) {
        if (rav < 1.0) FOV = 2.0*atan(tan(FOV_prev/2.0)/rav);
    }
    else { //ortho
        if (rav > 1.0) {
            left = -radi*rav;
            right = radi*rav;
        }
        else {
            bottom = -radi/rav;
            top = radi/rav;
        }
    }
    

    projectTransform();
    glViewport(0,0,w,h);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e) {
    makeCurrent();
    if (e->x() > x_ant) girPsi -= 0.03;
    else if (e->x() < x_ant) girPsi += 0.03;

    if (e->y() > y_ant) girTheta -= 0.03;
    else if (e->y() < y_ant) girTheta += 0.03;

    x_ant = e->x();
    y_ant = e->y();
    viewTransform();
    update();
}

void MyGLWidget::controlFOV(int n) {                            // Exercici 1   
    makeCurrent();
    FOV -= float(n - dif)/360 * float(M_PI);
    dif = n; 
    projectTransform();
    update();
}

void MyGLWidget::SelectCharacter()  {               // Exercici 2
    makeCurrent();
    lego_pj = !(lego_pj);
    calcula();
    paintGL();
    update();
}

void MyGLWidget::Optic() {                          // Exercici 3
    makeCurrent();
    perspective = !(perspective);
    projectTransform();
    update();
}


void MyGLWidget::controlScale(int n) {               // Exercici 4
    makeCurrent();
    escala += (float(n) - float(dif))/2;
    dif = n;
    update();

}








#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <vector>

using namespace std;

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Es cridat quan es prem una tecla
    virtual void keyPressEvent (QKeyEvent *event);

  private:
    
    void creaBuffersRectangle();

    void creaBuffersCano();

    void creaBuffersRoda();
    
    void carregaShaders();
    
    void modelTransformQuadrat(glm::vec3 posicio, glm::vec3 escala);

    void modelTransformQuadratCano();

    void modelTransformQuadratRoda(float shift, float angle);

    void pintaTanc();
    void pintaCos();
    void pintaCano();
    void pintaRodes();
    
    // program
    QOpenGLShaderProgram *program;
    // attribute locations
    GLuint vertexLoc, colorLoc, rotLoc;
    // uniform locations
    GLuint TGLoc;

    // VAOs
    GLuint VAORec, VAOCano, VAOGW, VAOBW;
    // viewport
    GLint ample, alt;    

    float rotacio = 0.0, tx = 0.0, rotacio2 = 0.0;

    float vecRot[6] = {0.0,M_PI/3,2*(M_PI/3),M_PI, 4*M_PI/3, 5*M_PI/3};
    float shift_x[4] = {0.0,0.25, 0.5, 0.75};
    
    // colors
    glm::vec3 verd = glm::vec3(0.0,0.6,0.0);
    glm::vec3 negre = glm::vec3(0.0);    
    glm::vec3 gris = glm::vec3(0.5,0.5,0.5);
};

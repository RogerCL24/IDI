#define GLM_FORCE_RADIANS
#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "glm/gtc/matrix_transform.hpp"
#define GLM_FORCE_RADIANS

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
    
    void creaBuffersQuadrat();
    
    void carregaShaders();
    
    void modelTransformQuadrat(glm::vec3 posicio, glm::vec3 escala);

    void modelTransformQuadratCano(glm::vec3 escala);

    void modelTransformQuadratRoda(glm::vec3 escala, float shift, float angle);

    void pintaTanc();
    void pintaCos();
    void pintaCano();
    void pintaRoda();
    
    // program
    QOpenGLShaderProgram *program;
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniform locations
    GLuint TGLoc;

    // VAOs
    GLuint VAOQuadrat;

    // viewport
    GLint ample, alt;    

    float tx = 0.0, rotacio2 = 0.0, rotacio = 0.0;

    float shift_x[4] = {0.0,0.25, 0.5, 0.75};
    float vecRot[12] = {0.0,M_PI/6,M_PI/3,M_PI/2, 2*(M_PI/3),5*M_PI/6,M_PI,7*M_PI/6 ,4*M_PI/3, 3*M_PI/2, 5*M_PI/3, 11*M_PI/6};
    
    // colors
    glm::vec3 verd = glm::vec3(0.0,0.6,0.0);
    glm::vec3 negre = glm::vec3(0.0);    
    glm::vec3 gris = glm::vec3(0.5,0.5,0.5);
};

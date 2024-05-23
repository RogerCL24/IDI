// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();
    
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformTerra ();
    virtual void iniCamera();
    virtual void iniEscena ();
    virtual void projectTransform ();
    virtual void viewTransform();
    virtual void paintGL ();
    virtual void modelTransformRoad (glm::vec3 pos, float angle);
    virtual void modelTransformPipe ();
    virtual void modelTransformCar (float radi, float angle, float X, float Z);
    virtual void carregaShaders();
    virtual void initializeGL ();
    virtual void iniMaterialTerra ();

  public slots:
    void animar();

  private:
    int printOglError(const char file[], int line, const char func[]);
    float distancia, fov, znear, zfar;
    float minx, minz, miny, maxz, maxx, maxy;
    float angleX;
    float trackX[4] = {5,-5,-5, 5};           
    float trackY[4] = {-5,-5,5, 5}; 
    float carDist[2] = {6.5, 9};
    float carAngle[2] = {0.0, 0.0};
    float driveCar[4] = {0.0, 0.0, 0.0 ,0.0};
    float driveLight[4] = {0.0, 0.0, 0.0, 0.0};
    GLuint paintCarLoc, posFocusLoc, colorFocusLoc, posFocusC1Loc, posFocusC2Loc, colorFocusCLoc, transFocus1Loc, transFocus2Loc;
    int paintC;
    bool clock, shut_down;

    QTimer timer;

    void drive();
    void modelTransformFocus1(float X, float Y);
    void modelTransformFocus2(float X, float Y);

    glm::vec3 maxim, minim,colorFocus, posFocus, colorFocusC, posFocusC1, posFocusC2;
};

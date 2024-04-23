// MyGLWidget.h
#include "BL2GLWidget.h"
#include "model.h"


class MyGLWidget : public BL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0) : BL2GLWidget(parent) {}
    ~MyGLWidget();

  protected: 
  virtual void carregaShaders();
  virtual void initializeGL();
  virtual void paintGL();
  virtual void creaBuffers();
  virtual void keyPressEvent(QKeyEvent *event);
  virtual void modelTransformPatricio();
  virtual void modelTransformTerra();
  virtual void resizeGL(int w, int h);
  virtual void mouseMoveEvent(QMouseEvent *e);



  private:
    int printOglError(const char file[], int line, const char func[]);
    void projectTransform ();
    void viewTransform();
    void calculaCapsaModel();
    void calculaCapsaEscena();
    void calculaCentreEscena();
    void calculaRadiEscena();
    void camera();
    void modelTransformLego();
    void calcula();

    GLuint projLoc, viewLoc;
    Model Patricio, Lego;
    float angle = 0.0, FOV, raw, Znear, Zfar, radi, distancia, FOV_prev, scale;
    float left, bottom, right, top;
    glm::vec3 pos, centreEscena, maximEscena, minimEscena, centreBase;
    int perspective = true;
    float girTheta, girPsi, x_ant, y_ant;
    int dif = 0;
    int dif_2 = 0;
    bool lego_pj;

    GLuint VAO_Patricio, VAO_terra, VAO_lego;

  public slots:
    void controlFOV(int n);             //Exercici 1
    void SelectCharacter();             //Exercici 2
    void Optic();                       //Exercici 3
    void controlScale(int n);           //Exercici 4

};

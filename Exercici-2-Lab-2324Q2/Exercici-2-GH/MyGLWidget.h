#include "LL2GLWidget.h"

#include <vector>

#include <QTimer>

class MyGLWidget : public LL2GLWidget {
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent);
    ~MyGLWidget();

  protected:
  
    virtual void keyPressEvent (QKeyEvent *event);

    // funció per comprovar si una nova posició de la serp és vàlida
    virtual bool checkPosition(glm::vec3 pos);
    virtual void TerraTransform();
    virtual void PipeTransform (glm::vec3 pos);
    virtual void paintGL ();
    virtual void viewTransform ();
    virtual void iniCamera();
    virtual void iniEscena();
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void MarbleTransform ();
    virtual void SnakeHeadTransform ();
    virtual void SnakeTailTransform ();
    virtual void projectTransform ();
    virtual void resizeGL (int w, int h);
    virtual void initializeGL ();

  public slots:
    void animar();

  private:

    void resizeOrtho();
    int printOglError(const char file[], int line, const char func[]);
    float Theta, Psi;
    float dist, left, right, bottom, top;
    float x_anterior, y_anterior;
    float minx, miny, minz, maxx, maxy, maxz;
    int perspectiva = true, clock = false;
    glm::vec3 max, min;
    QTimer timer;
   
};

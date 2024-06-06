#include "ExamGLWidget.h"

class MyGLWidget:public ExamGLWidget
{
  Q_OBJECT

  public:
    MyGLWidget(QWidget *parent=0);
    ~MyGLWidget();

  protected:
    virtual void paintGL ();
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void mouseMoveEvent (QMouseEvent *event);

    virtual void iniEscena ();
    virtual void iniCamera ();
    virtual void projectTransform ();
    virtual void viewTransform ();
    
    virtual void modelTransformCurrentBrick();

    virtual void resizeGL (int w, int h);
  public slots:
    
  signals:


  private:
    int printOglError(const char file[], int line, const char func[]);

    float d, fovini;
    glm::vec3 Pmin, Pmax;
    float movX, movZ, movY, angle;
    glm::mat4 TG;
    float left, right, bottom, top;
    bool camera_2;

  public slots:
    void changeModel(int i);
    void paintRed(int i);
    void paintGreen(int i);
    void paintBlue(int i);

  signals:
    void modelChanged(int i);

    
};

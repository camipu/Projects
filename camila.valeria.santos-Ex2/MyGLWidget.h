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
    virtual void paintGL ( );
    virtual void modelTransformPatricio (glm::vec3 pos);
    virtual void modelTransformTerra ();
    virtual void viewTransform ();
    virtual void projectTransform ();
    virtual void modelTransformCub (int aux);
    virtual void modelTransformLegoMan (glm::vec3 pos);
    virtual void mouseMoveEvent (QMouseEvent *event);

  private:
  
    int printOglError(const char file[], int line, const char func[]);
    glm::vec3 VRP = glm::vec3(0,2,0);
    int radi = glm::distance(glm::vec3(0,2,0),glm::vec3(-12,0,8));
    float theta = float(M_PI/4.f);
    float psi = 0.0f;
    bool perspectiva = true;
    bool first = true;

public slots:
  void animar();
};

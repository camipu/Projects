// MyGLWidget.h
#include "LL4GLWidget.h"
#include <QTimer>

class MyGLWidget : public LL4GLWidget {
  Q_OBJECT
  public:
    MyGLWidget(QWidget *parent=0) : LL4GLWidget(parent) {}
    ~MyGLWidget();
    void encenderApagar();
public slots:
    void animar();
  protected:
    virtual void mouseMoveEvent(QMouseEvent *e);
    virtual void keyPressEvent(QKeyEvent* event);
    virtual void modelTransformDiscoBall (float angleDisco);
    virtual void modelTransformMorty (float angle);
    virtual void modelTransformFantasma (float posX, float angle);
    virtual void initializeGL ();
    virtual void paintGL ();
    float angleDisco = 0.0f;
    float angleDancers = 0.0f;
    GLuint locTGBola, LocBoolLuz, LocVecLuz;
    bool LuzEscenaOn = true;
    glm::vec4 luzBool;
    GLint pos = 0;
    bool ida = true;
    QTimer timer;
    bool anima = false;

  private:
    int printOglError(const char file[], int line, const char func[]);
};

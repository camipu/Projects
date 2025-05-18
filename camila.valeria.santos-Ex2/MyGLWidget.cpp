#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

#define printOpenGLError() printOglError(__FILE__, __LINE__)
#define CHECK() printOglError(__FILE__, __LINE__,__FUNCTION__)
#define DEBUG() std::cout << __FILE__ << " " << __LINE__ << " " << __FUNCTION__ << std::endl;

MyGLWidget::MyGLWidget(QWidget *parent=0) : LL2GLWidget(parent) 
{
}

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

MyGLWidget::~MyGLWidget()
{

}
void  MyGLWidget::paintGL ()
{
  // descomentar per canviar paràmetres
  //glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  if (first) {
    connect (&timer, SIGNAL (timeout()), this, SLOT (animar ()));
    first = false;
  }


  // Patricio
  glBindVertexArray (VAO_models[LEGO]);
  modelTransformLegoMan(posPorter);
  glDrawArrays(GL_TRIANGLES, 0, models[PATRICIO].faces().size()*3);

  // Pilota
  glBindVertexArray (VAO_models[PILOTA]);
  modelTransformPilota ();
  glDrawArrays(GL_TRIANGLES, 0, models[PILOTA].faces().size()*3);

  // Pared 1
  glBindVertexArray (VAO_Cub);
  modelTransformCub (int(1));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  // Pared 2
  glBindVertexArray (VAO_Cub);
  modelTransformCub (int(2));
  glDrawArrays(GL_TRIANGLES, 0, 36);

  //   // Pared 3
  glBindVertexArray (VAO_Cub);
  modelTransformCub (3);
  glDrawArrays(GL_TRIANGLES, 0, 36);

// Terra
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  viewTransform ();

  glBindVertexArray (0);
}

void MyGLWidget::modelTransformPatricio (glm::vec3 pos)
{
  glm::mat4 TG(1.0f);
  TG = glm::translate(TG, pos);
  TG = glm::rotate(TG,float(M_PI/2.f),glm::vec3(0,-1,0));
  TG = glm::translate(TG, -centreBaseModels[PATRICIO]);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformLegoMan (glm::vec3 pos)
{
  glm::mat4 TG(1.0f);
  //TG = glm::translate(TG, pos);
  //TG = glm::rotate(TG,float(M_PI/2.f),glm::vec3(0,-1,0));
  TG = glm::translate(TG, pos);
  TG = glm::rotate(TG,float(M_PI/2.f),glm::vec3(0,-1,0));
  TG = glm::scale(TG,glm::vec3(escalaModels[LEGO]));
  TG = glm::translate(TG, -centreBaseModels[LEGO]);

  glUniformMatrix4fv(transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformCub (int aux)
{
  glm::mat4 TG(1.0f);
  if (aux == 1) {
    TG = glm::translate(TG,glm::vec3(0,0,-7.9));
    TG = glm::scale(TG,glm::vec3(24,2,0.2));
  }
  else if (aux == 2) {
    TG = glm::translate(TG,glm::vec3(0,0,7.9));
    TG = glm::scale(TG,glm::vec3(24,2,0.2));
  }
  else {
    TG = glm::translate(TG,glm::vec3(-11.9,0,0));
    TG = glm::scale(TG,glm::vec3(0.2,2,16));

  }
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformTerra ()
{
  glm::mat4 TG(1.0f);
  TG = glm::scale(TG, glm::vec3(3,0,2));
  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::viewTransform ()
{
  // Matriu de posició i orientació de l'observador
  glm::mat4 View(1.0f);
  //View = glm::lookAt (glm::vec3(0,0,2*radiEscena), glm::vec3(0,2,0), glm::vec3(0,1,0));
  if (perspectiva) {
    View = glm::translate(View,glm::vec3(0.0f,0.0f,-2.0f*glm::distance(glm::vec3(0,2,0),glm::vec3(-12,0,8))));

    View = glm::rotate(View,theta,glm::vec3(1.0f,0.0f,0.0f));

    View = glm::rotate(View,-psi,glm::vec3(0.0f,1.0f,0.0f));

    View = glm::translate(View,-VRP);
  }
  else {
      View = glm::lookAt (glm::vec3(0,18,0),VRP, glm::vec3(-1,0,0));
  }

  glUniformMatrix4fv (viewLoc, 1, GL_FALSE, &View[0][0]);
}

void MyGLWidget::projectTransform ()
{
  glm::mat4 Proj(1.0f);
  if (perspectiva) Proj = glm::perspective (float(M_PI/3.0), 1.0f, 0.2f, 100.0f);
  else {
    Proj = glm::ortho (-16.0f, 16.0f, -16.0f, 16.0f, 0.2f, 100.0f);
  }
  glUniformMatrix4fv (projLoc, 1, GL_FALSE, &Proj[0][0]);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();

  if (perspectiva) {
    psi -= (e->x() - xClick) *factorAngleY;
    theta += (e->y() - yClick) *factorAngleX;
  }

  xClick = e->x();
  yClick = e->y();

  viewTransform ();

  update ();
}

void MyGLWidget::animar() {
  makeCurrent ();
  posPilota += dirPilota*0.5f;


  if (rebotaPorter()) canviaDireccio();


  if (posPilota.x <= -11) dirPilota[0] = -dirPilota[0];
  if (posPilota.z >= 7 or posPilota.z <= -7) dirPilota[2] = -dirPilota[2];
  if (posPilota.x >= 20) {
    timer.stop ();
    posPilota = glm::vec3(9.0, 0.0, 0.0);
    LL2GLWidget::dirInicialPilota();
  }


  update ();

}

void MyGLWidget::keyPressEvent(QKeyEvent* event) 
{
  makeCurrent();
  switch (event->key()) {
    case Qt::Key_Up: { 
      timer.start (100);
      break;
    }
    case Qt::Key_I: { 
      break;
        }           
    case Qt::Key_Left: { 
      if (glm::distance(posPorter,glm::vec3(11,0,6)) != 0) {
        posPorter = posPorter + glm::vec3(0,0,0.5);
      }
      break;
        }  
    case Qt::Key_Right: { 
      if (glm::distance(posPorter,glm::vec3(11,0,-6)) != 0) {
        posPorter = posPorter - glm::vec3(0,0,0.5);
      }
      break;
        }
    case Qt::Key_C: { 
      if (perspectiva) perspectiva = false;
      else perspectiva = true;
      viewTransform();
      projectTransform ();
      break;
        }           
    case Qt::Key_R: { 
      perspectiva = true;
      posPilota = glm::vec3(9.0, 0.0, 0.0);
      theta = float(M_PI/4.f);
      psi = 0;
      posPorter = glm::vec3(11.0, 0.0, 0.0);

      viewTransform();
      projectTransform();

      break;
        }  
    default: event->ignore(); break;
  }
  update();
}


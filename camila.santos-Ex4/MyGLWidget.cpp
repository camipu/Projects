// MyGLWidget.cpp
#include "MyGLWidget.h"
#include <iostream>
#include <stdio.h>

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
}

void MyGLWidget::initializeGL (){
    LL4GLWidget::initializeGL();
    locTGBola = glGetUniformLocation (program->programId(), "TGbola");
    LocVecLuz = glGetUniformLocation (program->programId(), "luzBool");
    LocBoolLuz = glGetUniformLocation (program->programId(), "LuzEscenaOn");

    luzBool = glm::vec4(1.0,1.0,1.0,1.0);

    glUniform4fv(LocVecLuz, 1, &luzBool[0]);

    glUniform1i(LocBoolLuz, LuzEscenaOn);

    connect (&timer, SIGNAL (timeout()), this, SLOT (animar ()));
}

//pintar la DiscoBall 5 posiciones mas arriba
void MyGLWidget::modelTransformDiscoBall (float angleDisco)
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,5,5));
  TG = glm::scale(TG, glm::vec3(escalaDiscoBall, escalaDiscoBall, escalaDiscoBall));
  TG = glm::rotate(TG, float(glm::radians(angleDisco)),glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseDiscoBall);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);

  glUniformMatrix4fv (locTGBola, 1, GL_FALSE, &TG[0][0]);
}

//muevo al Morty a la posicion inicial del discoball
void MyGLWidget::modelTransformMorty (float angle)
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.f), glm::vec3(5,0,5));
  TG = glm::scale(TG, glm::vec3(escalaMorty, escalaMorty, escalaMorty));
  TG = glm::rotate(TG, float(glm::radians(angle)),glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseMorty);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::modelTransformFantasma (float posX, float angle)
{
  glm::mat4 TG;
  TG = glm::translate(glm::mat4(1.0f), glm::vec3(posX,0.5,5));
  TG = glm::scale(TG, glm::vec3(escalaFantasma, escalaFantasma, escalaFantasma));
  TG = glm::rotate(TG, float(glm::radians(angle)),glm::vec3(0,1,0));
  TG = glm::translate(TG, -centreBaseFantasma);

  glUniformMatrix4fv (transLoc, 1, GL_FALSE, &TG[0][0]);
}

void MyGLWidget::paintGL () {
  // En cas de voler canviar els paràmetres del viewport, descomenteu la crida següent i
  // useu els paràmetres que considereu (els que hi ha són els de per defecte)
  // glViewport (0, 0, ample, alt);

  // Esborrem el frame-buffer i el depth-buffer
  glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TERRA
  glBindVertexArray (VAO_Terra);
  modelTransformTerra ();
  glDrawArrays(GL_TRIANGLES, 0, 30);

  // MORTY
  glBindVertexArray (VAO_Morty);
  modelTransformMorty (angleDancers);
  glDrawArrays(GL_TRIANGLES, 0, morty.faces().size()*3);

  // FANTASMA1
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (1.0f,angleDancers);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

    // FANTASMA2
  glBindVertexArray (VAO_Fantasma);
  modelTransformFantasma (9.0f,angleDancers);
  glDrawArrays(GL_TRIANGLES, 0, fantasma.faces().size()*3);

  // DISCO BALL
  glBindVertexArray (VAO_DiscoBall);
  modelTransformDiscoBall (angleDisco);
  glDrawArrays(GL_TRIANGLES, 0, discoBall.faces().size()*3);

  glBindVertexArray(0);
}

void MyGLWidget::mouseMoveEvent(QMouseEvent *e)
{
  makeCurrent();
  // Aqui cal que es calculi i s'apliqui la rotacio o el zoom com s'escaigui...
  if (DoingInteractive == ROTATE)
  {
    // Fem la rotació
    angleY += (e->x() - xClick) * M_PI / ample;
    viewTransform ();
  }

  xClick = e->x();
  yClick = e->y();

  update ();
}

void MyGLWidget::animar() {
    makeCurrent();
    encenderApagar();
    angleDisco += 5.0f;
    update();
}

void MyGLWidget::encenderApagar(){
    if (ida) {
      luzBool[pos] = 0;
      ++pos;
      if (pos == 4) {
        pos = 3;
        ida = false;
      }
    }
    else {
      luzBool[pos] = 1;
      --pos;
      if (pos == -1) {
        pos = 0;
        ida = true;
      }
    }

    glUniform4fv(LocVecLuz, 1, &luzBool[0]);

}

void MyGLWidget::keyPressEvent(QKeyEvent* event) {
  makeCurrent();
  switch (event->key()) {
  case Qt::Key_A: {
      angleDancers -= 45.0f;
    break;
	}
  case Qt::Key_D: {
      angleDancers += 45.0f;
    break;
	}
  case Qt::Key_E: {
    makeCurrent();
      if (LuzEscenaOn) LuzEscenaOn = false;
      else LuzEscenaOn = true;
      glUniform1i(LocBoolLuz, LuzEscenaOn);
      update();
    break;
	}
  case Qt::Key_B: {
      encenderApagar();
      update();
    break;
	}	
  case Qt::Key_R: {
     angleDisco += 5.0f;
    break;
	}
  case Qt::Key_S: {
    if (not anima) {
      anima = true;
      timer.start(100);
    }
    else {
      anima = false;
      timer.stop();
    }
    break;
	}	
  default: LL4GLWidget::keyPressEvent(event); break;
  }
  update();
}

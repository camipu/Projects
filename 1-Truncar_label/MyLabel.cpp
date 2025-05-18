#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent) 
	:QLabel(parent) {
		// Inicialitzacio datributs si cal
}

void MyLabel::copiarText(QString s) {
    corda = s;
}

// implementacio slots
void MyLabel::truncarLabel(int n)
{
    QString s;
    s = corda;
    s.truncate(n);
    setText(s);
}

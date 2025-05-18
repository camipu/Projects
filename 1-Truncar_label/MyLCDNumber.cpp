#include "MyLCDNumber.h"

MyLCDNumber::MyLCDNumber(QWidget *parent)
	:QLCDNumber(parent) {
		// Inicialitzacio datributs si cal
}


void MyLCDNumber::PosarAZero() {
        display(int(0));
}

void MyLCDNumber::ChangeColor(int n) {
    if (n == 0) {
        setStyleSheet("color: green;");
    }
    else if (n%2 == 1) {
        setStyleSheet("color: red;");
    }
    else {
        setStyleSheet("color: blue;");
    }
    display(n);
}

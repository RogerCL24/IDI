#include "MyPushButton.h"


MyPushButton::MyPushButton(QWidget *parent) : QPushButton(parent) {
    qsrand(time(NULL));
    random = qrand()%4;
    if (not random) setStyleSheet("background-color : green;");
    else if (random == 1) setStyleSheet("background-color: black;");
    else if (random == 2) setStyleSheet("background-color: blue;");
    else setStyleSheet("background-color: pink");
}

void MyPushButton::color(int n) {
    random = (qrand() + n)%4;
    if (not random) setStyleSheet("background-color: green;");
    else if (random == 1) setStyleSheet("background-color: black;");
    else if (random == 2) setStyleSheet("background-color: blue;");
    else setStyleSheet("background-color: pink");
}

void MyPushButton::calcRand() {                 //la funcion color calcula nou random
    emit spreadRandom(0);
}



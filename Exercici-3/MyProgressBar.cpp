#include "MyProgressBar.h"
#include <cmath>

MyProgressBar::MyProgressBar(QWidget *parent) : QProgressBar(parent) {
    clock = false;
    connect(&timer,SIGNAL(timeout()), this, SLOT(animate()));
}

void MyProgressBar::inicio() {

    wideness = 0.5;
    phase = 0;
    x = value();                            //Iniciar los valores desde Qt_designer

   int fx = (wideness * sin(double(x + phase)))*100;
   setValue(fx);
}

void MyProgressBar::cambiarAmp(int n) {

    wideness = float(n)/100;

    int fx = (wideness * sin(double(x + phase)))*100;
    setValue(fx);
}

void MyProgressBar::cambiarFase(int n) {

    phase = float(n)/10;

    int fx = (wideness * sin(double(x + phase)))*100;
    setValue(fx);

}

void MyProgressBar::Alwaqt() {
    if (!clock) {
        timer.start(100);
        clock = true;
    }
    else {
        timer.stop();
        clock = false;
    }
}

void MyProgressBar::animate() {
    ++shift_x;
    int fx = (wideness * sin(double(x + shift_x + phase)))*100;
    setValue(fx);
}
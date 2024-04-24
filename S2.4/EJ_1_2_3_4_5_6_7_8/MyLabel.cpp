#include "MyLabel.h"
#include <QPalette>

MyLabel::MyLabel(QWidget *parent):QLabel(parent) {
    R = 0.5 * 255;
    G = 0.7 *255;
    B = 255;
    setAutoFillBackground(true);
}


void MyLabel::tabdiilR(int n) {
    R = float(n);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(R,G,B,255));      //QPalette::Window -> fondo del widget
    setPalette(palette);
    emit shareColor(R,G,B);

}

void MyLabel::tabdiilG(int n) {
    G = float(n);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(R,G,B,255));
    setPalette(palette);
    emit shareColor(R,G,B);
}

void MyLabel::tabdiilB(int n) {
    B = float(n);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Window, QColor(R,G,B,255));
    setPalette(palette);
    emit shareColor(R,G,B);

}
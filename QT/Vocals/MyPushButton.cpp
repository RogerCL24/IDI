#include "MyPushButton.h"

MyPushButton::MyPushButton(QWidget * parent) : QPushButton(parent) {
    connect(this,SIGNAL(clicked()),this,SLOT(castLetter()));
    onCapital = false;
    first = true;
}


void MyPushButton::capitalLetter() {
    if (first) {
        letter = text();
        first = false;
    }
    if (not onCapital) letter = letter.toUpper();
    else letter = letter.toLower();
    onCapital = !(onCapital);
}

void MyPushButton::castLetter() {
    if (first) {
        letter = text();
        first = false;
    }
    emit writeLetter(letter);
}

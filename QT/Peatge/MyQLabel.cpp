#include "MyQLabel.h"

MyQLabel::MyQLabel(QWidget *parent):QLabel(parent){
    camions = cotxes = motos = diners = 0;
    obert = false;
}

void MyQLabel::incCotxe() {
    if (obert) {
        ++cotxes;
        setNum(cotxes);
        emit linkActivated(1);
    }
}

void MyQLabel::incCamio() {
    if (obert) {
        ++camions;
        setNum(camions);
        emit linkActivated(4);
    }   
}

void MyQLabel::incMoto() {
    if (obert) {
        ++motos;
        setNum(motos);
        emit linkActivated(1);
    }
}

void MyQLabel::obrir_i_tancar() {
    if (not obert) {
        setStyleSheet("background-color: green;");
        setText("Obert");
        obert = true;
    }
    else {
        setStyleSheet("background-color: red;");
        setText("Tancat");
        obert = false;
    }
}

void MyQLabel::updBool() {
    obert = !(obert);   
}

void MyQLabel::incRecaudacio(int recaudacio) {
    diners += recaudacio;
    setNum(diners);
}
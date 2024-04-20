#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent) {
    aamal = true;
    check = 0;
    fields = 0;
}

void MyLabel::OkPulsed() {
    
    emit sendHeader(text());

}

void MyLabel::concatenate(QString alnas) {
    if (alnas.size() > 0) ++check;                      //Campos obligatarios rellenados
    ++fields;
    if (aamal and fields < 4) {
        QString alnasaljadid = text() + alnas + " ";
        setText(alnasaljadid);
    } 
    if (fields == 4) {                                  //Campos obligatorios
        if (check < 4) {
            clear();
            aamal = true;
        }
        fields = check = 0;
    }
    
}

void MyLabel::tabdiilOkState_1() {
    if (text().size() > 0 ) aamal = false;
}

void MyLabel::tabdiilOkState_2() {
    aamal = true;
    clear();
}
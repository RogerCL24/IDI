#include "MyLineEdit.h"


MyLineEdit::MyLineEdit(QWidget *parent): QLineEdit(parent) {
}

void MyLineEdit::omplirLabel() {
    QString str = text();
    str = str.toUpper();                // To Capital Letters
    ghalat = 5;
    int errors = str.size();
    QString d = "**********";

    if (str.contains('D')) {
        d.replace(0, 1, 'D');
        --errors;
    }
    if (str.contains('I')) {
        d.replace(1, 1, 'I');
        d.replace(4,1,'I');
        d.replace(9,1,'I');
        --errors;
    }
    if (str.contains('C')) {
        d.replace(2, 1, 'C');
        d.replace(3, 1, 'C');
        --errors;
    }
    if (str.contains('O')) {
        d.replace(5, 1, 'O');
        --errors;
    }
    if (str.contains('N')) {
        d.replace(6, 1, 'N');
        --errors;
    }
    if (str.contains('A')) {
        d.replace(7, 1, 'A');
        --errors;
    }
    if (str.contains('R')) {
        d.replace(8, 1, 'R');
        --errors;
    }
    if (errors < 0) errors = 0;
    ghalat -= errors;
    emit enviaNumErrors(ghalat);
    emit enviaText(d);
    if (str.size() == 12 or errors >= 5) {
        clear();
        setStyleSheet("background-color : red;");
        setEnabled(false);                                //No pots escriure més
    }
    else if (not d.contains('*')) {
        clear();
        setStyleSheet("background-color : green;");
        setEnabled(false); 
    }
}

void MyLineEdit::reset() {
    setEnabled(true);
    emit enviaNumErrors(5);
    setStyleSheet("background-color : none;");
}
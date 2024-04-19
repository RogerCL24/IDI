#include "MyLineEdit.h"
// constructor
MyLineEdit::MyLineEdit(QWidget *parent)
    : QLineEdit(parent) {
    connect(this, SIGNAL(textChanged(const QString &)),this, SLOT(setCursorPosition(text().length())));     //Si se llena QLineEdit mueve el texto
}
// implementació slots
void MyLineEdit::concatenate(QString qabla) {
    // Implementació de tractaReturn
    QString jadid = text() + qabla;
    setText(jadid);
}
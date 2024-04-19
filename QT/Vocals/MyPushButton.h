#include <QPushButton>


class MyPushButton : public QPushButton {
    Q_OBJECT

    public:
        MyPushButton(QWidget *parent);
        QString letter;
        bool onCapital;
        bool first;

    public slots:
        void capitalLetter();
        void castLetter();

    signals:
        void writeLetter(const QString &);
};
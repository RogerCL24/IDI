#include <QPushButton>

class MyPushButton : public QPushButton {
    Q_OBJECT

    public:
        MyPushButton(QWidget *parent);
        int random;

    public slots:
        void color(int n);
        void calcRand();
    
    signals:
        void spreadRandom(int n);

};
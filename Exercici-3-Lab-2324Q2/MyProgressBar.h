#include <QProgressBar>
#include <QTimer>

class MyProgressBar : public QProgressBar {
    Q_OBJECT;

    public:
        MyProgressBar(QWidget *parent);
        int x;
        float wideness;
        float phase;
        QTimer timer;
        bool clock;
        int shift_x;

    public slots:
        void inicio();
        void cambiarAmp(int n);
        void cambiarFase(int n);
        void animate();
        void Alwaqt();
};



#include <QLabel>

class MyLabel : public QLabel {
    Q_OBJECT

    public:
        MyLabel(QWidget *parent);
        float R,G,B;

    public slots:
        void tabdiilR(int n);
        void tabdiilG(int n);
        void tabdiilB(int n);

    signals:
        void shareColor(float R, float G, float B);

};
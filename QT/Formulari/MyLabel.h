#include <QLabel>

class MyLabel : public QLabel {
    Q_OBJECT

    public:
        MyLabel(QWidget * parent);
        bool aamal;
        int check, fields;

    public slots:
        void OkPulsed();
        void concatenate(QString);
        void tabdiilOkState_1();
        void tabdiilOkState_2();

    signals:
        void sendHeader(QString);

};
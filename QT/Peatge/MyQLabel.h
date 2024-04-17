#include <QLabel>

class MyQLabel : public QLabel
{
    Q_OBJECT
    public:
            MyQLabel(QWidget *parent);
            int camions, cotxes, motos, diners;
            bool obert;
    
    public slots:
        void incCotxe();
        void incCamio();
        void incMoto();
        void obrir_i_tancar();
        void updBool();
        void incRecaudacio(int recaudacio);

    signals:
        void linkActivated(int);
};
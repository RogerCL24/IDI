#include <QLineEdit>

class MyLineEdit : public QLineEdit {

    Q_OBJECT

    public:
        MyLineEdit(QWidget *parent);
        int ghalat;
    
    public slots:
        void omplirLabel();
        void reset();

    signals:
        void enviaText(const QString &);
        void enviaNumErrors(const int);



};
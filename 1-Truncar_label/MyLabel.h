#include <QLabel>
class MyLabel: public QLabel
{
	Q_OBJECT
	public:
		MyLabel (QWidget *parent);
        QString corda;


    public slots:
        void truncarLabel(int);
        void copiarText(QString);
};

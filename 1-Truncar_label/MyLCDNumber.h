#include <QLCDNumber>
#include <QWidget>
class MyLCDNumber: public QLCDNumber
{
	Q_OBJECT
	public:
		MyLCDNumber (QWidget *parent);



    public slots:
        void PosarAZero();
        void ChangeColor(int n);
};

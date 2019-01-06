#ifndef MAINCALC_H
#define MAINCALC_H

#include <QMainWindow>

namespace Ui {
class MainCalc;
}

class MainCalc : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainCalc(QWidget *parent = 0);
    ~MainCalc();

private:
    Ui::MainCalc *ui;

private slots:

    void digit_pressed();
    void opr_pressed();
    void on_pushButton_Clr_clicked();
    void on_pushButton_Hex_clicked();
    void on_pushButton_Dec_clicked();
};

#endif // MAINCALC_H

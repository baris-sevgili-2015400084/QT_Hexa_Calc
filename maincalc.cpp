#include "maincalc.h"
#include "ui_maincalc.h"
#include <string>

#include <QDebug>
#include <QApplication>

#include <QMessageBox>
#include <QInputDialog>

#include <QIcon>
MainCalc::MainCalc(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainCalc)



{      //we connect the necessary buttons with the functions that are going to be activated when specified act made
    ui->setupUi(this);

    ui->label->setText("0");
    connect(ui->pushButton_0,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_1,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_2,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_3,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_4,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_5,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_6,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_7,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_8,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_9,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_A,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_B,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_C,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_D,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_E,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_F,SIGNAL(released()),this,SLOT(digit_pressed()));
    connect(ui->pushButton_divide,SIGNAL(released()),this,SLOT(opr_pressed()));
    connect(ui->pushButton_minus,SIGNAL(released()),this,SLOT(opr_pressed()));
    connect(ui->pushButton_mult,SIGNAL(released()),this,SLOT(opr_pressed()));
    connect(ui->pushButton_plus,SIGNAL(released()),this,SLOT(opr_pressed()));
    connect(ui->pushButton_Clr,SIGNAL(released()),this,SLOT(on_pushButton_Clr_clicked()));
    connect(ui->pushButton_equal,SIGNAL(released()),this,SLOT(opr_pressed()));
    ui->pushButton_Hex->setCheckable( true );  //check whether this button is active at that moment, and to toggle easily
    ui->pushButton_Dec->setCheckable( true );

    connect(ui->pushButton_Hex,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_Hex_clicked()));
    connect(ui->pushButton_Dec,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_Dec_clicked()));

}


MainCalc::~MainCalc()
{
    delete ui;
}


//method thats being activated when any of the 1-9 or A-F pressed
void MainCalc::digit_pressed(){

    QPushButton * button = (QPushButton*) sender();

    if(ui->pushButton_Dec->isChecked()){ //means we are in the Decimal mode
        if(!(button->text()=="A"||button->text()=="B"||button->text()=="C"||button->text()=="D"||button->text()=="E"||button->text()=="F")){ // letters not printed in Dec mode

            if(ui->label->text()!="0")
                ui->label->setText(ui->label->text()+button->text()); //adds pressed button's text to the current monitored text
            else
                ui->label->setText(button->text());
        }
    }else if(ui->label->text()!="0") //the same opearation above, allows all to be printed
        ui->label->setText(ui->label->text()+button->text());
    else
        ui->label->setText(button->text());

}

void MainCalc::on_pushButton_Clr_clicked() //sets the monitored text to "0"
{
    ui->label->setText("0");
}
void MainCalc::opr_pressed(){ //the function which is activated when any of the operator buttons is pressed

    QPushButton * butt = (QPushButton*) sender();

    bool divideByZero=0; // a boolean checks dividing by zero

    if(ui->label->text()!="0"){

        QString curLabel=ui->label->text();
        if(curLabel.contains("+")||curLabel.contains("-")||curLabel.contains("*")||curLabel.contains("/")){
            QChar lastChar=ui->label->text().at(ui->label->text().length()-1);
            if(lastChar=='+'||lastChar=='-'||lastChar=='*'||lastChar=='/'){ //if last char is an operator sign, when new one is pressed just keeps the newly pressed opr. sign
                QString newStr="";
                for(int i=0; i<ui->label->text().length()-1; i++){
                    newStr+=ui->label->text().at(i);
                }
                if(butt->text()!="=")
                    newStr+=butt->text();

                ui->label->setText(newStr);
            }else{      //if we find an operator sign thats not at the end, that means its time to do some calculation


                QString finLabel="";  //strings, created to handle with - sign at the beginning
                QString new_label="";
                int negChecker=-1;      //int to check the existance of the - sign at the beginning of the monitored text
                if(curLabel.startsWith('-')){ //deletes the minus at the 0th index in order to easen the further operations

                    for(int i=0; i<curLabel.length()-1; i++){
                        finLabel+=curLabel.at(i+1);
                    }
                    negChecker=1;
                }else{
                    finLabel=curLabel; //finLabel is the string thats we run our operations on
                }

                if(ui->pushButton_Dec->isChecked()){  //checks the mode we are in

                    if(finLabel.contains("+")){ //if finds a "+" sums the right and left sides of it, multiplies left side by -1 if negChecker is equals to 1
                        int rslt=((finLabel.split('+').first()).toInt()*(negChecker*(-1)) + (finLabel.split('+').last()).toInt());
                        new_label= QString::number(rslt,'g',10); //turn the rslt integer to a string in decimal mode


                    }else if(finLabel.contains("-")){//if finds a "-" substracts the right side from the left side of it, multiplies left side by -1 if negChecker is equals to 1
                        int rslt=((finLabel.split('-').first()).toInt()*(negChecker*(-1)) - (finLabel.split('-').last()).toInt());
                        new_label= QString::number(rslt,'g',10);//turn the rslt integer to a string in decimal mode


                    }else if(finLabel.contains("/")){//if finds a "/" divides the left side to the right side of it, multiplies left side by -1 if negChecker is equals to 1

                        if((finLabel.split('/').last()).toInt()==0){ //pops up an error message if detects dividing by zero
                            divideByZero=1;
                            QMessageBox::warning(
                                        this,
                                        tr("Divide by Zero Alert"),
                                        tr("if limit from the right side of the 0 --> (+)infinity / if left --> (-)infinity") );

                        }else{

                            int rslt=((finLabel.split('/').first()).toInt()*(negChecker*(-1)) / (finLabel.split('/').last()).toInt());
                            new_label= QString::number(rslt,'g',10);//turn the rslt integer to a string in decimal mode


                        }
                    }else if(finLabel.contains("*")){//if finds a "*" multiplies the right and left sides of it, multiplies left side by -1 if negChecker is equals to 1
                        int rslt=((finLabel.split('*').first()).toInt()*(negChecker*(-1)) * (finLabel.split('*').last()).toInt());
                        new_label= QString::number(rslt,'g',10);//turn the rslt integer to a string in decimal mode


                    }else if(butt->text()=="="){ //if couldnt find any of the signs above, it means we have - at the beginning, so it keeps the current text
                        new_label=ui->label->text();
                    }

                }else{      //if its a hex operation


                    bool ok; // needed for the x.toInt() method

                    if(finLabel.contains("+")){//if finds a "+"

                        QString str1 =finLabel.split('+').first(); //turns right and left sides to string
                        QString str2 =finLabel.split('+').last();

                        int hex1 = str1.toInt(&ok, 16)*(-negChecker); //then casts to decimal form,multiplies left side by -1 if it was initially a negative expression
                        int hex2 = str2.toInt(&ok, 16);

                        int decimal = hex1+hex2; //does the operation in decimal

                        if(decimal<0){ //if result is negative firstly changes it to positive by convention of the function, then adds the beginning a "-" sign
                            decimal=decimal*(-1);
                            new_label="-"+QString::number(decimal,16).toUpper(); // changes the decimal formed expression to the hexadecimal form
                        }else
                            new_label=QString::number(decimal,16).toUpper();

                    }else if(finLabel.contains("-")){//if finds a "-"

                        QString str1 =finLabel.split('-').first(); //turns right and left sides to string
                        QString str2 =finLabel.split('-').last();

                        int hex1 = str1.toInt(&ok, 16)*(-negChecker);//then casts to decimal form,multiplies left side by -1 if it was initially a negative expression
                        int hex2 = str2.toInt(&ok, 16);

                        int decimal = hex1-hex2;//does the operation in decimal

                        if(decimal<0){//if result is negative firstly changes it to positive by convention of the function, then adds the beginning a "-" sign
                            decimal=decimal*(-1);
                            new_label="-"+QString::number(decimal,16).toUpper(); // changes the decimal formed expression to the hexadecimal form
                        }else
                            new_label=QString::number(decimal,16).toUpper();

                    }else if(finLabel.contains("/")){//if finds a "/"

                        if((finLabel.split('/').last()).toInt()==0){//checks for the error
                            divideByZero=1;
                            QMessageBox::warning(
                                        this,
                                        tr("Calculator"),
                                        tr("if limit from the right side of the 0 --> (+)infinity / if left --> (-)infinity") );

                        }else{

                            QString str1 =finLabel.split('/').first();//turns right and left sides to string
                            QString str2 =finLabel.split('/').last();

                            int hex1 = str1.toInt(&ok, 16)*(-negChecker);//then casts to decimal form,multiplies left side by -1 if it was initially a negative expression
                            int hex2 = str2.toInt(&ok, 16);

                            int decimal = hex1/hex2;        //does the operation in decimal

                            if(decimal<0){//if result is negative firstly changes it to positive by convention of the function, then adds the beginning a "-" sign
                                decimal=decimal*(-1);
                                new_label="-"+QString::number(decimal,16).toUpper();// changes the decimal formed expression to the hexadecimal form
                            }else
                                new_label=QString::number(decimal,16).toUpper();
                        }
                    }else if(finLabel.contains("*")){   //if finds a "*"

                        QString str1 =finLabel.split('*').first();//turns right and left sides to string
                        QString str2 =finLabel.split('*').last();

                        int hex1 = str1.toInt(&ok, 16)*(-negChecker);//then casts to decimal form,multiplies left side by -1 if it was initially a negative expression
                        int hex2 = str2.toInt(&ok, 16);

                        int decimal = hex1*hex2;        //does the operation in decimal

                        if(decimal<0){  //if result is negative firstly changes it to positive by convention of the function, then adds the beginning a "-" sign
                            decimal=decimal*(-1);
                            new_label="-"+QString::number(decimal,16).toUpper();// changes the decimal formed expression to the hexadecimal form
                        }else
                            new_label=QString::number(decimal,16).toUpper();
                    }else if(butt->text()=="="){
                        new_label=ui->label->text();    //if couldnt find any of the signs above, it means we have - at the beginning, so it keeps the current text


                    }





                }


                if(divideByZero){ //sets text to zero when trying to divide by 0
                    ui->label->setText("0");
                }else if(butt->text()=="="){// changes the monitored text to newly created text
                    ui->label->setText(new_label);
                }else // changes the monitored text to newly created text, adds the pressed opr. sign to the end of it
                    ui->label->setText(new_label+butt->text());
            }

        }else {
            if(butt->text()!="="){ //if no opr. signs can found, just adds the newly pressed opr. sign to the end of monitored text

                ui->label->setText(ui->label->text()+butt->text());
            }
        }
    }else //if monitored text is "0" keeps it as "0"  when an opr. button is pressed
        ui->label->setText("0");



}







void MainCalc::on_pushButton_Hex_clicked()  //when Hex button is pressed, makes it Checked, changes buttons background, set the opposite as unChecked, sets the monitored text as 0
{

    ui->pushButton_Hex->setChecked(true);

    ui->pushButton_Hex->setStyleSheet("QPushButton {background-color: green  }");

    ui->pushButton_Dec->setChecked(false);

    ui->label->setText("0");

}

void MainCalc::on_pushButton_Dec_clicked() //when Dec button is pressed, makes it Checked, changes buttons background, set the opposite as unChecked, sets the monitored text as 0
{
    ui->pushButton_Dec->setChecked(true);
    ui->pushButton_Dec->setStyleSheet("QPushButton{ background-color: green }");

    ui->pushButton_Hex->setChecked(false);
    ui->label->setText("0");


}

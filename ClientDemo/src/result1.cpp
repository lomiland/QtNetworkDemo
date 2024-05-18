#include "result1.h"
#include "ui_result1.h"

result1::result1(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::result1)
{
    ui->setupUi(this);
    ui->label->setText("我方获胜");
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        this->hide();
    });
}

result1::~result1()
{
    delete ui;
}

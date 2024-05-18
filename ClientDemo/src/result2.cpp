#include "result2.h"
#include "ui_result2.h"

result2::result2(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::result2)
{
    ui->setupUi(this);
    ui->label->setText("对方获胜");
    connect(ui->pushButton,&QPushButton::clicked,[=](){
        this->hide();
    });
}

result2::~result2()
{
    delete ui;
}

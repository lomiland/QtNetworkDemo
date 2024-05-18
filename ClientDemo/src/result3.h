#ifndef RESULT3_H
#define RESULT3_H

#include <QWidget>

namespace Ui {
class result3;
}

class result3 : public QWidget
{
    Q_OBJECT

public:
    explicit result3(QWidget *parent = nullptr);
    ~result3();

private:
    Ui::result3 *ui;
};

#endif // RESULT3_H

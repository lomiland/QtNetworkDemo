#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../../NetworkLibrary/networkdata.h"
#include "../../NetworkLibrary/networksocket.h"
#include "surakarta_game.h"
#include "surakarta_board.h"
#include "surakarta_common.h"
#include "surakarta_agent_mine.h"
#include "surakarta_rule_manager.h"
#include <QPainter>
#include <QBrush>
#include <QPainterPath>
#include<QPaintEvent>
#include <QDialog>
#include <QTimer>
#include <QFile>
#include"Step.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

enum piececolor{
    BLACK,
    WHITE,
    NONE
};
class Piece
{
public:
    Piece(){};
    int _row; // 行
    int _col; // 列
    int _id;
    bool _dead;
    bool _black;
    void init(int id)
    {
        struct
        {
            int row, col;
        }pos[12] = {
                     {0, 0},
                     {0, 1},
                     {0, 2},
                     {0, 3},
                     {0, 4},
                     {0, 5},
                     {1, 0},
                     {1, 1},
                     {1, 2},
                     {1, 3},
                     {1, 4},
                     {1, 5},
                     };
        _id = id;
        _dead = false;
        _black = id < 12;
        if (id < 12)
        {
            _row = pos[id].row;
            _col = pos[id].col;
        }
        else
        {
            _row = 5 - pos[id - 12].row;
            _col = 5 - pos[id - 12].col;
        }
    }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    bool blackturn=true;
    QVector<Step*>player_steps;

    QVector<Step*>white_steps;

    QVector<Step*>black_steps;

    QVector<Step*>ai_steps;

    void computer_move();

    bool computer_canMove(int moveid,int row,int col,int killid);

    void getAllStep(QVector<Step*>&steps);//获得所有可能的行棋

    int Calculate1(Step*&step);//估分

    int Calculate2(Step*&step);//估分

    bool judgeLoc(int moveid,int row,int col,int);

    bool judgeNum(int moveid,int row,int col,int killid);
    void getBlackStep(QVector<Step*>&steps);
    void getWhiteStep(QVector<Step*>&steps);
    int getLine(int moveid,int row,int col,int);
    void black_computer_move();
    void white_computer_move();
    int getId(int row,int col);
    int step;
    QFile file;
    bool fileopen=false,isapper=false,forward=true;
    QString change(QString a);
    SurakartaGame *game;
    SurakartaAgentMine *agent;
    bool isAI=false;
    void AIMove();
    piececolor playercolor1;
    piececolor playercolor2;
    QString player1;
    QString player2;
    QTimer *ti;
    int board_size_=6;
    int time;
    int selectid;
    int _r;
    void paintEvent(QPaintEvent *);
    QPoint center(int row,int col);
    QPoint center(int id);
    void drawpiece(QPainter&painter,int id);
    Piece p[24];
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    class result1 *page3 = NULL;
    class result2 *page4 = NULL;
    class result3 *page2 = NULL;
    void saveStep(int moveid, int killid, int row, int col,QVector<Step*>&steps);
    bool same_white(int row,int col);
    bool same_black(int row,int col);

    struct myPoint
    {
        int x;int y;
    };

    //构造四个方向数组;
    //向上数组等价于向右数组
    myPoint a1[24]=
        {
            {1,1},
            {0,1},
            {1,0},
            {1,1},
            {1,2},
            {1,3},
            {1,4},
            {1,5},
            {0,4},
            {1,4},
            {2,4},
            {3,4},
            {4,4},
            {5,4},
            {4,5},
            {4,4},
            {4,3},
            {4,2},
            {4,1},
            {4,0},
            {5,1},
            {4,1},
            {3,1},
            {2,1}
        };
    //向左数组等价于向下数组；
    myPoint a2[24]=
        {
            {1,1},
            {1,0},
            {0,1},
            {1,1},
            {2,1},
            {3,1},
            {4,1},
            {5,1},
            {4,0},
            {4,1},
            {4,2},
            {4,3},
            {4,4},
            {4,5},
            {5,4},
            {4,4},
            {3,4},
            {2,4},
            {1,4},
            {0,4},
            {1,5},
            {1,4},
            {1,3},
            {1,2}

        };

    //向上数组等价于向右数组；
    myPoint b1[24]=
        {
            {2,2},
            {1,2},
            {0,2},
            {2,0},
            {2,1},
            {2,2},
            {2,3},
            {2,4},
            {2,5},
            {0,3},
            {1,3},
            {2,3},
            {3,3},
            {4,3},
            {5,3},
            {3,5},
            {3,4},
            {3,3},
            {3,2},
            {3,1},
            {3,0},
            {5,2},
            {4,2},
            {3,2}
        };

    //向左数组等价于向下数组；
    myPoint b2[24]=
        {
         {2,2},
         {2,1},
         {2,0},
         {0,2},
         {1,2},
         {2,2},
         {3,2},
         {4,2},
         {5,2},
         {3,0},
         {3,1},
         {3,2},
         {3,3},
         {3,4},
         {3,5},
         {5,3},
         {4,3},
         {3,3},
         {2,3},
         {1,3},
         {0,3},
         {2,5},
         {2,4},
         {2,3},
         };


private:
    int port = 10086;
    QString ip = "127.0.0.1";
    NetworkSocket *socket;
    Ui::MainWindow *ui;

private slots:
    void TimeOut();
    void connected_successfully();
    void connectToServer();
    void disconnectFromServer();
    void move();
    void ready();
    void receiveMessage(NetworkData);
};
#endif // MAINWINDOW_H

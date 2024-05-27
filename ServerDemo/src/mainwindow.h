#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include <QMessageBox>
#include "../../NetworkLibrary/networkdata.h"
#include "../../NetworkLibrary/networkserver.h"
#include "Stone.h"
#include <QFile>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE




enum piececolor
{
    WHITE,
    BLACK,
    NONE
};

enum class SurakartaIllegalMoveReason {
    LEGAL,                     // unused
    LEGAL_CAPTURE_MOVE,        // capture a opponent's piece, and the move consists at least one corner loop
    LEGAL_NON_CAPTURE_MOVE,    // just as the name
    ILLIGAL,                   // unused
    NOT_PLAYER_TURN,           // move when it's not the player's turn.
    OUT_OF_BOARD,              // from or to position is out of board
    NOT_PIECE,                 // move a position that is not a piece
    NOT_PLAYER_PIECE,          // move a piece that is not the player's
    ILLIGAL_CAPTURE_MOVE,      // try to capture a opponent's piece, but the move can't consist any corner loop
    ILLIGAL_NON_CAPTURE_MOVE,  // otherwise
    GAME_ALREADY_END,          // unused
    GAME_NOT_START             // unused
};

enum class SurakartaEndReason {
    NONE,          // not end
    STALEMATE,     // both players can't make more move
    CHECKMATE,     // one player's all pieces are captured
    TRAPPED,       // unused, one player's pieces are all trapped, no legal move can be made.
    RESIGN,        // one player resigns.
    TIMEOUT,       // one player's time is out.
    ILLIGAL_MOVE,  // one player makes an illegal move
};



class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QFile file;
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int maxstep=40;
    QString player1_Name;
    QString player2_Name;
    piececolor player_color1;
    piececolor player_color2;
    Stone p[24];
    int step=0;
    struct myPoint
    {
        int x;
        int y;
    };
    myPoint change(QString str);
    int findId(myPoint mypoint);
    int getLine(int moveid,int row,int col,int);
    bool judgeCatch(int moveid,int row,int col,int killid);
    int getBlackNum();
    int getWhiteNum();
    SurakartaEndReason whyEnd();
    bool judgeEnd();
    myPoint a1[24]={
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
    myPoint a2[24]={
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
    myPoint b1[24]={
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
    myPoint b2[24]={
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
    const int max_clients = 2;
    Ui::MainWindow *ui;
    QSet<QTcpSocket*> clients;
    NetworkServer* server = nullptr;
    QTcpSocket* client1 = nullptr;
    QTcpSocket* client2 = nullptr;
    void send_to_another_client(QTcpSocket* another, NetworkData data);
    void remove_client(QTcpSocket* client);

private slots:
    void listen_port();
    void restart_server();
    void receive_from_client(QTcpSocket* client, NetworkData data);
};
#endif // MAINWINDOW_H

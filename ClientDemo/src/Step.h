#ifndef STEP_H
#define STEP_H


class Step
{
public:
    Step();
    int _moveid;
    int _killid;
    int _rowPre;
    int _colPre;
    int _rowNow;
    int _colNow;
    bool _blackTurnPre;
};

#endif // STEP_H

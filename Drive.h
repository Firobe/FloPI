#ifndef DRIVE_H
#define DRIVE_H
#define MAXPOS 140
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <wiringPi.h>

class Drive {
public:
    Drive();
    virtual ~Drive();
    void Move();
    bool isBusy() {
        return _busy;
    }
    bool ShouldMove(unsigned int, bool);
    void NewNote(int, unsigned int, unsigned int);
    void Connect(int,int);
protected:
private:
    bool _busy;
    int _curPos, _stepPin, _dirPin, _note;
    unsigned int _len, _curLen, _time, _curDel;
};

typedef struct Note Note;
struct Note {
    unsigned int absTime, length;
    int note, channel;
};
#endif // DRIVE_H

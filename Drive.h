#ifndef DRIVE_H
#define DRIVE_H
#define MAXPOS 140
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>
#include <string>
#include <wiringPi.h>

using namespace std;

class Drive {
    public:
        Drive();
        virtual ~Drive();
        void Move();
        bool isBusy();
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

typedef struct Event Event;
struct Event {
    unsigned int time, note, duration, channel;
    };

void nextNote( int*, unsigned int*, unsigned int* , int*);
void parser (string fileName, unsigned int&, vector<Event>&);
unsigned int t2m(unsigned int micro);
#endif // DRIVE_H

#include "Drive.h"

using namespace std;

Drive::Drive() {
    _curPos=0;
    _busy=false;
    }

Drive::~Drive() {
    //dtor
    }

bool Drive::isBusy() {
    if(!_busy)
        digitalWrite(_stepPin, LOW);
    return _busy;
    }

void Drive::Connect(int dP, int sP) {
    _dirPin=dP;
    _stepPin=sP;
    pinMode( _dirPin, OUTPUT ) ;
    pinMode( _stepPin, OUTPUT );
    digitalWrite( _dirPin, HIGH );
    for(int i =0; i<100; i++) {
        digitalWrite(_stepPin, HIGH);
        delay(3);
        digitalWrite(_stepPin, LOW);
        }
    digitalWrite( _dirPin, LOW );
    }

void Drive::Move() {
    _curPos++;
    //DIRECTION
    if( _curPos == MAXPOS / 2 )
        digitalWrite(_dirPin,HIGH);
    if( _curPos == MAXPOS ) {
        _curPos = 0;
        digitalWrite(_dirPin,LOW);
        }
    digitalWrite(_stepPin , LOW);
    digitalWrite(_stepPin, HIGH);
    }

bool Drive::ShouldMove(unsigned int newTime, bool disp) {
    if(disp) {
        cout<<" C1+"<<_note;
        if(_note>=10)
            cout<<"   #"<<endl;
        else if(_note>=0)
            cout<<"    #"<<endl;
        else
            cout<<"   #"<<endl;
        }
    if(newTime-_time>=_curDel) {
        _curLen+=newTime-_time;
        _time=newTime;
        if(_curLen>=_len)
            _busy=false;
        return true;
        }
    else
        return false;
    }

void Drive::NewNote(int note, unsigned int length, unsigned int instant) {
    _note=note;
    float freq;
    _curLen=0;
    _len=length;
    freq = 32.70 * pow( 1.05946309436, note );
    _curDel = floor(( 1.0 / freq ) * 1000000.0);
    _busy = true;
    _time = instant;
    }

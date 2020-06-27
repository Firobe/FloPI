#include "Drive.h"

#define NBDRIVES 5

using namespace std;

vector<Event> events;
unsigned int tempo, bpm;

bool order(Event a, Event b) {
    return a.time < b.time;
    }

int main( void ) {
    unsigned int timeRef, defnote;
    Note temp;
    bool present, disChange;
    vector<Note> buffer;
    vector<int> association;
    wiringPiSetup() ;
    Drive drive[NBDRIVES];
    cout << "FloPI - Virgile ROBLES\n---------------------------\nDrive initialization..." << endl;
    for(int i=0; i<2*NBDRIVES; i+=2)
        drive[i/2].Connect(i, i+1);
    int note = 0, pause, staccato;
    string trackPath;
    cout << "-> MIDI file : ";
    cin >> trackPath;
    parser(trackPath, tempo, events);
    cout << "-> Reference note (36 recommended) : ";
    cin >> defnote;
    cout << "-> Tempo (BPM) : ";
    cin >> bpm;
    timeRef=micros();
    for( ;; ) {
        if(disChange) {
            cout<<"####################"<<endl;
            }
        for(int i=0; i<NBDRIVES; i++) {
            if(disChange)
                cout<<"#  DRIVE "<<i;
            if(drive[i].isBusy()) {
                if(drive[i].ShouldMove(micros(), disChange))
                    drive[i].Move();
                }
            else if(disChange)
                cout<<"         #"<<endl;
            }
        if(disChange)
            cout<<"####################"<<endl;
        disChange=false;
        while(buffer.size()<2) {
            present=false;
            nextNote( &temp.note, &temp.length, &temp.absTime, &temp.channel );
            if(temp.note==-1)
                break;
            for(int i=0; i<association.size(); i++)
                if(association[i] == temp.channel)
                    present=true;
            if(!present)
                association.push_back(temp.channel);
            buffer.push_back(temp);
            }
        if(buffer.empty())
            break;
        for(int i=buffer.size()-1; i>=0; i--)
            if(micros()-timeRef>=buffer[i].absTime) {
                int j;
                for(j=0; i<association.size(); j++)
                    if(association[j]==buffer[i].channel)
                        break;
                delay(5); //STACCATO
                drive[j].NewNote(buffer[i].note, buffer[i].length, micros());
                disChange=true;
                if(i<buffer.size()-1)
                    buffer[i]=buffer.back();
                buffer.pop_back();
                break;
                }
        }
    return 0 ;
    }

void nextNote( int* noteD, unsigned int* lengthD, unsigned int* timeD, int* channelD ) {
    static int i = 0;
    if(i >= events.size())
        *noteD = -1;
    else
        *noteD = events[i].note ;
    *lengthD = t2m(events[i].duration) ;
    *timeD = t2m(events[i].time) ;
    *channelD = events[i].channel ;
    i++;
    }

unsigned int t2m(unsigned int micro) {
    return (micro*6000000)/(bpm*tempo);
    }

void parser(string fileName, unsigned int& tempo, vector<Event>& events) {
    ifstream file;
    unsigned int timeStamp, absoluteTime;
    unsigned char buffer[256];
    unsigned char temp = 0;
    file.open(fileName.c_str(), ios::in | ios::binary);
    ///HEADER
    file.read((char*)buffer, 14);
    if(buffer[0] != 0x4D || buffer[1] != 0x54 || buffer[2] != 0x68 || buffer[3] != 0x64) {
        cout << "Error : input is not a MIDI file" << endl;
        exit(EXIT_FAILURE);
        }
    tempo = buffer[13];
    cout << "Parsing..." << endl;
    while(!file.eof()) {
        ///PLAYING
        file.read((char*)buffer, 8);
        absoluteTime = 0;
        while(!file.eof()) {
            timeStamp = 0;
            do {
                file.read((char*)(&temp), 1);
                timeStamp = timeStamp*16 + temp;
                }
            while(temp >= 0x80);
            absoluteTime += timeStamp;
            file.read((char*)(&temp), 1);
            if(temp == 0xFF) {
                unsigned int length = 0;
                file.read((char*)buffer, 1);
                do {
                    file.read((char*)(&temp), 1);
                    length = length*16 + temp;
                    }
                while(temp >= 0x80);
                file.read((char*)buffer, length);
                if(length == 0)
                    break;
                }
            else {
                unsigned char event = temp >> 4;
                unsigned char n = temp - (event * 16);
                if(event != 0xC && event != 0xD) {
                    file.read((char*)buffer, 2);
                    if(event == 0x8 || (event == 0x9 && buffer[1] == 0)) {
                        int i = events.size();
                        do {
                            i--;
                            }
                        while(events[i].channel != n && events[i].note != buffer[0]);
                        events[i].duration = absoluteTime - events[i].time;
                        }
                    else if(event == 0x9) {
                        Event temp;
                        temp.channel = n;
                        temp.note = buffer[0];
                        temp.time = absoluteTime;
                        events.push_back(temp);
                        }
                    }
                else {
                    file.read((char*)buffer, 1);
                    }
                }
            }
        }
    file.close();
    sort(events.begin(), events.end(), order);
    }


#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <wiringPi.h>
#include <vector>
#include "Drive.h"

#define NBDRIVES 5

using namespace std;
void parser( int*, unsigned int*, unsigned int* , int*);
ifstream track;
int tempo;
float mesure;
int defnote;
int main( void )
{
    unsigned int timeRef;
    Note temp;
    bool present, disChange;
    vector<Note> buffer;
    vector<int> association;
    wiringPiSetup() ;
    Drive drive[NBDRIVES];
    cout << "Initialisation des lecteurs..." << endl;
    for(int i=0; i<2*NBDRIVES; i+=2)
        drive[i/2].Connect(i, i+1);
    int note = 0, pause, staccato;
    string trackPath;
    cout << "FloPI - Virgile ROBLES\n  Track : ";
    cin >> trackPath;
    cout << "  Tempo : ";
    cin >> tempo;
    cout << "  Mesure : ";
    cin >> mesure;
    cout << " DEFNOTE : ";
    cin >> defnote;
    track.open( trackPath.c_str() );
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
            } else if(disChange)
                cout<<"         #"<<endl;
        }
        if(disChange)
            cout<<"####################"<<endl;
        disChange=false;
        while(buffer.size()<2) {
            present=false;
            parser( &temp.note, &temp.length, &temp.absTime, &temp.channel );
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
            if(micros()-timeRef>=buffer[i].absTime)
                /*for(int j=0; j<NBDRIVES; j++)
                    if(!drive[j].isBusy())*/
            {
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
    track.close();
    return 0 ;
}

void parser( int* noteD, unsigned int* lengthD, unsigned int* timeD, int* channelD )
{
    string curStr;
    int bar = 0, note = 0, length = 0, locTime = 0, absTime = 0, pause, channel;
    if(!getline( track, curStr ))
        *noteD = -1;
    else {
        string first = curStr.substr( 0, 30 );
        string second = curStr.substr( 52, 30 );
        string noteStr;
        bool complexi = false, divi = false;
        int i1 = 0, i2 = 0, i3 = 0, j;
        for( int i = 0; i < first.length(); i++ )
            if( first[i] == '+' )
                complexi = true;
            else if( first[i] == '/' )
                divi = true;
        if( complexi && divi ) {
            sscanf( first.c_str(), "BA %d CR %d+%d/%d", &bar, &i1, &i2, &i3 );
            absTime = i1 * 1000 + ( i2 * 1000 ) / i3 + bar * mesure * 1000;
        } else if( !complexi && divi ) {
            sscanf( first.c_str(), "BA %d CR %d/%d", &bar, &i2, &i3 );
            absTime = ( i2 * 1000 ) / i3 + bar * mesure * 1000;
        } else {
            sscanf( first.c_str(), "BA %d CR %d", &bar, &i3 );
            absTime = i3 * 1000 + bar * mesure * 1000;
        }
        //PARSING LENGTH
        complexi = divi = false;
        i1 = i2 = i3 = 0;
        for( int i = 0; i < second.length(); i++ )
            if( second[i] == '+' )
                complexi = true;
            else if( second[i] == '/' )
                divi = true;
        if( complexi && divi ) {
            sscanf( second.c_str(), " %d+%d/%d", &i1, &i2, &i3 );
            length = i1 * 1000 + ( i2 * 1000 ) / i3;
        } else if( !complexi && divi ) {
            sscanf( second.c_str(), " %d/%d", &i2, &i3 );
            length = ( i2 * 1000 ) / i3;
        } else {
            sscanf( second.c_str(), " %d", &i3 );
            length = i3 * 1000;
        }
        //PARSING CHANNEL
        for( j = 0; j < curStr.length(); j++ )
            if( curStr[j] == 'H' )
                break;
        channel=curStr[j+3];

        //PARSING NOTE
        for( j = 0; j < curStr.length(); j++ )
            if( curStr[j] == 'N' )
                break;
        switch( curStr[j + 4] ) {
        case 'A':
            note = 0;
            break;
        case 'B':
            note = 2;
            break;
        case 'C':
            note = 3;
            break;
        case 'D':
            note = 5;
            break;
        case 'E':
            note = 7;
            break;
        case 'F':
            note = 8;
            break;
        case 'G':
            note = 10;
            break;
        default:
            cout << "PARSING ERROR\n";
            break;
        }
        switch( curStr[j + 5] ) {
        case '#':
            note++;
            break;
        case 'b':
            note--;
            break;
        case '-':
            note -= 12;
            break;
        case '\'':
            note += 12;
            break;
        }
        if( curStr[j + 6] == '-' )
            note -= 12;
        if( curStr[j + 6] == '\'' )
            note += 12;
        if( curStr[j + 7] == '-' )
            note -= 12;
        if( curStr[j + 7] == '\'' )
            note += 12;
        note += defnote;
        length*=1000;
        absTime*=1000;
        length /= tempo / 60.0;
        absTime /= tempo / 60.0;
        *timeD = absTime;
        *noteD = note;
        *lengthD = length;
        *channelD = channel;
    }
}

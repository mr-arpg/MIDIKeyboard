#include "Arduino.h"
#include "MIDI.h"

const int table [6][9] = {
  {1,7,19,49,43,37,31,25,13},{2,8,20,-1,44,38,26,14},{3,9,21,-1,45,39,27,15},{4,10,22,-1,46,40,28,16},
  {5,11,23,-1,47,41,29,17},{6,12,24,-1,48,42,30,18}
  };
 
//These will be the output pins (black pins)
const int pin_o [6] = {14,15,16,17,12,11};

//These will be the input pins (red pins)
const int pin_i [9] = {2,3,4,5,6,7,8,9,10};

//These will save if note is on or off
bool note_state[49] = {false};

// Create and bind the MIDI interface to the default hardware Serial port
MIDI_CREATE_DEFAULT_INSTANCE();


void setup()
{
  for(int i = 0; i < 6; i++)
  {
    pinMode(pin_o[i], OUTPUT);
  }

  for(int i = 0; i < 9; i++)
  {
    pinMode(pin_i[i], INPUT_PULLUP);
  }
  pinMode(18,OUTPUT);
  //MIDI.begin(MIDI_CHANNEL_OFF);
  
  Serial.begin(38400);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Native USB only
  }
  Serial.println("Goodnight moon!");

  
}

void sendTone(int note, bool state)
{
  if(state)
  {
    double power = ((double)note-34)/12;
    tone(18, 440*pow(2, power));
  }

  else
  {
    noTone(18);
  }
}

void loop() {
  
  int note = 0;
  for(int o = 0; o < 6; o++)
  {
    //cycle through output pins and send GND signal
    digitalWrite(pin_o[o], LOW);

    //read input signals for GND, meaning made connection
    for(int i = 0; i < 9; i++)
    {

      if(i == 4 && o > 0)
      {
        break;
      }

      
      note = table[o][i]-1;

      //if connection made, aka note on:
      if(digitalRead(pin_i[i])==LOW && !note_state[note])
      {
        Serial.println(table[o][i]);
        //Serial.println(pin_i[i]);
        //Serial.println(pin_o[o]);
        //sendTone(note, 1);
        
        //this is .sendNoteOn(number of note, volume, midi channel)
        //MIDI.sendNoteOn(note+35, 127, 1);
        note_state[note] = 1;
      }

      
      //if connection not made, aka note off:
      else if(digitalRead(pin_i[i])==HIGH && note_state[note])
      {  
        note_state[note] = 0;
        Serial.println(table[o][i]);
        //sendTone(note, 0);
        //MIDI.sendNoteOff(note+35, 0, 1);
      }
    }
    digitalWrite(pin_o[o], HIGH);
  }
}

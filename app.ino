#include "Max7219.h"

////////////////////////////////////////////////////////////////
// App Fns --- 
////////////////////////////////////////////////////////////////
namespace App {
  static char * _message = NULL;

  void SetMessage(char * message) {
    App::_message = message;
  }

  const char * GetMessage() {
    return App::_message;
  }

  void ReadMessage() {
    char buf[512];
    unsigned int i = 0;

    if (App::_message) {
      free(App::_message);
    }

    delay(25);

    while (Serial.available() && (i < (sizeof(buf) - 1))) {
      char in = (char)Serial.read();

      if (!(in == '\r' || in == '\n')) {
        buf[i++] = in;
      }
    }

    buf[i] = 0x00;

    App::SetMessage(strdup(buf));
  }
}

////////////////////////////////////////////////////////////////
// Arduino Callbacks --- 
////////////////////////////////////////////////////////////////
void setup() {
  Serial.begin(9600);

  App::SetMessage(strdup("Test"));

  Max7219::Init(8, 9, 10);

  Max7219::SetBrightness(4);
  Max7219::SetEffect(Max7219::Effects::StringFX_Scroll);
  Max7219::SetCharacterDelay(40);
  Max7219::SetCompletionDelay(1000);
  Max7219::SetString(App::GetMessage());
}

void loop() {
  if (Serial.available()) {
    App::ReadMessage();

    Max7219::SetString(App::GetMessage());
  }

  /* 
    Loop the string continuously (when it finishes showing the whole string, set it again)
  */
  if (!Max7219::IsDisplayingString()) {
    Max7219::SetString(App::GetMessage());
  }

  Max7219::Update();
}

void brightnessRotaryISR() {

}

void speedRotaryISR() {

}

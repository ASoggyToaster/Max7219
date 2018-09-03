#include "Max7219.h"

////////////////////////////////////////////////////////////////
// App Fns --- 
////////////////////////////////////////////////////////////////
namespace App {
  static const char * _message = NULL;

  void SetMessage(const char * message) {
    App::_message = message;
  }

  const char * GetMessage() {
    return App::_message;
  }

  void ReadMessage() {
    char buf[512];
    unsigned int i = 0;

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
  
  Max7219::Init(8, 9, 10);

  Max7219::SetBrightness(4);
  Max7219::SetEffect(Max7219::Effects::StringFX_Scroll);
  Max7219::SetCharacterDelay(40);
  Max7219::SetCompletionDelay(1000);

  App::SetMessage("Test");
}

void loop() {
  if (Serial.available()) {
    App::ReadMessage();
  }

  if (!Max7219::IsDisplayingString()) {

    // write string is idempotent -- 
    // calling won't actually have an impact unless the string is finished drawing
    Max7219::WriteString(App::GetMessage());
  }

  Max7219::Update();
}

void brightnessRotaryISR() {

}

void speedRotaryISR() {

}

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
    char buf[256];
    unsigned int i = 0;

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
  Max7219::SetCharacterDelay(150);
  Max7219::SetCompletionDelay(500);

  App::SetMessage("Thank you for the monster bubs I love you");
}

void loop() {
  if (Serial.available()) {
    App::ReadMessage();
  }

  Max7219::WriteString(App::GetMessage());
}

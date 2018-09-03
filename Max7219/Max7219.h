#ifndef __HAVE_MAX7219_H__
#define __HAVE_MAX7219_H__

#include <Arduino.h>

namespace Max7219 {// effects
    enum Effects {
        StringFX_None,
        StringFX_Wipe,
        StringFX_Scroll,
        StringFX_Count,
    };

    void WriteString(const char * string);

    void SetBrightness(unsigned char brightness);
    unsigned char GetBrightness();

    void SetEffect(unsigned int effect);
    unsigned int GetEffect();

    void SetCharacterDelay(unsigned int characterDelay);
    unsigned int GetCharacterDelay();

    void SetCompletionDelay(unsigned int completionDelay);
    unsigned int GetCompletionDelay();

    unsigned char IsDisplayingString();
    
    void Init(unsigned int pinDIN, unsigned int pinCS, unsigned int pinCLK);
    void Update();
}

#endif // __HAVE_MAX7219_H__
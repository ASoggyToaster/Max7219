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

    // Sets the string being displayed on the matrix
    void SetString(const char * string);

    // sets the display brightness
    void SetBrightness(unsigned char brightness);

    // get the display brightness
    unsigned char GetBrightness();

    // set the text animation (e.g., display each character, wipe/fade, scroll whole string)
    void SetEffect(unsigned int effect);

    // get the current effect-- corresponds to an enum value 
    unsigned int GetEffect();

    // set delay between characters
    void SetCharacterDelay(unsigned int characterDelay);

    // get delay between characters
    unsigned int GetCharacterDelay();

    // set delay amount for IsDisplayingString() to return false after last character has been displayed
    void SetCompletionDelay(unsigned int completionDelay);

    // get ^ 
    unsigned int GetCompletionDelay();

    // is the matrix currently displaying a string? 
    unsigned char IsDisplayingString();
    
    // initialize the display (data in, chip select, clock)
    void Init(unsigned int pinDIN, unsigned int pinCS, unsigned int pinCLK);

    // update (tick) the display
    void Update();
}

#endif // __HAVE_MAX7219_H__
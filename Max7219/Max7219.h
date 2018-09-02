#ifndef __HAVE_MAX7219_H__
#define __HAVE_MAX7219_H__


namespace Max7219 {// effects
    enum Effects {
        StringFX_None,
        StringFX_Wipe,
        StringFX_Scroll
    };

    void WriteString(const char * string);

    void SetBrightness(unsigned char brightness);
    void SetEffect(unsigned int effect);
    void SetCharacterDelay(unsigned int characterDelay);
    void SetCompletionDelay(unsigned int completionDelay);

    void Init(unsigned int pinDIN, unsigned int pinCS, unsigned int pinCLK);
}

#endif // __HAVE_MAX7219_H__
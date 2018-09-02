#include "Max7219.h"

////////////////////////////////////////////////////////////////
// library settings (instance variables)
////////////////////////////////////////////////////////////////
static unsigned int _effect = 0;
static unsigned int _characterDelay = 250;
static unsigned int _completionDelay = 500;

// GPIO Definitions 
static int _pinCLK = 10;
static int _pinCS = 9;
static int _pinDIN = 8;

////////////////////////////////////////////////////////////////
// Constants ---
////////////////////////////////////////////////////////////////
static unsigned char font[39][8] = {
  {0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//0 
  {0x10,0x18,0x14,0x10,0x10,0x10,0x10,0x10},//1 
  {0x7E,0x2,0x2,0x7E,0x40,0x40,0x40,0x7E},//2 
  {0x3E,0x2,0x2,0x3E,0x2,0x2,0x3E,0x0},//3  
  {0x8,0x18,0x28,0x48,0xFE,0x8,0x8,0x8},//4 
  {0x3C,0x20,0x20,0x3C,0x4,0x4,0x3C,0x0},//5 
  {0x3C,0x20,0x20,0x3C,0x24,0x24,0x3C,0x0},//6 
  {0x3E,0x22,0x4,0x8,0x8,0x8,0x8,0x8},//7  
  {0x0,0x3E,0x22,0x22,0x3E,0x22,0x22,0x3E},//8 
  {0x3E,0x22,0x22,0x3E,0x2,0x2,0x2,0x3E},//9 
  {0x8,0x14,0x22,0x3E,0x22,0x22,0x22,0x22},//A 
  {0x3C,0x22,0x22,0x3E,0x22,0x22,0x3C,0x0},//B 
  {0x3C,0x40,0x40,0x40,0x40,0x40,0x3C,0x0},//C 
  {0x7C,0x42,0x42,0x42,0x42,0x42,0x7C,0x0},//D 
  {0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x7C},//E 
  {0x7C,0x40,0x40,0x7C,0x40,0x40,0x40,0x40},//F 
  {0x3C,0x40,0x40,0x40,0x40,0x44,0x44,0x3C},//G
  {0x44,0x44,0x44,0x7C,0x44,0x44,0x44,0x44},//H
  {0x7C,0x10,0x10,0x10,0x10,0x10,0x10,0x7C},//I
  {0x3C,0x8,0x8,0x8,0x8,0x8,0x48,0x30},//J
  {0x0,0x24,0x28,0x30,0x20,0x30,0x28,0x24},//K
  {0x40,0x40,0x40,0x40,0x40,0x40,0x40,0x7C},//L
  {0x81,0xC3,0xA5,0x99,0x81,0x81,0x81,0x81},//M
  {0x0,0x42,0x62,0x52,0x4A,0x46,0x42,0x0},//N
  {0x3C,0x42,0x42,0x42,0x42,0x42,0x42,0x3C},//O
  {0x3C,0x22,0x22,0x22,0x3C,0x20,0x20,0x20},//P
  {0x1C,0x22,0x22,0x22,0x22,0x26,0x22,0x1D},//Q
  {0x3C,0x22,0x22,0x22,0x3C,0x24,0x22,0x21},//R
  {0x0,0x1E,0x20,0x20,0x3E,0x2,0x2,0x3C},//S
  {0x0,0x3E,0x8,0x8,0x8,0x8,0x8,0x8},//T
  {0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x1C},//U
  {0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x18},//V
  {0x0,0x49,0x49,0x49,0x49,0x2A,0x1C,0x0},//W
  {0x0,0x41,0x22,0x14,0x8,0x14,0x22,0x41},//X
  {0x41,0x22,0x14,0x8,0x8,0x8,0x8,0x8},//Y
  {0x0,0x7F,0x2,0x4,0x8,0x10,0x20,0x7F},//Z
  {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0},
};

////////////////////////////////////////////////////////////////
// Animation Functions ---
////////////////////////////////////////////////////////////////
static unsigned char blendChars(unsigned char a, unsigned char b, unsigned int alpha, unsigned int bitstring) {
  int maskA = ~((int)pow(2, alpha + 1) - 1), maskB = ((int)pow(2, alpha + 1) - 1);

  return (font[a][bitstring] & maskA|font[b][bitstring] & maskB);
}

static unsigned char scrollChars(unsigned char a, unsigned char b, unsigned int alpha, unsigned int bitstring) {
  int shiftA = alpha, shiftB = 8 - alpha;

  return (font[a][bitstring] << shiftA|font[b][bitstring] >> shiftB);
}

////////////////////////////////////////////////////////////////
// String Functions --- 
////////////////////////////////////////////////////////////////
static bool isAlNum(unsigned char c) {
  return (c >= '0' && c <= '9' || c >= 'a' && c <= 'z' || c >= 'A' && c <= 'Z');
}

static bool isNum(unsigned char c) {
  return (c >= '0' && c <= '9');
}

static bool isLower(unsigned char c) {
  return (c >= 'a' && c <= 'z');
}

static bool isUpper(unsigned char c) {
  return (c >= 'A' && c <= 'Z');
}

static unsigned int decodeCharacter(unsigned char c) {
  if (isAlNum(c)) {
    if (isNum(c)) {
      return c - '0';
    }

    if (isLower(c)) {
      return 10 + (c - 'a');
    }

    if (isUpper(c)) {
      return 10 + (c - 'A');
    }
  }

  return 38;
}

////////////////////////////////////////////////////////////////
// MAX 7219 functions ---
////////////////////////////////////////////////////////////////
static void Write_Max7219_byte(unsigned char DATA) {   
  unsigned char i;

  digitalWrite(_pinCS, LOW);		

  for(i=8;i>=1;i--) {		  
    digitalWrite(_pinCLK, LOW);

    // Extracting a bit data
    digitalWrite(_pinDIN, DATA&0x80);

    DATA = DATA<<1;

    digitalWrite(_pinCLK, HIGH);
  }                                 
}

static void Write_Max7219(unsigned char address,unsigned char character) {
  digitalWrite(_pinCS,LOW);
  
  //address，code of LED
  Write_Max7219_byte(address);

  //data，figure on LED 
  Write_Max7219_byte(character);

  digitalWrite(_pinCS, HIGH);
}

static void WriteString_Animated_Max7219(const char * string) {
  size_t len = (strlen(string) + 1);

  for (int i = 0; i < (len - 1); i++) {
    for (int k = 0; k<8; k++) {
      unsigned char decodedA = decodeCharacter(string[i]);
      unsigned char decodedB = decodeCharacter(string[i+1]); 

      for (int j = 1; j<9; j++) {
        switch (_effect) {
          case Max7219::Effects::StringFX_Wipe:
            Write_Max7219(j, blendChars(decodedA, decodedB, k, j - 1));
            break;

          case Max7219::Effects::StringFX_Scroll:
            Write_Max7219(j, scrollChars(decodedA, decodedB, k, j - 1));
            break;
        }
      }

      delay(_characterDelay/2);
    }

    delay(_characterDelay/2);
  }

  delay(_completionDelay);
}

static void WriteString_Char_Max7219(const char * string) {
  size_t len = (strlen(string) + 1);

  for (int i = 0; i < len; i++) {
    for (int j = 1; j<9; j++) {
      Write_Max7219(j, font[decodeCharacter(string[i])][j-1]);
    }    

    delay(_characterDelay);
  }

  delay(_completionDelay);
}



namespace Max7219 {
    void WriteString(const char * string) {
        switch (_effect) {
            case Max7219::Effects::StringFX_Wipe:
            case Max7219::Effects::StringFX_Scroll:
            WriteString_Animated_Max7219(string);
            break;

            case Max7219::Effects::StringFX_None:
            default:
            WriteString_Char_Max7219(string);
            break;
        }
    }

    void SetBrightness(unsigned char brightness) {
        Write_Max7219(0x0a, brightness);
    }

    void SetEffect(unsigned int effect) {
        ::_effect = effect;
    }

    void SetCharacterDelay(unsigned int characterDelay) {
        ::_characterDelay = characterDelay;
    }

    void SetCompletionDelay(unsigned int completionDelay) {
        ::_completionDelay = completionDelay;
    }

    void Init(unsigned int pinDIN, unsigned int pinCS, unsigned int pinCLK) {
        ::_pinDIN = pinDIN;
        ::_pinCS = pinCS;
        ::_pinCLK = pinCLK;

        pinMode(::_pinCLK, OUTPUT);
        pinMode(::_pinCS, OUTPUT);
        pinMode(::_pinDIN, OUTPUT);

        delay(50);

        //decoding ：BCD
        Write_Max7219(0x09, 0x00);

        //scanlimit；8 LEDs
        Write_Max7219(0x0b, 0x07);

        //power-down mode：0，normal mode：1
        Write_Max7219(0x0c, 0x01);

        //test display：1；EOT，display：0
        Write_Max7219(0x0f, 0x00);
    }
}
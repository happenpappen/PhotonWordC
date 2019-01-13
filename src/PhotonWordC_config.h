#ifndef __PHOTONWORDC_CONFIG_H__
#define  __PHOTONWORDC_CONFIG_H__

#include "FastLED.h"
FASTLED_USING_NAMESPACE;

/* Die Zeit wird auf dieser Matrix angezeigt: 

    00 01 02 03 04 05 06 07 08 09 10

09   E  S  K  I  S  T  L  F  Ü  N  F

08   Z  E  H  N  Z  W  A  N  Z  I  G

07   D  R  E  I  V  I  E  R  T  E  L

06   T  G  N  A  C  H  V  O  R  J  M

05   H  A  L  B  X  Z  W  Ö  L  F  P

04   Z  W  E  I  N  S  I  E  B  E  N

03   K  D  R  E  I  R  H  F  Ü  N  F

02   E  L  F  N  E  U  N  V  I  E  R

01   W  A  C  H  T  Z  E  H  N  R  S

00   B  S  E  C  H  S  F  M  U  H  R

*/

#define WORD_EINS       1
#define WORD_ZWEI       2
#define WORD_DREI       3
#define WORD_VIER       4
#define WORD_FUENF      5
#define WORD_SECHS      6
#define WORD_SIEBEN     7
#define WORD_ACHT       8
#define WORD_NEUN       9
#define WORD_ZEHN       10
#define WORD_ELF        11
#define WORD_ZWOELF     12
#define WORD_FUENFM     13
#define WORD_ZEHNM      14
#define WORD_ZWANZIGM   15
#define WORD_ES         16
#define WORD_IST        17
#define WORD_UHR        18
#define WORD_HALB       19
#define WORD_VIERTEL    20
#define WORD_VOR        21
#define WORD_NACH       22
#define WORD_EIN        23

#define HOUR_TOKEN      50

#define WORD_POS    0
#define WORD_COUNT  1

const bool kMatrixSerpentineLayout = true;
const uint8_t kMatrixWidth  = 10;
const uint8_t kMatrixHeight = 11;

#define MAX_DIMENSION ((kMatrixWidth > kMatrixHeight) ? kMatrixWidth : kMatrixHeight)
#define NUM_LEDS_WC 110
#define NUM_LEDS_AM 52

#endif

#include "particle-dst.h"

DST dst;

dst_limit_t beg1;
dst_limit_t end1;
dst_limit_t beg2;
dst_limit_t end2;

void setup() {
  Serial.begin(9600);

  // first test
  // 2nd tue of feb @ 2AM
  beg1.hour = 2;
  beg1.day = 3; // tuesday
  beg1.month = 2; // february
  beg1.occurrence = 2;

  // 2nd wed of oct @ 3AM
  end1.hour = 3;
  end1.day = 4; // wednesday
  end1.month = 10; // october
  end1.occurrence = -2;

  // second test
  // 4th mon of nov @ 1AM
  beg2.hour = 1;
  beg2.day = 2; // monday
  beg2.month = 11; // november
  beg2.occurrence = 4;

  // 1st mon of apr @ 4AM
  end2.hour = 4;
  end2.day = 2; // monday
  end2.month = 4; // april
  end2.occurrence = -1;

  Serial.println("first test");
  dst.begin(beg1, end1, 1);

  test(1484501689); // before
  test(1497548089); // between
  test(1513359289); // after

  Serial.println();
  Serial.println("second test");
  dst.begin(beg2, end2, 1);

  test(1508088889); // before
  test(1513421491); // end of the year
  test(1518778291); // start of the year
  test(1497611064); // after
}

void loop() {

}

void test(int date)
{
  Serial.printlnf("%d: DST %s\n", date, dst.check(date) ? "enabled" : "disabled");
}

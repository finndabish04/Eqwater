
#include <RTClib.h>

RTC_DS3231 rtc;

// the pin that is connected to SQW
#define CLOCK_INTERRUPT_PIN 2

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(115200);
  delay(500);
  Serial.println("Hello!");
  pinMode(ledpin, OUTPUT);

    // initializing the rtc
    if(!rtc.begin()) {
        Serial.println("Couldn't find RTC!");
        Serial.flush();
        while (1) delay(10);
    }

    //we don't need the 32K Pin, so disable it
    rtc.disable32K();

    // Making it so, that the alarm will trigger an interrupt
    pinMode(CLOCK_INTERRUPT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(CLOCK_INTERRUPT_PIN), onAlarm, FALLING);

    // set alarm 1, 2 flag to false (so alarm 1, 2 didn't happen so far)
    // if not done, this easily leads to problems, as both register aren't reset on reboot/recompile
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);

    // stop oscillating signals at SQW Pin
    // otherwise setAlarm1 will fail
    rtc.writeSqwPinMode(DS3231_OFF);

    // schedule an alarm 10 seconds in the future
    if(!rtc.setAlarm1(
            rtc.now() + TimeSpan(10),
            DS3231_A1_Second // this mode triggers the alarm when the seconds match. See Doxygen for other options
    )) {
        Serial.println("Error, alarm wasn't set!");
    }else {
        Serial.println("Alarm will happen in 10 seconds!");
    }

}

// the loop function runs over and over again forever
void loop() {
      // print current time
    char date[10] = "hh:mm:ss";
    rtc.now().toString(date);
    Serial.print(date);

    // the stored alarm value + mode
    DateTime alarm1 = rtc.getAlarm1();
    Ds3231Alarm1Mode alarm1mode = rtc.getAlarm1Mode();
    char alarm1Date[12] = "DD hh:mm:ss";
    alarm1.toString(alarm1Date);
    Serial.print(" [Alarm1: ");
    Serial.print(alarm1Date);
    Serial.print(", Mode: ");
    Serial.print("Second");
    // switch (alarm1mode) {
    //   case DS3231_A1_PerSecond: Serial.print("PerSecond"); break;
    //   case DS3231_A1_Second: Serial.print("Second"); break;
    //   case DS3231_A1_Minute: Serial.print("Minute"); break;
    //   case DS3231_A1_Hour: Serial.print("Hour"); break;
    //   case DS3231_A1_Date: Serial.print("Date"); break;
    //   case DS3231_A1_Day: Serial.print("Day"); break;
    // }

    // the value at SQW-Pin (because of pullup 1 means no alarm)
    Serial.print("] SQW: ");
    Serial.print(digitalRead(CLOCK_INTERRUPT_PIN));

    // whether a alarm fired
    Serial.print(" Fired: ");
    Serial.print(rtc.alarmFired(1));

    if (rtc.alarmFired(1)) {
        delay(2000);
        rtc.clearAlarm(1);
        Serial.print(" - Alarm cleared");
        //sets a new alarm for 10 seconds in the future
        rtc.setAlarm1(rtc.now()+TimeSpan(10), DS3231_A1_Second);
    }
    Serial.println();

    delay(2000);

}

void onAlarm() {
    Serial.println("Alarm occured!");
}


/**
 * Dallas family thermometers access example (Arduino).
 *
 * Required configuration:
 * - @c CONFIG_SEARCH_ENABLED for non single sensor setup,
 * - @c CONFIG_PWR_CTRL_ENABLED if @c PWR_CTRL_PIN is configured.
 */
#include "OneWireNg_CurrentPlatform.h"
#include "drivers/DSTherm.h"
#include "utils/Placeholder.h"

/*
 * 1-wire bus pin number.
 */
#ifndef OW_PIN
# define OW_PIN         6
#endif

/*
 * If defined: sensors powered parasitically.
 */
//#define PARASITE_POWER

/*
 * If defined: only one sensor device is allowed to be connected to the bus.
 * The library may be configured with 1-wire search activity disabled to
 * reduce its footprint.
 */
// #define SINGLE_SENSOR


#ifdef PARASITE_POWER
# define PARASITE_POWER_ARG true
#else
# define PARASITE_POWER_ARG false
#endif

static Placeholder<OneWireNg_CurrentPlatform> ow;
String tempString;

/* returns false if not supported */
static bool printId(const OneWireNg::Id& id)
{
    const char *name = DSTherm::getFamilyName(id);

    Serial.print(id[0], HEX);
    for (size_t i = 1; i < sizeof(OneWireNg::Id); i++) {
        Serial.print(':');
        Serial.print(id[i], HEX);
    }
    if (name) {
        Serial.print(" -> ");
        Serial.print(name);
    }
    Serial.println();

    return (name != NULL);
}

static void printScratchpad(const DSTherm::Scratchpad& scrpd)
{
    const uint8_t *scrpd_raw = scrpd.getRaw();

    long temp = scrpd.getTemp2();
    //Serial.print("Temp:");
    temp = (float)temp / 16;
    Serial.print(temp);
    tempString = String(temp);
    Serial.print(" C");

    Serial.println();
}

void setup()
{
  new (&ow) OneWireNg_CurrentPlatform(OW_PIN, false);
  DSTherm drv(ow);

  Serial.begin(115200);
  delay(1000);
  Serial.println("serial");
}

void loop()
{
    DSTherm drv(ow);

    /* convert temperature on all sensors connected... */
    drv.convertTempAll(DSTherm::MAX_CONV_TIME, PARASITE_POWER_ARG);

    /* read sensors one-by-one */
    Placeholder<DSTherm::Scratchpad> scrpd;

    for (const auto& id: *ow) {
      if (drv.readScratchpad(id, scrpd) == OneWireNg::EC_SUCCESS)
        printScratchpad(scrpd);
      else
        Serial.println("  Read scratchpad error.");
    }
    Serial.println(tempString);
    delay(1000);
}


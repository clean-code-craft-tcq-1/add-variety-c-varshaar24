#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "test/catch.hpp"
#include "typewise-alert.h"

TEST_CASE("infers the breach according to limits") {
  REQUIRE(inferBreach(12, 20, 30) == TOO_LOW);
}

TEST_CASE("Classify temperature") {
  REQUIRE(classifyTemperatureBreach(PASSIVE_COOLING, 30) == NORMAL);
}

TEST_CASE("check and alert:email alert for too high") {
  BatteryCharacter batteryChar;
  batteryChar.coolingType = HI_ACTIVE_COOLING;
  REQUIRE(checkAndAlert(TO_EMAIL, batteryChar, 50)==SentToEmail_TooHigh);

}

TEST_CASE("check and alert:email alert for tool low") {
  BatteryCharacter batteryChar;
  batteryChar.coolingType = PASSIVE_COOLING;
  REQUIRE(checkAndAlert(TO_EMAIL, batteryChar, -1)==SentToEmail_TooLow);
}

TEST_CASE("check and alert2") {
  BatteryCharacter batteryChar;
  batteryChar.coolingType = MED_ACTIVE_COOLING;
  REQUIRE( checkAndAlert(TO_CONTROLLER, batteryChar, -1)== SentToController);

}

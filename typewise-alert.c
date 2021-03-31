#include "typewise-alert.h"
#include <stdio.h>

CoolingInfoType CoolingInfo[] = {{PASSIVE_COOLING,0,35} , {HI_ACTIVE_COOLING,0,45} , {MED_ACTIVE_COOLING,0,40}};

void EmailNormal(const char* recepient);
void EmailHigh(const char* recepient);
void EmailLow(const char* recepient);

void (*TargetType[])(BreachType) = {sendToController,sendToEmail};
void (*Email[])(const char*) ={EmailNormal,EmailHigh,EmailLow};

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) 
{
  return inferBreach(temperatureInC, CoolingInfo[coolingType].LowerLimit, CoolingInfo[coolingType].UpperLimit);
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) 
{

  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  
  (*TargetType[alertTarget])(breachType);
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void EmailNormal(const char* recepient)
{
    printf("To: %s\n", recepient);
    printf("Hi, the temperature is normal\n");
}

void EmailHigh(const char* recepient)
{
    printf("To: %s\n", recepient);
    printf("Hi, the temperature too High\n");
}

void EmailLow(const char* recepient)
{
    printf("To: %s\n", recepient);
    printf("Hi, the temperature too Low\n");
}

void sendToEmail(BreachType breachType) 
{
  const char* recepient = "a.b@c.com";
  
  (*Email[breachType])(recepient);
  
}

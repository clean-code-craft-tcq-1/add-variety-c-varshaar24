#include "typewise-alert.h"
#include <stdio.h>

unsigned int SentSuccessfullytoController=0;
CoolingInfoType CoolingInfo[] = {{PASSIVE_COOLING,0,35} , {HI_ACTIVE_COOLING,0,45} , {MED_ACTIVE_COOLING,0,40}};

Successtype EmailNormal(const char* recepient);
Successtype EmailHigh(const char* recepient);
Successtype EmailLow(const char* recepient);

Successtype (*TargetType[])(BreachType) = {sendToController,sendToEmail};
Successtype (*Email[])(const char*) ={EmailNormal,EmailLow,EmailHigh};

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

Successtype checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) 
{

  BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
  
  return ((*TargetType[alertTarget])(breachType));
}
void DioWriteToController(const unsigned short header, BreachType breachType)
{
	(void)header;
	(void)breachType;
    SentSuccessfullytoController = 1;
}

Successtype sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
    SentSuccessfullytoController=0;
  DioWriteToController( header, breachType );
  return SentToController;
}

Successtype sendToConsole(BreachType breachType) {
  printf("the temperature is %x \n", breachType);
  return SentToConsole;
}

Successtype EmailNormal(const char* recepient)
{
    printf("To: %s\n", recepient);
    printf("Hi, the temperature is normal\n");
	
	 return SentToEmail_Normal;
}

Successtype EmailHigh(const char* recepient)
{
    printf("To: %s\n", recepient);
    printf("Hi, the temperature too High\n");
	
	return SentToEmail_TooHigh;
}

Successtype EmailLow(const char* recepient)
{
    printf("To: %s\n", recepient);
    printf("Hi, the temperature too Low\n");
	
	return SentToEmail_TooLow;
}

Successtype sendToEmail(BreachType breachType) 
{
  const char* recepient = "a.b@c.com";
  
  return (*Email[breachType])(recepient);
  
}

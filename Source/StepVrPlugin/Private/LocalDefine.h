#pragma once

/************************************************************************/
/* Debug                                                                     */
/************************************************************************/
#define  UE_LogScreen(delayTime,textConten)    GEngine->AddOnScreenDebugMessage(-1,delayTime, FColor::Blue,textConten)
//UE_LOG(LogTemp, Warning, TEXT("plugin begin!!!!!!!"));


/************************************************************************/
/* AnimInstance                                                                     */
/************************************************************************/
#define AnimCategory SkeletonRotaotr


/**  Version  */
#define AFTER_ENGINEVERSION_410 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION>10))
#define AFTER_ENGINEVERSION_411 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION>11))
#define AFTER_ENGINEVERSION_412 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION>12))
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
#define BEFOPRE_ENGINEVERSION_410 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION<10))
#define BEFOPRE_ENGINEVERSION_411 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION<11))
#define BEFOPRE_ENGINEVERSION_412 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION<12))

#define SDKNODEID(_ID_)	((StepVR::SingleNode::NodeID)(_ID_))
#define SDKKEYID(_ID_)	((StepVR::SingleNode::KeyID)(_ID_))
namespace StepVrInfo {

	//standard component ID
	enum StepVrNodeId {
		//Node Id
		DLeftController = 1,
		DRightController = 2,
		DGun = 4,
		DHead = 6,
	};
}
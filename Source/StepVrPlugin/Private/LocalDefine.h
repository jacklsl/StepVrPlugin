#pragma once


//GEngine->AddOnScreenDebugMessage(-1,delayTime, FColor::Blue,textConten)
#define  SHOWLOG(_String_) UE_LOG(LogTemp, Warning, TEXT("%s"),*_String_)


/************************************************************************/
/* AnimInstance                                                                     */
/************************************************************************/
#define AnimCategory


/**  Version  */
#define AFTER_ENGINEVERSION_410 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION>10))
#define AFTER_ENGINEVERSION_411 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION>11))
#define AFTER_ENGINEVERSION_412 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION>12))
#define BEFOPRE_ENGINEVERSION_410 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION<10))
#define BEFOPRE_ENGINEVERSION_411 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION<11))
#define BEFOPRE_ENGINEVERSION_412 ((ENGINE_MAJOR_VERSION==4)&&(ENGINE_MINOR_VERSION<12))


#define SDK_MCAP_NODE(_ID_)	((StepVR::WholeBody::SkeletonID)(_ID_))
#define SDKNODEID(_ID_)	((StepVR::SingleNode::NodeID)(_ID_))
#define SDKKEYID(_ID_)	((StepVR::SingleNode::KeyID)(_ID_))


namespace StepVrInfo {

	/**
	*   //standard component ID
	*/
	enum StepVrNodeId {
		DLeftController = 1,
		DRightController = 2,
		DGun = 4,
		DHead = 6,
	};
}
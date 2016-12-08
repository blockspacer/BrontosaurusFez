#include "AudioInterface.h"
#include "WwiseManager.h"
#define SAFE_DELETE(ptr) delete ptr; ptr = nullptr

namespace Audio
{

	CAudioInterface* CAudioInterface::myInstance = nullptr;
	CAudioInterface::CAudioInterface()
		:myWwiseManager(nullptr), myGameObjectCounter(0)
	{
	}

	CAudioInterface::~CAudioInterface()
	{
		SAFE_DELETE(myWwiseManager);
	}

	bool CAudioInterface::Init(const char* aInitBank)
	{
		bool result;
		myWwiseManager = new CWwiseManager();
		result = myWwiseManager->Init(aInitBank);

		myGameObjectID = RegisterGameObject();
		return result;
	}

	void CAudioInterface::Update()
	{
		if (myWwiseManager)
		{
			myWwiseManager->Update();
		}

	}

	void CAudioInterface::SetListenerPosition(const CU::Matrix44f & aTranslation)
	{
		AkListenerPosition listenerposition;

		listenerposition.Position.X = aTranslation.myPosition.X;
		listenerposition.Position.Y = aTranslation.myPosition.Y;
		listenerposition.Position.Z = aTranslation.myPosition.Z;
		
		listenerposition.OrientationFront.X = aTranslation.myForwardVector.X;
		listenerposition.OrientationFront.Y = aTranslation.myForwardVector.Y;
		listenerposition.OrientationFront.Z = aTranslation.myForwardVector.Z;
		
		listenerposition.OrientationTop.X = aTranslation.myUpVector.X;
		listenerposition.OrientationTop.Y = aTranslation.myUpVector.X;
		listenerposition.OrientationTop.Z = aTranslation.myUpVector.X;

		myWwiseManager->SetListenerPosition(listenerposition);
	}

	void CAudioInterface::SetGameObjectPosition(const GameObjectID aGameObjectID, const CU::Vector3f & aPosition, const CU::Vector3f & aOrientation)
	{
		AkSoundPosition soundPosition;

		soundPosition.Position.X = aPosition.X;
		soundPosition.Position.Y = aPosition.Y;
		soundPosition.Position.Z = aPosition.Z;

		soundPosition.Orientation.X = aOrientation.X;
		soundPosition.Orientation.Y = aOrientation.Y;
		soundPosition.Orientation.Z = aOrientation.Z;

		myWwiseManager->SetGameObjectPosition(aGameObjectID, soundPosition);
	}

	void CAudioInterface::SetRTPCValue(const char * aRTPCName, const float aValue)
	{
		myWwiseManager->SetRTPC(aRTPCName, aValue);
	}

	GameObjectID CAudioInterface::RegisterGameObject()
	{
		GameObjectID newID = myGameObjectCounter;
		myWwiseManager->RegisterGameObject(newID);
		myGameObjectCounter++;

		return newID;
	}

	GameObjectID CAudioInterface::RegisterGameObject(const char * aGameObjectName)
	{
		GameObjectID newID = myGameObjectCounter;
		myWwiseManager->RegisterGameObject(newID, aGameObjectName);
		myGameObjectCounter++;

		return newID;
	}

	void CAudioInterface::UnregisterGameObject(const GameObjectID aGameObjectID)
	{
		myWwiseManager->UnregisterGameObject(aGameObjectID);
	}

	void CAudioInterface::UnregisterAllGameOBjects()
	{
		myWwiseManager->UnregisterAllGameObjects();
	}

	bool CAudioInterface::LoadBank(const char* aBankPath)
	{
		if (myWwiseManager)
		{
			return myWwiseManager->LoadBank(aBankPath);
		}
		return false;
	}

	void CAudioInterface::UnLoadBank(const char* aBankPath)
	{
		if (myWwiseManager)
		{
			return myWwiseManager->UnLoadBank(aBankPath);
		}
	}

	void CAudioInterface::PostEvent(const char* aEvent, const GameObjectID aGameObjectID)
	{
		if (myWwiseManager)
		{
			return myWwiseManager->PostEvent(aEvent, aGameObjectID);
		}
	}

	void CAudioInterface::PostEvent(const char* aEvent)
	{
		if (myWwiseManager)
		{
			return myWwiseManager->PostEvent(aEvent, myGameObjectID);
		}
	}

	void CAudioInterface::SetErrorCallBack(callback_function aErrorCallback)
	{
		if (myWwiseManager)
		{
			return myWwiseManager->SetErrorCallBack(aErrorCallback);
		}
	}
}
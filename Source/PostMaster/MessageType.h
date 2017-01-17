#pragma once

enum class eMessageType
{
	eWindowSizeChanged,
	eMouseMessage,
	eKeyboardMessage,
	eKeyPressed,
	eInputMessagePressed,
	eStateStackMessage,
	eFokusChanged,
	eDrawCallsThisFrame,
	ePlayerDied,
	eLoadPlayer,
	ePlayerPickedPowerUp,
	ePlayerCreated,
	ePlayerMissed,
	ePlayerHitSomething,
	eEnemyDied,
	eScore,
	eDestroyComponent,
	eObjectives,
	eLevelOver,
	eNextLevelPlease,
	eConsoleCalledUpon,
	QuestRelated,
	eHatAdded,
	eDroppedFile,
	eBuyButtonPressed,
	ePlayerHealthChanged,
	ePlayerManaChanged,
	eLength,
};
//Ifall du vill l�gga till en till messagetyp s� kompilera om Postmaster Projekter annars kraschar det. 
//Och lenght ska vara l�ngst ner.


const int MessageTypeLength = static_cast<int>(eMessageType::eLength);

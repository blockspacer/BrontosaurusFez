#pragma once
#include <map>
class CGameObject;
class CollisionManager;

template <typename ValueType>
class ValueObserver;
struct PlayerData;
class CComponent;
class CNavmesh;


template<class _Elem, class _Traits, class _Alloc>
class basic_string;

template<class _Elem>
struct char_traits;

template<class _Ty>
class allocator;

typedef basic_string<char, char_traits<char>, allocator<char>> string;
typedef basic_string<wchar_t, char_traits<wchar_t>, allocator<wchar_t>> wstring;

struct PollingStation
{
	static void NullifyLevelSpecificData();
	static CComponent* PlayerInput;
	static CGameObject* playerObject;
	static PlayerData* playerData;
	static CU::GrowingArray<CGameObject*> myThingsEnemiesShouldAvoid;
	static CNavmesh* Navmesh;
	static CU::GrowingArray<std::string> playerHatList;
	static std::string currentDialog;
	static CU::GrowingArray<int,char> OpenPortals;

	static std::map<std::string, unsigned int>  QuestProgress;
};

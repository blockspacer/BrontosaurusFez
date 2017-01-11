#pragma once
#define cool temp
namespace KLoader
{
	struct SLoadedComponentData;
}

int LoadWanderController(KLoader::SLoadedComponentData someData);

int LoadSeekController(KLoader::SLoadedComponentData someData);

int LoadFleeController(KLoader::SLoadedComponentData someData);

int LoadAIController(KLoader::SLoadedComponentData someData);

//ladda klart allt sedan kallen en function som får alla object att skicka ett minipostmaster message som får dem att skicka sedan skicka tillbaka sig själva som pekare som sedan AIControllern lyssnar på och registrerar dem
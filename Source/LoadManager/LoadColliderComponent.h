#pragma once

namespace KLoader
{
	struct SLoadedComponentData;
}

int LoadSphereCollider(KLoader::SLoadedComponentData someData);

int LoadBoxCollider(KLoader::SLoadedComponentData someData);

int LoadPointCollider(KLoader::SLoadedComponentData someData);

#pragma once
#include "ObjectTypes.h"
#include <string>

bool GetGameObject(GameObject** object, const int& c) {
	bool staticObj{false};
	delete* object;

	switch (c) {
	case 255:
		*object = new E_Fiery();
		break;
	case 254:
		*object = new E_Richard();
		break;
	case 253:
		*object = new E_Pum();
		break;
	case 252:
		*object = new E_Mini();
		break;
	case 251:
		*object = new E_Tato();
		break;
	case 250:
		*object = new E_Grum();
		break;
	case 80:
		*object = new DungeonDoor();
		break;
	case 81:
		*object = new DungeonDoorVerti();
		break;
	case 125:
		*object = new Table();
		break;
	case 4:
		*object = new Chest();
		break;
	case 0:
		*object = new Boss();
		break;
	}
	return staticObj;
}
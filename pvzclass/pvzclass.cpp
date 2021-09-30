#include "pvzclass.h"
#include <iostream>
#include "events.h"

namespace Datas { 
}

void OnPoleVaultingJumping(EventZombiePoleVaultingJumped* e, PVZ* pvz) {
	e->zombie->Speed = 0.15f;
}

void OnPoleVaultingWalked(EventZombiePoleVaultingWalked* e, PVZ* pvz) {
	e->zombie->Speed = 0.1f;
}

void OnLevelStart(EventLevelStart* e, PVZ* pvz) {
	if (pvz->LevelScene == SceneType::Roof || pvz->LevelScene == SceneType::MoonNight) {
		for (int i = 0; i < 9; i++)
			pvz->GetLawn()->SetGridType(2, i, LawnType::Water);
		pvz->GetLawn()->SetRouteType(2, RouteType::Pool);
	}
}

int main()
{
	DWORD pid = ProcessOpener::Open();

	if (!pid) {
		
		for (int i = 0; i < 1000; i++) {
			std::wstring proc(TEXT("PlantsVsZombies"));
			static LPWSTR buff[9];
			_itow_s(i, (wchar_t*)buff, 9, 10);
			proc += (LPCWSTR)(void*)buff;
			proc += TEXT(".exe");
			pid = ProcessOpener::OpenByProcessName(proc.c_str());
			if (pid)
				break;
		}
		if (!pid) {
			for (int i = 0; i < 1000; i++) {
				std::wstring proc(TEXT("PlantsVsZombies"));
				static LPWSTR buff[9];
				_itow_s(i, (wchar_t*)buff, 9, 10);
				proc += (LPCWSTR)(void*)buff;
				proc += TEXT(".exe");
				pid = ProcessOpener::OpenByFilePath(TEXT("./"), proc.c_str());
				if (pid)
					break;
			}
			if (!pid)
				return 1;
		}
	}
	PVZ* pvz = new PVZ(pid);

	EventHandler e(pvz);
	e.RegistryListeners(OnPoleVaultingJumping);
	e.RegistryListeners(OnPoleVaultingWalked);
	e.RegistryListeners(OnLevelStart);
	while (! ProcessOpener::Closed(pid))
	{
		if (pvz->BaseAddress) {
			e.Run();
		}
	}

	delete pvz;
	return 0;
}
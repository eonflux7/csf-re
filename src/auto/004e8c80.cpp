#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, WeatherResources_Refresh, 0x0051B3B0, (void* self, void* edx));

static int __fastcall SceneLoader_RefreshWeatherResources(C_0085e7b8* self, void* /*edx*/)
{
    void* weatherResource = CSF_AT(void*, self, 0x1FC0);
    return WeatherResources_Refresh(weatherResource, nullptr);
}

HOOK(0x4E8C80, SceneLoader_RefreshWeatherResources, "auto");

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdPropTabDestroy, 0x0057C770, (uint32_t* a1));
CSF_FUNC(uint32_t*, __cdecl, RpPrtStdPropTabCreate, 0x0057C9A0, (int a1, uint8_t* a2, char* a3));
CSF_FUNC(int, __cdecl, RpPrtStdEmitterClassSetPropTab, 0x0057CE90, (int a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RpPrtStdEmitterClassGetPropTab, 0x0057CED0, (int a1, uint32_t* a2));

static int __stdcall GfxParticleEmitters_RebuildEmitterPropertyTable(
    int emitterClass, int emitterType)
{
    alignas(8) uint8_t propertyIds[0x400];
    alignas(8) uint8_t propertySizes[0x400];
    uint32_t propertyTableAddress = 0;

    RpPrtStdEmitterClassGetPropTab(emitterClass, &propertyTableAddress);
    auto* propertyTable = reinterpret_cast<uint32_t*>(propertyTableAddress);
    int propertyCount = CSF_AT(int, propertyTable, 0x10);

    for (int i = 0; i < propertyCount; ++i)
    {
        reinterpret_cast<uint32_t*>(propertyIds)[i] =
            CSF_AT(uint32_t*, propertyTable, 0x18)[i];
        reinterpret_cast<uint32_t*>(propertySizes)[i] =
            CSF_AT(uint32_t*, propertyTable, 0x20)[i];
    }

    if (emitterType == 3)
    {
        reinterpret_cast<uint32_t*>(propertyIds)[propertyCount] = 8;
        reinterpret_cast<uint32_t*>(propertySizes)[propertyCount] = 4;
        ++propertyCount;
    }

    uint32_t* replacement = RpPrtStdPropTabCreate(
        propertyCount, propertyIds, reinterpret_cast<char*>(propertySizes));
    RpPrtStdEmitterClassSetPropTab(emitterClass, replacement);
    return RpPrtStdPropTabDestroy(replacement);
}

HOOK(0x006E05D0, GfxParticleEmitters_RebuildEmitterPropertyTable, "auto");

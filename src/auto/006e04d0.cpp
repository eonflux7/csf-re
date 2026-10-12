#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdPropTabDestroy, 0x0057C770, (uint32_t* a1));
CSF_FUNC(uint32_t*, __cdecl, RpPrtStdPropTabCreate, 0x0057C9A0, (int a1, uint8_t* a2, char* a3));
CSF_FUNC(int, __cdecl, RpPrtStdEmitterClassSetPropTab, 0x0057CE90, (int a1, uint32_t* a2));
CSF_FUNC(int, __cdecl, RpPrtStdEmitterClassGetPropTab, 0x0057CED0, (int a1, uint32_t* a2));

static int __stdcall GfxParticleEmitters_AddEmitterProperties(
    int emitterClass, int emitterType)
{
    alignas(8) uint8_t propertyIdBytes[0x400];
    alignas(8) uint8_t propertySizeBytes[0x400];
    auto* propertyIds = reinterpret_cast<uint32_t*>(propertyIdBytes);
    auto* propertySizes = reinterpret_cast<uint32_t*>(propertySizeBytes);

    uint32_t originalTableAddress = 0;
    RpPrtStdEmitterClassGetPropTab(emitterClass, &originalTableAddress);

    auto* originalTable = reinterpret_cast<uint8_t*>(originalTableAddress);
    int propertyCount = CSF_AT(int, originalTable, 0x10);

    for (int i = 0; i < propertyCount; ++i)
    {
        propertyIds[i] = CSF_AT(uint32_t*, originalTable, 0x18)[i];
        propertySizes[i] = CSF_AT(uint32_t*, originalTable, 0x20)[i];
    }

    switch (emitterType)
    {
    case 0:
    case 2:
        propertyIds[propertyCount] = 10;
        propertySizes[propertyCount] = 8;
        ++propertyCount;
        break;

    case 1:
        propertyIds[propertyCount] = 9;
        propertySizes[propertyCount] = 68;
        ++propertyCount;
        break;

    case 3:
        propertyIds[propertyCount] = 10;
        propertySizes[propertyCount] = 8;
        ++propertyCount;
        propertyIds[propertyCount] = 11;
        propertySizes[propertyCount] = 4;
        ++propertyCount;
        break;

    default:
        break;
    }

    uint32_t* newTable = RpPrtStdPropTabCreate(
        propertyCount, propertyIdBytes,
        reinterpret_cast<char*>(propertySizeBytes));
    RpPrtStdEmitterClassSetPropTab(emitterClass, newTable);
    return RpPrtStdPropTabDestroy(newTable);
}

HOOK(0x006E04D0, GfxParticleEmitters_AddEmitterProperties, "auto");

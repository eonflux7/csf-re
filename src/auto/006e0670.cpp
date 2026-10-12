#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, RpPrtStdEmitterClassSetCallBack, 0x0057CE10, (int a1, int a2, int a3));

static int __stdcall GfxParticleEmitters_SetEmitterClassCallbacks(int emitterClass, int emitterType)
{
    alignas(8) uint8_t callbackBytes[0x84] = {};

    CSF_AT(void*, callbackBytes, 0x00) = reinterpret_cast<void*>(CSF_ADDR_FN(0x57DBA0));
    CSF_AT(void*, callbackBytes, 0x04) = reinterpret_cast<void*>(CSF_ADDR_FN(0x57EF90));
    CSF_AT(void*, callbackBytes, 0x08) = reinterpret_cast<void*>(CSF_ADDR_FN(0x57F050));
    CSF_AT(void*, callbackBytes, 0x0C) = reinterpret_cast<void*>(CSF_ADDR_FN(0x57F0B0));
    CSF_AT(void*, callbackBytes, 0x14) = reinterpret_cast<void*>(CSF_ADDR_FN(0x57ED00));
    CSF_AT(void*, callbackBytes, 0x18) = reinterpret_cast<void*>(CSF_ADDR_FN(0x57EDA0));
    CSF_AT(void*, callbackBytes, 0x1C) = reinterpret_cast<void*>(CSF_ADDR_FN(0x4B2A30));
    CSF_AT(void*, callbackBytes, 0x20) = reinterpret_cast<void*>(CSF_ADDR_FN(0x4B2A30));
    CSF_AT(void*, callbackBytes, 0x24) = reinterpret_cast<void*>(CSF_ADDR_FN(0x4B2A30));
    CSF_AT(void*, callbackBytes, 0x28) = reinterpret_cast<void*>(CSF_ADDR_FN(0x4B2A30));

    int callbackSetCount = 1;
    switch (emitterType)
    {
    case 0:
        callbackSetCount = 2;
        CSF_AT(void*, callbackBytes, 0x0C) = reinterpret_cast<void*>(CSF_ADDR_FN(0x703C40));
        CSF_AT(void*, callbackBytes, 0x38) = reinterpret_cast<void*>(CSF_ADDR_FN(0x57F0B0));
        break;

    case 1:
        CSF_AT(void*, callbackBytes, 0x14) = reinterpret_cast<void*>(CSF_ADDR_FN(0x703910));
        CSF_AT(void*, callbackBytes, 0x00) = reinterpret_cast<void*>(CSF_ADDR_FN(0x6C2AB0));
        CSF_AT(void*, callbackBytes, 0x04) = reinterpret_cast<void*>(CSF_ADDR_FN(0x704200));
        return RpPrtStdEmitterClassSetCallBack(
            emitterClass, callbackSetCount, reinterpret_cast<int>(callbackBytes));

    case 2:
        callbackSetCount = 2;
        CSF_AT(void*, callbackBytes, 0x0C) = reinterpret_cast<void*>(CSF_ADDR_FN(0x703D20));
        CSF_AT(void*, callbackBytes, 0x38) = reinterpret_cast<void*>(CSF_ADDR_FN(0x57F0B0));
        break;

    case 3:
        callbackSetCount = 3;
        CSF_AT(void*, callbackBytes, 0x6C) = reinterpret_cast<void*>(CSF_ADDR_FN(0x7038E0));
        CSF_AT(void*, callbackBytes, 0x58) = reinterpret_cast<void*>(CSF_ADDR_FN(0x6C2AB0));
        CSF_AT(void*, callbackBytes, 0x5C) = reinterpret_cast<void*>(CSF_ADDR_FN(0x6C2AB0));
        CSF_AT(void*, callbackBytes, 0x60) = reinterpret_cast<void*>(CSF_ADDR_FN(0x704CC0));
        CSF_AT(void*, callbackBytes, 0x0C) = reinterpret_cast<void*>(CSF_ADDR_FN(0x704000));
        break;

    default:
        return RpPrtStdEmitterClassSetCallBack(
            emitterClass, callbackSetCount, reinterpret_cast<int>(callbackBytes));
    }

    CSF_AT(void*, callbackBytes, 0x40) = reinterpret_cast<void*>(CSF_ADDR_FN(0x7038B0));
    CSF_AT(void*, callbackBytes, 0x2C) = reinterpret_cast<void*>(CSF_ADDR_FN(0x7043D0));
    CSF_AT(void*, callbackBytes, 0x30) = reinterpret_cast<void*>(CSF_ADDR_FN(0x6C2AB0));
    CSF_AT(void*, callbackBytes, 0x34) = reinterpret_cast<void*>(CSF_ADDR_FN(0x704C30));

    return RpPrtStdEmitterClassSetCallBack(
        emitterClass, callbackSetCount, reinterpret_cast<int>(callbackBytes));
}

HOOK(0x006E0670, GfxParticleEmitters_SetEmitterClassCallbacks, "auto");

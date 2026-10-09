#include <cstdint>

#include "globals.h"
#include "hook.h"

// Float bias used to convert unsigned 32-bit values (2^32) when the high bit is set.
CSF_GLOBAL(float, g_UnsignedIntegerBias, 0x0082D744);
CSF_GLOBAL(uint32_t, g_ScreenWidth, 0x00899B9C);
CSF_GLOBAL(uint32_t, g_ScreenHeight, 0x00899BA0);

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(void*, __fastcall, LookupObjectById, 0x00652160, (void* self, void* edx));

// Converts an unsigned 32-bit value to float the way the original does:
// signed int->float conversion, then add the 2^32 bias when the high bit was set.
static float UnsignedToFloat(uint32_t value)
{
    float result = static_cast<float>(static_cast<int32_t>(value));
    if (static_cast<int32_t>(value) < 0)
        result += g_UnsignedIntegerBias;
    return result;
}

// Writes the normalized (value / screen size) pair into out[0] and out[1].
// Returns the second raw value (the one divided by g_ScreenHeight).
static unsigned int __cdecl Hud_WriteNormalizedScreenSize(float* out)
{
    void* globalObjA = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
    void* objectA = LookupObjectById(globalObjA, nullptr);
    void* (__fastcall* getRecordA)(void*, void*) =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, objectA, 0)[6]);
    void* recordA = getRecordA(objectA, nullptr);
    uint32_t widthSource = CSF_AT(uint32_t, recordA, 0);

    void* globalObjB = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
    void* objectB = LookupObjectById(globalObjB, nullptr);
    void* (__fastcall* getRecordB)(void*, void*) =
        reinterpret_cast<void* (__fastcall*)(void*, void*)>(CSF_AT(void**, objectB, 0)[6]);
    void* recordB = getRecordB(objectB, nullptr);
    unsigned int result = CSF_AT(uint32_t, recordB, 4);

    out[0] = UnsignedToFloat(widthSource) / UnsignedToFloat(g_ScreenWidth);
    out[1] = UnsignedToFloat(result) / UnsignedToFloat(g_ScreenHeight);
    return result;
}

HOOK(0x42DE60, Hud_WriteNormalizedScreenSize, "auto");

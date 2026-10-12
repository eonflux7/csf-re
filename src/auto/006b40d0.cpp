#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(unsigned int, __fastcall, WeaponFirearms_DecodeColor, 0x00476430, (float* self, void* , unsigned int color));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* , int key));

CSF_GLOBAL(float, g_ColorBlack, 0x008A7DA0);
CSF_GLOBAL(float, g_ColorBlack_G, 0x008A7DA4);
CSF_GLOBAL(float, g_ColorBlack_B, 0x008A7DA8);
CSF_GLOBAL(float, g_ColorBlack_A, 0x008A7DAC);

using EntryTableGetterFn = C_0085e7b8* (__fastcall*)(void* self, void* edx);

static uint32_t FloatBitsOf(float value)
{
    uint32_t bits;
    std::memcpy(&bits, &value, sizeof(bits));
    return bits;
}

// Returns the tint color as four dwords (RGBA). If the object's entry table has
// a payload under key 6, the color is decoded from it; otherwise the default
// black color globals are used.
static uint32_t* __fastcall C_00860288_GetTintColorDwords(C_00860288* self, void* /*edx*/, uint32_t* outColor)
{
    void* entryTableOwner = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0068));
    // Virtual slot 7 (byte offset 0x1C) on the owner object; no stack arguments.
    auto getEntryTable = reinterpret_cast<EntryTableGetterFn>(CSF_AT(void**, entryTableOwner, 0)[7]);
    C_0085e7b8* entryTable = getEntryTable(entryTableOwner, nullptr);

    int payloadAddress = C_0085e7b8_FindEntryPayloadByKey(entryTable, nullptr, 6);
    if (payloadAddress != 0)
    {
        auto* entryPayload = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(static_cast<uint32_t>(payloadAddress)));

        uint32_t colorBuffer[4];
        colorBuffer[0] = 0;
        colorBuffer[1] = 0;
        colorBuffer[2] = 0;
        colorBuffer[3] = 0x3F800000u;

        WeaponFirearms_DecodeColor(reinterpret_cast<float*>(colorBuffer), nullptr, *entryPayload);

        outColor[0] = colorBuffer[0];
        outColor[1] = colorBuffer[1];
        outColor[2] = colorBuffer[2];
        outColor[3] = colorBuffer[3];
        return outColor;
    }

    outColor[0] = FloatBitsOf(g_ColorBlack);
    outColor[1] = FloatBitsOf(g_ColorBlack_G);
    outColor[2] = FloatBitsOf(g_ColorBlack_B);
    outColor[3] = FloatBitsOf(g_ColorBlack_A);
    return outColor;
}

HOOK(0x006B40D0, C_00860288_GetTintColorDwords, "auto");

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, GfxIm3d_GetObjectValue, 0x006E2E90, (int entryHandle));
CSF_FUNC(char, __cdecl, GfxObject_CheckObjectState, 0x006E2F30, (int objectHandle));
CSF_FUNC(bool, __cdecl, GfxIm3d_IsSpecialResourceName, 0x006E2F80, (char* source));

static char* __cdecl GfxAnimInterpolator_AccumulateObjectStatistics(
    char* object, std::uint32_t* counts)
{
    char* resource = CSF_AT(char*, object, 0);
    if (!counts || !resource)
        return object;

    const bool special = GfxIm3d_IsSpecialResourceName(object + 16);
    if (special)
    {
        ++counts[1];
        const int value = GfxIm3d_GetObjectValue(reinterpret_cast<int>(object));
        counts[3] += value;
    }
    else
    {
        ++counts[0];
        const int value = GfxIm3d_GetObjectValue(reinterpret_cast<int>(object));
        counts[2] += value;
    }

    if (GfxObject_CheckObjectState(reinterpret_cast<int>(object)))
    {
        if (special)
            ++counts[5];
        else
            ++counts[4];
    }

    const int flags = static_cast<int>(
        CSF_AT(std::uint8_t, resource, 0x23)) << 8;

    if ((flags & 0x2000) != 0)
    {
        if (special)
            ++counts[7];
        else
            ++counts[6];
    }

    switch (flags & 0xF00)
    {
    case 768:
        if (special)
            ++counts[14];
        else
            ++counts[9];
        break;
    case 256:
        if (special)
            ++counts[13];
        else
            ++counts[8];
        break;
    case 512:
        if (special)
            ++counts[17];
        else
            ++counts[12];
        break;
    case 1280:
        if (special)
            ++counts[15];
        else
            ++counts[10];
        break;
    case 1536:
        if (special)
            ++counts[16];
        else
            ++counts[11];
        break;
    default:
        break;
    }

    return object;
}

HOOK(0x00707E50, GfxAnimInterpolator_AccumulateObjectStatistics, "auto");

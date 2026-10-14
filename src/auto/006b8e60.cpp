#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(uint32_t*, __cdecl, RwStreamReadInt32, 0x00629C90, (uint32_t* a1, void* a2, unsigned int a3));

CSF_GLOBAL(int32_t, g_ObjectPropsMinVersion, 0x008A7E38);
CSF_GLOBAL(int32_t, g_ObjectPropsMaxVersion, 0x008A7E24);
CSF_GLOBAL(uint8_t*, g_ObjectPropsData, 0x008BB484);

static int __cdecl GfxObjectProps_ReadObjectType(
    int streamAddress, int /*unused*/, int propertyOffset)
{
    int32_t* objectType =
        CSF_AT(int32_t*, g_ObjectPropsData, propertyOffset + 4);
    *objectType = 3;

    auto* stream = reinterpret_cast<uint32_t*>(streamAddress);
    RwStreamReadInt32(stream, &propertyOffset, 4);
    if (propertyOffset < g_ObjectPropsMinVersion ||
        propertyOffset > g_ObjectPropsMaxVersion)
        return 0;

    alignas(8) uint8_t valueBytes[0x4];
    if (propertyOffset == 1)
    {
        RwStreamReadInt32(stream, valueBytes, 4);

        const bool firstFlag = RwStreamReadInt32(stream, valueBytes, 4)
            ? CSF_AT(int32_t, valueBytes, 0) != 0
            : (static_cast<uint32_t>(propertyOffset) >> 24) != 0;

        const bool secondFlag = RwStreamReadInt32(stream, valueBytes, 4)
            ? CSF_AT(int32_t, valueBytes, 0) != 0
            : (static_cast<uint32_t>(propertyOffset) >> 24) != 0;

        *objectType = firstFlag ? (secondFlag ? 2 : 0) : 3;
    }
    else
    {
        RwStreamReadInt32(stream, valueBytes, 4);
        *objectType = CSF_AT(int32_t, valueBytes, 0);
    }

    return streamAddress;
}

HOOK(0x006B8E60, GfxObjectProps_ReadObjectType, "auto");

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GfxObjectProps_SetFlag, 0x0040ED00,
         (void* self, void* edx, int a2, char a3));
CSF_FUNC(uint32_t*, __cdecl, RwStreamReadInt32, 0x00629C90,
         (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(float*, __fastcall, S_006abb80_InitializeBounds, 0x006ABB80,
         (S_006abb80* self, void*, float xFirst, float xSecond,
          float yFirst, float ySecond, float zFirst, float zSecond));
CSF_FUNC(uint32_t*, __fastcall, GfxObjectProps_EnsureProperties, 0x006B8020,
         (void* self, void* edx));
CSF_FUNC(uint32_t*, __fastcall, GfxObjectProps_EnsureBounds, 0x006B8AE0,
         (void* self, void* edx));
CSF_FUNC(void, __fastcall, GfxObjectProps_FinalizeProperties, 0x006B97A0,
         (void* self, void* edx));
CSF_FUNC(char, __fastcall, GfxObjectProps_ResetProperties, 0x006B9930,
         (void* self, void* edx));
CSF_FUNC(void*, __cdecl, GfxObjectProps_ReadString, 0x006BA610,
         (uint32_t* a1));

CSF_GLOBAL(int, g_ObjectPropsMaximumVersion, 0x008A7E18);
CSF_GLOBAL(int, g_ObjectPropsMinimumVersion, 0x008A7E2C);
CSF_GLOBAL(void*, g_ObjectPropsPluginBase, 0x008BB478);

static int __cdecl GfxObjectProps_ReadFromStream(
    int streamAddress, int /*unused*/, int propertyOffset)
{
    void* object = CSF_AT(
        void*, static_cast<uint8_t*>(g_ObjectPropsPluginBase) + propertyOffset, 0x04);
    GfxObjectProps_ResetProperties(object, nullptr);

    auto* stream = reinterpret_cast<uint32_t*>(streamAddress);
    int version = 0;
    RwStreamReadInt32(stream, &version, 4);
    if (version < g_ObjectPropsMinimumVersion ||
        version > g_ObjectPropsMaximumVersion)
        return 0;

    uint32_t wordCount;
    switch (version)
    {
    case 1: wordCount = 4; break;
    case 2: wordCount = 5; break;
    case 3: wordCount = 6; break;
    case 4: wordCount = 7; break;
    case 5: wordCount = 8; break;
    case 6: wordCount = 9; break;
    case 7:
    case 8: wordCount = 10; break;
    case 9: wordCount = 12; break;
    case 10:
    case 11: wordCount = 13; break;
    default: wordCount = static_cast<uint32_t>(propertyOffset); break;
    }

    const uint32_t byteCount = 4 * wordCount;
    void* serialized = Mem_Alloc(byteCount);
    RwStreamReadInt32(stream, serialized, byteCount);

    // These packet vtable tags identify the called functions; no vtable is stored.
    (void)CSF_VTBL(0x006B8020);
    GfxObjectProps_EnsureProperties(object, nullptr);

    CSF_AT(uint32_t, object, 0x00) = CSF_AT(uint32_t, serialized, 0x00);
    CSF_AT(uint8_t, CSF_AT(void*, object, 0x10), 0x0C) =
        CSF_AT(uint8_t, serialized, 0x04);
    CSF_AT(uint32_t, CSF_AT(void*, object, 0x10), 0x14) =
        CSF_AT(uint32_t, serialized, 0x08);
    CSF_AT(uint32_t, CSF_AT(void*, object, 0x10), 0x18) =
        CSF_AT(uint32_t, serialized, 0x0C);

    switch (version)
    {
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
        CSF_AT(uint32_t, CSF_AT(void*, object, 0x10), 0x10) =
            CSF_AT(uint32_t, serialized, 0x10);
        GfxObjectProps_SetFlag(static_cast<uint8_t*>(object) + 4, nullptr,
                              64, CSF_AT(uint32_t, serialized, 0x14) != 0);
        if (version >= 4)
        {
            GfxObjectProps_SetFlag(static_cast<uint8_t*>(object) + 4, nullptr,
                                  2, CSF_AT(uint32_t, serialized, 0x18) != 0);
        }
        if (version >= 5)
        {
            CSF_AT(uint32_t, CSF_AT(void*, object, 0x10), 0x08) =
                CSF_AT(uint32_t, serialized, 0x1C);
        }
        if (version >= 6)
        {
            CSF_AT(uint8_t, CSF_AT(void*, object, 0x10), 0x0D) =
                CSF_AT(uint8_t, serialized, 0x20);
        }
        if (version >= 7)
        {
            GfxObjectProps_SetFlag(static_cast<uint8_t*>(object) + 4, nullptr,
                                  128, CSF_AT(uint32_t, serialized, 0x24) != 0);
            if (version >= 9)
            {
                GfxObjectProps_SetFlag(static_cast<uint8_t*>(object) + 4, nullptr,
                                      256, CSF_AT(uint32_t, serialized, 0x28) != 0);
                GfxObjectProps_SetFlag(static_cast<uint8_t*>(object) + 4, nullptr,
                                      512, CSF_AT(uint32_t, serialized, 0x2C) != 0);
            }
            if (version >= 10)
                CSF_AT(uint32_t, object, 0x18) =
                    CSF_AT(uint32_t, serialized, 0x30);
        }
        break;
    default:
        break;
    }

    Mem_Free(serialized);

    RwStreamReadInt32(stream, &propertyOffset, 4);
    CSF_AT(int, CSF_AT(void*, object, 0x10), 0x1C) = propertyOffset;
    if (version >= 8)
    {
        RwStreamReadInt32(stream, &propertyOffset, 4);
        CSF_AT(int, CSF_AT(void*, object, 0x10), 0x20) = propertyOffset;
        RwStreamReadInt32(stream, &propertyOffset, 4);
        CSF_AT(int, CSF_AT(void*, object, 0x10), 0x24) = propertyOffset;
    }
    if (version >= 11)
    {
        RwStreamReadInt32(stream, &propertyOffset, 4);
        CSF_AT(int, CSF_AT(void*, object, 0x10), 0x28) = propertyOffset;
        RwStreamReadInt32(stream, &propertyOffset, 4);
        CSF_AT(int, CSF_AT(void*, object, 0x10), 0x2C) = propertyOffset;
    }

    CSF_AT(void*, CSF_AT(void*, object, 0x10), 0x00) =
        GfxObjectProps_ReadString(stream);
    if (version >= 5)
    {
        CSF_AT(void*, CSF_AT(void*, object, 0x10), 0x04) =
            GfxObjectProps_ReadString(stream);
    }

    if (version >= 6)
    {
        int hasBounds = 0;
        RwStreamReadInt32(stream, &hasBounds, 4);
        if (hasBounds)
        {
            GfxObjectProps_EnsureBounds(object, nullptr);

            alignas(8) uint8_t boundsBytes[0x18] = {};
            RwStreamReadInt32(stream, boundsBytes, 24);
            auto* bounds = reinterpret_cast<float*>(boundsBytes);

            if (bounds[0] > bounds[1])
            {
                const float second = bounds[1];
                bounds[1] = bounds[0];
                bounds[0] = second;
            }
            if (bounds[2] > bounds[3])
            {
                const float second = bounds[3];
                bounds[3] = bounds[2];
                bounds[2] = second;
            }
            if (bounds[4] > bounds[5])
            {
                const float second = bounds[5];
                bounds[5] = bounds[4];
                bounds[4] = second;
            }

            (void)CSF_VTBL(0x006ABB80);
            S_006abb80_InitializeBounds(
                CSF_AT(S_006abb80*, object, 0x14), nullptr,
                bounds[1], bounds[0], bounds[3], bounds[2], bounds[5], bounds[4]);
        }
    }

    GfxObjectProps_FinalizeProperties(object, nullptr);
    return streamAddress;
}

HOOK(0x006BB220, GfxObjectProps_ReadFromStream, "auto");

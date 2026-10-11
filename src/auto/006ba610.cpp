#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __cdecl, RwStreamRead, 0x006293E0, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(uint32_t*, __cdecl, RwStreamReadInt32, 0x00629C90, (uint32_t* a1, void* a2, unsigned int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(void*, __fastcall, GfxObjectProps_ResolveProperty, 0x006BA370, (void* self, void* edx, void* a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int32_t, g_ObjectPropsPluginOffset, 0x008BB474);

static C_0082d410* __cdecl GfxObjectProps_ReadStreamProperty(uint32_t* stream)
{
    alignas(8) uint8_t lengthBytes[0x4];
    int32_t& length = *reinterpret_cast<int32_t*>(lengthBytes);

    RwStreamReadInt32(stream, &length, 4);
    if (length <= 0)
        return nullptr;

    char* text = static_cast<char*>(
        Mem_Alloc(static_cast<uint32_t>(length) + 1u));
    RwStreamRead(stream, text, static_cast<unsigned int>(length));
    text[length] = 0;

    alignas(8) uint8_t propertyBytes[0x1C];
    C_0082d410& property =
        *reinterpret_cast<C_0082d410*>(propertyBytes);

    CSF_AT(uint32_t, &property, 0x18) = 15;
    CSF_AT(uint32_t, &property, 0x14) = 0;
    reinterpret_cast<uint8_t*>(&property.field_0004)[0] = 0;

    AssignStringRange_429B60(
        &property, nullptr, text,
        static_cast<unsigned int>(std::strlen(text)));

    void* properties = *reinterpret_cast<void**>(
        static_cast<uint8_t*>(g_pRwEngineInstance) +
        g_ObjectPropsPluginOffset);
    C_0082d410* result = static_cast<C_0082d410*>(
        GfxObjectProps_ResolveProperty(properties, nullptr, &property));

    if (text)
        Mem_Free(text);

    if (CSF_AT(uint32_t, &property, 0x18) >= 0x10)
    {
        if (property.field_0004)
            Mem_FreeDebug(reinterpret_cast<void*>(property.field_0004));
    }

    return result;
}

HOOK(0x006BA610, GfxObjectProps_ReadStreamProperty, "auto");

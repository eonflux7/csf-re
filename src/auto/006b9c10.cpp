#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __fastcall, GfxObjectProps_ReleasePropertyValue, 0x006B96C0,
         (void* self, void* edx, int* a2));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int32_t, g_ObjectPropsEngineOffset, 0x008BB474);

static int __fastcall GfxObjectProps_ClearProperties(void* self, void* /*edx*/)
{
    for (uint32_t index = 0; ; ++index)
    {
        void* begin = CSF_AT(void*, self, 0x4);
        if (!begin)
            break;

        const uint32_t byteCount =
            CSF_AT(uint32_t, self, 0x8) -
            static_cast<uint32_t>(reinterpret_cast<uintptr_t>(begin));
        const uint32_t count =
            static_cast<uint32_t>(static_cast<int32_t>(byteCount) >> 3);
        if (index >= count)
            break;

        const uint32_t entryOffset = 8 * index;
        void* property = CSF_AT(void*, begin, entryOffset);
        if (property)
        {
            const int32_t engineOffset = g_ObjectPropsEngineOffset;
            CSF_AT(uint8_t, property, 0x1) = 0;
            CSF_AT(uint8_t, property, 0x0) = 0;
            CSF_AT(int32_t, property, 0x8) = -1;

            void* manager = CSF_AT(void*, g_pRwEngineInstance, engineOffset);
            GfxObjectProps_ReleasePropertyValue(
                manager, nullptr,
                reinterpret_cast<int*>(static_cast<uint8_t*>(property) + 0x4));

            CSF_AT(uint8_t, property, 0x11) = 0;
            Mem_Free(property);
            CSF_AT(void*, CSF_AT(void*, self, 0x4), entryOffset) = nullptr;
        }
    }

    void* storage = CSF_AT(void*, self, 0x4);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(storage));
    if (storage)
        result = Mem_FreeDebug(storage);

    CSF_AT(uint32_t, self, 0x4) = 0;
    CSF_AT(uint32_t, self, 0x8) = 0;
    CSF_AT(uint32_t, self, 0xC) = 0;
    return result;
}

HOOK(0x006B9C10, GfxObjectProps_ClearProperties, "auto");

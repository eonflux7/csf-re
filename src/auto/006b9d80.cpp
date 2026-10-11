#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, sub_6B96C0, 0x006B96C0, (void* self, void* edx, int* a2));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

CSF_GLOBAL(void*, g_pRwEngineInstance, 0x008B68C0);
CSF_GLOBAL(int32_t, dword_8BB474, 0x008BB474);

// Frees every non-null entry in the entry list (8-byte slots), then releases the list itself.
// Each entry is detached from the engine via sub_6B96C0 before being freed.
static int __fastcall GfxObjectProps_FreeEntriesAndReset(void* self, void* /*edx*/)
{
    for (uint32_t i = 0; ; ++i)
    {
        uint32_t listBegin = CSF_AT(uint32_t, self, 0x4);
        if (listBegin == 0)
            break;
        int32_t listEnd = static_cast<int32_t>(CSF_AT(uint32_t, self, 0x8));
        int32_t entryCount = (listEnd - static_cast<int32_t>(listBegin)) >> 3;
        if (i >= static_cast<uint32_t>(entryCount))
            break;

        uint32_t entryValue = CSF_AT(uint32_t, reinterpret_cast<void*>(listBegin), 8 * i);
        if (entryValue != 0)
        {
            void* entry = reinterpret_cast<void*>(entryValue);
            CSF_AT(int32_t, entry, 0x4) = -1;

            void* engineSubobject = *reinterpret_cast<void**>(
                reinterpret_cast<char*>(g_pRwEngineInstance) + dword_8BB474);
            sub_6B96C0(engineSubobject, nullptr, reinterpret_cast<int*>(entry));
            Mem_Free(entry);

            CSF_AT(uint32_t, reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x4)), 8 * i) = 0;
        }
    }

    int result = static_cast<int>(CSF_AT(uint32_t, self, 0x4));
    if (result != 0)
        result = Mem_FreeDebug(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x4)));

    CSF_AT(uint32_t, self, 0x4) = 0;
    CSF_AT(uint32_t, self, 0x8) = 0;
    CSF_AT(uint32_t, self, 0xC) = 0;
    return result;
}

HOOK(0x6B9D80, GfxObjectProps_FreeEntriesAndReset, "auto");

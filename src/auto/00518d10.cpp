#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(unsigned int, __fastcall, NetEntry_ReleaseObject, 0x0069AE00, (void* self, void* edx));

// Import slot holding the address of user32!DestroyWindow (called indirectly in the original).
CSF_GLOBAL(void*, g_DestroyWindowImport, 0x0082D2FC);

static void DestroyWindowHandle(uint32_t hwnd)
{
    using DestroyWindowFn = int(__stdcall*)(void*);
    DestroyWindowFn destroyWindow = reinterpret_cast<DestroyWindowFn>(g_DestroyWindowImport);
    destroyWindow(reinterpret_cast<void*>(hwnd));
}

static int __fastcall NetProfile_ReleaseWindowsAndEntries(void* self, void* /*edx*/)
{
    if (CSF_AT(uint32_t, self, 0x04) != 0)
    {
        DestroyWindowHandle(CSF_AT(uint32_t, self, 0x04));
        CSF_AT(uint32_t, self, 0x04) = 0;
    }
    if (CSF_AT(uint32_t, self, 0x08) != 0)
    {
        DestroyWindowHandle(CSF_AT(uint32_t, self, 0x08));
        CSF_AT(uint32_t, self, 0x08) = 0;
    }

    uint32_t entryCount = CSF_AT(uint32_t, self, 0x14);
    if (entryCount != 0)
    {
        uint32_t remaining = entryCount;
        uint32_t offset = 0;
        do
        {
            uint8_t* entries = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C));
            uint32_t entryObject = CSF_AT(uint32_t, entries, offset);
            if (entryObject != 0)
            {
                NetEntry_ReleaseObject(reinterpret_cast<void*>(entryObject), nullptr);
                Mem_Free(reinterpret_cast<void*>(entryObject));
                uint8_t* entriesAfterFree = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C));
                CSF_AT(uint32_t, entriesAfterFree, offset) = 0;
            }

            uint8_t* entriesForWindow = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x0C));
            uint32_t entryWindow = CSF_AT(uint32_t, entriesForWindow, offset + 4);
            if (entryWindow != 0)
            {
                DestroyWindowHandle(entryWindow);
            }

            offset += 24;
            --remaining;
        } while (remaining != 0);

        CSF_AT(uint32_t, self, 0x14) = 0;
    }

    return 0;
}

HOOK(0x00518D10, NetProfile_ReleaseWindowsAndEntries, "auto");

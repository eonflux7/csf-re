#include <cstdint>
#include "globals.h"
#include "hook.h"

// Callees (bound by address, original signatures)
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(int, __cdecl, sub_6A18C0, 0x006A18C0, (uint32_t* a1));

// Data
CSF_GLOBAL(uint32_t, g_LogEntryTable, 0x008A6BAC);
CSF_GLOBAL(uint32_t, g_LogEntryAux, 0x008A6BB0);
CSF_GLOBAL(uint32_t, g_LogEntryCount, 0x008A6BB4);

static int __fastcall LogWindow_ReleaseBuffers(uint32_t* self, void* /*edx*/)
{
    if (*self)
    {
        sub_6A18C0(reinterpret_cast<uint32_t*>(*self));
        if (*self)
        {
            Mem_Free(reinterpret_cast<void*>(*self));
            *self = 0;
        }
    }

    uint32_t entryIndex = 0;
    if (g_LogEntryCount)
    {
        uint32_t byteOffset = 0;
        do
        {
            void* entryBlock = *reinterpret_cast<void**>(static_cast<uint8_t*>(reinterpret_cast<void*>(g_LogEntryTable)) + byteOffset);
            Mem_FreeDebug(entryBlock);
            ++entryIndex;
            byteOffset += 12;
        } while (entryIndex < g_LogEntryCount);
    }

    g_LogEntryAux = 0;
    g_LogEntryCount = 0;
    if (g_LogEntryTable)
    {
        Mem_Free(reinterpret_cast<void*>(g_LogEntryTable));
        g_LogEntryTable = 0;
    }
    return 0;
}

HOOK(0x006A2F00, LogWindow_ReleaseBuffers, "auto");

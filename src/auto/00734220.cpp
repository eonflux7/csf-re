#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_Buffer, 0x008A88BC);
CSF_GLOBAL(uint32_t, g_BufferCapacity, 0x008A88C0);
CSF_GLOBAL(uint32_t, g_BufferCount, 0x008A88C4);

CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));
CSF_FUNC(char, __fastcall, GrowBuffer, 0x00732D00, (void* self, void* edx, int a2));

static int __cdecl ResetBufferTo35Entries()
{
    if (g_BufferCapacity <= 0x23)
        return 0;

    void* oldBuffer = g_Buffer;
    g_BufferCapacity = 0;
    g_BufferCount = 0;
    if (oldBuffer)
    {
        Mem_Free(oldBuffer);
        g_Buffer = nullptr;
    }

    SetAllocationMode(3);
    if (g_BufferCapacity < 0x23)
        GrowBuffer(&g_Buffer, nullptr, static_cast<int>(35 - g_BufferCapacity));

    g_BufferCount = 35;
    return SetAllocationMode(0);
}

HOOK(0x00734220, ResetBufferTo35Entries, "auto");

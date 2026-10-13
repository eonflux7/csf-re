#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ForwardBlockHandler, 0x0069DFA0, (void* self, void* edx, void* Block));
CSF_FUNC(size_t, __fastcall, C_0085eac0_RoundBlockSizeToClass, 0x0069E260, (C_0085eac0* self, void* , void* Block));
CSF_FUNC(bool, __fastcall, C_0085eb30_WaitForEventHandle, 0x0069E3A0, (C_0085eb30* self, void* , uint32_t dwMilliseconds));
CSF_FUNC(int, __fastcall, C_0085eb30_ReleaseMutexIfSet, 0x0069E3D0, (C_0085eb30* self, void*));

CSF_GLOBAL(uint8_t, g_bMemDebugTrackingEnabled, 0x008BAD88);
CSF_GLOBAL(int, g_MemDebugTotalAllocatedBytes, 0x008BAD8C);
CSF_GLOBAL(uint8_t, g_unk_8BAD98, 0x008BAD98);
CSF_GLOBAL(void*, g_MemDebugMutex, 0x008BAEB4);

static char __cdecl MemDebug_FreeDebugBlock(void* block)
{
    if (!C_0085eb30_WaitForEventHandle(
            reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex), nullptr, 0))
        return 0;

    if (g_bMemDebugTrackingEnabled != 0)
    {
        if (block != nullptr)
        {
            size_t roundedSize = C_0085eac0_RoundBlockSizeToClass(
                reinterpret_cast<C_0085eac0*>(&g_unk_8BAD98), nullptr, block);
            g_MemDebugTotalAllocatedBytes -= static_cast<int>(roundedSize);
        }
    }

    ForwardBlockHandler(&g_unk_8BAD98, nullptr, block);
    return static_cast<char>(C_0085eb30_ReleaseMutexIfSet(
        reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex), nullptr));
}

HOOK(0x00690E60, MemDebug_FreeDebugBlock, "auto");

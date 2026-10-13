#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(size_t, __fastcall, C_0085eac0_RoundBlockSizeToClass, 0x0069E260, (C_0085eac0* self, void* /*edx*/, void* Block));
CSF_FUNC(void, __fastcall, C_0085eac0_ForwardBlock, 0x0069E040, (C_0085eac0* self, void* /*edx*/, void* Block));
CSF_FUNC(bool, __fastcall, C_0085eb30_WaitForEventHandle, 0x0069E3A0, (C_0085eb30* self, void* /*edx*/, uint32_t dwMilliseconds));
CSF_FUNC(int, __fastcall, C_0085eb30_ReleaseMutexIfSet, 0x0069E3D0, (C_0085eb30* self, void* /*edx*/));

CSF_GLOBAL(uint8_t, g_bMemDebugTrackingEnabled, 0x008BAD88);
CSF_GLOBAL(int, g_MemDebugTotalAllocatedBytes, 0x008BAD8C);
CSF_GLOBAL(uint8_t, g_unk_8BAD98, 0x008BAD98);
CSF_GLOBAL(void*, g_MemDebugMutex, 0x008BAEB4);

static char MemDebug_FreeBlock(void* block)
{
    // The original tests the wait result as a byte (test al, al).
    uint8_t acquired = C_0085eb30_WaitForEventHandle(
        reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex), nullptr, 0) ? 1 : 0;
    if (acquired == 0)
        return 0;

    // Tracking flag is a byte in the original; test it as a byte, not as a bool.
    if (g_bMemDebugTrackingEnabled != 0)
    {
        if (block != nullptr)
        {
            size_t roundedSize = C_0085eac0_RoundBlockSizeToClass(
                reinterpret_cast<C_0085eac0*>(&g_unk_8BAD98), nullptr, block);
            g_MemDebugTotalAllocatedBytes -= static_cast<int>(roundedSize);
        }
    }

    C_0085eac0_ForwardBlock(reinterpret_cast<C_0085eac0*>(&g_unk_8BAD98), nullptr, block);
    return static_cast<char>(
        C_0085eb30_ReleaseMutexIfSet(reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex), nullptr));
}

HOOK(0x690F70, MemDebug_FreeBlock, "auto");

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, CoreMemDebug_TryRecoverAllocation, 0x00690D40, ());
CSF_FUNC(void*, __fastcall, Tokenizer_AllocInner, 0x0069DEF0, (void* self, void* edx, uint32_t Size, int a3, int a4));
CSF_FUNC(bool, __fastcall, C_0085eb30_WaitForEventHandle, 0x0069E3A0, (C_0085eb30* self, void* , uint32_t dwMilliseconds));
CSF_FUNC(int, __fastcall, C_0085eb30_ReleaseMutexIfSet, 0x0069E3D0, (C_0085eb30* self, void*));

CSF_GLOBAL(const char*, g_MemDebugFile, 0x008BAD78);
CSF_GLOBAL(int, g_MemDebugLine, 0x008BAD7C);
CSF_GLOBAL(bool, g_bMemDebugTrackingEnabled, 0x008BAD88);
CSF_GLOBAL(int, g_MemDebugTotalAllocatedBytes, 0x008BAD8C);
CSF_GLOBAL(uint8_t, g_MemDebugAllocationStatus, 0x008BAD90);
CSF_GLOBAL(uint8_t, g_unk_8BAD98, 0x008BAD98);
CSF_GLOBAL(void*, g_MemDebugMutex, 0x008BAEB4);

static void* __cdecl CoreMemDebug_Allocate(
    size_t size, int file, int line)
{
    void* allocation = nullptr;
    if (C_0085eb30_WaitForEventHandle(
            reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex), nullptr, 0))
    {
        if (g_bMemDebugTrackingEnabled)
            g_MemDebugTotalAllocatedBytes += size;

        if (file)
        {
            allocation = Tokenizer_AllocInner(
                &g_unk_8BAD98, nullptr, size, file, line);
        }
        else
        {
            allocation = Tokenizer_AllocInner(
                &g_unk_8BAD98, nullptr, size,
                reinterpret_cast<int>(g_MemDebugFile), g_MemDebugLine);
        }

        if (allocation || !size)
        {
            g_MemDebugAllocationStatus = 0;
        }
        else if (CoreMemDebug_TryRecoverAllocation())
        {
            allocation = CoreMemDebug_Allocate(size, file, line);
            C_0085eb30_ReleaseMutexIfSet(
                reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex), nullptr);
            return allocation;
        }

        C_0085eb30_ReleaseMutexIfSet(
            reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex), nullptr);
    }
    return allocation;
}

HOOK(0x00690DC0, CoreMemDebug_Allocate, "auto");

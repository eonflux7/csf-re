#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, CoreMemDebugFree_TryRecoverAllocation, 0x00690D40, ());
CSF_FUNC(int, __stdcall, GetFirstArgument, 0x0069E010, (int first, int));
CSF_FUNC(void*, __fastcall, C_0085eac0_ForwardAlloc, 0x0069E020, (C_0085eac0* self, void* edx, size_t Size, int a3, int a4, int a5));
CSF_FUNC(bool, __fastcall, C_0085eb30_WaitForEventHandle, 0x0069E3A0, (C_0085eb30* self, void* , uint32_t dwMilliseconds));
CSF_FUNC(int, __fastcall, C_0085eb30_ReleaseMutexIfSet, 0x0069E3D0, (C_0085eb30* self, void*));

CSF_GLOBAL(const char*, g_MemDebugFile, 0x008BAD78);
CSF_GLOBAL(int, g_MemDebugLine, 0x008BAD7C);
CSF_GLOBAL(uint8_t, g_bMemDebugTrackingEnabled, 0x008BAD88);
CSF_GLOBAL(int, g_MemDebugTotalAllocatedBytes, 0x008BAD8C);
CSF_GLOBAL(uint8_t, g_MemDebugAllocationStatus, 0x008BAD90);
CSF_GLOBAL(uint8_t, g_unk_8BAD98, 0x008BAD98);
CSF_GLOBAL(void*, g_MemDebugMutex, 0x008BAEB4);

static void* __cdecl CoreMemDebugFree_AllocateAlignedBuffer(
    size_t size, int alignment, int file, int line)
{
    if (!C_0085eb30_WaitForEventHandle(
            reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex), nullptr, 0))
    {
        return nullptr;
    }

    if (g_bMemDebugTrackingEnabled != 0)
    {
        const int trackedBytes = GetFirstArgument(static_cast<int>(size), alignment);
        g_MemDebugTotalAllocatedBytes = static_cast<int>(
            static_cast<uint32_t>(g_MemDebugTotalAllocatedBytes) +
            static_cast<uint32_t>(trackedBytes));
    }

    void* buffer;
    if (file != 0)
    {
        buffer = C_0085eac0_ForwardAlloc(
            reinterpret_cast<C_0085eac0*>(&g_unk_8BAD98),
            nullptr, size, alignment, file, line);
    }
    else
    {
        buffer = C_0085eac0_ForwardAlloc(
            reinterpret_cast<C_0085eac0*>(&g_unk_8BAD98),
            nullptr, size, alignment,
            reinterpret_cast<int>(g_MemDebugFile), g_MemDebugLine);
    }

    if (buffer || size == 0)
    {
        g_MemDebugAllocationStatus = 0;
    }
    else if (CoreMemDebugFree_TryRecoverAllocation() != 0)
    {
        buffer = CoreMemDebugFree_AllocateAlignedBuffer(size, alignment, file, line);
        C_0085eb30_ReleaseMutexIfSet(
            reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex), nullptr);
        return buffer;
    }

    C_0085eb30_ReleaseMutexIfSet(
        reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex), nullptr);
    return buffer;
}

HOOK(0x00690EB0, CoreMemDebugFree_AllocateAlignedBuffer, "auto");

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, CoreMemDebugFree_ShouldRetryAllocation, 0x00690D40, ());
CSF_FUNC(uint32_t*, __fastcall, CoreMemDebugFree_ReallocateBlock, 0x0069E050, (void* self, void* edx, uint32_t* Block, uint32_t Size, int a4, int a5));
CSF_FUNC(size_t, __fastcall, C_0085eac0_RoundBlockSizeToClass, 0x0069E260, (C_0085eac0* self, void* , void* Block));
CSF_FUNC(bool, __fastcall, C_0085eb30_WaitForEventHandle, 0x0069E3A0, (C_0085eb30* self, void* , uint32_t dwMilliseconds));
CSF_FUNC(int, __fastcall, C_0085eb30_ReleaseMutexIfSet, 0x0069E3D0, (C_0085eb30* self, void*));

CSF_GLOBAL(const char*, g_MemDebugFile, 0x008BAD78);
CSF_GLOBAL(int, g_MemDebugLine, 0x008BAD7C);
CSF_GLOBAL(uint8_t, g_bMemDebugTrackingEnabled, 0x008BAD88);
CSF_GLOBAL(int, g_MemDebugTotalAllocatedBytes, 0x008BAD8C);
CSF_GLOBAL(uint8_t, g_MemDebugFailureFlag, 0x008BAD90);
CSF_GLOBAL(uint8_t, g_unk_8BAD98, 0x008BAD98);
CSF_GLOBAL(void*, g_MemDebugMutex, 0x008BAEB4);

static uint32_t* __cdecl CoreMemDebugFree_ReallocateDebugBlock(
    uint32_t* block, size_t size, int file, int line)
{
    auto* mutex = reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex);
    if (!C_0085eb30_WaitForEventHandle(mutex, nullptr, 0))
        return nullptr;

    auto* allocator = reinterpret_cast<C_0085eac0*>(&g_unk_8BAD98);
    if (g_bMemDebugTrackingEnabled != 0 && block != nullptr)
    {
        const size_t oldSize =
            C_0085eac0_RoundBlockSizeToClass(allocator, nullptr, block);
        g_MemDebugTotalAllocatedBytes = static_cast<int>(
            static_cast<uint32_t>(g_MemDebugTotalAllocatedBytes) +
            static_cast<uint32_t>(size - oldSize));
    }

    uint32_t* result;
    if (file != 0)
    {
        result = CoreMemDebugFree_ReallocateBlock(
            allocator, nullptr, block, size, file, line);
    }
    else
    {
        const int defaultLine = g_MemDebugLine;
        const int defaultFile =
            static_cast<int>(reinterpret_cast<intptr_t>(g_MemDebugFile));
        result = CoreMemDebugFree_ReallocateBlock(
            allocator, nullptr, block, size, defaultFile, defaultLine);
    }

    if (result != nullptr || size == 0)
    {
        g_MemDebugFailureFlag = 0;
    }
    else if (CoreMemDebugFree_ShouldRetryAllocation() != 0)
    {
        result = CoreMemDebugFree_ReallocateDebugBlock(block, size, file, line);
        C_0085eb30_ReleaseMutexIfSet(mutex, nullptr);
        return result;
    }

    C_0085eb30_ReleaseMutexIfSet(mutex, nullptr);
    return result;
}

HOOK(0x00690FC0, CoreMemDebugFree_ReallocateDebugBlock, "auto");

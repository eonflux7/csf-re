#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __cdecl, sub_690FC0, 0x00690FC0, (uint32_t* Block, uint32_t Size, int a3, int a4));
CSF_FUNC(int, __stdcall, GetFirstArgument, 0x0069E010, (int first, int));
CSF_FUNC(uint32_t*, __fastcall, sub_69E200, 0x0069E200, (void* self, void* edx, uint32_t* Block, uint32_t Size, int a4, int a5, int a6));
CSF_FUNC(uint32_t, __fastcall, sub_69E260, 0x0069E260, (void* self, void* edx, void* Block));
CSF_FUNC(bool, __fastcall, sub_69E3A0, 0x0069E3A0, (void* self, void* edx, uint32_t dwMilliseconds));
CSF_FUNC(int, __fastcall, C_0085eb30_ReleaseMutexIfSet, 0x0069E3D0, (C_0085eb30* self, void*));

CSF_GLOBAL(const char*, g_MemDebugFile, 0x008BAD78);
CSF_GLOBAL(int, g_MemDebugLine, 0x008BAD7C);
CSF_GLOBAL(uint8_t, g_bMemDebugTrackingEnabled, 0x008BAD88);
CSF_GLOBAL(int, g_MemDebugTotalAllocatedBytes, 0x008BAD8C);
CSF_GLOBAL(uint8_t, g_unk_8BAD98, 0x008BAD98);
CSF_GLOBAL(void*, g_MemDebugMutex, 0x008BAEB4);

static uint32_t* __cdecl CoreMemory_ReallocateTagged(
    uint32_t* block, std::size_t size, uint16_t tag)
{
    if (tag == 27 || tag == 22 || tag == 24 || tag == 1286 || tag == 287)
        return sub_690FC0(block, static_cast<uint32_t>(size), 0, 0);

    uint32_t* result = nullptr;
    if (sub_69E3A0(&g_MemDebugMutex, nullptr, 0))
    {
        // The original tests the flag byte with `test al, al`: any nonzero value enables tracking.
        if (g_bMemDebugTrackingEnabled != 0 && block)
        {
            const uint32_t oldSize =
                sub_69E260(&g_unk_8BAD98, nullptr, block);
            const uint32_t remainingBytes =
                static_cast<uint32_t>(g_MemDebugTotalAllocatedBytes) - oldSize;
            const int requestedBytes =
                GetFirstArgument(static_cast<int>(size), 64);
            g_MemDebugTotalAllocatedBytes = static_cast<int>(
                remainingBytes + static_cast<uint32_t>(requestedBytes));
        }

        const int debugLine = g_MemDebugLine;
        const int debugFile = reinterpret_cast<int>(g_MemDebugFile);
        result = sub_69E200(
            &g_unk_8BAD98, nullptr, block, static_cast<uint32_t>(size),
            64, debugFile, debugLine);

        C_0085eb30_ReleaseMutexIfSet(
            reinterpret_cast<C_0085eb30*>(&g_MemDebugMutex), nullptr);
    }
    return result;
}

HOOK(0x00691190, CoreMemory_ReallocateTagged, "auto");

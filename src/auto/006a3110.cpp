// Module: core_zlib_log - DlgLog shared log window accessor.
#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Shared log window instance (dword_8BB414) and its acquire count (dword_8BB410).
CSF_GLOBAL(uint32_t, g_SharedLogWindowInstance, 0x008BB414);
CSF_GLOBAL(int, g_SharedLogWindowRefCount, 0x008BB410);

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(void**, __fastcall, LogWindow_Construct, 0x006A30A0, (void* self, void* edx));

static void** __cdecl GetSharedLogWindow()
{
    [[maybe_unused]] const void* const logWindowVtbl = CSF_VTBL(0x006A30A0);

    void** result = reinterpret_cast<void**>(g_SharedLogWindowInstance);
    if (result == nullptr)
    {
        void* allocated = Mem_Alloc(4);
        if (allocated != nullptr)
        {
            result = LogWindow_Construct(allocated, nullptr);
            g_SharedLogWindowInstance = reinterpret_cast<uint32_t>(result);
            ++g_SharedLogWindowRefCount;
            return result;
        }
        result = nullptr;
        g_SharedLogWindowInstance = 0;
    }
    ++g_SharedLogWindowRefCount;
    return result;
}

HOOK(0x006A3110, GetSharedLogWindow, "auto");

#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"

// Original game functions this routine calls, bound by address.
CSF_FUNC(void*, __fastcall, OnlineEnv_ReleaseSubobjects, 0x0054E700, (void* self, void* edx));
CSF_FUNC(char, __fastcall, OnlineEnv_InitializeSubobjects, 0x0054EA90, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// Global holding the current online-menu environment object (0 when none).
CSF_GLOBAL(int, g_Global_8B0680, 0x008B0680);

static int __cdecl OnlineMenu_PrepareEnvironment()
{
    // Release any previously created environment object.
    if (g_Global_8B0680 != 0)
    {
        void* previousEnv = reinterpret_cast<void*>(static_cast<std::uintptr_t>(g_Global_8B0680));
        OnlineEnv_ReleaseSubobjects(previousEnv, nullptr);

        void* ownedBuffer = CSF_AT(void*, previousEnv, 0x04);
        if (ownedBuffer != nullptr)
        {
            Mem_FreeDebug(ownedBuffer);
        }
        CSF_AT(void*, previousEnv, 0x04) = nullptr;
        CSF_AT(int, previousEnv, 0x08) = 0;
        CSF_AT(int, previousEnv, 0x0C) = 0;
        Mem_Free(previousEnv);
        g_Global_8B0680 = 0;
    }

    // Allocate and initialize the new environment object (20 bytes).
    void* newEnv = Mem_Alloc(20);
    if (newEnv != nullptr)
    {
        CSF_AT(int, newEnv, 0x04) = 0;
        CSF_AT(int, newEnv, 0x08) = 0;
        CSF_AT(int, newEnv, 0x0C) = 0;
        CSF_AT(int, newEnv, 0x10) = 0;
        OnlineEnv_InitializeSubobjects(newEnv, nullptr);
        g_Global_8B0680 = static_cast<int>(reinterpret_cast<std::uintptr_t>(newEnv));
        return newEnv != nullptr ? 1 : 0;
    }

    g_Global_8B0680 = 0;
    return 0;
}

HOOK(0x0054EC30, OnlineMenu_PrepareEnvironment, "auto");

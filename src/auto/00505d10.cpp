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

// Win32 imports, called indirectly through the game's IAT slots.
typedef uint32_t(__stdcall* WaitForSingleObjectFn)(void* handle, uint32_t timeoutMs);
typedef int(__stdcall* CloseHandleFn)(void* handle);

CSF_GLOBAL(WaitForSingleObjectFn, g_pfnWaitForSingleObject, 0x0082D20C);
CSF_GLOBAL(CloseHandleFn, g_pfnCloseHandle, 0x0082D0E0);

CSF_FUNC(int, __fastcall, SceneLoader_ReleaseResources, 0x005035D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, Mem_Free, 0x00691110, (void* block));

// CSceneLoader::~CSceneLoader-style teardown: releases the load-done event handle when
// it is owned, resets the object's vtable/base-vtable slots, and frees the block when
// the low bit of the flag argument is set. Returns the object pointer.
static void* __fastcall CSceneLoader_Destroy(CSceneLoader* self, void* /*edx*/, char flags)
{
    // Read the ownership flag before the vtable is rewritten (original order).
    uint8_t handleOwned = CSF_AT(uint8_t, self, 0xB5);

    self->vtbl = CSF_VTBL(0x0083F5D8);
    self->field_0004 = (uint32_t)CSF_ADDR_DATA(0x0083F5D0);

    if (handleOwned)
    {
        void* eventHandle = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x110));
        CSF_AT(uint8_t, self, 0xB5) = 0;
        g_pfnWaitForSingleObject(eventHandle, 0);
        g_pfnCloseHandle(reinterpret_cast<void*>(CSF_AT(uint32_t, self, 0x110)));
    }

    SceneLoader_ReleaseResources(self, nullptr);

    if ((flags & 1) != 0)
    {
        Mem_Free(self);
    }

    return self;
}

HOOK(0x505D10, CSceneLoader_Destroy, "auto");

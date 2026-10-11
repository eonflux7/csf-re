#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, RenderHandle_FromObject, 0x006B2D50, (void* self, void* edx));
CSF_FUNC(void, __fastcall, SceneLoader_SetRenderWorld, 0x006D6A40, (void* self, void* edx, int a2));

// Releases the scene's two owned sub-objects: the one at +0x9C (after detaching it
// from the render world) and the one at +0x98. Each is released via vtable slot 1
// with arg 1, then its pointer is cleared.
static int __fastcall SceneLoader_ReleaseResources(void* self, void* /*edx*/)
{
    int result = CSF_AT(int, self, 0x9C);

    if (result != 0)
    {
        int renderWorld = CSF_AT(int, self, 0x9C);
        void* globalObject = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalInt()));
        int renderHandle = RenderHandle_FromObject(globalObject, nullptr);
        SceneLoader_SetRenderWorld(reinterpret_cast<void*>(static_cast<uintptr_t>(renderHandle)),
                                   nullptr, renderWorld);
    }

    void* ownedAt98 = CSF_AT(void*, self, 0x98);
    if (ownedAt98 != nullptr)
    {
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedAt98, 0)[1])(ownedAt98, nullptr, 1);
        CSF_AT(void*, self, 0x98) = nullptr;
    }

    void* ownedAt9C = CSF_AT(void*, self, 0x9C);
    if (ownedAt9C != nullptr)
    {
        result = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, ownedAt9C, 0)[1])(ownedAt9C, nullptr, 1);
        CSF_AT(void*, self, 0x9C) = nullptr;
    }

    return result;
}

HOOK(0x006CADD0, SceneLoader_ReleaseResources, "auto");

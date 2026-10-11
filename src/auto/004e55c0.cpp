#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, sub_44E1C0, 0x0044E1C0, (void* self, void* edx, int a2, int a3, int a4));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Returns true when the handle entry's owner tag already equals the owner's
// current tag; otherwise resolves the handle in the current scene and, unless
// it is inactive (or a3 == 10), forwards it to the scene subsystem.
static bool __fastcall C_0083e9d0_ResolveAndDispatchHandleEntry(C_0083e9d0* self, void* /*edx*/, int a2, int a3)
{
    void* handleEntry = reinterpret_cast<void*>(static_cast<uintptr_t>(a2));

    uint32_t entryTag = CSF_AT(uint32_t, handleEntry, 0x4);
    uint32_t ownerTag = CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0008), 0x20);
    if (entryTag == ownerTag)
        return true;

    int resolvedHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, a2);
    if (resolvedHandle == 0)
        return false;

    void* sceneObj = reinterpret_cast<void*>(static_cast<uintptr_t>(resolvedHandle));
    if (a3 == 10 || !SceneObj_IsInactive(sceneObj, nullptr))
    {
        void* sceneSubsystem = CSF_AT(void*, g_pCurrentScene, 0xB0);
        void* dispatcher = CSF_AT(void*, sceneSubsystem, 0x114);

        uint8_t* ownerBase = reinterpret_cast<uint8_t*>(self->field_0008);
        int ownerTagAddress = static_cast<int>(reinterpret_cast<uintptr_t>(ownerBase + 0x1C));

        return sub_44E1C0(dispatcher, nullptr, ownerTagAddress, a2, a3);
    }
    return false;
}

HOOK(0x4E55C0, C_0083e9d0_ResolveAndDispatchHandleEntry, "auto");

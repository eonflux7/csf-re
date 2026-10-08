#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, ResolveClumpManager, 0x00652020, (int managerId, void* edx));

// Calls the virtual slot `slotIndex` of `object` with one stack argument (the handle).
static int InvokeVirtualWithHandle(int object, std::size_t slotIndex, uint32_t handle)
{
    void* obj = reinterpret_cast<void*>(object);
    void* fn = CSF_AT(void**, obj, 0)[slotIndex];
    return reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(fn)(obj, nullptr, handle);
}

// Releases the scene, camera and light handles held by the preview and clears the clump slots.
static int __fastcall CGuiCharacterPreview_ReleaseResources(CGuiCharacterPreview* self, void* /*edx*/)
{
    int result = static_cast<int>(self->m_pScene);

    if (self->m_pScene != 0) {
        int managerId = GetGlobalInt();
        int manager = ResolveRwManager(managerId, nullptr);
        result = InvokeVirtualWithHandle(manager, 7, self->m_pScene);
        self->m_pScene = 0;
    }

    if (self->m_pCamera != 0) {
        int managerId = GetGlobalInt();
        int manager = ResolveRwManager(managerId, nullptr);
        result = InvokeVirtualWithHandle(manager, 5, self->m_pCamera);
        self->m_pCamera = 0;
    }

    if (self->m_pLight != 0) {
        int managerId = GetGlobalInt();
        int manager = ResolveClumpManager(managerId, nullptr);
        result = InvokeVirtualWithHandle(manager, 11, self->m_pLight);
        self->m_pLight = 0;
    }

    self->m_pCharClump = 0;
    self->m_pWeaponClump = 0;
    return result;
}

HOOK(0x00541ED0, CGuiCharacterPreview_ReleaseResources, "auto");

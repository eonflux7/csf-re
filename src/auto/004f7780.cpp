#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalInt, 0x006B1380, ());
CSF_FUNC(int, __fastcall, ResolveRwManager, 0x006B13B0, (int managerId, void* edx));
CSF_FUNC(int, __fastcall, SceneLoader_GetSceneController, 0x00652010, (void* self, void* edx));
CSF_FUNC(void, __fastcall, ResetCharacterSubobject11C, 0x004F7730, (void* self, void* edx));
CSF_FUNC(void, __fastcall, DestroyEmbeddedMemberB, 0x006B10A0, (void* self, void* edx));

// Tears down the RW model resources owned by a C_0083f1a0 (character head model) and
// restores the base-class vtable. Returns 0 (the original's eax is not meaningful).
static int __fastcall C_0083f1a0_ReleaseModelResources(C_0083f1a0* self, void* /*edx*/)
{
    CSF_AT(void*, self, 0) = CSF_VTBL(0x0083F1A0);

    // Release the RW object held in field_0004 via the RW manager's vtable slot 5.
    void* rwManager = reinterpret_cast<void*>(ResolveRwManager(GetGlobalInt(), nullptr));
    ((void(__fastcall*)(void*, void*, uint32_t))(CSF_AT(void**, rwManager, 0)[5]))(
        rwManager, nullptr, self->field_0004);

    // Release the scene-controller entry held in field_0010 via vtable slot 6.
    if (self->field_0010) {
        void* sceneController = reinterpret_cast<void*>(
            SceneLoader_GetSceneController(reinterpret_cast<void*>(GetGlobalInt()), nullptr));
        ((void(__fastcall*)(void*, void*, uint32_t))(CSF_AT(void**, sceneController, 0)[6]))(
            sceneController, nullptr, self->field_0010);
        self->field_0010 = 0;
    }

    ResetCharacterSubobject11C(&self->field_011c, nullptr);

    self->field_00dc = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_00d0 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));
    self->field_0074 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D410));

    DestroyEmbeddedMemberB(reinterpret_cast<uint8_t*>(self) + 0x40, nullptr);
    DestroyEmbeddedMemberB(reinterpret_cast<uint8_t*>(self) + 0x28, nullptr);

    CSF_AT(void*, self, 0) = CSF_VTBL(0x0082D410);
    return 0;
}

HOOK(0x004F7780, C_0083f1a0_ReleaseModelResources, "auto");

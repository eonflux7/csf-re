// Reimplementation of GfxObject_InitAtomicParams (0x006E6A20), module gfx_object_instances.
#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GfxObjectProps_ApplyWaterSourceState, 0x006B8860, (int offset, int value));
CSF_FUNC(int, __cdecl, GfxObjectProps_GetObjectAttachment, 0x006B87A0, (int objectIndex));
CSF_FUNC(int, __fastcall, GfxEnvironment_GetAttachmentId, 0x006B8080, (void* self, void* edx));
CSF_FUNC(int, __cdecl, RpMatFXAtomicQueryEffects, 0x005712A0, (int a1));
CSF_FUNC(int, __cdecl, GfxFrames_ConfigureMaterialEffects, 0x006C07D0, (int object, int attachmentOwner));
CSF_FUNC(char, __fastcall, GfxObjectAtomic_FinishSetup, 0x006E67C0, (void* self, void* edx));

static char __fastcall GfxObject_InitAtomicParams(C_00861c08* self, void* /*edx*/, uint32_t a2, uint32_t a3, uint32_t a4)
{
    self->field_000c = a2;
    self->field_0010 = a3;

    GfxObjectProps_ApplyWaterSourceState(
        static_cast<int>(a2),
        static_cast<int>(reinterpret_cast<uintptr_t>(self)));

    uint32_t objectIndex = self->field_000c;
    self->field_002c = a4;

    int objectAttachment = GfxObjectProps_GetObjectAttachment(static_cast<int>(objectIndex));
    self->field_0014 = static_cast<uint32_t>(
        GfxEnvironment_GetAttachmentId(reinterpret_cast<void*>(static_cast<uintptr_t>(objectAttachment)), nullptr));

    *reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2) + 2) = 5;

    if (RpMatFXAtomicQueryEffects(static_cast<int>(a2)))
    {
        GfxFrames_ConfigureMaterialEffects(
            static_cast<int>(self->field_000c),
            static_cast<int>(self->field_0040));
    }

    return GfxObjectAtomic_FinishSetup(self, nullptr);
}

HOOK(0x006E6A20, GfxObject_InitAtomicParams, "auto");

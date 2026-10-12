#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callees (bound by address).
CSF_FUNC(int, __fastcall, C_0082daf4_ResetLinkedState, 0x0040B5B0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082daf4_ResetPendingState, 0x0040B690, (void* self, void* edx));
CSF_FUNC(int, __cdecl, C_0082daf4_ReleaseTrackedBuffer, 0x0040B480, (int a1, int a2, int a3, int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));

// Destructor body for C_0082daf4: releases both owned buffer slots, then restores the base vtable.
static int __fastcall C_0082daf4_ReleaseOwnedBuffers(C_0082daf4* self, void* /*edx*/)
{
    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082DAF4);

    C_0082daf4_ResetLinkedState(self, nullptr);
    C_0082daf4_ResetPendingState(self, nullptr);

    // First slot: field_0020 owns a block; field_0024 and the sub-object at +0x1c describe it.
    if (self->field_0020) {
        C_0082daf4_ReleaseTrackedBuffer(
            static_cast<int>(self->field_0020),
            static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x1C)),
            static_cast<int>(self->field_0024),
            0);
        if (self->field_0020) {
            Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0020)));
        }
    }
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;

    // Second slot: field_0010 owns a block; field_0014 and the sub-object at +0x0c describe it.
    int result = static_cast<int>(self->field_0010);
    if (result) {
        C_0082daf4_ReleaseTrackedBuffer(
            result,
            static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x0C)),
            static_cast<int>(self->field_0014),
            0);
        result = static_cast<int>(self->field_0010);
        if (result) {
            result = Mem_FreeDebug(reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0010)));
        }
    }
    self->field_0010 = 0;
    self->field_0014 = 0;
    self->field_0018 = 0;

    *reinterpret_cast<void***>(self) = CSF_VTBL(0x0082D410);
    return result;
}

HOOK(0x0040BCB0, C_0082daf4_ReleaseOwnedBuffers, "auto");

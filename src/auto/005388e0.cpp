// CMenuLoading: releases owned sub-objects and restores the base vtables.
#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Callees bound by address.
CSF_FUNC(void, __fastcall, ReleaseOwnedBlockAtPlus4, 0x006923C0, (void* block /* this+4 */, void* edx));
CSF_FUNC(int, __fastcall, DestroyEmbeddedBaseAtPlus10, 0x0069AE90, (void* embedded, void* edx));

static int __fastcall CMenuLoading_ReleaseMembersAndResetVtable(CMenuLoading* self, void* /*edx*/)
{
    // Read the object at +0x2C before any vtable is overwritten.
    void* ownedAt2C = reinterpret_cast<void*>(self->field_002c);

    // Install the CMenuLoading vtable and the embedded/base vtables (same order as original).
    CSF_AT(uint32_t, self, 0x00) = 0x00842150u;
    CSF_AT(uint32_t, self, 0x04) = 0x00842100u;
    CSF_AT(uint32_t, self, 0x10) = 0x008420F8u;

    // Release the object at +0x2C (virtual slot 0, flag 1).
    if (ownedAt2C != nullptr) {
        ((void(__fastcall*)(void*, void*, int))(*reinterpret_cast<void***>(ownedAt2C))[0])(ownedAt2C, nullptr, 1);
        self->field_002c = 0;
    }

    // Release the block at +0x3C (heap block, pointer offset +4).
    uint32_t blockAt3C = self->field_003c;
    if (blockAt3C != 0) {
        ReleaseOwnedBlockAtPlus4(reinterpret_cast<void*>(blockAt3C + 4), nullptr);
        self->field_003c = 0;
    }

    // Release the object at +0x54.
    void* ownedAt54 = reinterpret_cast<void*>(self->field_0054);
    if (ownedAt54 != nullptr) {
        ((void(__fastcall*)(void*, void*, int))(*reinterpret_cast<void***>(ownedAt54))[0])(ownedAt54, nullptr, 1);
        self->field_0054 = 0;
    }

    // Release the object at +0x40.
    void* ownedAt40 = reinterpret_cast<void*>(self->field_0040);
    if (ownedAt40 != nullptr) {
        ((void(__fastcall*)(void*, void*, int))(*reinterpret_cast<void***>(ownedAt40))[0])(ownedAt40, nullptr, 1);
        self->field_0040 = 0;
    }

    // Release the object at +0x4C.
    void* ownedAt4C = reinterpret_cast<void*>(self->field_004c);
    if (ownedAt4C != nullptr) {
        ((void(__fastcall*)(void*, void*, int))(*reinterpret_cast<void***>(ownedAt4C))[0])(ownedAt4C, nullptr, 1);
        self->field_004c = 0;
    }

    // Tear down the embedded sub-object at +0x10 (its address, not its contents).
    int result = DestroyEmbeddedBaseAtPlus10(reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x10), nullptr);

    // Restore the base vtable pointers at +0x04 and finally +0x00.
    CSF_AT(uint32_t, self, 0x04) = 0x00830488u;
    CSF_AT(uint32_t, self, 0x04) = 0x00830438u;
    CSF_AT(uint32_t, self, 0x00) = 0x0083042Cu;

    return result;
}

HOOK(0x005388E0, CMenuLoading_ReleaseMembersAndResetVtable, "auto");

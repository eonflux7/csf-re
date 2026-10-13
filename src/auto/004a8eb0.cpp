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

CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void*, int index));
CSF_FUNC(double, __fastcall, GfxObjectProps_GetAttachmentHeight, 0x006B8360, (void* self, void* edx));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));
CSF_FUNC(char, __fastcall, C_00860700_SetPositionVector, 0x006BFEF0, (C_00860700* self, void*, const uint32_t* a2));

// Reads the skin target object from this->field_0014->field_004c, looks up table entry 0 on it,
// and if that entry exists asks it (vtable slot 13) to fill an embedded state. The attachment
// height from that state becomes the Y component of a position vector {0, height, 0} written
// back into the same state. The stack argument (a2) is pushed by the caller but unused here.
static int __fastcall C_00835228_ApplyAttachmentHeightOffset(C_00835228* self, void* /*edx*/, int /*a2*/)
{
    // owner->field_004c->vGetSkinTargetObject() (vtable slot 15); its result is the `this` for the lookup.
    void* owner = reinterpret_cast<void*>(self->field_0014);
    void* skinOwner = CSF_AT(void*, owner, 0x4c);
    void* skinTarget = reinterpret_cast<void*(__fastcall*)(void*, void*)>(CSF_AT(void**, skinOwner, 0)[15])(skinOwner, nullptr);

    // Table entry lookup with index 0.
    int tableEntryDword = GetTableEntryDwordByIndex(skinTarget, nullptr, 0);
    void* tableEntry = reinterpret_cast<void*>(static_cast<uintptr_t>(tableEntryDword));

    alignas(8) uint8_t embeddedBytes[0x20];
    C_00860700* embeddedState = reinterpret_cast<C_00860700*>(embeddedBytes);
    GfxObject_ConstructEmbeddedState(embeddedState, nullptr);

    if (tableEntry) {
        // tableEntry->vtable[13](embeddedState)
        reinterpret_cast<void(__fastcall*)(void*, void*, C_00860700*)>(CSF_AT(void**, tableEntry, 0)[13])(tableEntry, nullptr, embeddedState);

        double attachmentHeight = GfxObjectProps_GetAttachmentHeight(embeddedState, nullptr);

        // Position vector {x, y, z} = {0, height, 0}.
        alignas(8) uint8_t positionBytes[0xC];
        uint32_t* positionWords = reinterpret_cast<uint32_t*>(positionBytes);
        *reinterpret_cast<float*>(&positionWords[1]) = static_cast<float>(attachmentHeight);
        positionWords[0] = 0;
        positionWords[2] = 0;
        C_00860700_SetPositionVector(embeddedState, nullptr, positionWords);
    }

    GfxObject_DestroyEmbeddedState(reinterpret_cast<C_00860630*>(embeddedBytes), nullptr);
    return 0;
}

HOOK(0x4A8EB0, C_00835228_ApplyAttachmentHeightOffset, "auto");

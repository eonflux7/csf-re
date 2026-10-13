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

CSF_FUNC(int, __fastcall, GetTableEntryDwordByIndex, 0x004084C0, (void* self, void* , int index));
CSF_FUNC(double, __fastcall, GfxObjectProps_GetAttachmentHeight, 0x006B8360, (void* self, void* edx));
CSF_FUNC(void, __fastcall, GfxObject_DestroyEmbeddedState, 0x006BF180, (C_00860630* self, void*));
CSF_FUNC(int, __fastcall, GfxObject_ConstructEmbeddedState, 0x006BFD00, (C_00860700* self, void*));
CSF_FUNC(char, __fastcall, C_00860700_SetPositionVector, 0x006BFEF0, (C_00860700* self, void* , const uint32_t* a2));

typedef void* (__fastcall* SkinTargetGetterFn)(void* self, void* edx);
typedef void (__fastcall* EntryFillStateFn)(void* self, void* edx, C_00860700* state);

// Runs only for a2 == 0. Looks up table entry 25 on the skin target, asks it to fill a
// temporary embedded state, reads the attachment height into the state's position vector,
// then destroys the state. Returns a2 unchanged.
static int __fastcall C_0083c280_UpdateAttachmentOffsetEntry25(C_0083c280* self, void* /*edx*/, int a2)
{
    int result = a2;
    if (a2 == 0)
    {
        // Stack storage for the embedded state (room 0x20 bytes at ebp-0x2C).
        alignas(8) uint8_t stateBytes[0x20];
        C_00860700& state = *reinterpret_cast<C_00860700*>(stateBytes);

        // self->field_0014->field_004c->vGetSkinTargetObject()  (vtable slot 0x3C / 4 = 15)
        void* owner = reinterpret_cast<void*>(self->field_0014);
        void* skinOwner = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x4C));
        SkinTargetGetterFn getSkinTarget = reinterpret_cast<SkinTargetGetterFn>(CSF_AT(void**, skinOwner, 0)[15]);
        void* skinTarget = getSkinTarget(skinOwner, nullptr);

        int tableEntry = GetTableEntryDwordByIndex(skinTarget, nullptr, 25);
        GfxObject_ConstructEmbeddedState(&state, nullptr);

        if (tableEntry != 0)
        {
            void* entryObj = reinterpret_cast<void*>(static_cast<uintptr_t>(tableEntry));
            // entry->vFillState(&state)  (vtable offset 52 / 4 = 13)
            EntryFillStateFn fillState = reinterpret_cast<EntryFillStateFn>(CSF_AT(void**, entryObj, 0)[13]);
            fillState(entryObj, nullptr, &state);

            float attachmentHeight = static_cast<float>(GfxObjectProps_GetAttachmentHeight(&state, nullptr));

            // Position vector { 0, attachmentHeight, 0 } stored as raw dwords at ebp-0x0C.
            uint32_t positionVector[3];
            *reinterpret_cast<float*>(&positionVector[1]) = attachmentHeight;
            positionVector[0] = 0;
            positionVector[2] = 0;
            C_00860700_SetPositionVector(&state, nullptr, positionVector);
        }

        GfxObject_DestroyEmbeddedState(reinterpret_cast<C_00860630*>(&state), nullptr);
    }
    return result;
}

HOOK(0x004D55C0, C_0083c280_UpdateAttachmentOffsetEntry25, "auto");

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, C_00860a30_ClearField000c, 0x006C9FD0, (C_00860a30* self, void* , int));
CSF_FUNC(int, __fastcall, GetCurrentItemCount, 0x00681F40, (void* self, void* edx));
CSF_FUNC(int, __fastcall, GfxSkin_SetKeyedValue, 0x006C4BF0, (void* self, void* , int key, unsigned int value));

// Resets the per-instance animation state; if a keyed skin value was pending,
// clears the flag and removes key 6 from the skin of the current item.
static char __fastcall C_00862490_ResetAnimStateAndClearSkinKey(C_00862490* self, void* /*edx*/, int a2)
{
    C_00860a30_ClearField000c(self, nullptr, a2);

    void* ownerObj = reinterpret_cast<void*>(self->field_0040);
    self->field_0038 = 1.0f;
    self->field_003c = 1.0f;
    self->field_0028 = 0;
    // vtable slot +0x4 on ownerObj, called with one stack arg 0
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, ownerObj, 0)[1])(ownerObj, nullptr, 0);

    self->field_0084 = 0;
    self->field_0088 = 0xFFFFFFFFu;

    uint8_t pendingFlag = self->field_004c;
    if (pendingFlag)
    {
        self->field_004c = 0;
        void* currentItem = reinterpret_cast<void*>(GetCurrentItemCount(self, nullptr));
        // vtable slot +0x1C on currentItem; its return value becomes the skin object
        void* skinObj = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, currentItem, 0)[7])(currentItem, nullptr);
        return static_cast<char>(GfxSkin_SetKeyedValue(skinObj, nullptr, 6, 0));
    }
    return static_cast<char>(pendingFlag);
}

HOOK(0x006F1B20, C_00862490_ResetAnimStateAndClearSkinKey, "auto");

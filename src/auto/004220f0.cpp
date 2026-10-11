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

// Returns true when a pending state change was consumed and no slot call is needed.
CSF_FUNC(char, __fastcall, TryConsumePendingStateChange, 0x004218D0, (void* self, void* edx));

// Returns the blend value for a state code, stored as float into field_0200.
CSF_FUNC(double, __fastcall, GetStateBlendWeight, 0x0048E510, (void* self, void* edx, int a2));

static void __fastcall S_00421750_RefreshStateFromAnim(S_00421750* self, void* /*edx*/)
{
    // The first dword of this->field_0014 points at the animation data block (C_008342e8).
    C_008342e8* animData = reinterpret_cast<C_008342e8*>(self->field_0014);

    // Map the animation data's +0x1E0 selector to a state code.
    uint32_t selector = CSF_AT(uint32_t, animData, 0x1E0);
    uint32_t stateCode;
    if (selector == 0)
    {
        stateCode = 1;
    }
    else if (selector == 1)
    {
        stateCode = 3;
    }
    else if (selector == 2)
    {
        stateCode = 23;
    }
    else
    {
        stateCode = 0xFFFFFFFFu;
    }

    self->field_0238 = stateCode;
    self->field_02bc = CSF_AT(uint32_t, animData, 0x1D8);
    self->field_02c0 = CSF_AT(uint32_t, animData, 0x1C4);
    self->field_02c4 = CSF_AT(uint32_t, animData, 0x1E0);

    if (!TryConsumePendingStateChange(self, nullptr))
    {
        // Slot 0x8C / 4 = 35 on the animation object's vtable: (stateCode, 0, -1, 0).
        C_008342e8* target = reinterpret_cast<C_008342e8*>(self->field_0014);
        ((void(__fastcall*)(void*, void*, uint32_t, int, int, int))CSF_AT(void**, target, 0)[35])(
            target, nullptr, self->field_0238, 0, -1, 0);
    }

    // GetStateBlendWeight's `this` is the animation object stored in field_0014.
    void* blendSelf = reinterpret_cast<void*>(self->field_0014);
    self->field_0200 = static_cast<float>(GetStateBlendWeight(blendSelf, nullptr, static_cast<int>(self->field_0238)));
}

HOOK(0x004220F0, S_00421750_RefreshStateFromAnim, "auto");

#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));
CSF_FUNC(char, __fastcall, SceneEntity_Activate, 0x004E51B0, (void* self, void* edx, char a2));
CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, TAutonomo_PrepareActiveState, 0x004D73E0, (void* self, void* edx));

static int __fastcall TAutonomo_UpdateAndDispatch(TAutonomo* self, void* /*edx*/)
{
    CharTaskBase_Prepare(self, nullptr);

    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0008);
    if (CSF_AT(int, owner, 0x2F8) == 2)
    {
        // Object at owner+0x14, then its member at +0x3C is the 'this' for the activation.
        void* entityOwner = CSF_AT(void*, owner, 0x14);
        void* entity = CSF_AT(void*, entityOwner, 0x3C);
        SceneEntity_Activate(entity, nullptr, 0);
    }

    int result = SceneObj_IsInactive(CSF_AT(void*, owner, 0x14), nullptr);
    if (!static_cast<uint8_t>(result))
    {
        TAutonomo_PrepareActiveState(self, nullptr);

        // Owner must be re-read: the call above may have changed it.
        owner = reinterpret_cast<uint8_t*>(self->field_0008);
        void* target = CSF_AT(void*, owner, 0x14);
        auto slotFn = reinterpret_cast<int(__fastcall*)(void*, void*, uint32_t)>(
            CSF_AT(void**, target, 0)[180 / 4]);
        return slotFn(target, nullptr, self->field_007c);
    }
    return result;
}

HOOK(0x004D7450, TAutonomo_UpdateAndDispatch, "auto");

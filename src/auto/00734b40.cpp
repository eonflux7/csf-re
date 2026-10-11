#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(char, __fastcall, UpdatePrimaryState, 0x00734470,
    (void* self, void* edx, uint32_t* a2, uint32_t* a3));
static CSF_FUNC(char, __fastcall, UpdateSecondaryState, 0x007345E0,
    (void* self, void* edx, uint32_t* a2, int* a3));
static CSF_FUNC(void, __fastcall, UpdateInactiveState, 0x00734770,
    (void* self, void* edx));

static void __fastcall C_008670f0_UpdateState(C_008670f0* self, void* /*edx*/)
{
    if (self->field_00b8 || self->field_00bc)
    {
        if (self->field_0060 || self->field_0070)
        {
            UpdatePrimaryState(self, nullptr, &self->field_0058,
                &self->field_0078);
            UpdateSecondaryState(self, nullptr, &self->field_0068,
                reinterpret_cast<int*>(&self->field_0088));
        }
    }
    else
    {
        UpdateInactiveState(self, nullptr);
    }
}

HOOK(0x00734B40, C_008670f0_UpdateState, "auto");

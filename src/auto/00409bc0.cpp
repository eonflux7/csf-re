#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

CSF_FUNC(char, __fastcall, C_0082d844_ResetPresetBlock, 0x00409930, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0082d844_QueryStateCode, 0x004092A0, (void* self, void* edx));
CSF_FUNC(void, __fastcall, C_0082d844_ApplyComputedState, 0x00409300, (void* self, void* edx));

static void __fastcall C_0082d844_StoreScaledFieldValue(C_0082d844* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0008);
    void* child =
        ((void*(__fastcall*)(void*, void*))CSF_AT(void**, owner, 0)[0x28 / 4])(
            owner, nullptr);

    float value;
    if (child)
    {
        void* source =
            ((void*(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x18 / 4])(
                child, nullptr);
        value =
            ((float(__fastcall*)(void*, void*))CSF_AT(void**, source, 0)[0x28 / 4])(
                source, nullptr);
    }
    else
    {
        value = g_fZero;
    }

    self->field_000c = value;
    C_0082d844_ResetPresetBlock(self, nullptr);
    self->field_0024 = 0;

    if (C_0082d844_QueryStateCode(self, nullptr) == 1)
        self->field_0010 = 0.0f;
    else
        self->field_0010 = self->field_0020 * g_fZero;

    C_0082d844_ApplyComputedState(self, nullptr);
}

HOOK(0x409BC0, C_0082d844_StoreScaledFieldValue, "auto");

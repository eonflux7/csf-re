#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetReferencedObject, 0x006683F0, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_42B640, 0x0042B640, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_42C780, 0x0042C780, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_428850, 0x00428850, (void* self, void* edx, float a2));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Embedded sub-object at this+0x04 is checked through its own vtable slot 0x48 / 4 = 18.
static char __fastcall C_00830578_ProcessCheatToggle(C_00830578* self, void* /*edx*/)
{
    void* embedded = &self->field_0004;
    char result = 0;

    // If the referenced object is active (+0x18 != 0), its virtual check must also pass.
    if (CSF_AT(uint32_t, GetReferencedObject(embedded, nullptr), 0x18) != 0)
    {
        result = reinterpret_cast<char(__fastcall*)(void*, void*)>(
            CSF_AT(void**, embedded, 0)[18])(embedded, nullptr);
        if (result == 0)
            return result;
    }

    result = static_cast<char>(self->field_0020);
    if (result)
    {
        void* sceneSub = reinterpret_cast<void*>(CSF_AT(uint32_t, g_pCurrentScene, 0xC0));
        if (CSF_AT(uint8_t, sceneSub, 9))
        {
            sub_42B640(self, nullptr);
            return static_cast<char>(
                sub_428850(reinterpret_cast<void*>(self->field_0014), nullptr, 0.0f));
        }
        else
        {
            if (!self->field_003c)
                sub_42C780(self, nullptr);
            return static_cast<char>(
                sub_428850(reinterpret_cast<void*>(self->field_0014), nullptr, 0.0f));
        }
    }
    return result;
}

HOOK(0x0042CA40, C_00830578_ProcessCheatToggle, "auto");

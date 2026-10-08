#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "globals.h"

CSF_FUNC(int, __fastcall, IsSelectableAndNotType10, 0x00718700,
         (CGuiEdit* self, void* edx));

// This helper additionally consumes the incoming EBX implicitly.
CSF_FUNC(char, __fastcall, ProcessEditKey, 0x007227D0,
         (CGuiEdit* self, void* edx, int key));

CSF_FUNC(char, __fastcall, ControlOnKeyDown, 0x007184C0,
         (CGuiEdit* self, void* edx, int key, int flags));

// Keep this wrapper unoptimized to preserve the implicit EBX input.
static __attribute__((optnone)) char __fastcall CGuiEdit_OnKeyDown(
    CGuiEdit* self, void* /*edx*/, int key, int flags)
{
    const char enabled = static_cast<char>(
        ((int(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0x6C / 4])(self, nullptr));
    if (enabled == 0)
        return enabled;

    const char selectable = static_cast<char>(
        IsSelectableAndNotType10(self, nullptr));
    if (selectable == 0)
        return selectable;

    if (self->field_0148 != 0 ||
        ((std::uint8_t(__fastcall*)(void*, void*))
            CSF_AT(void**, self, 0)[0xBC / 4])(self, nullptr) != 0)
    {
        if (ProcessEditKey(self, nullptr, key) != 0)
        {
            self->field_01b4 = 0;
            self->field_01b8 = 0.0f;
            self->field_01bc = static_cast<std::uint32_t>(key);
            self->field_013c = 1;
        }
    }

    return ControlOnKeyDown(self, nullptr, key, flags);
}

HOOK(0x00722940, CGuiEdit_OnKeyDown, "auto");

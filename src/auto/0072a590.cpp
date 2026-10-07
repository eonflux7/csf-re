#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static int __fastcall CGuiPanel_SetFloatOption(
    CGuiPanel* self, void* /*edx*/, int valueBits, int option)
{
    int result = reinterpret_cast<int (__fastcall*)(void*, void*)>(
        CSF_AT(void**, self, 0)[0x194 / 4])(self, nullptr);

    if (result != 0)
        return result;

    void** panelVtable = CSF_AT(void**, self, 0);
    std::memcpy(&self->field_0178, &valueBits, sizeof(valueBits));
    self->field_017c = static_cast<std::uint8_t>(option);

    result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
        panelVtable[0x23C / 4])(self, nullptr, 1);

    void* linkedControl = reinterpret_cast<void*>(self->field_014c);
    if (linkedControl != nullptr)
    {
        result = reinterpret_cast<int (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, linkedControl, 0)[0x198 / 4])(
                linkedControl, nullptr, valueBits, option);
    }

    return result;
}

HOOK(0x0072A590, CGuiPanel_SetFloatOption, "auto");

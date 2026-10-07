#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static void __fastcall CGuiStatic_CopyPropertiesToObject(
    CGuiStatic* self, void* /*edx*/, int* target)
{
    if (!target)
        return;

    alignas(4) std::uint8_t propertyBuffer[8];

    void** sourceVtable = CSF_AT(void**, self, 0);
    void** targetVtable = CSF_AT(void**, target, 0);

    const int firstProperty =
        reinterpret_cast<int(__fastcall*)(CGuiStatic*, void*, std::uint8_t*)>(
            sourceVtable[0x160 / 4])(self, nullptr, propertyBuffer);

    reinterpret_cast<void(__fastcall*)(int*, void*, int)>(
        targetVtable[0x28 / 4])(target, nullptr, firstProperty);

    sourceVtable = CSF_AT(void**, self, 0);
    targetVtable = CSF_AT(void**, target, 0);

    const int secondProperty =
        reinterpret_cast<int(__fastcall*)(CGuiStatic*, void*)>(
            sourceVtable[0xA4 / 4])(self, nullptr);

    reinterpret_cast<void(__fastcall*)(int*, void*, int)>(
        targetVtable[0x38 / 4])(target, nullptr, secondProperty);
}

HOOK(0x007181E0, CGuiStatic_CopyPropertiesToObject, "auto");

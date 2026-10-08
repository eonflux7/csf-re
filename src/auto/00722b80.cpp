#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(int*, __fastcall, SynchronizeAttachedObjectText, 0x00722320,
         (void* self, void* edx, std::int16_t* text));

static void __fastcall ReplaceAttachedObject(
    void* self, void* /*edx*/, void* replacement)
{
    void* attached = CSF_AT(void*, self, 0x134);
    if (attached == replacement)
        return;

    if (attached)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, attached, 0)[0])(attached, nullptr, 1);
        CSF_AT(void*, self, 0x134) = nullptr;
    }

    CSF_AT(void*, self, 0x134) = replacement;
    if (!replacement)
        return;

    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, replacement, 0)[26])(replacement, nullptr, 1);

    attached = CSF_AT(void*, self, 0x134);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, attached, 0)[27])(attached, nullptr, 1);

    attached = CSF_AT(void*, self, 0x134);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, attached, 0)[43])(attached, nullptr, 0);

    attached = CSF_AT(void*, self, 0x134);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, attached, 0)[44])(attached, nullptr, 1);

    attached = CSF_AT(void*, self, 0x134);
    reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
        CSF_AT(void**, attached, 0)[37])(attached, nullptr, 0);

    attached = CSF_AT(void*, self, 0x134);
    std::int16_t* text =
        reinterpret_cast<std::int16_t* (__fastcall*)(void*, void*)>(
            CSF_AT(void**, attached, 0)[17])(attached, nullptr);

    SynchronizeAttachedObjectText(self, nullptr, text);
}

HOOK(0x00722B80, ReplaceAttachedObject, "auto");

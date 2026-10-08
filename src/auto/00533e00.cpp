#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original: int __thiscall sub_533E00(this, a2, a3), ret 8.
// Sets the selected language value, applies the global setting, invokes the
// child's virtual slot 0x16C, scrolls the view and returns the child object.

CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, ());
CSF_FUNC(void, __fastcall, ApplyGlobalValueToObject, 0x521DE0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, MenuPause_ScrollViewToTrackedDelta, 0x533B40, (CMenuLanguage* self, void* edx));

static int __fastcall CMenuLanguage_SelectLanguage(CMenuLanguage* self, void* /*edx*/, int selectedLanguage, int unused3)
{
    (void)unused3;

    if (self->field_0030 == 0)
        return 0;

    self->field_0058 = 1;
    self->field_0034 = static_cast<uint32_t>(selectedLanguage);

    void* globalValueObject = reinterpret_cast<void*>(GetGlobalValue());
    ApplyGlobalValueToObject(globalValueObject, nullptr);

    void* child = reinterpret_cast<void*>(self->field_0030);
    void** childVtbl = *reinterpret_cast<void***>(child);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(childVtbl[0x16C / 4])(child, nullptr);

    MenuPause_ScrollViewToTrackedDelta(self, nullptr);

    return static_cast<int>(self->field_0030);
}

HOOK(0x533E00, CMenuLanguage_SelectLanguage, "auto");

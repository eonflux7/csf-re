#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"
#include "globals.h"

// 0x00521BB0 : int __cdecl GetGlobalValue()
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, ());

// 0x00521F10 : __thiscall, no stack args (rets=[0]); ecx set explicitly by caller
CSF_FUNC(int, __fastcall, Sub_521F10, 0x521F10, (void* self, void* edx));

// 0x0043F890 : __thiscall, no stack args (rets=[0]); target of the tail jump
CSF_FUNC(int, __fastcall, Sub_43F890, 0x43F890, (void* self, void* edx));

// Releases the object owned at +0x18 (virtual slot 0 with argument 1), clears the
// slot, runs the global-value + 0x521F10 sequence, then finalizes the sub-object
// embedded at +0x44 (identical to the original tail jump into 0x43F890).
static int __fastcall CMenuScenarioSelect_ReleaseOwnedObject(CMenuScenarioSelect* self, void* /*edx*/)
{
    void* owned = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0018));
    if (owned != nullptr)
    {
        void** vtbl = *reinterpret_cast<void***>(owned);
        reinterpret_cast<void(__fastcall*)(void*, void*, int)>(vtbl[0])(owned, nullptr, 1);
        self->field_0018 = 0;
        void* globalValue = reinterpret_cast<void*>(static_cast<uintptr_t>(GetGlobalValue()));
        Sub_521F10(globalValue, nullptr);
        return Sub_43F890(reinterpret_cast<uint8_t*>(self) + 0x44, nullptr);
    }
    return 0;
}

HOOK(0x52E2E0, CMenuScenarioSelect_ReleaseOwnedObject, "auto");

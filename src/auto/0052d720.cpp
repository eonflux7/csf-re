#include <cstdint>
#include "hook.h"
#include "core/memory.h"
#include "globals.h"

// Address-bound originals this function calls.
CSF_FUNC(int, __cdecl, GetGlobalValue, 0x521BB0, ());
CSF_FUNC(int, __fastcall, ProcessMenuGlobal, 0x521F10, (void* self, void* edx));

// Forwards an action to the sub-object stored at +0x14 via its vtbl slot 0x60,
// then processes the global menu value through the object returned by GetGlobalValue.
static int __fastcall CMenuScenarioSelect_ForwardAction(void* self, void* /*edx*/, int action)
{
    void* subObject = CSF_AT(void*, self, 0x14);
    void** vtbl = *reinterpret_cast<void***>(subObject);

    using ForwardActionFn = void(__fastcall*)(void*, void*, void*, int);
    reinterpret_cast<ForwardActionFn>(vtbl[0x60 / 4])(subObject, nullptr, self, action);

    return ProcessMenuGlobal(reinterpret_cast<void*>(GetGlobalValue()), nullptr);
}

HOOK(0x52D720, CMenuScenarioSelect_ForwardAction, "auto");

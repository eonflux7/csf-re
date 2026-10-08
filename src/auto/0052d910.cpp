#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// 0x69B700 - cdecl, no stack args (rets=[0]); returns the global object used for the lookup below.
CSF_FUNC(int, __cdecl, GetGlobalValue_69B700, 0x69B700, ());

// 0x69BC50 - __thiscall (ecx = object, one stack arg, ret 4); lookup keyed by a string.
CSF_FUNC(int, __fastcall, MenuScenarioSelect_LookupString, 0x69BC50, (void* self, void* edx, const char* key));

// 0x52D910 - takes a pointer to an SSO string-like object, resolves its character data
// (capacity at +0x18 < 0x10 => inline buffer at +4, otherwise the pointer stored at +4),
// looks it up on the global object and returns the looked-up value, or 8 when it is absent.
static int __stdcall CMenuScenarioSelect_LookupValueByName(CMenuScenarioSelect* a1)
{
    const char* key;
    if (a1->field_0018 < 0x10u)
        key = reinterpret_cast<const char*>(&a1->field_0004);
    else
        key = reinterpret_cast<const char*>(static_cast<uintptr_t>(a1->field_0004));

    void* globalObject = reinterpret_cast<void*>(GetGlobalValue_69B700());
    int value = MenuScenarioSelect_LookupString(globalObject, nullptr, key);
    if (!value)
        return 8;
    return value;
}

HOOK(0x52D910, CMenuScenarioSelect_LookupValueByName, "auto");

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Game-global lookup (__cdecl, no stack arguments). Returns the global value object.
CSF_FUNC(void*, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());

// Applies the global value to the game (__thiscall, one ecx argument, no stack arguments).
CSF_FUNC(int, __fastcall, Menu_5225D0, 0x005225D0, (void* self, void* edx));

// Shuts down a child panel object (__thiscall, one ecx argument, no stack arguments).
CSF_FUNC(int, __fastcall, CGuiCharacterPreview_ReleaseResources, 0x00541ED0, (void* self, void* edx));

// Releases the two owned child objects stored at +0x50 and +0x54.
// Each non-null child gets its virtual slot 0 invoked with flag 1 (scalar deleting destructor),
// and its slot is cleared afterwards. The returned value is the result of the last call made.
static int __fastcall MenuScenarioSelect_DestroyOwnedChildren(void* self, void* /*edx*/)
{
    void* globalValue = GetGlobalValue_521BB0();
    int result = Menu_5225D0(globalValue, nullptr);

    if (CSF_AT(void*, self, 0x54)) {
        result = CGuiCharacterPreview_ReleaseResources(CSF_AT(void*, self, 0x54), nullptr);
    }

    void* firstChild = CSF_AT(void*, self, 0x54);
    if (firstChild) {
        result = ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, firstChild, 0)[0]))(firstChild, nullptr, 1);
        CSF_AT(void*, self, 0x54) = nullptr;
    }

    void* secondChild = CSF_AT(void*, self, 0x50);
    if (secondChild) {
        result = ((int(__fastcall*)(void*, void*, int))(CSF_AT(void**, secondChild, 0)[0]))(secondChild, nullptr, 1);
        CSF_AT(void*, self, 0x50) = nullptr;
    }

    return result;
}

HOOK(0x0052F420, MenuScenarioSelect_DestroyOwnedChildren, "auto");

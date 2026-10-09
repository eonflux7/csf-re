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

// Original calls GetGlobalValue_4FBDE0 with one stack argument (push 1) and
// ignores its declared "()" signature; see notes in the metadata.
CSF_FUNC(int, __cdecl, GetGlobalValue_4FBDE0, 0x004FBDE0, ());
CSF_FUNC(int, __fastcall, sub_4FC160, 0x004FC160, (void* self, void* edx, int a2));

static int __stdcall SetLookupValidFlagFromGlobal(int a1, int a2, int a3)
{
    // Original: v3 = *(a1 + 4)
    uint8_t* flagOwner = CSF_AT(uint8_t*, a1, 4);

    // Original: push 1; call GetGlobalValue_4FBDE0; mov ecx, eax
    using GlobalGetterFn = int(__cdecl*)(int);
    int globalValue = reinterpret_cast<GlobalGetterFn>(GetGlobalValue_4FBDE0)(1);

    // Original: call sub_4FC160 with ecx = globalValue, stack arg = 1
    int result = sub_4FC160(reinterpret_cast<void*>(static_cast<intptr_t>(globalValue)), nullptr, 1);

    // Original: *(byte*)(v3 + 1) = (result != -1)
    CSF_AT(uint8_t, flagOwner, 1) = static_cast<uint8_t>(result != -1);
    return result;
}

HOOK(0x0045AFD0, SetLookupValidFlagFromGlobal, "auto");

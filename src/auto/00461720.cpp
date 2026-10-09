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

CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x0043E010, ());
CSF_FUNC(double, __fastcall, EvaluateFloatForSubobject, 0x0043F6A0, (void* self, void* edx, int* a2));

// Resolves the nested pointer chain from args, fetches the global handle, evaluates the
// subobject expression, and stores the result as a float at +4 of the object referenced by ctx+4.
static void __stdcall ScriptCommands_StoreComputedFloat(int ctx, int cmd, int args)
{
    auto* ctxBytes = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(ctx));
    uint8_t* targetObject = CSF_AT(uint8_t*, ctxBytes, 4);

    auto* argBytes = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(args));
    uint8_t* firstLevel = CSF_AT(uint8_t*, argBytes, 0);
    uint8_t* secondLevel = CSF_AT(uint8_t*, firstLevel, 4);
    int* subobjectArg = reinterpret_cast<int*>(secondLevel + 4);

    int globalHandle = GetGlobal8afc7c();
    double evaluated = EvaluateFloatForSubobject(
        reinterpret_cast<void*>(static_cast<uintptr_t>(globalHandle)), nullptr, subobjectArg);

    CSF_AT(float, targetObject, 4) = static_cast<float>(evaluated);
}

HOOK(0x00461720, ScriptCommands_StoreComputedFloat, "auto");

#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// WriteConfigPair (0x656480): (ecx=self, edx unused, 2 stack args)
CSF_FUNC(void*, __fastcall, WriteConfigPair, 0x00656480, (void* self, void* edx, char a2, int a3));

// Reads the two dwords stored in the pair object (this[0], this[1]) and forwards them
// to WriteConfigPair with a2 as the callee's `this`. Returns the callee's result.
static void* __fastcall NetGhostDDL_WriteConfigPairFromPairObject(
    int* pairObject, void* /*edx*/, C_008622e8* configTarget, int a3, int a4, int a5)
{
    int secondValue = pairObject[1];
    int firstValue = pairObject[0];
    return WriteConfigPair(configTarget, nullptr, static_cast<char>(firstValue), secondValue);
}

HOOK(0x0074B1C0, NetGhostDDL_WriteConfigPairFromPairObject, "auto");

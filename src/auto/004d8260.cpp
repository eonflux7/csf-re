#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Callee bindings (one name and signature per address).
CSF_FUNC(char, __fastcall, Owner_QueueAction, 0x004A8710, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, Owner_QueueActionWithParam, 0x004A8770, (void* self, void* edx, int a2, int a3));

// Thiscall method: self is the TAutonomo-like object; self+0x08 points to its owner.
// Sets bit 0 of owner+0x18, then forwards to the owner's dispatcher at +0x310
// (no-parameter form when a3 == -1, parameterized form otherwise).
static int __fastcall Autonomo_DispatchObjectAction(void* self, void* /*edx*/, int a2, int a3)
{
    CSF_AT(uint32_t, CSF_AT(void*, self, 0x08), 0x18) |= 1u;

    if (a3 == -1)
    {
        return Owner_QueueAction(CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x310), nullptr, a2);
    }
    return Owner_QueueActionWithParam(CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x310), nullptr, a2, a3);
}

HOOK(0x004D8260, Autonomo_DispatchObjectAction, "auto");

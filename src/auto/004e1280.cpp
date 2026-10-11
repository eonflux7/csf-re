#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Virtual predicate calls on C_0083e160. Slots are taken from the disassembly
// (byte offset 0xB4 -> slot 45, byte offset 0xB0 -> slot 44). Both take only `this`.
static bool C_0083e160_CallSlotB4Predicate(void* self)
{
    return ((bool(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xB4 / 4])(self, nullptr);
}

static bool C_0083e160_CallSlotB0Predicate(void* self)
{
    return ((bool(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0xB0 / 4])(self, nullptr);
}

// Returns TRUE only when both virtual predicates report false.
// The original tests the first predicate and short-circuits on true.
static int __fastcall C_0083e160_AreBothPredicatesFalse(C_0083e160* self, void* /*edx*/)
{
    void* obj = self;
    if (C_0083e160_CallSlotB4Predicate(obj))
        return 0;
    if (C_0083e160_CallSlotB0Predicate(obj))
        return 0;
    return 1;
}

HOOK(0x4E1280, C_0083e160_AreBothPredicatesFalse, "auto");

#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"

// Stores five values into a hierarchy node and sets its bound flag.
// Stack args: a2 -> +0x0C, a3 -> +0x10, a4 -> +0x14, a5 -> +0x18, a6 -> +0x08 (returned).
// Flag byte +0x20 is set to 1. Callee pops 5 dwords (ret 0x14).
static int __fastcall AnimInterpolator_BindHierarchyNode(
    uint8_t* self, void* /*edx*/, int a2, int a3, int a4, int a5, int a6)
{
    CSF_AT(int, self, 0x0C) = a2;
    CSF_AT(int, self, 0x10) = a3;
    CSF_AT(int, self, 0x14) = a4;
    CSF_AT(int, self, 0x18) = a5;
    CSF_AT(uint8_t, self, 0x20) = 1;
    CSF_AT(int, self, 0x08) = a6;
    return a6;
}

HOOK(0x707590, AnimInterpolator_BindHierarchyNode, "auto");

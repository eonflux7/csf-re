#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Base-class initializer (C_00833908 : C_0082d428), __fastcall with two stack args.
CSF_FUNC(int, __fastcall, C_00833908_InitializeAndForward, 0x0048E620,
         (C_00833908* self, void* /*edx*/, int initializationData, int argument));

// CPlayerCharacter::InitializeAndEnableFlag
// Forwards the two stack arguments to the base initializer, then sets the
// byte flag at (field_03e8 + 0x10) to 1. Returns the base initializer's result.
static int __fastcall CPlayerCharacter_InitializeAndEnableFlag(CPlayerCharacter* self, void* /*edx*/,
                                                              int initializationData, int argument)
{
    int result = C_00833908_InitializeAndForward(static_cast<C_00833908*>(self), nullptr,
                                                 initializationData, argument);
    uint8_t* flagBase = reinterpret_cast<uint8_t*>(self->field_03e8);
    *(flagBase + 0x10) = 1;
    return result;
}

HOOK(0x495790, CPlayerCharacter_InitializeAndEnableFlag, "auto");

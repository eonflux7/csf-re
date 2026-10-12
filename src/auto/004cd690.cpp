#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Base-class constructor (TCharTask chain): ConstructTaskBase(this, a2), ret 4
CSF_FUNC(uint32_t*, __fastcall, AiAutonomous_ConstructTaskBase, 0x004D1660, (C_0083b958* self, void* , int a2));

// Auxiliary task initializer on the embedded sub-object at +0x50, ecx = this
CSF_FUNC(uint32_t*, __fastcall, AiAttackTasks_InitAuxiliaryTask, 0x0048C810, (void* self, void* edx));

// Constructs the C_0083ab58 search-sweep task on top of its base; returns this.
static C_0083ab58* __fastcall C_0083ab58_Construct(C_0083ab58* self, void* /*edx*/, int a2)
{
    AiAutonomous_ConstructTaskBase(reinterpret_cast<C_0083b958*>(self), nullptr, a2);

    CSF_AT(void*, self, 0x00) = CSF_VTBL(0x0083AB58);
    self->field_0004 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0083AB38));
    self->field_0044 = reinterpret_cast<uint32_t>(CSF_VTBL(0x0082D448));
    self->field_0048 = 0xFFFFFFFFu;
    self->field_004c = 0xFFFFFFFFu;

    AiAttackTasks_InitAuxiliaryTask(static_cast<uint8_t*>(static_cast<void*>(self)) + 0x50, nullptr);

    self->field_0040 = 0;
    return self;
}

HOOK(0x004CD690, C_0083ab58_Construct, "auto");

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

// Initializes an AI query object to its default "no result yet" state.
// Returns the object's address (the original returns &this->__vftable, which is the object itself).
static uint32_t* __fastcall AiCore_InitializeQuery(C_0083124c* self, void* /*edx*/) {
    self->vtbl = CSF_VTBL(0x83124C);
    self->field_000c = 0xFFFFFFFFu;
    self->field_0010 = 0xFFFFFFFFu;
    CSF_AT(void*, self, 0x14) = CSF_VTBL(0x82D448);
    self->field_0018 = 0xFFFFFFFFu;
    self->field_001c = 0xFFFFFFFFu;
    self->field_0020 = 0;
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_0004 = 0xFFFFFFFFu;
    self->field_0008 = 0xFFFFFFFFu;
    self->field_002c = 2139095039u;  // 0x7F7FFFFF (FLT_MAX bit pattern)
    self->field_0030 = 0;
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x0044E030, AiCore_InitializeQuery, "auto");

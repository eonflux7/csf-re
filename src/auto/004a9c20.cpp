#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

// Resets the base AI state of the object before its derived fields are set.
CSF_FUNC(uint32_t*, __fastcall, ResetAiBaseState, 0x004DC060, (void* self, void* edx));

// Constructs the AI core base sub-object: resets base state, stores the
// constructor argument, and installs the vtables. Returns the object's first dword address.
static uint32_t* __fastcall AiCore_ConstructBase(C_00835398* self, void* /*edx*/, int a2)
{
    ResetAiBaseState(self, nullptr);
    self->field_002c = static_cast<uint32_t>(a2);
    self->vtbl = CSF_VTBL(0x00835398);
    self->field_0004 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_ADDR_DATA(0x00835310)));
    return reinterpret_cast<uint32_t*>(self);
}

HOOK(0x004A9C20, AiCore_ConstructBase, "auto");

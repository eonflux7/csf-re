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

// Callee: clears state on the task before the flag update (bound as given by the packet).
CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));

// Clears bit 0x10 in the 32-bit flags word at +0x1F0 of the task's owner object
// (owner pointer is stored at +0x14 of the object referenced by the task's +0x08 field).
// Returns the address of that flags word.
static unsigned int* __fastcall TCharTask_ClearOwnerFlag10(TCharTask* self, void* /*edx*/)
{
    CharTaskBase_Prepare(self, nullptr);

    uint8_t* linkedObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, self, 0x08));
    uint8_t* ownerObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, linkedObj, 0x14));

    CSF_AT(uint32_t, ownerObj, 0x1F0) = CSF_AT(uint32_t, ownerObj, 0x1F0) & 0xFFFFFFEFu;

    return reinterpret_cast<unsigned int*>(ownerObj + 0x1F0);
}

HOOK(0x00421C80, TCharTask_ClearOwnerFlag10, "auto");

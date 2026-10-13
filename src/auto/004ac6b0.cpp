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

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, C_0082f9f8_StartMovementAndResetFlags, 0x0047BF90, (C_0082f9f8* self, void* , int));

// Starts the cover movement on the linked AI object and sets its flag at +0xB0.
static char __stdcall AiStateFactory_StartLinkedMovementAndSetFlag(int a1)
{
    void* self = reinterpret_cast<void*>(a1);

    // Resolve the linked object; its base is 4 bytes before the returned pointer.
    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
    uint8_t* linkedBase = nullptr;
    if (linkedValue != 0) {
        linkedBase = reinterpret_cast<uint8_t*>(linkedValue) - 4;
    }

    uint8_t* field10 = reinterpret_cast<uint8_t*>(GetLinkedObjectField10(self, nullptr));
    C_0082f9f8* movementObject = CSF_AT(C_0082f9f8*, linkedBase, 0x718);
    int subObjectAddress = static_cast<int>(reinterpret_cast<uintptr_t>(field10) + 0x94);
    C_0082f9f8_StartMovementAndResetFlags(movementObject, nullptr, subObjectAddress);

    uint8_t* fieldAfter = reinterpret_cast<uint8_t*>(GetLinkedObjectField10(self, nullptr));
    CSF_AT(uint8_t, fieldAfter, 0xB0) = 1;

    return 0;
}

HOOK(0x004AC6B0, AiStateFactory_StartLinkedMovementAndSetFlag, "auto");

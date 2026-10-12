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
CSF_FUNC(int, __fastcall, AiCoverAttackTasks_ResetLinkedTaskState, 0x004DC380, (void* self, void* edx, int a2));

// Copies the 12-byte task state block (from the object's linked table at +0x64C,
// relative to the linked value - 4) into the linked field10 block at +16, then
// resets the linked task state. Non-active (field +28 != 1) path resets at +12.
static int __stdcall Estatico_SyncLinkedStateAndReset(int a1, int a2, int a3)
{
    void* self = reinterpret_cast<void*>(a1);

    if (CSF_AT(int, reinterpret_cast<uint8_t*>(GetLinkedObjectField10(self, nullptr)), 28) == 1)
    {
        int linkedObjectValue = GetLinkedObjectValue_4DB5C0(self, nullptr);
        int base = linkedObjectValue ? linkedObjectValue - 4 : 0;
        uint8_t* source = reinterpret_cast<uint8_t*>(base + 0x64C);

        uint8_t* destination = reinterpret_cast<uint8_t*>(GetLinkedObjectField10(self, nullptr) + 16);
        CSF_AT(int, destination, 0) = CSF_AT(int, source, 0);
        CSF_AT(int, destination, 4) = CSF_AT(int, source, 4);
        CSF_AT(int, destination, 8) = CSF_AT(int, source, 8);

        int linkedField10 = GetLinkedObjectField10(self, nullptr);
        return AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField10 + 4);
    }

    int linkedField10 = GetLinkedObjectField10(self, nullptr);
    return AiCoverAttackTasks_ResetLinkedTaskState(self, nullptr, linkedField10 + 12);
}

HOOK(0x004B0670, Estatico_SyncLinkedStateAndReset, "auto");

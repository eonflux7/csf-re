#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));

// Returns the object pointed to by the handle stored at list slot `index`.
// Re-reads field_0018 and the handle on every call, as the original does.
static uint8_t* CCharWeaponFsm_GetHandleObjectAt(CCharWeaponFsm* self, uint32_t index)
{
    void* handle = reinterpret_cast<void*>(reinterpret_cast<uint32_t*>(self->field_0018)[index]);
    int objectAddress = Handle_GetValuePointer(handle, nullptr);
    return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(objectAddress));
}

// Scans the handle list and returns the index of the first entry whose object has
// field +0x74 != a3 and field +0xDC == a2; returns -1 if none matches.
static int __fastcall CCharWeaponFsm_FindFirstMatchingHandleIndex(CCharWeaponFsm* self, void* /*edx*/, int a2, int a3)
{
    int result = -1;
    int matchIndex = -1;
    uint32_t count = 0;

    if (self->field_0018 != 0)
        count = static_cast<uint32_t>(static_cast<int>(self->field_001c - self->field_0018) >> 2);

    if (count != 0)
    {
        uint32_t index = 0;
        do
        {
            if (result != -1)
                break;

            if (CSF_AT(int, CCharWeaponFsm_GetHandleObjectAt(self, index), 0x74) != a3
                && CSF_AT(int, CCharWeaponFsm_GetHandleObjectAt(self, index), 0xDC) == a2)
            {
                matchIndex = static_cast<int>(index);
            }

            ++index;
            result = matchIndex;
        } while (index < count);
    }

    return result;
}

HOOK(0x004813C0, CCharWeaponFsm_FindFirstMatchingHandleIndex, "auto");

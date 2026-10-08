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

// Stores/looks up a key in the indexed value table at the given buffer (cdecl).
CSF_FUNC(char, __cdecl, ReadFirstStoredIndexedValue, 0x540FF0, (uint32_t key, void* table));

// Thiscall helper (ecx = this). Returns non-zero when the pending state is blocked.
CSF_FUNC(char, __fastcall, MenuProfiles_IsPendingStateBlocked, 0x53BB80, (void* self, void* edx));

// Signature of a __thiscall virtual handler taking one stack argument.
using MenuProfilesHandlerFn = int(__fastcall*)(void* self, void* edx, void* message);

// Handles an incoming event message for the menu profiles object.
// message->+4 holds the source id that is matched against the stored ids at +0x14, +0x20, +0x18, +0x1C.
static int __fastcall MenuProfiles_HandleEvent(void* self, void* /*edx*/, void* message)
{
    // Sub-object at +0x24: slot 0x104 is a query that must return non-zero to continue.
    void* stateObject = CSF_AT(void*, self, 0x24);
    int queryResult = ((int(__fastcall*)(void*, void*))CSF_AT(void**, stateObject, 0)[0x104 / 4])(stateObject, nullptr);
    if (queryResult == 0)
        return queryResult;

    // Primary match: id at +0x14 (non-zero) equals message id, or alternate id at +0x18 (non-zero) equals it.
    if (CSF_AT(uint32_t, self, 0x14) != 0 &&
        (CSF_AT(uint32_t, self, 0x14) == CSF_AT(uint32_t, message, 4) ||
         (CSF_AT(uint32_t, self, 0x18) != 0 && CSF_AT(uint32_t, self, 0x18) == CSF_AT(uint32_t, message, 4))))
    {
        void* table = static_cast<uint8_t*>(self) + 0x30;
        if (ReadFirstStoredIndexedValue(CSF_AT(uint32_t, self, 0x14), table))
        {
            // Original loads the vtable before storing the state value.
            void** vtbl = CSF_AT(void**, self, 0);
            CSF_AT(uint32_t, self, 0x38) = 5;
            return ((MenuProfilesHandlerFn)vtbl[0x48 / 4])(self, nullptr, message);
        }
        void** vtbl = CSF_AT(void**, self, 0);
        return ((MenuProfilesHandlerFn)vtbl[0x48 / 4])(self, nullptr, message);
    }

    // Secondary match: id at +0x20 (non-zero) equals message id.
    if (CSF_AT(uint32_t, self, 0x20) != 0 && CSF_AT(uint32_t, self, 0x20) == CSF_AT(uint32_t, message, 4))
    {
        void* table = static_cast<uint8_t*>(self) + 0x30;
        if (ReadFirstStoredIndexedValue(CSF_AT(uint32_t, self, 0x14), table))
        {
            ReadFirstStoredIndexedValue(CSF_AT(uint32_t, self, 0x14), table);
            // Original loads the vtable before storing the state value.
            void** vtbl = CSF_AT(void**, self, 0);
            CSF_AT(uint32_t, self, 0x38) = 2;
            return ((MenuProfilesHandlerFn)vtbl[0x48 / 4])(self, nullptr, message);
        }
        void** vtbl = CSF_AT(void**, self, 0);
        return ((MenuProfilesHandlerFn)vtbl[0x48 / 4])(self, nullptr, message);
    }

    // Tertiary match: id at +0x1C (non-zero) equals message id, and the blocking check passes.
    if (CSF_AT(uint32_t, self, 0x1C) != 0 && CSF_AT(uint32_t, self, 0x1C) == CSF_AT(uint32_t, message, 4))
    {
        if (!MenuProfiles_IsPendingStateBlocked(self, nullptr))
            CSF_AT(uint32_t, self, 0x38) = 0;
    }

    void** vtbl = CSF_AT(void**, self, 0);
    return ((MenuProfilesHandlerFn)vtbl[0x48 / 4])(self, nullptr, message);
}

HOOK(0x0053BCA0, MenuProfiles_HandleEvent, "auto");

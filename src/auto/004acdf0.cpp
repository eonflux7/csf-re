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

CSF_FUNC(int, __fastcall, HasNonzeroLinkedValueDifferentFrom, 0x0048E680, (void* self, void* , int comparisonValue));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));

// Converts a raw integer address into a pointer usable by the helpers above.
static void* AddressAsPointer(int address) {
    return reinterpret_cast<void*>(static_cast<intptr_t>(address));
}

// Resolves the linked order object for a1 and returns the pointer minus 4
// (the original's "v - 4" adjustment), or 0 when the link is empty.
static int LinkedOrderBaseOrZero(int a1) {
    int linked = GetLinkedObjectValue_4DB5C0(AddressAsPointer(a1), nullptr);
    return linked ? linked - 4 : 0;
}

static int __stdcall TOrden_ResetPendingOrderState(int a1) {
    int firstBase = LinkedOrderBaseOrZero(a1);

    int secondBase = LinkedOrderBaseOrZero(a1);

    // Field at +0x2C of the second base is the object tested against firstBase.
    void* testedObject = CSF_AT(void*, AddressAsPointer(secondBase), 0x2C);
    if (HasNonzeroLinkedValueDifferentFrom(testedObject, nullptr, firstBase) == 0) {
        int thirdBase = LinkedOrderBaseOrZero(a1);
        void* notifiedObject = CSF_AT(void*, AddressAsPointer(thirdBase), 0x2C);
        // Virtual slot 0x8C / 4 = 35: called with (0, 0, -1, 0).
        reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
            CSF_AT(void**, notifiedObject, 0)[35])(notifiedObject, nullptr, 0, 0, -1, 0);
    }

    int result = GetLinkedObjectField10(AddressAsPointer(a1), nullptr);
    CSF_AT(int, AddressAsPointer(result), 0xC0) = 0;
    return result;
}

HOOK(0x4ACDF0, TOrden_ResetPendingOrderState, "auto");

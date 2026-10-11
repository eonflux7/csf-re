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

CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_492720, 0x00492720, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_4C6F80, 0x004C6F80, (void* self, void* edx, int a2, char a3));

// Vtable/type tag referenced by the original's call site (S_004c6f80 side).
[[maybe_unused]] static void* const kCoverAttackStateTag = CSF_VTBL(0x004C6F80);

// Original: value ? value - 4 : 0
static uint8_t* LinkedBaseFromValue(int linkedValue)
{
    if (linkedValue)
        return reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(linkedValue) - 4);
    return nullptr;
}

static uint8_t* ResolveLinkedBase(int a2)
{
    return LinkedBaseFromValue(GetLinkedObjectValue_4DB5C0(reinterpret_cast<void*>(static_cast<uintptr_t>(a2)), nullptr));
}

static int __fastcall CoverAttackTask_ResetStateAndExecute(S_004c6f80* self, void* /*edx*/, int a2)
{
    // Clear the state flag on the linked object.
    uint8_t* firstBase = ResolveLinkedBase(a2);
    CSF_AT(int, firstBase, 0x6dc) = 0;

    // Dispatch: sub_492720(3) on the linked object's +0x2c member.
    uint8_t* secondBase = ResolveLinkedBase(a2);
    void* member = CSF_AT(void*, secondBase, 0x2c);
    sub_492720(member, nullptr, 3);

    // Virtual call (slot 0x8c / 4 = 35) on the object at +0x2c of the third lookup.
    uint8_t* thirdBase = ResolveLinkedBase(a2);
    void* object = CSF_AT(void*, thirdBase, 0x2c);
    reinterpret_cast<int(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, object, 0)[0x8c / 4])(object, nullptr, 0, 0, -1, 0);

    return sub_4C6F80(self, nullptr, a2, static_cast<char>(1));
}

HOOK(0x4CA3A0, CoverAttackTask_ResetStateAndExecute, "auto");

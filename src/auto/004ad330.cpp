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
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));

// TOrden: resolves the linked order object for `a1`, invokes its slot-45 virtual
// method with 0, then dispatches with no linked target.
static int __stdcall Order_ResolveLinkedTargetAndDispatch(int a1)
{
    void* self = reinterpret_cast<void*>(static_cast<uintptr_t>(a1));

    int linkedValue = GetLinkedObjectValue_4DB5C0(self, nullptr);

    int v2;
    if (linkedValue)
        v2 = linkedValue - 4;
    else
        v2 = 0;

    uint8_t* base = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(v2));
    void* linkedObj = CSF_AT(void*, base, 0x2c);

    ((void(__fastcall*)(void*, void*, int))(CSF_AT(void**, linkedObj, 0)[0xb4 / 4]))(linkedObj, nullptr, 0);

    return Dispatch_NoLinkedTarget(self, nullptr, 0);
}

HOOK(0x004AD330, Order_ResolveLinkedTargetAndDispatch, "auto");

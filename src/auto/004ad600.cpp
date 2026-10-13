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
CSF_FUNC(int, __fastcall, CoverAttack_SetState_4DC150, 0x004DC150, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, UnnamedApply_4DC180, 0x004DC180, (void* self, void* edx, int a2));

// Copies the three cached dwords of a3 into self+0x0C..0x14, then dispatches
// through the linked object's provider (vtable slot 47) to pick the apply path.
static int __fastcall AiFncStatic_ApplyCachedProvider(void* self, void* /*edx*/, void* a2, const uint32_t* a3)
{
    CSF_AT(uint32_t, self, 0x0C) = a3[0];
    CSF_AT(uint32_t, self, 0x10) = a3[1];
    CSF_AT(uint32_t, self, 0x14) = a3[2];

    int linkedValue = GetLinkedObjectValue_4DB5C0(a2, nullptr);
    int v5 = linkedValue ? linkedValue - 4 : 0;

    void* ownerObj = reinterpret_cast<void*>(v5);
    void* subObj = CSF_AT(void*, ownerObj, 0x2C);
    void* providerObj = CSF_AT(void*, subObj, 0x44);
    int providerResult = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, providerObj, 0)[188 / 4])(providerObj, nullptr, 6);

    if (providerResult != -1)
    {
        return CoverAttack_SetState_4DC150(a2, nullptr, reinterpret_cast<int>(static_cast<uint8_t*>(self) + 4));
    }

    int v7 = GetLinkedObjectValue_4DB5C0(a2, nullptr);
    if (v7)
        return UnnamedApply_4DC180(a2, nullptr, v7 - 4 + 416);
    return UnnamedApply_4DC180(a2, nullptr, 416);
}

HOOK(0x004AD600, AiFncStatic_ApplyCachedProvider, "auto");

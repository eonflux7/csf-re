#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RpHAnimHierarchyUpdateMatrices, 0x0056D3F0, (int* a1));
CSF_FUNC(int, __fastcall, sub_6E8BC0, 0x006E8BC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, sub_6E9030, 0x006E9030, (void* self, void* edx, int a2));

// Updates the HAnim hierarchy matrices of this object when it is active and not flagged (bit 0x4 at +0x58),
// then runs the post-update step and clears the dirty bit (0x200) on the returned hierarchy node.
static int __fastcall C_00861e98_UpdateHierarchyMatrices(C_00861e98* self, void* /*edx*/, int a2)
{
    int result = CSF_AT(int, self, 0x1c);
    if (result != 0 && (CSF_AT(uint32_t, self, 0x58) & 4) == 0) {
        result = sub_6E9030(self, nullptr, a2);
        void* hierarchyNode = reinterpret_cast<void*>(result);

        bool forceUpdate = (CSF_AT(uint8_t, hierarchyNode, 0x14) & 4) != 0;
        if (!forceUpdate) {
            if (a2 == 0) {
                result = CSF_AT(int, self, 0x20);
                forceUpdate = (result != 0);
            }
        }

        if (forceUpdate) {
            RpHAnimHierarchyUpdateMatrices(reinterpret_cast<int*>(CSF_AT(int, hierarchyNode, 0)));
            result = sub_6E8BC0(self, nullptr);
            CSF_AT(uint32_t, hierarchyNode, 0x14) &= ~0x200u;
        }
    }
    return result;
}

HOOK(0x006E9640, C_00861e98_UpdateHierarchyMatrices, "auto");

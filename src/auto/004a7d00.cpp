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

CSF_FUNC(char*, __fastcall, S_00492b20_GetPtr0x1B4, 0x00492B20, (void* self, void*));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, sub_49F140, 0x0049F140, (void* self, void* edx, void* a2));
CSF_FUNC(unsigned int, __fastcall, sub_49F1A0, 0x0049F1A0, (void* self, void* edx, int a2));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Vtable of S_00492b20, referenced by the GetPtr0x1B4 call sites.
[[maybe_unused]] static void* const s_00492b20Vtable = CSF_VTBL(0x00492B20);

static unsigned int* __fastcall C_00834fa8_ProcessMountOrSeatTarget(C_00834fa8* self, void* /*edx*/)
{
    // Linked object lives at owner(+0x08)+0x118.
    char* linkedFlagPtr = S_00492b20_GetPtr0x1B4(CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118), nullptr);
    if (CSF_AT(int, linkedFlagPtr, 0x08) >= 0) {
        void* sceneObj = g_pCurrentScene;
        char* handlePtr = S_00492b20_GetPtr0x1B4(CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118), nullptr);
        int resolved = CHandleTable_Resolve(sceneObj, nullptr,
                                            static_cast<int>(reinterpret_cast<uintptr_t>(handlePtr)));
        if (resolved != 0) {
            void* resolvedObj = reinterpret_cast<void*>(static_cast<uintptr_t>(resolved));
            void* linkedObj = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118);
            int r = sub_49F140(resolvedObj, nullptr, linkedObj);
            return reinterpret_cast<unsigned int*>(static_cast<uintptr_t>(r));
        }
    }

    int seatHandle = CSF_AT(int, self, 0x108);
    if (seatHandle >= 0) {
        void* sceneObj = g_pCurrentScene;
        int resolved = CHandleTable_Resolve(sceneObj, nullptr,
                                            static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(self) + 0x100)));
        if (resolved != 0) {
            void* resolvedObj = reinterpret_cast<void*>(static_cast<uintptr_t>(resolved));
            void* linkedObj = CSF_AT(void*, CSF_AT(void*, self, 0x08), 0x118);
            int slotAddr = static_cast<int>(reinterpret_cast<uintptr_t>(reinterpret_cast<uint8_t*>(linkedObj) + 0x1c));
            unsigned int r = sub_49F1A0(resolvedObj, nullptr, slotAddr);
            return reinterpret_cast<unsigned int*>(static_cast<uintptr_t>(r));
        }
    }

    return reinterpret_cast<unsigned int*>(static_cast<uintptr_t>(static_cast<uint32_t>(CSF_AT(int, self, 0x108))));
}

HOOK(0x004A7D00, C_00834fa8_ProcessMountOrSeatTarget, "auto");

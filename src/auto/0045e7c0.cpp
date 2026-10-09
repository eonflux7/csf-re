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

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// __thiscall with three stack arguments (ret 12). Looks up the handle entry
// referenced by the first argument, notifies the resolved control through its
// vtable slot 0xA8, and copies two cached values from the entry into self.
static int __fastcall RefreshRectFromHandleEntry(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, uint32_t* a4) {
    // v4 = *(*a4 + 4)
    uint8_t* entryBase = reinterpret_cast<uint8_t*>(a4[0]);
    uint8_t* v4 = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, entryBase, 4));

    int result = CSF_AT(int, v4, 12);
    uint8_t* v6 = v4 + 4;  // handle pointer used for the table lookup and entry fields

    if (result >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(reinterpret_cast<uintptr_t>(v6)));
        if (result) {
            uint8_t* resolvedControl = reinterpret_cast<uint8_t*>(result);
            uint8_t* v8 = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, resolvedControl, 52));
            if (v8) {
                uint8_t* v9 = reinterpret_cast<uint8_t*>(a4[1]);

                int callArgs[3];
                callArgs[1] = -1;
                callArgs[0] = 2;
                callArgs[2] = 0;

                uint8_t* sourceObj = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, v9, 4));
                callArgs[1] = CSF_AT(int, sourceObj, 4);

                // (*(void (__thiscall **)(int, int *))(*(std::uint32_t *)v8 + 168))(v8, &v10);
                using NotifyFn = void(__fastcall*)(void* self, void* edx, int* args);
                NotifyFn notify = reinterpret_cast<NotifyFn>(CSF_AT(void**, v8, 0)[42]);
                notify(v8, nullptr, callArgs);

                result = CSF_AT(int, v6, 4);
                CSF_AT(int, self, 96) = result;
                CSF_AT(int, self, 100) = CSF_AT(int, v6, 8);
                CSF_AT(uint8_t, self, 130) = 0;
            }
        }
    }
    return result;
}

HOOK(0x0045E7C0, RefreshRectFromHandleEntry, "auto");

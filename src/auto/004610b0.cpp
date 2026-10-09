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

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(uint32_t, __fastcall, SetFlagBit40, 0x0045BC90, (void* self, void* , char enable));
CSF_FUNC(int, __cdecl, IsStateCodeAllowed, 0x00755810, (unsigned int a1));

// Resolves a handle from a record chain in the current scene. If the resolved
// object's state code is allowed, sets its bit-40 flag from a byte in the second
// record and returns that result; otherwise returns the intermediate value.
static int __stdcall SceneHandle_ApplyFlagBitFromRecord(int a1, int a2, uint32_t* a3)
{
    uint8_t* recordA = reinterpret_cast<uint8_t*>(a3[0]);
    uint8_t* handleRecord = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, recordA, 4));
    int handleIndexField = CSF_AT(int, handleRecord, 12);
    int result = static_cast<int>(reinterpret_cast<uintptr_t>(handleRecord + 4));

    if (handleIndexField >= 0) {
        void* resolved = reinterpret_cast<void*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr, result));
        result = static_cast<int>(reinterpret_cast<uintptr_t>(resolved));
        if (result != 0) {
            void* object = reinterpret_cast<void*>(result);
            int stateCode = CSF_AT(int, object, 80);
            result = IsStateCodeAllowed(static_cast<unsigned int>(stateCode));
            if (static_cast<uint8_t>(result) != 0) {
                uint8_t* controlRecord = reinterpret_cast<uint8_t*>(a3[1]);
                uint8_t* flagRecord = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, controlRecord, 4));
                uint8_t flagByte = CSF_AT(uint8_t, flagRecord, 1);
                return static_cast<int>(SetFlagBit40(object, nullptr, static_cast<char>(flagByte)));
            }
        }
    }
    return result;
}

HOOK(0x004610B0, SceneHandle_ApplyFlagBitFromRecord, "auto");

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

// Binding for the original CHandleTable::Resolve (__thiscall: ecx = this, one stack arg, ret 4).
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Global current scene pointer, read as the Resolve() table owner.
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves a handle record from a3, clears the state byte of the object referenced by a1,
// and (when the resolved entry has a non-null child at +0x34 and the record's flags are
// non-negative) stores the low byte of the child's virtual result (slot 0xB0) into the state byte.
static int __stdcall HandleRecord_ResolveAndRefreshFlag(int a1, int a2, int a3)
{
    uint8_t* owner = reinterpret_cast<uint8_t*>(a1);

    // Clear the state byte at owner->stateBlock + 1.
    CSF_AT(uint8_t*, owner, 0x04)[1] = 0;

    // Walk a3 -> record -> header to get the handle record and its flags.
    uint32_t recordRefValue = *reinterpret_cast<uint32_t*>(a3);
    uint8_t* header = CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(recordRefValue), 0x04);
    int32_t recordFlags = CSF_AT(int32_t, header, 0x0C);
    int result = reinterpret_cast<int>(header + 4);

    if (recordFlags >= 0)
    {
        int resolvedEntry = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        result = resolvedEntry;

        if (result != 0)
        {
            uint8_t* childObject = CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(result), 0x34);
            if (childObject != nullptr)
            {
                uint8_t* stateBlock = CSF_AT(uint8_t*, owner, 0x04);
                auto virtualFn = reinterpret_cast<int(__fastcall*)(void*, void*)>(
                    CSF_AT(void**, childObject, 0)[0xB0 / 4]);
                result = virtualFn(childObject, nullptr);
                stateBlock[1] = static_cast<uint8_t>(result);
            }
        }
    }

    return result;
}

HOOK(0x0045D270, HandleRecord_ResolveAndRefreshFlag, "auto");

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

// CHandleTable::Resolve(this = g_pCurrentScene, handle) -> entry pointer or 0
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
// Unnamed __cdecl helper at 0x755810 taking one unsigned argument; result tested via low byte (al)
CSF_FUNC(int, __cdecl, CheckResolvedObjectFlag, 0x00755810, (unsigned int a1));

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// __stdcall, 3 stack args, callee pops 12 bytes.
static int __stdcall CopyResolvedRecordValueToSlot(int a1, int a2, int a3) {
    // mov eax, [a1+4]; mov [eax+4], -1
    uintptr_t outRecordAddr = CSF_AT(uintptr_t, (void*)(uintptr_t)a1, 4);
    CSF_AT(int32_t, (void*)outRecordAddr, 4) = -1;

    // edx = *a3; eax = [edx+4]; ecx = [eax+0xC]
    uintptr_t handleSourceAddr = CSF_AT(uintptr_t, (void*)(uintptr_t)a3, 0);
    uintptr_t recordAddr = CSF_AT(uintptr_t, (void*)handleSourceAddr, 4);
    int32_t tableIndex = CSF_AT(int32_t, (void*)recordAddr, 12);

    int result = (int)(recordAddr + 4);
    if (tableIndex >= 0) {
        // Resolve(g_pCurrentScene, recordAddr + 4)
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, (int)(recordAddr + 4));
        int resolvedEntry = result;
        if (resolvedEntry != 0) {
            // sub_755810([entry+0x50])
            result = CheckResolvedObjectFlag(CSF_AT(uint32_t, (void*)(uintptr_t)resolvedEntry, 0x50));
            if ((uint8_t)result != 0) {
                // result = [a1+4]; [result+4] = [entry+0x1D8]
                uintptr_t reloadedOutRecord = CSF_AT(uintptr_t, (void*)(uintptr_t)a1, 4);
                result = (int)reloadedOutRecord;
                CSF_AT(uint32_t, (void*)reloadedOutRecord, 4) =
                    CSF_AT(uint32_t, (void*)(uintptr_t)resolvedEntry, 0x1D8);
            }
        }
    }
    return result;
}

HOOK(0x45CA60, CopyResolvedRecordValueToSlot, "auto");

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

// Calls GetGlobal8afc7c (sub_43E010), a __cdecl no-argument getter returning a pointer-like int.
CSF_FUNC(int, __cdecl, GetGlobal8afc7c, 0x43E010, ());

// Returns the eax value the original produces: the low byte is a boolean flag
// (all entries of the list have type 5), the upper bits are leftover register contents.
static int __stdcall CMenuMissionInfo_AllListedEntriesAreFive(CMenuMissionInfo* self)
{
    // Entry count of the self list at +0x38 (begin) / +0x3C (end), 16-byte records.
    uint32_t entryCount = 0;
    uint32_t listBegin = CSF_AT(uint32_t, self, 0x38);
    if (listBegin) {
        int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, self, 0x3C) - listBegin);
        entryCount = static_cast<uint32_t>(byteSpan >> 4);
    }

    // Global object: count of 196-byte records in its list at +0x34 / +0x38.
    int globalObject = GetGlobal8afc7c();
    uint8_t* globalBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(globalObject));
    uint32_t globalCount = 0;
    uint32_t globalBegin = CSF_AT(uint32_t, globalBase, 0x34);
    if (globalBegin) {
        int32_t byteSpan = static_cast<int32_t>(CSF_AT(uint32_t, globalBase, 0x38) - globalBegin);
        globalCount = static_cast<uint32_t>(byteSpan / 196);
    }

    // eax after the signed division by 196 is the quotient (0 when the begin pointer is null).
    uint32_t eaxValue = globalCount;
    bool countsMatch = (entryCount == globalCount);
    eaxValue = (eaxValue & 0xFFFFFF00u) | static_cast<uint32_t>(countsMatch);

    uint32_t index = 0;
    uint32_t byteOffset = 0;
    if (countsMatch) {
        while (index < entryCount) {
            // "mov eax, [edi+0x38]" reloads the full register before "sete al".
            uint32_t entriesBase = CSF_AT(uint32_t, self, 0x38);
            uint8_t* entryPtr = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(entriesBase));
            bool isFive = (CSF_AT(uint32_t, entryPtr, byteOffset + 12) == 5);
            eaxValue = (entriesBase & 0xFFFFFF00u) | static_cast<uint32_t>(isFive);
            ++index;
            byteOffset += 16;
            if ((eaxValue & 0xFFu) == 0)
                break;
        }
    }

    return static_cast<int>(eaxValue);
}

HOOK(0x0053E1C0, CMenuMissionInfo_AllListedEntriesAreFive, "auto");

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// Resolves the handle stored in the first record's nested owner (offset +4, index at +12)
// against the current scene. If it resolves, sets or clears bit 0x200 of the object's flags
// at +0x170, depending on a byte taken from the second record's nested owner (+1).
// Returns the handle-table lookup key (owner+4), or the resolved object when the lookup ran.
static int __stdcall SetSceneObjectFlag200FromRecord(int a1, int a2, uint32_t* a3)
{
    (void)a1;
    (void)a2;

    uint32_t firstRecord = a3[0];
    uint32_t handleOwner = *CSF_AT(uint32_t*, firstRecord, 4);
    int32_t handleIndex = *CSF_AT(int32_t*, handleOwner, 12);
    int result = static_cast<int>(handleOwner + 4);

    if (handleIndex >= 0) {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr, result);
        if (result != 0) {
            uint32_t resolvedObj = static_cast<uint32_t>(result);
            uint32_t flags = *CSF_AT(uint32_t*, resolvedObj, 0x170);

            uint32_t secondRecord = a3[1];
            uint32_t flagSource = *CSF_AT(uint32_t*, secondRecord, 4);
            uint8_t flagByte = *CSF_AT(uint8_t*, flagSource, 1);

            if (flagByte != 0) {
                *CSF_AT(uint32_t*, resolvedObj, 0x170) = flags | 0x200;
            } else {
                *CSF_AT(uint32_t*, resolvedObj, 0x170) = flags & 0xFFFFFDFFu;
            }
        }
    }
    return result;
}

HOOK(0x460F70, SetSceneObjectFlag200FromRecord, "auto");

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

CSF_FUNC(int, __fastcall, ListNodeGetOwnerTag, 0x0066FC30, (void* self, void* edx));
CSF_FUNC(char*, __fastcall, CreateListNodeFromEntry, 0x006524B0, (void* self, void* edx, int a2, int a3, void** a4));
CSF_FUNC(int, __fastcall, ApplyPendingChangeFlag, 0x00651160, (void* self, void* edx, unsigned int a2));

// Walks the circular list at self+0x64. Every node whose payload tag matches a2 is
// replaced by a freshly created node (CreateListNodeFromEntry), which is linked in
// place of the old one. Returns the last value produced in the walk (eax of the original).
static void** __fastcall NetGatheringDDL_ReplaceMatchingEntries(void* self, void* /*edx*/, uintptr_t a2, void* a3)
{
    uintptr_t result = reinterpret_cast<uintptr_t>(CSF_AT(uint8_t*, self, 0x64));
    uint8_t* sentinel = CSF_AT(uint8_t*, self, 0x64);

    if (CSF_AT(uint8_t*, sentinel, 0) != sentinel)
    {
        uint8_t* current = CSF_AT(uint8_t*, sentinel, 0);
        while (true)
        {
            uint8_t* payload = current + 8;
            result = static_cast<uintptr_t>(ListNodeGetOwnerTag(payload, nullptr));
            if (result == a2)
            {
                uint8_t* ownerDatum = CSF_AT(uint8_t*, a3, 4);
                uint32_t vNoOp = CSF_AT(uint32_t, ownerDatum, 4);
                uint8_t* newNode = reinterpret_cast<uint8_t*>(
                    CreateListNodeFromEntry(a3, nullptr,
                        static_cast<int>(reinterpret_cast<uintptr_t>(ownerDatum)),
                        static_cast<int>(vNoOp),
                        reinterpret_cast<void**>(payload)));
                result = static_cast<uintptr_t>(ApplyPendingChangeFlag(a3, nullptr, 1));
                CSF_AT(uint8_t*, ownerDatum, 4) = newNode;
                uint8_t* prevNode = CSF_AT(uint8_t*, newNode, 4);
                CSF_AT(uint8_t*, prevNode, 0) = newNode;
            }
            uint8_t* next = CSF_AT(uint8_t*, current, 0);
            if (next == CSF_AT(uint8_t*, self, 0x64))
                break;
            current = next;
        }
    }
    return reinterpret_cast<void**>(result);
}

HOOK(0x006AA900, NetGatheringDDL_ReplaceMatchingEntries, "auto");

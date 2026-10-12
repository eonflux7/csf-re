#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, C_0085e7b8_FindEntryPayloadByKey, 0x006C4870, (C_0085e7b8* self, void* /*edx*/, int key));
CSF_FUNC(int, __fastcall, C_008670f0_SetEntry, 0x006C4B00, (C_008670f0* self, void* /*edx*/, int key, int value));
CSF_FUNC(void*, __fastcall, sub_6E5210, 0x006E5210, (void* self, void* edx, void** a2, char a3));

// Returns the payload stored under key 5, creating and registering a new one when absent.
static int __fastcall C_00860288_GetOrCreateEntry5Payload(C_00860288* self, void* /*edx*/)
{
    // First lookup: owner->vtbl[7](5) gives the table, then find the entry for key 5.
    void* ownerObj = reinterpret_cast<void*>(self->field_0068);
    void* tableObj = ((void* (__fastcall*)(void*, void*, int))CSF_AT(void**, ownerObj, 0)[7])(ownerObj, nullptr, 5);
    C_0085e7b8* table = reinterpret_cast<C_0085e7b8*>(tableObj);

    int payloadValue = C_0085e7b8_FindEntryPayloadByKey(table, nullptr, 5);
    int* payload = reinterpret_cast<int*>(static_cast<std::uintptr_t>(payloadValue));
    if (payload != nullptr) {
        int existing = *payload;
        if (existing != 0) {
            return existing;
        }
    }

    // Not found: owner->vtbl[7]() gives the table to register into.
    ownerObj = reinterpret_cast<void*>(self->field_0068);
    void* ownerTable = ((void* (__fastcall*)(void*, void*))CSF_AT(void**, ownerObj, 0)[7])(ownerObj, nullptr);

    void* allocated = Mem_Alloc(20);
    if (allocated != nullptr) {
        bool useFlag = (self->field_0064 & 0x800) != 0x800;
        void* newPayload = sub_6E5210(allocated, nullptr, static_cast<void**>(ownerTable), useFlag ? 1 : 0);
        int newPayloadValue = static_cast<int>(reinterpret_cast<std::uintptr_t>(newPayload));
        C_008670f0_SetEntry(reinterpret_cast<C_008670f0*>(ownerTable), nullptr, 5, newPayloadValue);
        return newPayloadValue;
    }

    C_008670f0_SetEntry(reinterpret_cast<C_008670f0*>(ownerTable), nullptr, 5, 0);
    return 0;
}

HOOK(0x006B2A10, C_00860288_GetOrCreateEntry5Payload, "auto");


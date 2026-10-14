#include <cstdint>
#include "globals.h"
#include "hook.h"

// Original callees (bound by address)
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(void*, __fastcall, LookupStateEntry, 0x004E9540, (void* self, void* edx, int a2, int a3, int a4, int a5));

// Virtual slot signatures for the table object (__thiscall written as __fastcall)
typedef int   (__fastcall* TableIntSlotFn)(void* self, void* edx, int arg);
typedef void* (__fastcall* TablePtrSlotFn)(void* self, void* edx, int arg);

static int __fastcall SyncCoordsFromLookup(void* self, void* /*edx*/, int a2, int a3, int a4)
{
    (void)a3;

    uint8_t* root = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(GetValue8B0554()));
    void* table = CSF_AT(void*, root, 8136);

    // a4 -> object -> +4 -> +4 gives the key
    int firstWord = *reinterpret_cast<int*>(static_cast<uintptr_t>(a4));
    int secondWord = *reinterpret_cast<int*>(static_cast<uintptr_t>(firstWord) + 4);
    int key = *reinterpret_cast<int*>(static_cast<uintptr_t>(secondWord) + 4);

    int result = a4;

    if (key == -1 ||
        (result = reinterpret_cast<TableIntSlotFn>(CSF_AT(void**, table, 0)[4])(table, nullptr, key),
         result == -1))
    {
        CSF_AT(uint8_t, self, 0x82) = 0;
        CSF_AT(uint8_t, self, 0x80) = 0;
    }
    else
    {
        void* info = reinterpret_cast<TablePtrSlotFn>(CSF_AT(void**, table, 0)[2])(table, nullptr, result);
        int stateValue = CSF_AT(int, info, 180);

        // 0045be34: call 0x4e8c50 once; 0045be39: mov ecx, eax; 0045be3b: call 0x4e9540
        void* manager = reinterpret_cast<void*>(static_cast<uintptr_t>(GetValue8B0554()));
        uint8_t* entry = static_cast<uint8_t*>(
            LookupStateEntry(manager, nullptr, stateValue, key, 0, 0));

        // Destination record: a2 -> +4 -> +4 holds the output words
        uint8_t* argBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a2));
        uint8_t* dest = reinterpret_cast<uint8_t*>(
            static_cast<uintptr_t>(CSF_AT(uint32_t, argBase, 4)));
        uint8_t* out = dest + 4;

        CSF_AT(uint32_t, out, 4) = CSF_AT(uint32_t, entry, 0x20);
        result = static_cast<int>(CSF_AT(uint32_t, entry, 0x24));
        CSF_AT(uint32_t, out, 8) = static_cast<uint32_t>(result);
    }

    return result;
}

HOOK(0x0045BDF0, SyncCoordsFromLookup, "auto");

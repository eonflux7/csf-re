#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());
CSF_FUNC(int, __cdecl, GetGlobalValue_44ACE0, 0x0044ACE0, ());
CSF_FUNC(uint32_t*, __fastcall, sub_44BAB0, 0x0044BAB0, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __fastcall, sub_7686E0, 0x007686E0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_768740, 0x00768740, (void* self, void* edx, int a2));

// Resolves the slot index for the entry referenced by the third argument and,
// when the lookup succeeds, forwards the derived slot pointer to the global handler.
static uint32_t* __stdcall RecordTable_ResolveSlotPointer(uint32_t a1, uint32_t a2, uint32_t a3)
{
    (void)a1;
    (void)a2;

    uint32_t* argRef = reinterpret_cast<uint32_t*>(a3);
    uint32_t* record = reinterpret_cast<uint32_t*>(argRef[0]);
    uint32_t* entry = reinterpret_cast<uint32_t*>(record[1]);

    void* firstOwner = reinterpret_cast<void*>(GetValue8B0554());
    void* firstManager = CSF_AT(void*, firstOwner, 0x1F90);
    uint32_t entryKey = entry[1];
    int slot = sub_7686E0(firstManager, nullptr, static_cast<int>(entryKey));

    if (slot != -1)
    {
        void* secondOwner = reinterpret_cast<void*>(GetValue8B0554());
        void* secondManager = CSF_AT(void*, secondOwner, 0x1F90);
        uint32_t* slotPtr = reinterpret_cast<uint32_t*>(sub_768740(secondManager, nullptr, slot) + 4);
        void* globalValue = reinterpret_cast<void*>(GetGlobalValue_44ACE0());
        return sub_44BAB0(globalValue, nullptr, slotPtr);
    }

    return reinterpret_cast<uint32_t*>(slot);
}

HOOK(0x0045D8E0, RecordTable_ResolveSlotPointer, "auto");

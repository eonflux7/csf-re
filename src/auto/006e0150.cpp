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

// Callees (bound by address, signatures as given in the packet)
CSF_FUNC(uint32_t*, __cdecl, PTankCreate_AllocateAndInit, 0x006EF3C0, ());
CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* /*edx*/, int additionalCapacity));
CSF_FUNC(int, __cdecl, GetGlobalValue_6C5760, 0x006C5760, ());
CSF_FUNC(unsigned int, __fastcall, GfxSkin_ApplyRegionFieldsByFlag, 0x006C5820, (C_0085e7b8** self, void* /*edx*/, int a2));

// Vtable of the slot-list helper (S_0041e5f0). Referenced by the original's call path; no store here.
[[maybe_unused]] static void* const kSlotListVtbl = (void*)CSF_VTBL(0x0041E5F0);

// Creates a tank object, initializes it with the given argument, stores it in the
// first empty slot of the owner's pointer list (or appends it), then applies region fields.
static uint32_t* __fastcall C_00861988_CreateTankAndRegister(C_00861988* self, void* /*edx*/, int a2)
{
    uint32_t* tank = PTankCreate_AllocateAndInit();

    // vtable slot 4 (+0x10): init(tank, a2)
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, tank, 0)[4])(tank, nullptr, a2);

    uint32_t slotCount = self->field_002c;
    S_0041e5f0* slotList = reinterpret_cast<S_0041e5f0*>(reinterpret_cast<uint8_t*>(self) + 0x24);

    // Search for the first empty (zero) entry among the first slotCount slots.
    uint32_t foundIndex = 0;
    bool foundEmpty = false;
    if (slotCount != 0)
    {
        uint32_t* slot = reinterpret_cast<uint32_t*>(slotList->field_0000);
        while (true)
        {
            if (*slot == 0)
            {
                foundEmpty = true;
                break;
            }
            ++foundIndex;
            ++slot;
            if (foundIndex >= slotCount)
                break;
        }
    }

    if (foundEmpty)
    {
        reinterpret_cast<uint32_t*>(slotList->field_0000)[foundIndex] = reinterpret_cast<uint32_t>(tank);
    }
    else
    {
        // Append at the end, growing the array first if it is full.
        if (slotList->field_0008 >= slotList->field_0004)
            S_0041e5f0_GrowArrayCapacity(slotList, nullptr, slotList->field_000c);

        uint32_t appendIndex = slotList->field_0008;
        uint32_t* appendSlot = reinterpret_cast<uint32_t*>(slotList->field_0000) + appendIndex;
        slotList->field_0008 = appendIndex + 1;
        *appendSlot = reinterpret_cast<uint32_t>(tank);
    }

    int globalValue = GetGlobalValue_6C5760();
    GfxSkin_ApplyRegionFieldsByFlag(
        reinterpret_cast<C_0085e7b8**>(static_cast<uintptr_t>(globalValue)),
        nullptr,
        (int)(uintptr_t)tank);

    return tank;
}

HOOK(0x006E0150, C_00861988_CreateTankAndRegister, "auto");

#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, LookupObjectByIndex, 0x0064AD00, (int a1));
CSF_FUNC(char, __stdcall, sub_65E750, 0x0065E750, (int a1, int a2));
CSF_FUNC(void**, __fastcall, sub_6622F0, 0x006622F0, (void* self, void* edx, void** a2, uint8_t* a3));
CSF_FUNC(int, __fastcall, sub_66CB70, 0x0066CB70, (void* self, void* edx, int a2, int a3, int a4, int a5));

static void DispatchSlot(void* self, int slot, uint32_t offset, int a5)
{
    uint8_t slotByte = static_cast<uint8_t>(slot);
    void* result = nullptr;

    int obj = LookupObjectByIndex(static_cast<int>(CSF_AT(uint32_t, self, 0x2c) >> 22));
    sub_6622F0(reinterpret_cast<uint8_t*>(obj) + 0xc, nullptr, &result, &slotByte);
    void* target = CSF_AT(void*, result, 0x10);
    sub_66CB70(target, nullptr,
               static_cast<int>(reinterpret_cast<uintptr_t>(self)),
               static_cast<int>(reinterpret_cast<uintptr_t>(static_cast<uint8_t*>(self) + offset)),
               slot, a5);
}

static char __fastcall NetCogibleDDL_HandleSlotMessage(void* self, void* /*edx*/, int a5, int a3)
{
    switch (static_cast<char>(a3)) {
    case 1:
        DispatchSlot(self, 1, 0x44, a5);
        return 1;
    case 2:
        DispatchSlot(self, 2, 0x6c, a5);
        return 1;
    case 3:
        DispatchSlot(self, 3, 0x6f, a5);
        return 1;
    case 4:
        DispatchSlot(self, 4, 0x72, a5);
        return 1;
    default:
        return sub_65E750(a5, a3);
    }
}

HOOK(0x00743280, NetCogibleDDL_HandleSlotMessage, "auto");

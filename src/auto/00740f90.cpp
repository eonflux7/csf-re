#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Binds to the original game functions by address.
CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(void, __fastcall, SetFlagAt14E, 0x0073C6F0, (uint8_t* self, void* /*edx*/, int a2));
CSF_FUNC(int, __fastcall, NetSession_ConsumeSlotIfMatches, 0x0073C700, (void* self, void* /*edx*/, int expectedValue));
CSF_FUNC(int, __fastcall, NetHost_EndEntry, 0x0074A470, (void* self, void* /*edx*/, int a2));
CSF_FUNC(void, __fastcall, NetHost_BeginEntry, 0x0074A5A0, (void* self, void* /*edx*/, int a2));

// Dispatches a slot action for `this` based on the result of a2->vtbl[1]():
//   result 2 -> begin entry on the host, then set the flag on the session object.
//   result 5 -> end entry on the host, then consume the matching slot.
static void __fastcall NetCogible_DispatchSlotAction(C_00867900* self, void* /*edx*/, int a2)
{
    void* sourceObject = reinterpret_cast<void*>(a2);
    int actionCode = ((int(__fastcall*)(void*, void*))CSF_AT(void**, sourceObject, 0)[1])(sourceObject, nullptr);

    switch (actionCode) {
    case 2: {
        int globalHandle = GetGlobalDword_73C020();
        NetHost_BeginEntry(CSF_AT(void*, reinterpret_cast<void*>(globalHandle), 0xC8), nullptr, reinterpret_cast<int>(self));
        int flagHandle = GetGlobalDword_73C020();
        SetFlagAt14E(reinterpret_cast<uint8_t*>(flagHandle), nullptr, reinterpret_cast<int>(self));
        break;
    }
    case 5: {
        int globalHandle = GetGlobalDword_73C020();
        NetHost_EndEntry(CSF_AT(void*, reinterpret_cast<void*>(globalHandle), 0xC8), nullptr, reinterpret_cast<int>(self));
        int slotHandle = GetGlobalDword_73C020();
        NetSession_ConsumeSlotIfMatches(reinterpret_cast<void*>(slotHandle), nullptr, reinterpret_cast<int>(self));
        break;
    }
    default:
        break;
    }
}

HOOK(0x740F90, NetCogible_DispatchSlotAction, "auto");

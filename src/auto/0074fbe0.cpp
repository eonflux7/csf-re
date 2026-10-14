#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Reads one stream value via the NetCogible DDL reader into the given slot.
CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));

// Forwards the eight decoded player-event values to the event dispatcher.
CSF_FUNC(int, __fastcall, S_00743570_ForwardPlayerEvent, 0x00743570,
         (S_00743570* self, void* /*edx*/, int a2, int a3, int a4, int a5, int a6, int a7, int a8, int a9));

static int __fastcall S_00743570_ReadEventAndForward(S_00743570* self, void* /*edx*/, C_008622e8* arg0)
{
    // The original keeps the reader pointer in a register before the parameter slot is reused.
    C_008622e8* reader = arg0;

    int a2;
    int a3;
    int a4;
    int a5;
    int a6;
    int a7;
    int a8;
    int ninthValue;

    // Reference to the S_00743570 vtable address, kept so the binding is present (no behavior).
    // Code-address annotation: function value, not a vtable.
    [[maybe_unused]] const void* vtableRef = reinterpret_cast<void**>(CSF_ADDR_FN(0x00743570));

    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a2));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a3));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a4));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a5));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a6));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a7));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&a8));
    NetCogibleDdl_ReadIntoSlot(reader, nullptr, reinterpret_cast<int>(&ninthValue));

    return S_00743570_ForwardPlayerEvent(self, nullptr, a2, a3, a4, a5, a6, a7, a8, ninthValue);
}

HOOK(0x0074FBE0, S_00743570_ReadEventAndForward, "auto");

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Dispatch entry at 0x749660 (7 stack args, ret 28). Name chosen locally; signature kept from the packet.
CSF_FUNC(char, __fastcall, SpawnPoint_DispatchRequest, 0x00749660,
         (void* self, void* edx, int arg0, int a3, int a4, int a5, int a6, int a7, int16_t a2));

// __thiscall method: ecx = this, 4 stack args, ret 16.
static int __fastcall C_00844694_ForwardSpawnRequest(C_00844694* self, void* /*edx*/,
                                                     int a2, int a3, int a4, int a5)
{
    int result = static_cast<int>(self->field_0c88);
    if (result)
    {
        result = CSF_AT(int, reinterpret_cast<void*>(result), 0x4);
        if (result)
        {
            // Object at (result - 0x440) is the dispatch target.
            return SpawnPoint_DispatchRequest(reinterpret_cast<void*>(result - 0x440), nullptr,
                                              -1, -1, a2, a3, a4, a5, 1);
        }
    }
    return result;
}

HOOK(0x00551680, C_00844694_ForwardSpawnRequest, "auto");

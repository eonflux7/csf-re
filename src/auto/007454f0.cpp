#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static CSF_FUNC(void, __stdcall, C_0085ec18_NullSub, 0x004B2A20, (int));
static CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));

static int __fastcall NetPlayerDdl_ReadIntoSlot(
    void* /*self*/, void* /*edx*/, C_008622e8* source)
{
    uint32_t* result = NetCogibleDdl_ReadIntoSlot(
        source, nullptr, reinterpret_cast<int>(&source));
    C_0085ec18_NullSub(reinterpret_cast<int>(source));
    return reinterpret_cast<int>(result);
}

HOOK(0x007454F0, NetPlayerDdl_ReadIntoSlot, "auto");

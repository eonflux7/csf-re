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

CSF_FUNC(uint32_t*, __fastcall, NetCogibleDdl_ReadIntoSlot, 0x00655F00, (void* self, void* edx, int a2));
CSF_FUNC(char, __fastcall, sub_73FF40, 0x0073FF40, (void* self, void* edx, int a2, int a3, uint8_t* a4));

// Reads three values out of the argument object through NetCogibleDdl_ReadIntoSlot,
// the first two into locals and the third back into the argument slot itself,
// then forwards them to sub_73FF40 together with this.
static char __fastcall NetProyectilDdl_ReadTripleAndForward(C_00858180* self, void* /*edx*/, C_008622e8* arg0)
{
    C_008622e8* argument = arg0;
    int a2 = 0;
    int a3 = 0;

    NetCogibleDdl_ReadIntoSlot(argument, nullptr, (int)(intptr_t)&a2);
    NetCogibleDdl_ReadIntoSlot(argument, nullptr, (int)(intptr_t)&a3);
    NetCogibleDdl_ReadIntoSlot(argument, nullptr, (int)(intptr_t)&arg0);

    return sub_73FF40(self, nullptr, a2, a3, reinterpret_cast<uint8_t*>(arg0));
}

HOOK(0x007479A0, NetProyectilDdl_ReadTripleAndForward, "auto");

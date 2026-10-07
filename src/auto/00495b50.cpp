#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "core/memory.h"

// 0x00495B50: ORs bit 0x00080000 into the dword state flags at object+0x2E4.
static void __fastcall SetObjectFlag80000(void* self, void* /*edx*/)
{
    CSF_AT(uint32_t, self, 0x2E4) |= 0x80000u;
}

HOOK(0x495B50, SetObjectFlag80000, "auto");

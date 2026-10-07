#include <cstdint>
#include "globals.h"
#include "hook.h"

static void __fastcall ClearMember0C(void* self, void* /*edx*/)
{
    CSF_AT(std::uint32_t, self, 0x0C) = 0;
}

HOOK(0x0075A1C0, ClearMember0C, "auto");

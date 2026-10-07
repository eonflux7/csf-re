#include <cstdint>
#include "globals.h"
#include "hook.h"

static int __fastcall GetSubObjectField18(void* self, void* /*edx*/)
{
    uint32_t subObject = CSF_AT(uint32_t, self, 0x10);
    if (subObject)
        return CSF_AT(int, reinterpret_cast<void*>(subObject), 0x18);
    return 0;
}

HOOK(0x6B8280, GetSubObjectField18, "auto");

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

static unsigned int __fastcall ForwardArgumentToChildren(void* self, void* /*edx*/, int argument)
{
    unsigned int count = CSF_AT(unsigned int, self, 0xA0);
    for (unsigned int index = 0; index < count; ++index)
    {
        void* child = CSF_AT(void**, self, 0x98)[index];
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, child, 0)[0x50 / 4])(child, nullptr, argument);
        count = CSF_AT(unsigned int, self, 0xA0);
    }
    return count;
}

HOOK(0x0048F9B0, ForwardArgumentToChildren, "auto");

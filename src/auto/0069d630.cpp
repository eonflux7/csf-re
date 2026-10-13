#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Forwards a 2-argument call to the sink object at this->field_0004 through its
// vtable slot 0x14 (index 5). The selector is 0/1/2 for the explicit cases and
// a3 itself otherwise; the sink receives (selector, a2) as stack arguments.
static int __fastcall C_0085ea2c_ForwardToSink(C_0085ea2c* self, void* /*edx*/, int a2, int a3)
{
    using SinkCall = int(__fastcall*)(void*, void*, int, int);

    void* sinkObject = reinterpret_cast<void*>(self->field_0004);
    SinkCall sinkSlot5 = reinterpret_cast<SinkCall>(CSF_AT(void**, sinkObject, 0)[5]);

    int selector = a3;
    switch (a3) {
    case 0:
        selector = 0;
        break;
    case 1:
        selector = 1;
        break;
    case 2:
        selector = 2;
        break;
    default:
        selector = a3;
        break;
    }

    return sinkSlot5(sinkObject, nullptr, selector, a2);
}

HOOK(0x0069D630, C_0085ea2c_ForwardToSink, "auto");

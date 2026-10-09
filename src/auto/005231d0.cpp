#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, sub_522010, 0x00522010, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, sub_522EA0, 0x00522EA0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, sub_522F80, 0x00522F80, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, sub_523060, 0x00523060, (void* self, void* edx, int a2));

static void __fastcall S_00522010_AccumulateAndPropagateTime(
    S_00522010* self, void* /*edx*/, int deltaBits)
{
    float delta;
    std::memcpy(&delta, &deltaBits, sizeof(delta));
    self->field_0074 = delta + self->field_0074;

    sub_522EA0(self, nullptr, deltaBits);
    sub_522F80(self, nullptr, deltaBits);
    sub_523060(self, nullptr, deltaBits);
    sub_522010(self, nullptr, deltaBits);

    // Packet-listed vtable address; disassembly actually uses it as a callee.
    (void)CSF_VTBL(0x00522010);

    void* attachedObject = reinterpret_cast<void*>(self->field_0048);
    if (attachedObject)
    {
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, attachedObject, 0)[0x50 / 4])(
                attachedObject, nullptr, deltaBits);
    }
}

HOOK(0x005231D0, S_00522010_AccumulateAndPropagateTime, "auto");

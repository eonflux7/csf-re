#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(int*, __fastcall, C_008442a0_AccumulateLastEntryMeasurement, 0x0054BD20, (C_008442a0* self, void* edx));
static CSF_FUNC(unsigned int, __fastcall, S_007721a0_Resize, 0x0054BDE0, (S_007721a0* self, void* , unsigned int count, char value, int));

// Pops the last entry of the scope stack and forwards the end notification to the child object.
static hook::Tail __fastcall C_008442a0_PopEntryAndForwardEnd(void* object, const uint32_t* /*args*/)
{
    auto* self = static_cast<C_008442a0*>(object);
    C_008442a0_AccumulateLastEntryMeasurement(self, nullptr);

    void* child = reinterpret_cast<void*>(self->field_00a4);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, child, 0)[0x90 / 4])(child, nullptr);

    const uint32_t begin = self->field_008c;
    const int32_t count = begin
        ? static_cast<int32_t>(self->field_0090 - begin) >> 3
        : 0;
    S_007721a0_Resize(reinterpret_cast<S_007721a0*>(reinterpret_cast<uint8_t*>(self) + 0x88),
                      nullptr, static_cast<unsigned int>(count - 1), 0, 0);

    const int32_t index = static_cast<int32_t>(self->field_0004) - 1;
    self->field_0004 = static_cast<uint32_t>(index);
    if (index >= 0)
        CSF_AT(uint8_t, self, 0x008 + 4 * static_cast<uint32_t>(index)) = 0;

    child = reinterpret_cast<void*>(self->field_00a4);
    if (child)
        return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x7C / 4]);
    return hook::tail_ret(1, 4);
}

HOOK_TAIL(0x0054BF40, C_008442a0_PopEntryAndForwardEnd, "type_registry");

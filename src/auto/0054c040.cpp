#include <cstdint>
#include "hook.h"
#include "globals.h"
#include "types/auto_classes.h"

static CSF_FUNC(int*, __fastcall, sub_54BD20, 0x0054BD20, (void* self, void* edx));
static CSF_FUNC(unsigned int, __fastcall, sub_54BDE0, 0x0054BDE0, (void* self, void* edx, unsigned int a2, int a3, int a4));

static hook::Tail __fastcall C_008442a0_EndScope(void* object, const uint32_t* /*args*/)
{
    auto* self = static_cast<C_008442a0*>(object);
    sub_54BD20(self, nullptr);

    void* child = reinterpret_cast<void*>(self->field_00a4);
    reinterpret_cast<void(__fastcall*)(void*, void*)>(
        CSF_AT(void**, child, 0)[0x90 / 4])(child, nullptr);

    const uint32_t begin = self->field_008c;
    const int32_t count = begin
        ? static_cast<int32_t>(self->field_0090 - begin) >> 3
        : 0;
    sub_54BDE0(reinterpret_cast<uint8_t*>(self) + 0x88, nullptr,
               static_cast<uint32_t>(count) - 1, 0, 0);

    const uint32_t index = self->field_0004 - 1;
    self->field_0004 = index;
    if (static_cast<int32_t>(index) >= 0)
        CSF_AT(uint8_t, self, 0x008 + 4 * index) = 0;

    child = reinterpret_cast<void*>(self->field_00a4);
    if (child)
        return hook::tail_jump(child, (*reinterpret_cast<void***>(child))[0x84 / 4]);
    return hook::tail_ret(1, 4);
}

HOOK_TAIL(0x0054C040, C_008442a0_EndScope, "type_registry");

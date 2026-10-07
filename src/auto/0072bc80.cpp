// Reimplementation of sub_72BC80 (0x0072BC80) - Commandos: Strike Force
// Trivial accessor: returns a pointer to the sub-object / member located at
// offset 0x134 inside the C_00865b40 instance (lea eax, [ecx+0x134]; ret).

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Returns &self->field_0134 (offset 0x134, currently inside pad_0131 in the
// declared layout). __thiscall(this) -> (ecx = self, edx unused), ret 0.
static uint8_t* __fastcall C_00865b40_GetMember0134(C_00865b40* self, void* /*edx*/)
{
    return reinterpret_cast<uint8_t*>(self) + 0x134;
}

HOOK(0x0072BC80, C_00865b40_GetMember0134, "auto");

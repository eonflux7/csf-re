#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Pops the top index from the stack at this->field_0024 (count in field_002c),
// stores a2 into the owner's table (owner = field_0044, table = owner+0x50)
// at slot [result*8+4], decrements the count, and returns the popped index.
static int __fastcall ScriptIndexStack_PopStore(S_0046c260* self, void* /*edx*/, int a2)
{
    uint8_t* indexStack = reinterpret_cast<uint8_t*>(self->field_0024);
    int result = CSF_AT(int, indexStack, self->field_002c * 4 - 4);

    uint8_t* owner = reinterpret_cast<uint8_t*>(self->field_0044);
    uint8_t* table = reinterpret_cast<uint8_t*>(CSF_AT(uint32_t, owner, 0x50));
    CSF_AT(int, table, result * 8 + 4) = a2;

    --self->field_002c;
    return result;
}

HOOK(0x46C540, ScriptIndexStack_PopStore, "auto");

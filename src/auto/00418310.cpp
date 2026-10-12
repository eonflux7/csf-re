#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, sub_4161B0, 0x004161B0, (void* self, void* edx, float* a2));

// Ensures the state block is initialized once, then copies the 100-byte block
// from a2 into the object's state buffer and marks it valid.
static char __fastcall C_0082e128_LoadStateBlock(C_0082e128* self, void* /*edx*/, float* a2)
{
    char result = static_cast<char>(self->field_01f1);
    if (!result)
    {
        uint32_t owner = self->field_0004;
        if (owner)
        {
            uint32_t child = CSF_AT(uint32_t, reinterpret_cast<void*>(owner), 0x18);
            if (child)
            {
                void* childObj = reinterpret_cast<void*>(child);
                // vtable slot 2 (+0x8) on the child object, called with no arguments
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, childObj, 0)[2])(childObj, nullptr);
            }
        }

        result = static_cast<char>(sub_4161B0(self, nullptr, a2));
        self->field_01f3 = 1;
    }

    std::memcpy(&self->field_0128, a2, 0x64);
    self->field_01f0 = 1;
    return result;
}

HOOK(0x00418310, C_0082e128_LoadStateBlock, "auto");

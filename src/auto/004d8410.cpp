#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, TCharTask_BaseUpdate, 0x004DB0F0, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, HasNonzeroLinkedValueDifferentFrom, 0x0048E680, (void* self, void* edx, int comparisonValue));
CSF_FUNC(int, __fastcall, sub_4AEB90, 0x004AEB90, (void* self, void* edx));
CSF_FUNC(char, __fastcall, sub_4D7670, 0x004D7670, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTask_LookupByOffset, 0x004DB440, (void* self, void* edx, int a2));

// Updates the character task: runs the base update, then, when no linked object
// reports a nonzero value, dispatches the lookup based on the character state.
static char __fastcall C_00834fa8_UpdateCharTask(C_00834fa8* self, void* /*edx*/, int a2)
{
    TCharTask_BaseUpdate(self, nullptr, a2);

    void* charObject = reinterpret_cast<void*>(self->field_0008);
    C_0082f9f8* linkedObject = CSF_AT(C_0082f9f8*, charObject, 0x14);
    int v3 = HasNonzeroLinkedValueDifferentFrom(linkedObject, nullptr, self->field_0008);

    if (static_cast<char>(v3) == 0)
    {
        v3 = sub_4D7670(charObject, nullptr);
        if (static_cast<char>(v3) != 0)
        {
            void* weaponObject = CSF_AT(void*, charObject, 0x30C);
            v3 = sub_4AEB90(weaponObject, nullptr);
            if (v3 != -1)
            {
                if (v3 != 0 || (v3 = CSF_AT(uint8_t, charObject, 0x33C), v3 == 0))
                {
                    int lookupArg = static_cast<int>(self->field_000c + 0x14);
                    v3 = CharTask_LookupByOffset(self, nullptr, lookupArg);
                }
            }
        }
    }
    return static_cast<char>(v3);
}

HOOK(0x004D8410, C_00834fa8_UpdateCharTask, "auto");

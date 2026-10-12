#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static uint32_t* __fastcall S_006de5b0_EraseRange(
    S_006de5b0* self, void* /*edx*/, uint32_t* result,
    char* first, char* last)
{
    if (first != last)
    {
        char* end = reinterpret_cast<char*>(self->field_0008);
        char* destination = first;
        char* source = last;

        if (source != end)
        {
            do
            {
                for (unsigned i = 0; i < 7; ++i)
                    CSF_AT(uint32_t, destination, i * 4) =
                        CSF_AT(uint32_t, source, i * 4);

                source += 28;
                destination += 28;
            } while (source != end);
        }

        self->field_0008 = reinterpret_cast<uint32_t>(destination);
    }

    *result = reinterpret_cast<uint32_t>(first);
    return result;
}

HOOK(0x006DE5B0, S_006de5b0_EraseRange, "auto");

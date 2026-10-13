#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, GetElementByIndex, 0x0069E4D0, (void* self, void* , int index));
CSF_FUNC(uint8_t*, __fastcall, S_00693740_ProcessSubtree, 0x00693C90, (void* self, void* edx, uint8_t* a2));

static uint8_t* __fastcall S_00693740_ParseNextIndexedEntry(S_00693740* self, void* /*edx*/, uint8_t* a2)
{
    uint32_t index = self->field_0010;
    uint8_t* result = reinterpret_cast<uint8_t*>(index);
    void* container = reinterpret_cast<void*>(self->field_000c);

    if (index < CSF_AT(uint32_t, container, 4))
    {
        uint8_t* element = reinterpret_cast<uint8_t*>(
            GetElementByIndex(container, nullptr, static_cast<int>(index)));
        self->field_0010++;
        result = element;

        // element+4 holds a name pointer; the first byte of that string is tested.
        uint8_t* nameString = CSF_AT(uint8_t*, element, 4);
        if (*nameString == 0x5B)
        {
            if (CSF_AT(uint8_t, a2, 0x0B) == 0xFF)
            {
                uint8_t* target = CSF_AT(uint8_t*, a2, 0);
                CSF_AT(uint32_t, target, 0) = 2;
            }
            else
            {
                CSF_AT(uint8_t, a2, 0x0A) = 2;
            }

            if (CSF_AT(uint8_t, a2, 0x0B) == 0xFF)
            {
                uint8_t* target = CSF_AT(uint8_t*, a2, 0);
                CSF_AT(uint32_t, target, 0x44) = CSF_AT(uint32_t, element, 4);
            }
            return S_00693740_ProcessSubtree(self, nullptr, a2);
        }
    }
    return result;
}

HOOK(0x006943C0, S_00693740_ParseNextIndexedEntry, "auto");

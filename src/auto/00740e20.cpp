#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

// Adds a delta to one of six 16-bit counters selected by index; returns the new value
// (or the low 16 bits of the index when it is out of range).
static int16_t __fastcall NetCogibleDDL_AddToCounter(S_00740e20* self, void* /*edx*/, int index, int16_t delta)
{
    uint16_t* counter;

    switch (index)
    {
    case 0:
        counter = &self->field_068c;
        break;
    case 1:
        counter = &CSF_AT(uint16_t, self, 0x680);
        break;
    case 2:
        counter = &CSF_AT(uint16_t, self, 0x698);
        break;
    case 3:
        counter = &CSF_AT(uint16_t, self, 0x692);
        break;
    case 4:
        counter = &CSF_AT(uint16_t, self, 0x69E);
        break;
    case 5:
        counter = &CSF_AT(uint16_t, self, 0x686);
        break;
    default:
        return (int16_t)index;
    }

    int16_t result = (int16_t)(delta + *counter);
    if ((uint16_t)result != *counter)
        *counter = (uint16_t)result;
    return result;
}

HOOK(0x740E20, NetCogibleDDL_AddToCounter, "auto");

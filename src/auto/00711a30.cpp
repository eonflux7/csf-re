#include <cstdint>
#include "globals.h"
#include "hook.h"

// Enables or disables an audio effect. A nonzero a2 activates it and notifies the
// child object at +8 (slot 2 with args 0, 1). Zero deactivates it, calls the object's
// own slot 6 with 1, then notifies the child with args 1, 0.
static char __fastcall AudioEffect_SetActiveState(uint8_t* self, void* /*edx*/, char a2)
{
    char result = a2;
    if (a2 != CSF_AT(char, self, 0x1B5))
    {
        CSF_AT(char, self, 0x1B5) = a2;
        if (a2)
        {
            CSF_AT(uint8_t, self, 0x1B3) = 0;
            uint32_t child = CSF_AT(uint32_t, self, 0x8);
            if (child)
            {
                void* childObj = reinterpret_cast<void*>(child);
                return ((char(__fastcall*)(void*, void*, int, int))CSF_AT(void**, childObj, 0)[2])(childObj, nullptr, 0, 1);
            }
        }
        else
        {
            CSF_AT(uint8_t, self, 0x1B3) = 1;
            result = ((char(__fastcall*)(void*, void*, int))CSF_AT(void**, self, 0)[6])(self, nullptr, 1);
            uint32_t child = CSF_AT(uint32_t, self, 0x8);
            if (child)
            {
                void* childObj = reinterpret_cast<void*>(child);
                return ((char(__fastcall*)(void*, void*, int, int))CSF_AT(void**, childObj, 0)[2])(childObj, nullptr, 1, 0);
            }
        }
    }
    return result;
}

HOOK(0x711A30, AudioEffect_SetActiveState, "auto");

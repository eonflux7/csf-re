#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, AutoTask_StartSecondaryOrder, 0x0048A030, (void* self, void* edx, int a2));

// Walks the 96-byte slot array of C_00833708. Each slot holds an object pointer and a state.
// Nonzero states get their object's vtable[10] called with a2. States 3 and 4 poll vtable[11]:
// state 3 completes by starting a secondary order, state 4 resets to 1.
static uint32_t __fastcall C_00833708_UpdateSlotStates(C_00833708* self, void* /*edx*/, int a2)
{
    uint32_t result = self->field_0014;
    uint32_t slotIndex = 0;

    if (result != 0)
    {
        uint32_t slotOffset = 0;
        do
        {
            uint8_t* slots = reinterpret_cast<uint8_t*>(self->field_000c);
            int slotState = CSF_AT(int, slots, slotOffset + 4);
            void* obj = CSF_AT(void*, slots, slotOffset);

            if (slotState)
            {
                ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[10])(obj, nullptr, a2);
            }

            slots = reinterpret_cast<uint8_t*>(self->field_000c);
            if (CSF_AT(int, slots, slotOffset + 4) == 3)
            {
                void* stateObj = CSF_AT(void*, slots, slotOffset);
                if (((bool(__fastcall*)(void*, void*))CSF_AT(void**, stateObj, 0)[11])(stateObj, nullptr))
                {
                    slots = reinterpret_cast<uint8_t*>(self->field_000c);
                    CSF_AT(int, slots, slotOffset + 4) = 3;
                    AutoTask_StartSecondaryOrder(self, nullptr, static_cast<int>(slotIndex));
                }
            }
            else if (CSF_AT(int, slots, slotOffset + 4) == 4)
            {
                void* stateObj = CSF_AT(void*, slots, slotOffset);
                if (((bool(__fastcall*)(void*, void*))CSF_AT(void**, stateObj, 0)[11])(stateObj, nullptr))
                {
                    slots = reinterpret_cast<uint8_t*>(self->field_000c);
                    CSF_AT(int, slots, slotOffset + 4) = 1;
                }
            }

            result = self->field_0014;
            ++slotIndex;
            slotOffset += 96;
        } while (slotIndex < result);
    }

    return result;
}

HOOK(0x0048A440, C_00833708_UpdateSlotStates, "auto");

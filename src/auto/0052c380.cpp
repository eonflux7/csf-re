#include <cstdint>

#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

// Original callees, bound by address.
CSF_FUNC(int, __cdecl, CrtSprintf, 0x007AF9BB, (char* buffer, const char* format, ...));
CSF_FUNC(void*, __cdecl, GetGlobalState, 0x0050D190, ());
CSF_FUNC(int, __fastcall, GlobalState_ApplyFlag, 0x0050DC80, (void* self, void* edx, char flag));
CSF_FUNC(int, __fastcall, MenuProgressInput_UpdateChildSlot, 0x0045A980, (void* self, void* edx));

// Fills four indexed child slots ("0".."3") of the screen, registers each child's
// component into the screen's 16-byte registration table, notifies the event target,
// then runs the sub-object update routine on three secondary entries.
static int __fastcall C_008414e8_BindFourIndexedChildren(C_008414e8* self, void* /*edx*/)
{
    unsigned int index = 0;
    uint8_t* registrationSlot = reinterpret_cast<uint8_t*>(&self->field_0150);

    do
    {
        char indexLabel[4];
        CrtSprintf(indexLabel, CSF_STR(0x0083090C, "%d"), static_cast<int>(index));

        // Lookup owner is re-read every iteration, as the original does.
        void* lookupOwner = reinterpret_cast<void*>(self->field_001c);
        uint32_t* indexedChildSlot =
            reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(&self->field_0024) + 4 * index);

        uint32_t indexedChild =
            ((uint32_t(__fastcall*)(void*, void*, char*))CSF_AT(void**, lookupOwner, 0)[63])(
                lookupOwner, nullptr, indexLabel);
        *indexedChildSlot = indexedChild;

        if (indexedChild != 0)
        {
            void* childObject = reinterpret_cast<void*>(indexedChild);
            void* childComponent =
                ((void*(__fastcall*)(void*, void*))CSF_AT(void**, childObject, 0)[68])(childObject, nullptr);
            ((void(__fastcall*)(void*, void*, uint8_t*))CSF_AT(void**, childComponent, 0)[24])(
                childComponent, nullptr, registrationSlot);
        }

        ++index;
        registrationSlot += 0x10;
    } while (index < 4);

    void* eventTarget = reinterpret_cast<void*>(self->field_0014);
    ((void(__fastcall*)(void*, void*, C_008414e8*, int))CSF_AT(void**, eventTarget, 0)[24])(
        eventTarget, nullptr, self, 0);

    void* globalState = GetGlobalState();
    GlobalState_ApplyFlag(globalState, nullptr, 0);

    uint8_t* subObject = reinterpret_cast<uint8_t*>(self) + 0x44;
    int lastResult = 0;
    int remaining = 3;
    do
    {
        lastResult = MenuProgressInput_UpdateChildSlot(subObject, nullptr);
        subObject += 0x4C;
        --remaining;
    } while (remaining != 0);

    return lastResult;
}

HOOK(0x0052C380, C_008414e8_BindFourIndexedChildren, "auto");

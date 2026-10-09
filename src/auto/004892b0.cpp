#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, CharActionFsm_InsertHandle, 0x00481A30,
         (void* self, void* edx, char* Src, unsigned int a3, uint32_t* a4));
CSF_FUNC(unsigned int, __fastcall, CharActionFsm_AppendHandle, 0x00481C90,
         (void* self, void* edx, uint32_t* a3));

static void __fastcall CharActionFsm_InsertActionByPriority(
    void* self, void* /*edx*/, C_00862490* action)
{
    if (!action)
        return;

    if (CSF_AT(int32_t,
               reinterpret_cast<void*>(Handle_GetValuePointer(action, nullptr)),
               0x84) != -1)
    {
        const uint32_t begin = CSF_AT(uint32_t, self, 0x18);
        const int32_t count = begin
            ? (static_cast<int32_t>(CSF_AT(uint32_t, self, 0x1C) - begin) >> 2)
            : 0;

        for (int32_t index = 0; index < count; ++index)
        {
            auto* entries = CSF_AT(C_00862490**, self, 0x18);
            const int32_t existingPriority = CSF_AT(
                int32_t,
                reinterpret_cast<void*>(
                    Handle_GetValuePointer(entries[index], nullptr)),
                0x84);

            const int32_t actionPriority = CSF_AT(
                int32_t,
                reinterpret_cast<void*>(
                    Handle_GetValuePointer(action, nullptr)),
                0x84);

            if (actionPriority < existingPriority)
            {
                CharActionFsm_InsertHandle(
                    static_cast<uint8_t*>(self) + 0x14, nullptr,
                    CSF_AT(char*, self, 0x18) + 4 * index, 1,
                    reinterpret_cast<uint32_t*>(&action));

                const int32_t currentIndex = CSF_AT(int32_t, self, 0x24);
                if (currentIndex != -1 && currentIndex >= index)
                {
                    CSF_AT(int32_t, self, 0x24) = static_cast<int32_t>(
                        static_cast<uint32_t>(currentIndex) + 1);
                }
                return;
            }
        }
    }

    CharActionFsm_AppendHandle(
        static_cast<uint8_t*>(self) + 0x14, nullptr,
        reinterpret_cast<uint32_t*>(&action));
}

HOOK(0x004892B0, CharActionFsm_InsertActionByPriority, "auto");

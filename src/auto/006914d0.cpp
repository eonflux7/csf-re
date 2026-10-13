#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char*, __fastcall, CoreMemory_ResolveDescribedReference, 0x00691D90, (void* self, void* edx, int a2, unsigned int a3, int a4));
CSF_FUNC(int, __fastcall, CoreMemory_FindDescribedObject, 0x00692150, (void* self, void* edx, int a2, unsigned int a3, int a4));
CSF_FUNC(int, __fastcall, AiAttackTasks_ApplySlotState, 0x00692200, (void* self, void* edx, unsigned int a2, int a3, int a4, char a5));
CSF_FUNC(uint32_t*, __fastcall, CoreMemory_ReadDescribedValue, 0x00692290, (void* self, void* edx, uint32_t* a2, unsigned int a3, int16_t a4, int a5));

static char __cdecl CoreMemory_ProcessDescribedValue(
    void* object, uint8_t* state, S_00691860* output, void* description)
{
    if (CSF_AT(uint8_t, description, 0x01))
    {
        CSF_AT(uint32_t, output, 0x00) = 0;
        return static_cast<char>(reinterpret_cast<uintptr_t>(output));
    }

    char result = static_cast<char>(CSF_AT(uint8_t, state, 0x15));
    if (!result)
    {
        CoreMemory_ReadDescribedValue(
            CSF_AT(void*, state, 0x08), nullptr,
            reinterpret_cast<uint32_t*>(output),
            CSF_AT(unsigned int, description, 0x04),
            static_cast<int16_t>(CSF_AT(uint16_t, description, 0x0C)),
            reinterpret_cast<int>(state));

        result = static_cast<char>(CSF_AT(uint8_t, state, 0x16));
        if (result)
        {
            result = static_cast<char>(reinterpret_cast<uintptr_t>(
                CoreMemory_ResolveDescribedReference(
                    CSF_AT(void*, state, 0x08), nullptr,
                    reinterpret_cast<int>(output),
                    CSF_AT(unsigned int, description, 0x04),
                    reinterpret_cast<int>(state))));
        }
    }

    if (!CSF_AT(uint8_t, description, 0x00))
    {
        void* target;
        int reference = CSF_AT(int, description, 0x08);
        if (reference)
        {
            int resolved = CoreMemory_FindDescribedObject(
                CSF_AT(void*, state, 0x08), nullptr, reference,
                CSF_AT(unsigned int, description, 0x04),
                reinterpret_cast<int>(state));
            result = static_cast<char>(resolved);
            if (!resolved)
            {
                CSF_AT(uint8_t, state, 0x00) = 1;
                return result;
            }

            target = reinterpret_cast<uint8_t*>(resolved)
                   + 4 * CSF_AT(int16_t, description, 0x0E);
        }
        else
        {
            target = object;
            int slotState =
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0x00)[0x14 / 4])(
                        object, nullptr);

            result = static_cast<char>(AiAttackTasks_ApplySlotState(
                CSF_AT(void*, state, 0x08), nullptr,
                CSF_AT(unsigned int, description, 0x04),
                slotState, reinterpret_cast<int>(state), 1));
        }

        if (!CSF_AT(uint8_t, state, 0x00))
        {
            result = static_cast<char>(
                ((int(__fastcall*)(void*, void*, uint8_t*))
                    CSF_AT(void**, target, 0x00)[0x08 / 4])(
                        target, nullptr, state));
        }
    }

    return result;
}

HOOK(0x006914D0, CoreMemory_ProcessDescribedValue, "auto");

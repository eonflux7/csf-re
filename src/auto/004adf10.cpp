#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __cdecl, MissionScript_ProcessDirectValue, 0x006913D0, (int* a2, int a3, uint16_t* a4));
CSF_FUNC(char, __cdecl, CoreMemory_ProcessDescribedValue, 0x006914D0, (void* object, uint8_t* state, S_00691860* output, void* description));

static char __cdecl Archive_SerializeObjectReference(S_00691860* output, bool* state)
{
    int* resolvedObject = nullptr;

    if (CSF_AT(uint8_t, state, 0x14))
    {
        void* object = CSF_AT(void*, output, 0x0);
        if (object)
        {
            resolvedObject =
                ((int* (__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0x0)[4 / 4])(object, nullptr);

            if (!resolvedObject)
            {
                CSF_AT(uint8_t, state, 0x0) = 1;
                return 0;
            }
        }

        return MissionScript_ProcessDirectValue(
            resolvedObject,
            static_cast<int>(reinterpret_cast<uintptr_t>(state)),
            reinterpret_cast<uint16_t*>(output));
    }

    alignas(8) uint8_t descriptionBytes[0x10];

    bool failed = false;
    if (!CSF_AT(uint8_t, state, 0x0))
    {
        void* archive = CSF_AT(void*, state, 0x4);
        failed =
            !((bool (__fastcall*)(void*, void*, void*, bool*))
                CSF_AT(void**, archive, 0x0)[0x88 / 4])(
                    archive, nullptr, descriptionBytes, state);
    }
    CSF_AT(uint8_t, state, 0x0) = failed;

    if (!descriptionBytes[1] && CSF_AT(uint8_t, state, 0x15))
    {
        void* object = CSF_AT(void*, output, 0x0);
        resolvedObject =
            ((int* (__fastcall*)(void*, void*))
                CSF_AT(void**, object, 0x0)[4 / 4])(object, nullptr);

        if (!resolvedObject)
        {
            CSF_AT(uint8_t, state, 0x0) = 1;
            return 0;
        }
    }

    return CoreMemory_ProcessDescribedValue(
        resolvedObject,
        reinterpret_cast<uint8_t*>(state),
        output,
        descriptionBytes);
}

HOOK(0x004ADF10, Archive_SerializeObjectReference, "auto");

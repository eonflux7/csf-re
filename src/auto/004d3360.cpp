#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Handle_GetValuePointer, 0x00476060, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_0083ba40_RefreshSelection, 0x004D31E0, (void* self, void* edx));

static uint32_t __fastcall C_0083ba40_UpdateSelectionsFromState(
    C_0083ba40* self, void* /*edx*/, uint32_t stateAddress)
{
    void* state = reinterpret_cast<void*>(stateAddress);
    uint32_t result = stateAddress;
    const uint32_t secondaryId = CSF_AT(uint8_t, state, 0x44F);
    const uint32_t primaryId = CSF_AT(uint8_t, state, 0x440);
    const bool primaryChanged = primaryId != self->field_0088;

    self->field_0084 = 0;

    if (primaryChanged && CSF_AT(uint8_t, state, 0x443))
    {
        int selectedIndex = -1;
        for (uint32_t index = 0; ; ++index)
        {
            void* collection = reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x44));
            const uint32_t count = static_cast<uint32_t>(
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, collection, 0)[0xA0 / 4])(collection, nullptr));

            if (index >= count || selectedIndex != -1)
                break;

            collection = reinterpret_cast<void*>(
                CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x44));
            C_00862490* handle =
                ((C_00862490*(__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, collection, 0)[0xC8 / 4])(
                        collection, nullptr, index);

            if (handle)
            {
                void* value = reinterpret_cast<void*>(
                    Handle_GetValuePointer(handle, nullptr));
                if (CSF_AT(uint32_t, value, 0) == primaryId)
                    selectedIndex = static_cast<int>(index);
            }
        }

        if (selectedIndex == -1)
            selectedIndex = 0;

        void* collection = reinterpret_cast<void*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x44));
        ((void(__fastcall*)(void*, void*, int))
            CSF_AT(void**, collection, 0)[0xF8 / 4])(
                collection, nullptr, selectedIndex);

        self->field_0088 = primaryId;
        self->field_0084 = 1;
        result = static_cast<uint32_t>(
            C_0083ba40_RefreshSelection(self, nullptr));
    }

    if (secondaryId != self->field_008c)
    {
        void* collection = reinterpret_cast<void*>(
            CSF_AT(uint32_t, reinterpret_cast<void*>(self->field_0014), 0x44));

        for (uint32_t index = 0; ; ++index)
        {
            result = static_cast<uint32_t>(
                ((int(__fastcall*)(void*, void*))
                    CSF_AT(void**, collection, 0)[0xA0 / 4])(collection, nullptr));
            if (index >= result)
                break;

            C_00862490* handle =
                ((C_00862490*(__fastcall*)(void*, void*, uint32_t))
                    CSF_AT(void**, collection, 0)[0xC8 / 4])(
                        collection, nullptr, index);

            if (handle)
            {
                void* value = reinterpret_cast<void*>(
                    Handle_GetValuePointer(handle, nullptr));
                if (CSF_AT(uint32_t, value, 0) == secondaryId)
                {
                    self->field_0090 = index;
                    self->field_008c = secondaryId;
                    return secondaryId;
                }
            }
        }
    }

    return result;
}

HOOK(0x004D3360, C_0083ba40_UpdateSelectionsFromState, "auto");

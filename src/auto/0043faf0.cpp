#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));

static int __stdcall Campaign_SerializeRecordBase(C_0082d410* record, int stateAddress)
{
    void* state = reinterpret_cast<void*>(stateAddress);

    if (CSF_AT(uint8_t, state, 0x14))
    {
        uint32_t* text;
        if (CSF_AT(uint32_t, record, 0x18) < 0x10)
            text = &record->field_0004;
        else
            text = reinterpret_cast<uint32_t*>(record->field_0004);

        int result = 0;
        if (!CSF_AT(uint8_t, state, 0x0))
        {
            void* serializer = CSF_AT(void*, state, 0x4);
            if (!reinterpret_cast<bool(__fastcall*)(void*, void*, uint32_t*, int)>(
                    CSF_AT(void**, serializer, 0)[0x68 / 4])(
                    serializer, nullptr, text, stateAddress))
            {
                result = 1;
            }
        }
        CSF_AT(uint8_t, state, 0x0) = static_cast<uint8_t>(result);
        return result;
    }

    alignas(8) uint8_t textBytes[0x100]{};
    char* text = reinterpret_cast<char*>(textBytes);
    int result = 0;

    if (!CSF_AT(uint8_t, state, 0x0))
    {
        void* serializer = CSF_AT(void*, state, 0x4);
        if (!reinterpret_cast<bool(__fastcall*)(void*, void*, char*, int, int)>(
                CSF_AT(void**, serializer, 0)[0x70 / 4])(
                serializer, nullptr, text, 256, stateAddress))
        {
            result = 1;
        }
    }

    CSF_AT(uint8_t, state, 0x0) = static_cast<uint8_t>(result);
    if (!result)
    {
        return reinterpret_cast<int>(
            AssignStringRange_429B60(
                record, nullptr, text,
                static_cast<unsigned int>(std::strlen(text))));
    }
    return result;
}

HOOK(0x0043FAF0, Campaign_SerializeRecordBase, "auto");

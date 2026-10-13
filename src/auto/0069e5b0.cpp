#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CoreTokenizer_ClassifyCurrentToken, 0x0069E480, (void* self, void* edx));

static bool __fastcall CoreTokenizer_TestScanCondition(S_0069e480* self, void* /*edx*/)
{
    bool result = false;

    switch (self->field_0020)
    {
    case 0u:
        result = CoreTokenizer_ClassifyCurrentToken(self, nullptr) != 0;
        break;
    case 1u:
        result = CoreTokenizer_ClassifyCurrentToken(self, nullptr) == 1;
        break;
    case 2u:
        result = self->field_0028 != 0;
        break;
    case 3u:
        result = CoreTokenizer_ClassifyCurrentToken(self, nullptr) != 3;
        break;
    case 4u:
    {
        const uint8_t* buffer = reinterpret_cast<const uint8_t*>(self->field_0008);
        uint8_t ch = buffer[self->field_0018];
        result = (ch == 10 || ch == 13);
        break;
    }
    case 5u:
    {
        uint32_t index = self->field_0018;
        if (index != self->field_000c - 1)
        {
            const uint8_t* buffer = reinterpret_cast<const uint8_t*>(self->field_0008);
            const uint8_t* at = buffer + index;
            result = (at[0] == 42 && at[1] == 47);
        }
        else
        {
            result = false;
        }
        break;
    }
    default:
        break;
    }

    return result;
}

HOOK(0x0069E5B0, CoreTokenizer_TestScanCondition, "auto");

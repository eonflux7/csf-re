#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint8_t, __fastcall, S_0069e480_ClassifyTokenStart, 0x0069E4E0, (void* self, void* edx));
CSF_FUNC(bool, __fastcall, S_0069e480_IsTokenEnd, 0x0069E5B0, (void* self, void* edx));

static uint8_t __fastcall S_0069e480_Tokenize(S_0069e480* self, void* /*edx*/)
{
    self->field_0024 = 0;
    self->field_0028 = 0;
    self->field_0018 = 0;

    for (;;)
    {
        S_0069e480_ClassifyTokenStart(self, nullptr);
        const int32_t tokenType = static_cast<int32_t>(self->field_0020);

        if (tokenType == 1)
            self->field_0018 += 1;
        else if (tokenType > 3 && tokenType <= 5)
            self->field_0018 += 2;

        uint32_t position = self->field_0018;
        self->field_0028 = 0;
        const uint32_t inputLength = self->field_000c;
        self->field_0024 = position;

        if (position < inputLength)
        {
            do
            {
                if (S_0069e480_IsTokenEnd(self, nullptr))
                    break;

                position = self->field_0018 + 1;
                const uint32_t tokenLength = self->field_0028 + 1;
                self->field_0018 = position;
                self->field_0028 = tokenLength;
            }
            while (position < self->field_000c);
        }

        position = self->field_0018;
        if (position < inputLength)
        {
            if (tokenType == 1)
            {
                self->field_0018 = ++position;
            }
            else if (tokenType == 5)
            {
                self->field_0018 = ++position;
                if (position < inputLength)
                    self->field_0018 = ++position;
            }
        }

        uint8_t result = static_cast<uint8_t>(position);
        if (self->field_0018 >= inputLength)
        {
            result = static_cast<uint8_t>(static_cast<uint32_t>(tokenType) - 1);
            if (tokenType == 1)
            {
                self->field_0014 = reinterpret_cast<uint32_t>(
                    CSF_STR(0x0085EB3C, "Cadena truncada en el final del fichero"));
                self->field_0010 = 1;
            }
            else
            {
                result = static_cast<uint8_t>(static_cast<uint32_t>(tokenType) - 5);
                if (tokenType == 5)
                {
                    self->field_0014 = reinterpret_cast<uint32_t>(
                        CSF_STR(0x0085EB64, "Comentario truncado en el final del fichero"));
                    self->field_0010 = 1;
                }
            }
        }

        if (tokenType > 0 && tokenType <= 3)
        {
            result = self->field_001c;
            if (result)
            {
                reinterpret_cast<uint32_t*>(self->field_0000)
                    [2 * self->field_0004 + 1] =
                        self->field_0024 + self->field_0008;

                const uint32_t tokenLength = self->field_0028;
                result = static_cast<uint8_t>(tokenLength);
                reinterpret_cast<uint32_t*>(self->field_0000)
                    [2 * self->field_0004] = tokenLength;
            }
            ++self->field_0004;
        }

        if (self->field_0018 >= self->field_000c)
            return result;

        result = self->field_0010;
        if (result)
            return result;
    }
}

HOOK(0x0069E640, S_0069e480_Tokenize, "auto");

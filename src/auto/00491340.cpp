#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, ByteBuffer_Grow, 0x0048EA50,
         (void* self, void* edx, int a2));

static unsigned int __stdcall ByteBuffer_Serialize(void* buffer, char* context)
{
    const char savedElementMode = CSF_AT(char, context, 0x15);
    const bool inPlace = CSF_AT(char, context, 0x14) != 0;
    const char initialError = CSF_AT(char, context, 0x00);

    uint32_t count = 0;
    if (inPlace)
        count = CSF_AT(uint32_t, buffer, 0x08);

    unsigned int result = 0;
    if (!initialError)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        const bool success =
            ((bool(__fastcall*)(void*, void*, uint32_t*, char*))
                 CSF_AT(void**, serializer, 0x00)[0x78 / 4])(
                serializer, nullptr, &count, context);
        result = !success;
    }
    CSF_AT(char, context, 0x00) = static_cast<char>(result);
    if (result)
    {
        CSF_AT(char, context, 0x00) = 1;
        return result;
    }

    if (inPlace)
    {
        result = count;
        for (uint32_t index = 0; index < count; ++index)
        {
            const uint32_t element = CSF_AT(uint32_t, buffer, 0x00) + index;
            const bool hasError = CSF_AT(char, context, 0x00) != 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!hasError)
            {
                CSF_AT(char, context, 0x15) = savedElementMode;
                void* serializer = CSF_AT(void*, context, 0x04);
                CSF_AT(char, context, 0x16) = 0;
                const int status =
                    ((int(__fastcall*)(void*, void*, uint32_t, char*))
                         CSF_AT(void**, serializer, 0x00)[0x44 / 4])(
                        serializer, nullptr, element, context);
                CSF_AT(char, context, 0x00) = status == -1;
            }
            result = count;
        }
    }
    else if (savedElementMode)
    {
        result = count;
        for (uint32_t index = 0; index < count; ++index)
        {
            const uint32_t element = CSF_AT(uint32_t, buffer, 0x00) + index;
            const bool hasError = CSF_AT(char, context, 0x00) != 0;
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (!hasError)
            {
                void* serializer = CSF_AT(void*, context, 0x04);
                CSF_AT(char, context, 0x15) = savedElementMode;
                CSF_AT(char, context, 0x16) = 0;
                const int status =
                    ((int(__fastcall*)(void*, void*, uint32_t, char*))
                         CSF_AT(void**, serializer, 0x00)[0x44 / 4])(
                        serializer, nullptr, element, context);
                CSF_AT(char, context, 0x00) = status == -1;
            }
            result = count;
        }
    }
    else
    {
        result = CSF_AT(uint32_t, buffer, 0x04);
        uint32_t requiredCount = count;
        CSF_AT(uint32_t, buffer, 0x08) = 0;
        if (requiredCount > result)
        {
            result = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(
                ByteBuffer_Grow(buffer, nullptr,
                                static_cast<int>(requiredCount - result))));
            requiredCount = count;
        }

        uint32_t index = 0;
        if (requiredCount)
        {
            do
            {
                if (CSF_AT(uint32_t, buffer, 0x08) >=
                    CSF_AT(uint32_t, buffer, 0x04))
                {
                    ByteBuffer_Grow(
                        buffer, nullptr,
                        static_cast<int>(CSF_AT(uint32_t, buffer, 0x0C)));
                }

                const uint32_t size = CSF_AT(uint32_t, buffer, 0x08);
                auto* appendedByte = reinterpret_cast<uint8_t*>(
                    static_cast<uintptr_t>(
                        CSF_AT(uint32_t, buffer, 0x00) + size));
                CSF_AT(uint32_t, buffer, 0x08) = size + 1;
                *appendedByte = 0;

                const uint32_t element =
                    CSF_AT(uint32_t, buffer, 0x00) + index;
                const bool hasError = CSF_AT(char, context, 0x00) != 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (!hasError)
                {
                    void* serializer = CSF_AT(void*, context, 0x04);
                    CSF_AT(char, context, 0x15) = 0;
                    CSF_AT(char, context, 0x16) = 0;
                    const int status =
                        ((int(__fastcall*)(void*, void*, uint32_t, char*))
                             CSF_AT(void**, serializer, 0x00)[0x44 / 4])(
                            serializer, nullptr, element, context);
                    CSF_AT(char, context, 0x00) = status == -1;
                }
                result = ++index;
            } while (index < count);
        }
    }

    char error = 0;
    if (CSF_AT(char, context, 0x00))
    {
        error = 1;
    }
    else
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        result =
            ((int(__fastcall*)(void*, void*, char*))
                 CSF_AT(void**, serializer, 0x00)[0x7C / 4])(
                serializer, nullptr, context);
        if (static_cast<uint8_t>(result) == 0)
            error = 1;
    }
    CSF_AT(char, context, 0x00) = error;
    return result;
}

HOOK(0x00491340, ByteBuffer_Serialize, "auto");

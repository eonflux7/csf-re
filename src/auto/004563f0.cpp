#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, StringArray_Clear, 0x00453860, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, StringArray_Reserve, 0x00455C50, (void* self, void* edx, unsigned int a2));
CSF_FUNC(void*, __fastcall, StringArray_Append, 0x00456360, (void* self, void* edx, int a3));
CSF_FUNC(int, __cdecl, Archive_SerializeString, 0x00496080, (int a1, void* a2));
CSF_FUNC(uint32_t*, __fastcall, C_00831278_Initialize, 0x004A0FC0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, C_00831278_Destroy, 0x004A0FF0, (void* self, void* edx));

static int __stdcall Archive_SerializeStringArray(S_00455ed0* strings, int archiveAddress)
{
    void* archive = reinterpret_cast<void*>(archiveAddress);
    const uint8_t writing = CSF_AT(uint8_t, archive, 0x14);
    const uint8_t inPlace = CSF_AT(uint8_t, archive, 0x15);
    unsigned int count = 0;

    if (writing)
    {
        if (strings->field_0004)
        {
            count = static_cast<int32_t>(
                strings->field_0008 - strings->field_0004) / 48;
        }

        unsigned int loopCount = count;
        bool failed = false;
        if (CSF_AT(uint8_t, archive, 0x00) == 0)
        {
            void* stream = CSF_AT(void*, archive, 0x04);
            failed = !reinterpret_cast<bool (__fastcall*)(
                void*, void*, unsigned int*, int)>(
                    CSF_AT(void**, stream, 0x00)[0x78 / 4])(
                        stream, nullptr, &count, archiveAddress);
            loopCount = count;
        }
        CSF_AT(uint8_t, archive, 0x00) = failed;
        if (failed)
        {
            CSF_AT(uint8_t, archive, 0x00) = 1;
            return 1;
        }

        unsigned int index = 0;
        uint32_t offset = 0;
        while (index < loopCount)
        {
            const int element = static_cast<int>(strings->field_0004 + offset);
            const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
            CSF_AT(uint32_t, archive, 0x1C) = 0;
            if (ready)
            {
                CSF_AT(uint8_t, archive, 0x15) = inPlace;
                CSF_AT(uint8_t, archive, 0x16) = 0;
                Archive_SerializeString(element, archive);
                loopCount = count;
            }
            ++index;
            offset += 48;
        }
    }
    else
    {
        bool failed = false;
        if (CSF_AT(uint8_t, archive, 0x00) == 0)
        {
            void* stream = CSF_AT(void*, archive, 0x04);
            failed = !reinterpret_cast<bool (__fastcall*)(
                void*, void*, unsigned int*, int)>(
                    CSF_AT(void**, stream, 0x00)[0x78 / 4])(
                        stream, nullptr, &count, archiveAddress);
        }
        CSF_AT(uint8_t, archive, 0x00) = failed;
        if (failed)
        {
            CSF_AT(uint8_t, archive, 0x00) = 1;
            return 1;
        }

        if (inPlace)
        {
            unsigned int index = 0;
            uint32_t offset = 0;
            while (index < count)
            {
                const int element = static_cast<int>(strings->field_0004 + offset);
                const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
                CSF_AT(uint32_t, archive, 0x1C) = 0;
                if (ready)
                {
                    CSF_AT(uint8_t, archive, 0x15) = inPlace;
                    CSF_AT(uint8_t, archive, 0x16) = 0;
                    Archive_SerializeString(element, archive);
                }
                ++index;
                offset += 48;
            }
        }
        else
        {
            StringArray_Clear(strings, nullptr);
            StringArray_Reserve(strings, nullptr, count);

            unsigned int index = 0;
            uint32_t offset = 0;
            while (index < count)
            {
                C_00831278 temporary;
                C_00831278_Initialize(&temporary, nullptr);
                StringArray_Append(strings, nullptr, reinterpret_cast<int>(&temporary));

                const int element = static_cast<int>(strings->field_0004 + offset);
                const bool ready = CSF_AT(uint8_t, archive, 0x00) == 0;
                CSF_AT(uint32_t, archive, 0x1C) = 0;
                if (ready)
                {
                    CSF_AT(uint8_t, archive, 0x15) = 0;
                    CSF_AT(uint8_t, archive, 0x16) = 0;
                    Archive_SerializeString(element, archive);
                }

                C_00831278_Destroy(&temporary, nullptr);
                ++index;
                offset += 48;
            }
        }
    }

    if (CSF_AT(uint8_t, archive, 0x00) == 0)
    {
        void* stream = CSF_AT(void*, archive, 0x04);
        if (reinterpret_cast<bool (__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, stream, 0x00)[0x7C / 4])(
                    stream, nullptr, archiveAddress))
        {
            CSF_AT(uint8_t, archive, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, archive, 0x00) = 1;
    return 1;
}

HOOK(0x004563F0, Archive_SerializeStringArray, "auto");

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __stdcall, sub_4692B0, 0x004692B0, (int a1, char* a2));
CSF_FUNC(unsigned int, __fastcall, sub_469880, 0x00469880, (void* self, void* edx, unsigned int a2));
CSF_FUNC(unsigned int, __fastcall, sub_46A3B0, 0x0046A3B0, (void* self, void* edx, uint32_t* a3));
CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790, (int* self, void* edx));

static char __stdcall MissionScript_ProcessEntryArray(S_0047ea40* entries, int contextAddress)
{
    char* context = reinterpret_cast<char*>(contextAddress);
    const char savedMode = CSF_AT(char, context, 0x15);
    unsigned int count = 0;
    char result = 0;
    char failed = 0;

    if (CSF_AT(char, context, 0x14))
    {
        const uint32_t begin = entries->field_0004;
        unsigned int limit = begin
            ? static_cast<unsigned int>(
                  static_cast<int32_t>(entries->field_0008 - begin) >> 3)
            : 0;
        count = limit;

        if (!CSF_AT(char, context, 0))
        {
            void* handler = CSF_AT(void*, context, 0x4);
            const char accepted =
                ((char(__fastcall*)(void*, void*, unsigned int*, int))
                     CSF_AT(void**, handler, 0)[0x78 / 4])(
                    handler, nullptr, &count, contextAddress);
            limit = count;
            result = !accepted;
        }
        else
        {
            result = 0;
        }

        CSF_AT(char, context, 0) = result;
        if (result)
            goto Failed;

        for (unsigned int index = 0; index < limit; ++index)
        {
            const uint32_t entryAddress = entries->field_0004 + 8 * index;
            const bool canProcess = CSF_AT(char, context, 0) == 0;
            result = static_cast<char>(entryAddress);
            CSF_AT(uint32_t, context, 0x1C) = 0;
            if (canProcess)
            {
                CSF_AT(char, context, 0x15) = savedMode;
                CSF_AT(char, context, 0x16) = 0;
                result = sub_4692B0(static_cast<int>(entryAddress), context);
                limit = count;
            }
        }
    }
    else
    {
        if (!CSF_AT(char, context, 0))
        {
            void* handler = CSF_AT(void*, context, 0x4);
            result = !((char(__fastcall*)(void*, void*, unsigned int*, int))
                           CSF_AT(void**, handler, 0)[0x78 / 4])(
                handler, nullptr, &count, contextAddress);
        }
        else
        {
            result = 0;
        }

        CSF_AT(char, context, 0) = result;
        if (result)
            goto Failed;

        if (savedMode)
        {
            result = static_cast<char>(count);
            for (unsigned int index = 0; index < count; ++index)
            {
                const uint32_t entryAddress = entries->field_0004 + 8 * index;
                const bool canProcess = CSF_AT(char, context, 0) == 0;
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canProcess)
                {
                    CSF_AT(char, context, 0x15) = savedMode;
                    CSF_AT(char, context, 0x16) = 0;
                    sub_4692B0(static_cast<int>(entryAddress), context);
                }
                result = static_cast<char>(count);
            }
        }
        else
        {
            DestroyPointerStorage(reinterpret_cast<int*>(entries), nullptr);
            result = static_cast<char>(sub_469880(entries, nullptr, count));

            unsigned int index = 0;
            if (count)
            {
                uint32_t emptyEntry[2] = {0, 0};
                do
                {
                    sub_46A3B0(entries, nullptr, emptyEntry);
                    const uint32_t entryAddress = entries->field_0004 + 8 * index;
                    const bool canProcess = CSF_AT(char, context, 0) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (canProcess)
                    {
                        CSF_AT(char, context, 0x15) = 0;
                        CSF_AT(char, context, 0x16) = 0;
                        sub_4692B0(static_cast<int>(entryAddress), context);
                    }
                    ++index;
                    result = static_cast<char>(index);
                } while (index < count);
            }
        }
    }

    if (CSF_AT(char, context, 0))
        goto Failed;

    {
        void* handler = CSF_AT(void*, context, 0x4);
        result = ((char(__fastcall*)(void*, void*, int))
                      CSF_AT(void**, handler, 0)[0x7C / 4])(
            handler, nullptr, contextAddress);
    }
    if (result)
        goto Finish;

Failed:
    failed = 1;
Finish:
    CSF_AT(char, context, 0) = failed;
    return result;
}

HOOK(0x0046A610, MissionScript_ProcessEntryArray, "auto");

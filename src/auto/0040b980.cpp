#include <cstdint>
#include "globals.h"
#include "hook.h"

CSF_FUNC(int, __fastcall, EntryArray_Grow, 0x0040AD70,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, EntryArray_SerializeEntry, 0x0040B780,
         (void* self, void* edx, int a2));

static int __stdcall EntryArray_Serialize(uint32_t* array, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint8_t existingMode = CSF_AT(uint8_t, context, 0x14);
    const uint8_t savedFlag = CSF_AT(uint8_t, context, 0x15);

    uint32_t entryCount = 0;
    if (existingMode)
        entryCount = CSF_AT(uint32_t, array, 0x8);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0x0) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x4);
        failed = !reinterpret_cast<bool(__fastcall*)(
            void*, void*, uint32_t*, int)>(
                CSF_AT(void**, serializer, 0x0)[120 / 4])(
                    serializer, nullptr, &entryCount, contextAddress);
    }
    CSF_AT(uint8_t, context, 0x0) = failed;

    if (!failed)
    {
        if (existingMode || savedFlag)
        {
            uint32_t index = 0;
            uint32_t byteOffset = 0;
            if (entryCount)
            {
                do
                {
                    void* entry =
                        CSF_AT(uint8_t*, array, 0x0) + byteOffset;
                    const bool active =
                        CSF_AT(uint8_t, context, 0x0) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (active)
                    {
                        CSF_AT(uint8_t, context, 0x15) = savedFlag;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        EntryArray_SerializeEntry(
                            entry, nullptr, contextAddress);
                    }
                    ++index;
                    byteOffset += 16;
                } while (index < entryCount);
            }
        }
        else
        {
            const uint32_t capacity = CSF_AT(uint32_t, array, 0x4);
            CSF_AT(uint32_t, array, 0x8) = 0;
            if (entryCount > capacity)
            {
                EntryArray_Grow(
                    array, nullptr, static_cast<int>(entryCount - capacity));
            }

            uint32_t index = 0;
            uint32_t byteOffset = 0;
            if (entryCount)
            {
                do
                {
                    if (CSF_AT(uint32_t, array, 0x8) >=
                        CSF_AT(uint32_t, array, 0x4))
                    {
                        EntryArray_Grow(
                            array, nullptr, CSF_AT(int, array, 0xC));
                    }

                    void* addedEntry =
                        CSF_AT(uint8_t*, array, 0x0) +
                        16 * CSF_AT(uint32_t, array, 0x8)++;
                    CSF_AT(uint32_t, addedEntry, 0x0) = 0;
                    CSF_AT(uint32_t, addedEntry, 0x4) = 0;
                    CSF_AT(uint32_t, addedEntry, 0x8) = 0;
                    CSF_AT(int32_t, addedEntry, 0xC) = -1082130432;

                    void* entry =
                        CSF_AT(uint8_t*, array, 0x0) + byteOffset;
                    const bool active =
                        CSF_AT(uint8_t, context, 0x0) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (active)
                    {
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        EntryArray_SerializeEntry(
                            entry, nullptr, contextAddress);
                    }
                    byteOffset += 16;
                    ++index;
                } while (index < entryCount);
            }
        }

        if (CSF_AT(uint8_t, context, 0x0) == 0)
        {
            void* serializer = CSF_AT(void*, context, 0x4);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, serializer, 0x0)[124 / 4])(
                        serializer, nullptr, contextAddress))
            {
                CSF_AT(uint8_t, context, 0x0) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, context, 0x0) = 1;
    return 1;
}

HOOK(0x0040B980, EntryArray_Serialize, "auto");

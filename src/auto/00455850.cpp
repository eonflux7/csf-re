#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, Storage_SerializeElement, 0x004536C0,
         (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __fastcall, Storage_ReserveElements, 0x004549E0,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(uint32_t*, __fastcall, Storage_AppendElement, 0x004553E0,
         (void* self, void* edx, uint32_t* a3));
CSF_FUNC(int, __fastcall, DestroyPointerStorage, 0x00558790,
         (int* self, void* edx));

static int __stdcall Storage_SerializeElements(S_0047ea40* storage, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);
    const uint8_t writing = CSF_AT(uint8_t, context, 0x14);
    const uint8_t existingElements = CSF_AT(uint8_t, context, 0x15);
    unsigned int count = 0;
    bool failed = false;

    if (writing)
    {
        const uint32_t begin = storage->field_0004;
        unsigned int loopCount = begin;
        if (begin)
            loopCount = static_cast<int32_t>(storage->field_0008 - begin) / 20;
        count = loopCount;

        if (CSF_AT(uint8_t, context, 0x00) == 0)
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, unsigned int*, int)>(
                CSF_AT(void**, serializer, 0x00)[30])(
                    serializer, nullptr, &count, contextAddress);
            loopCount = count;
        }
        CSF_AT(uint8_t, context, 0x00) = failed;

        if (!failed)
        {
            uint32_t index = 0;
            uint32_t offset = 0;
            if (loopCount)
            {
                do
                {
                    void* element = reinterpret_cast<void*>(storage->field_0004 + offset);
                    const bool canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (canSerialize)
                    {
                        CSF_AT(uint8_t, context, 0x15) = existingElements;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        Storage_SerializeElement(element, nullptr, contextAddress);
                        loopCount = count;
                    }
                    ++index;
                    offset += 20;
                } while (index < loopCount);
            }
        }
    }
    else
    {
        if (CSF_AT(uint8_t, context, 0x00) == 0)
        {
            void* serializer = CSF_AT(void*, context, 0x04);
            failed = !reinterpret_cast<bool(__fastcall*)(void*, void*, unsigned int*, int)>(
                CSF_AT(void**, serializer, 0x00)[30])(
                    serializer, nullptr, &count, contextAddress);
        }
        CSF_AT(uint8_t, context, 0x00) = failed;

        if (!failed)
        {
            if (existingElements)
            {
                uint32_t index = 0;
                uint32_t offset = 0;
                if (count)
                {
                    do
                    {
                        void* element = reinterpret_cast<void*>(storage->field_0004 + offset);
                        const bool canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
                        CSF_AT(uint32_t, context, 0x1C) = 0;
                        if (canSerialize)
                        {
                            CSF_AT(uint8_t, context, 0x15) = existingElements;
                            CSF_AT(uint8_t, context, 0x16) = 0;
                            Storage_SerializeElement(element, nullptr, contextAddress);
                        }
                        ++index;
                        offset += 20;
                    } while (index < count);
                }
            }
            else
            {
                DestroyPointerStorage(reinterpret_cast<int*>(storage), nullptr);
                Storage_ReserveElements(storage, nullptr, count);
                if (count)
                {
                    uint32_t emptyElement[5] = {};
                    uint32_t index = 0;
                    uint32_t offset = 0;
                    do
                    {
                        Storage_AppendElement(storage, nullptr, emptyElement);
                        void* element = reinterpret_cast<void*>(storage->field_0004 + offset);
                        const bool canSerialize = CSF_AT(uint8_t, context, 0x00) == 0;
                        CSF_AT(uint32_t, context, 0x1C) = 0;
                        if (canSerialize)
                        {
                            CSF_AT(uint8_t, context, 0x15) = 0;
                            CSF_AT(uint8_t, context, 0x16) = 0;
                            Storage_SerializeElement(element, nullptr, contextAddress);
                        }
                        ++index;
                        offset += 20;
                    } while (index < count);
                }
            }
        }
    }

    if (!failed && CSF_AT(uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[31])(
                    serializer, nullptr, contextAddress))
        {
            CSF_AT(uint8_t, context, 0x00) = 0;
            return 0;
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x00455850, Storage_SerializeElements, "auto");

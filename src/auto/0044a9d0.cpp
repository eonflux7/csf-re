#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, DestroyElement20, 0x00448560,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, S_0044a1e0_ClearElements, 0x00449C10,
         (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, S_0044a1e0_ReserveElements, 0x0044A030,
         (void* self, void* edx, unsigned int a2));
CSF_FUNC(int, __fastcall, ProcessEventThroughHandlers, 0x0044A2F0,
         (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, S_0044a1e0_AppendElement20, 0x0044A390,
         (void* self, void* edx, int* a3));

static unsigned int __stdcall S_0044a1e0_ProcessElements(
    S_0044a1e0* elements, int eventAddress)
{
    void* event = reinterpret_cast<void*>(eventAddress);
    const uint8_t reuseElements = CSF_AT(uint8_t, event, 0x15);
    unsigned int count = 0;
    unsigned int result = 0;

    if (CSF_AT(uint8_t, event, 0x14) != 0)
    {
        const uint32_t begin = elements->field_0004;
        if (begin != 0)
        {
            result = static_cast<unsigned int>(
                static_cast<int32_t>(elements->field_0008 - begin) / 20);
        }
        count = result;

        bool stopped = false;
        if (CSF_AT(uint8_t, event, 0x00) == 0)
        {
            void* receiver = CSF_AT(void*, event, 0x04);
            const bool accepted =
                reinterpret_cast<bool(__fastcall*)(
                    void*, void*, unsigned int*, int)>(
                    CSF_AT(void**, receiver, 0)[0x78 / 4])(
                        receiver, nullptr, &count, eventAddress);
            result = count;
            stopped = !accepted;
        }
        CSF_AT(uint8_t, event, 0x00) = stopped;
        if (stopped)
        {
            CSF_AT(uint8_t, event, 0x00) = 1;
            return result;
        }

        unsigned int index = 0;
        uint32_t offset = 0;
        if (result != 0)
        {
            do
            {
                void* element = reinterpret_cast<void*>(
                    static_cast<uintptr_t>(elements->field_0004 + offset));
                const bool process = CSF_AT(uint8_t, event, 0x00) == 0;
                CSF_AT(uint32_t, event, 0x1C) = 0;
                if (process)
                {
                    CSF_AT(uint8_t, event, 0x15) = reuseElements;
                    CSF_AT(uint8_t, event, 0x16) = 0;
                    ProcessEventThroughHandlers(element, nullptr, eventAddress);
                    result = count;
                }
                offset += 20;
                ++index;
            }
            while (index < result);
        }
    }
    else
    {
        bool stopped = false;
        if (CSF_AT(uint8_t, event, 0x00) == 0)
        {
            void* receiver = CSF_AT(void*, event, 0x04);
            stopped =
                !reinterpret_cast<bool(__fastcall*)(
                    void*, void*, unsigned int*, int)>(
                    CSF_AT(void**, receiver, 0)[0x78 / 4])(
                        receiver, nullptr, &count, eventAddress);
        }
        result = stopped ? 1u : 0u;
        CSF_AT(uint8_t, event, 0x00) = static_cast<uint8_t>(result);
        if (stopped)
        {
            CSF_AT(uint8_t, event, 0x00) = 1;
            return result;
        }

        if (reuseElements != 0)
        {
            unsigned int index = 0;
            uint32_t offset = 0;
            if (count != 0)
            {
                do
                {
                    void* element = reinterpret_cast<void*>(
                        static_cast<uintptr_t>(elements->field_0004 + offset));
                    const bool process = CSF_AT(uint8_t, event, 0x00) == 0;
                    CSF_AT(uint32_t, event, 0x1C) = 0;
                    if (process)
                    {
                        CSF_AT(uint8_t, event, 0x15) = reuseElements;
                        CSF_AT(uint8_t, event, 0x16) = 0;
                        ProcessEventThroughHandlers(element, nullptr, eventAddress);
                    }
                    result = index + 1;
                    offset += 20;
                    ++index;
                }
                while (index < count);
            }
        }
        else
        {
            S_0044a1e0_ClearElements(elements, nullptr);
            result = S_0044a1e0_ReserveElements(elements, nullptr, count);

            alignas(8) uint8_t elementBytes[0x18] = {};
            unsigned int index = 0;
            uint32_t offset = 0;
            if (count != 0)
            {
                do
                {
                    CSF_AT(int, elementBytes, 0x00) = 0;
                    std::memset(elementBytes + 0x04, 0, 12);
                    CSF_AT(int, elementBytes, 0x10) = 10;
                    S_0044a1e0_AppendElement20(
                        elements, nullptr, reinterpret_cast<int*>(elementBytes));

                    void* element = reinterpret_cast<void*>(
                        static_cast<uintptr_t>(elements->field_0004 + offset));
                    const bool process = CSF_AT(uint8_t, event, 0x00) == 0;
                    CSF_AT(uint32_t, event, 0x1C) = 0;
                    if (process)
                    {
                        CSF_AT(uint8_t, event, 0x15) = 0;
                        CSF_AT(uint8_t, event, 0x16) = 0;
                        ProcessEventThroughHandlers(element, nullptr, eventAddress);
                    }
                    DestroyElement20(elementBytes, nullptr);
                    result = index + 1;
                    ++index;
                    offset += 20;
                }
                while (index < count);
            }
        }
    }

    uint8_t stopped = 1;
    if (CSF_AT(uint8_t, event, 0x00) == 0)
    {
        void* receiver = CSF_AT(void*, event, 0x04);
        result = static_cast<unsigned int>(
            reinterpret_cast<int(__fastcall*)(void*, void*, int)>(
                CSF_AT(void**, receiver, 0)[0x7C / 4])(
                    receiver, nullptr, eventAddress));
        stopped = static_cast<uint8_t>(result) == 0;
    }
    CSF_AT(uint8_t, event, 0x00) = stopped;
    return result;
}

HOOK(0x0044A9D0, S_0044a1e0_ProcessElements, "auto");

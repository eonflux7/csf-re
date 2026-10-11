#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(unsigned int, __fastcall, RecordArray_Grow, 0x004E6300,
         (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, Record_Transfer, 0x004E6430,
         (void* self, void* edx, void* a2));

static int __stdcall RecordArray_Transfer(int* array, void* context)
{
    const uint8_t preserveElements = CSF_AT(uint8_t, context, 0x14);
    const uint8_t elementMode = CSF_AT(uint8_t, context, 0x15);

    uint32_t count = 0;
    if (preserveElements)
        count = CSF_AT(uint32_t, array, 0x08);

    bool failed = false;
    if (!CSF_AT(uint8_t, context, 0x00))
    {
        void* visitor = CSF_AT(void*, context, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, uint32_t*, void*))
                       CSF_AT(void**, visitor, 0x00)[120 / 4])(
            visitor, nullptr, &count, context);
    }
    CSF_AT(uint8_t, context, 0x00) = failed;

    if (!failed)
    {
        if (preserveElements || elementMode)
        {
            uint32_t offset = 0;
            for (uint32_t index = 0; index < count; ++index)
            {
                void* element = CSF_AT(uint8_t*, array, 0x00) + offset;
                const bool canTransfer = !CSF_AT(uint8_t, context, 0x00);
                CSF_AT(uint32_t, context, 0x1C) = 0;
                if (canTransfer)
                {
                    CSF_AT(uint8_t, context, 0x15) = elementMode;
                    CSF_AT(uint8_t, context, 0x16) = 0;
                    Record_Transfer(element, nullptr, context);
                }
                offset += 20;
            }
        }
        else
        {
            const uint32_t capacity = CSF_AT(uint32_t, array, 0x04);
            uint32_t initialCount = count;
            CSF_AT(uint32_t, array, 0x08) = 0;
            if (initialCount > capacity)
            {
                RecordArray_Grow(array, nullptr,
                                 static_cast<int>(initialCount - capacity));
                initialCount = count;
            }

            uint32_t offset = 0;
            uint32_t index = 0;
            if (initialCount)
            {
                do
                {
                    if (CSF_AT(uint32_t, array, 0x08) >=
                        CSF_AT(uint32_t, array, 0x04))
                    {
                        RecordArray_Grow(
                            array, nullptr, CSF_AT(int, array, 0x0C));
                    }

                    const uint32_t used = CSF_AT(uint32_t, array, 0x08);
                    uint8_t* storage = CSF_AT(uint8_t*, array, 0x00);
                    CSF_AT(uint32_t, array, 0x08) = used + 1;

                    void* added = storage + 20 * used;
                    CSF_AT(int, added, 0x00) = -1;
                    CSF_AT(int, added, 0x04) = -1;
                    CSF_AT(int, added, 0x0C) = -1;
                    CSF_AT(int, added, 0x10) = -1;

                    void* element = CSF_AT(uint8_t*, array, 0x00) + offset;
                    const bool canTransfer = !CSF_AT(uint8_t, context, 0x00);
                    CSF_AT(uint32_t, context, 0x1C) = 0;
                    if (canTransfer)
                    {
                        CSF_AT(uint8_t, context, 0x15) = 0;
                        CSF_AT(uint8_t, context, 0x16) = 0;
                        Record_Transfer(element, nullptr, context);
                    }

                    offset += 20;
                    ++index;
                } while (index < count);
            }
        }

        if (!CSF_AT(uint8_t, context, 0x00))
        {
            void* visitor = CSF_AT(void*, context, 0x04);
            if (((bool(__fastcall*)(void*, void*, void*))
                     CSF_AT(void**, visitor, 0x00)[124 / 4])(
                    visitor, nullptr, context))
            {
                CSF_AT(uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(uint8_t, context, 0x00) = 1;
    return 1;
}

HOOK(0x004E67B0, RecordArray_Transfer, "auto");

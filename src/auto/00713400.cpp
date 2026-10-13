#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

static char __fastcall CInputDirectInput_PollBufferedInput(
    CInputDirectInput* self, void* /*edx*/)
{
    alignas(8) uint8_t eventBytes[0x50C];
    uint32_t eventCount;

    void* keyboard = reinterpret_cast<void*>(self->field_0020);
    if (keyboard)
    {
        eventCount = 64;
        void** table = CSF_AT(void**, keyboard, 0);
        // The COM arguments remain on the stack. The two register arguments
        // preserve the registers used by the original indirect call.
        int32_t result =
            reinterpret_cast<int32_t(__fastcall*)(
                void*, void*, void*, uint32_t, void*, uint32_t*, uint32_t)>(
                table[40 / 4])(
                    table, eventBytes, keyboard, 20, eventBytes, &eventCount, 0);

        if (result)
        {
            keyboard = reinterpret_cast<void*>(self->field_0020);
            self->field_0028 = 0;
            result = reinterpret_cast<int32_t(__fastcall*)(
                void*, void*, void*)>(
                    CSF_AT(void**, keyboard, 0)[28 / 4])(
                        table, eventBytes, keyboard);
            while (result == -2147024866)
            {
                table = CSF_AT(void**, keyboard, 0);
                result = reinterpret_cast<int32_t(__fastcall*)(
                    void*, void*, void*)>(table[28 / 4])(
                        table, eventBytes, keyboard);
            }
        }
        else
        {
            self->field_0028 = eventCount;
            uint8_t* destination = reinterpret_cast<uint8_t*>(self) + 0x2C;
            uint8_t* event = eventBytes;
            for (uint32_t remaining = eventCount; remaining; --remaining)
            {
                const uint32_t data = CSF_AT(uint32_t, event, 4);
                CSF_AT(uint32_t, destination, 0) = CSF_AT(uint32_t, event, 0);
                CSF_AT(uint8_t, destination, 4) = (data & 0x80) != 0;
                destination += 8;
                event += 20;
            }
        }
    }

    if (!self->field_0c91)
    {
        self->field_022c = 0;
        return 0;
    }

    void* mouse = reinterpret_cast<void*>(self->field_0024);
    if (!mouse)
        return 0;

    eventCount = 64;
    void** mouseTable = CSF_AT(void**, mouse, 0);
    int32_t result =
        reinterpret_cast<int32_t(__fastcall*)(
            void*, void*, void*, uint32_t, void*, uint32_t*, uint32_t)>(
            mouseTable[40 / 4])(
                eventBytes, mouseTable, mouse, 20, eventBytes, &eventCount, 0);

    if (result)
    {
        self->field_022c = 0;
        mouse = reinterpret_cast<void*>(self->field_0024);
        do
        {
            result = reinterpret_cast<int32_t(__fastcall*)(
                void*, void*, void*)>(
                    CSF_AT(void**, mouse, 0)[28 / 4])(
                        eventBytes, mouseTable, mouse);
        } while (result == -2147024866);
        return static_cast<char>(result);
    }

    self->field_022c = eventCount;
    if (!eventCount)
        return 0;

    uint8_t* destination = reinterpret_cast<uint8_t*>(self) + 0x230;
    uint8_t* event = eventBytes;
    for (uint32_t remaining = eventCount; remaining; --remaining)
    {
        const uint32_t offset = CSF_AT(uint32_t, event, 0);
        const uint32_t data = CSF_AT(uint32_t, event, 4);

        switch (offset)
        {
        case 0:
        {
            const float delta = static_cast<float>(static_cast<int32_t>(data));
            CSF_AT(uint32_t, destination, 0) = 3;
            const float position =
                delta * reinterpret_cast<float&>(self->field_0c98) *
                    reinterpret_cast<float&>(self->field_0c94) +
                self->field_0c3c;
            self->field_0c3c = position;
            const int32_t minimum = static_cast<int32_t>(self->field_0ca4);
            const uint32_t integral =
                static_cast<uint32_t>(static_cast<int64_t>(position));
            self->field_0c44 = integral;
            if (static_cast<int32_t>(integral) < minimum)
            {
                self->field_0c44 = static_cast<uint32_t>(minimum);
                self->field_0c3c = static_cast<float>(minimum);
            }
            else
            {
                const int32_t end = static_cast<int32_t>(
                    self->field_0cac + static_cast<uint32_t>(minimum));
                if (static_cast<int32_t>(integral) >= end)
                {
                    const uint32_t maximum = static_cast<uint32_t>(end) - 1;
                    self->field_0c44 = maximum;
                    self->field_0c3c =
                        static_cast<float>(static_cast<int32_t>(maximum));
                }
            }
            reinterpret_cast<float&>(self->field_0c30) =
                delta * reinterpret_cast<float&>(self->field_0c94);
            break;
        }

        case 4:
        {
            const float delta = static_cast<float>(static_cast<int32_t>(data));
            CSF_AT(uint32_t, destination, 0) = 4;
            const float position =
                delta * reinterpret_cast<float&>(self->field_0c9c) *
                    reinterpret_cast<float&>(self->field_0c94) +
                self->field_0c40;
            self->field_0c40 = position;
            const int32_t minimum = static_cast<int32_t>(self->field_0ca8);
            const uint32_t integral =
                static_cast<uint32_t>(static_cast<int64_t>(position));
            self->field_0c48 = integral;
            if (static_cast<int32_t>(integral) < minimum)
            {
                self->field_0c48 = static_cast<uint32_t>(minimum);
                self->field_0c40 = static_cast<float>(minimum);
            }
            else
            {
                const int32_t end = static_cast<int32_t>(
                    self->field_0cb0 + static_cast<uint32_t>(minimum));
                if (static_cast<int32_t>(integral) >= end)
                {
                    const uint32_t maximum = static_cast<uint32_t>(end) - 1;
                    self->field_0c48 = maximum;
                    self->field_0c40 =
                        static_cast<float>(static_cast<int32_t>(maximum));
                }
            }
            reinterpret_cast<float&>(self->field_0c34) =
                delta * reinterpret_cast<float&>(self->field_0c94);
            break;
        }

        case 8:
            CSF_AT(uint32_t, destination, 0) = 5;
            self->field_0c38 = data;
            break;

        case 12:
            CSF_AT(uint32_t, destination, 0) = 0;
            self->field_0c4c = (data & 0x80) != 0;
            if (self->field_0c4c)
            {
                if (self->field_0c4f)
                {
                    self->field_0c4f = 0;
                }
                else if (!self->field_0cb4 &&
                         CSF_AT(uint32_t, event, 8) <
                             self->field_0ca0 + self->field_0cb8)
                {
                    uint32_t distanceX =
                        self->field_0cbc - self->field_0c44;
                    if (static_cast<int32_t>(distanceX) < 0)
                        distanceX = 0u - distanceX;
                    if (static_cast<int32_t>(distanceX) <= 1)
                    {
                        uint32_t distanceY =
                            self->field_0cc0 - self->field_0c48;
                        if (static_cast<int32_t>(distanceY) < 0)
                            distanceY = 0u - distanceY;
                        if (static_cast<int32_t>(distanceY) <= 1)
                            self->field_0c4f = 1;
                    }
                }

                const uint32_t timestamp = CSF_AT(uint32_t, event, 8);
                const uint32_t clickX = self->field_0c44;
                self->field_0cb8 = timestamp;
                const uint32_t clickY = self->field_0c48;
                self->field_0cb4 = 0;
                self->field_0cbc = clickX;
                self->field_0cc0 = clickY;
            }
            break;

        case 13:
            CSF_AT(uint32_t, destination, 0) = 1;
            self->field_0c4d = (data & 0x80) != 0;
            if (self->field_0c4d)
                self->field_0cb4 = 6;
            break;

        case 14:
            CSF_AT(uint32_t, destination, 0) = 2;
            self->field_0c4e = (data & 0x80) != 0;
            if (self->field_0c4e)
                self->field_0cb4 = 6;
            break;

        default:
            CSF_AT(uint32_t, destination, 0) = 6;
            break;
        }

        std::memcpy(destination + 4, &self->field_0c30, 0x20);
        destination += 36;
        event += 20;
    }

    return static_cast<char>(reinterpret_cast<uintptr_t>(destination));
}

HOOK(0x00713400, CInputDirectInput_PollBufferedInput, "auto");

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00833908_RefreshRelatedState, 0x0048FB00, (void* self, void* edx));

static char __fastcall C_00833908_DispatchPendingNotifications(C_00833908* self, void* /*edx*/)
{
    while (self->field_0175)
    {
        const uint32_t relatedState = self->field_0050;
        self->field_0175 = 0;
        if (relatedState)
        {
            void* related = reinterpret_cast<void*>(self->field_0038);
            if (related)
            {
                if (((uint8_t(__fastcall*)(void*, void*))CSF_AT(void**, related, 0)[176 / 4])(
                        related, nullptr))
                {
                    C_00833908_RefreshRelatedState(self, nullptr);
                }
            }
        }

        void* flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 27))
        {
            CSF_AT(uint8_t, flags, 27) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, listener, 0)[68 / 4])(
                    listener, nullptr, &self->field_0148);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 22))
        {
            CSF_AT(uint8_t, flags, 22) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, listener, 0)[60 / 4])(
                    listener, nullptr, &self->field_012c);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 23))
        {
            CSF_AT(uint8_t, flags, 23) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, listener, 0)[64 / 4])(
                    listener, nullptr, &self->field_013c);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 0))
        {
            CSF_AT(uint8_t, flags, 0) = 0;
            if (self->field_00c8)
            {
                uint32_t byteOffset = 0;
                uint32_t remaining = self->field_00c8;
                do
                {
                    for (uint32_t i = 0; i < self->field_00a4; ++i)
                    {
                        void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, listener, 0)[8 / 4])(
                            listener, nullptr, byteOffset + self->field_00c0);
                    }
                    byteOffset += 12;
                    --remaining;
                } while (remaining);
            }
            self->field_00c8 = 0;
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 29))
        {
            CSF_AT(uint8_t, flags, 29) = 0;
            if (self->field_00d8)
            {
                uint32_t byteOffset = 0;
                uint32_t remaining = self->field_00d8;
                do
                {
                    for (uint32_t i = 0; i < self->field_00a4; ++i)
                    {
                        void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                        ((void(__fastcall*)(void*, void*, uint32_t))CSF_AT(void**, listener, 0)[72 / 4])(
                            listener, nullptr, byteOffset + self->field_00d0);
                    }
                    byteOffset += 12;
                    --remaining;
                } while (remaining);
            }
            self->field_00d8 = 0;
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 1))
        {
            CSF_AT(uint8_t, flags, 1) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[12 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 2))
        {
            CSF_AT(uint8_t, flags, 2) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[16 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 3))
        {
            CSF_AT(uint8_t, flags, 3) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[20 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 4))
        {
            CSF_AT(uint8_t, flags, 4) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[24 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 5))
        {
            CSF_AT(uint8_t, flags, 5) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[28 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 6))
        {
            CSF_AT(uint8_t, flags, 6) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[32 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 28))
        {
            CSF_AT(uint8_t, flags, 28) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[36 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 7))
        {
            CSF_AT(uint8_t, flags, 7) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, listener, 0)[40 / 4])(
                    listener, nullptr, &self->field_00e0);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 8))
        {
            CSF_AT(uint8_t, flags, 8) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, listener, 0)[44 / 4])(
                    listener, nullptr, &self->field_00e4);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 12))
        {
            CSF_AT(uint8_t, flags, 12) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, listener, 0)[84 / 4])(
                    listener, nullptr, &self->field_00e8);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 13))
        {
            CSF_AT(uint8_t, flags, 13) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, listener, 0)[88 / 4])(
                    listener, nullptr, &self->field_00f4);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 14))
        {
            CSF_AT(uint8_t, flags, 14) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*, uint8_t*))CSF_AT(void**, listener, 0)[92 / 4])(
                    listener, nullptr, &self->field_0118);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 15))
        {
            CSF_AT(uint8_t, flags, 15) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[96 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 16))
        {
            CSF_AT(uint8_t, flags, 16) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[100 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 17))
        {
            CSF_AT(uint8_t, flags, 17) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, listener, 0)[104 / 4])(
                    listener, nullptr, &self->field_011c);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 18))
        {
            CSF_AT(uint8_t, flags, 18) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*, uint32_t*))CSF_AT(void**, listener, 0)[108 / 4])(
                    listener, nullptr, &self->field_0124);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 19))
        {
            CSF_AT(uint8_t, flags, 19) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[112 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 20))
        {
            CSF_AT(uint8_t, flags, 20) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[116 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 21))
        {
            CSF_AT(uint8_t, flags, 21) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[120 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 9))
        {
            CSF_AT(uint8_t, flags, 9) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[48 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 10))
        {
            CSF_AT(uint8_t, flags, 10) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[52 / 4])(
                    listener, nullptr);
            }
        }

        flags = reinterpret_cast<void*>(self->field_00ac);
        if (CSF_AT(uint8_t, flags, 11))
        {
            CSF_AT(uint8_t, flags, 11) = 0;
            for (uint32_t i = 0; i < self->field_00a4; ++i)
            {
                void* listener = CSF_AT(void*, reinterpret_cast<void*>(self->field_009c), 4 * i);
                ((void(__fastcall*)(void*, void*))CSF_AT(void**, listener, 0)[56 / 4])(
                    listener, nullptr);
            }
        }
    }
    return 0;
}

HOOK(0x0048FEF0, C_00833908_DispatchPendingNotifications, "auto");

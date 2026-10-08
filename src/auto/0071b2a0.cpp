#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, C_00864508_RemoveObjectAtIndex, 0x0071ABC0,
         (C_00864508* self, void* edx, int index));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60,
         (void* destination, const void* source, std::size_t size));

CSF_GLOBAL(float, g_fZero, 0x0082D4CC);

static int __fastcall C_00864508_ProcessPendingRemovals(
    C_00864508* self, void* /*edx*/)
{
    const std::uint32_t pendingBegin = self->field_0034;
    const std::uint32_t pendingCount = pendingBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_0038 - pendingBegin) >> 3)
        : 0;

    for (std::uint32_t index = 0; index < pendingCount; ++index)
    {
        const std::uint32_t objectIndex = CSF_AT(
            std::uint32_t, reinterpret_cast<void*>(self->field_0034), index * 8);
        void* object = reinterpret_cast<void*>(CSF_AT(
            std::uint32_t, reinterpret_cast<void*>(self->field_0048),
            objectIndex * 4));

        if (pendingCount == 1
            && ((std::uint8_t(__fastcall*)(void*, void*))
                    CSF_AT(void**, object, 0)[9])(object, nullptr)
            && !CSF_AT(std::uint8_t,
                       reinterpret_cast<void*>(self->field_0034), index * 8 + 4)
            && self->field_0078 >= g_fZero)
        {
            void* innerObject =
                ((void*(__fastcall*)(void*, void*))
                     CSF_AT(void**, object, 0)[8])(object, nullptr);

            const int state =
                ((int(__fastcall*)(void*, void*))
                     CSF_AT(void**, innerObject, 0)[101])(innerObject, nullptr);

            if (state == 0)
            {
                ((void(__fastcall*)(void*, void*, float, int))
                     CSF_AT(void**, innerObject, 0)[102])(
                    innerObject, nullptr, self->field_0078, 0);
            }
            else if (state == 1)
            {
                if (((std::uint8_t(__fastcall*)(void*, void*))
                         CSF_AT(void**, innerObject, 0)[104])(
                        innerObject, nullptr))
                {
                    ((void(__fastcall*)(void*, void*))
                         CSF_AT(void**, innerObject, 0)[106])(
                        innerObject, nullptr);
                }
            }
            else if (state == 2)
            {
                C_00864508_RemoveObjectAtIndex(
                    self, nullptr, static_cast<std::int32_t>(objectIndex));
                CSF_AT(std::uint32_t,
                       reinterpret_cast<void*>(self->field_0034), index * 8)
                    = UINT32_MAX;
            }
        }
        else
        {
            ((void*(__fastcall*)(void*, void*))
                 CSF_AT(void**, object, 0)[8])(object, nullptr);
            C_00864508_RemoveObjectAtIndex(
                self, nullptr, static_cast<std::int32_t>(objectIndex));
            CSF_AT(std::uint32_t,
                   reinterpret_cast<void*>(self->field_0034), index * 8)
                = UINT32_MAX;
        }
    }

    const std::uint32_t objectBegin = self->field_0048;
    const std::uint32_t objectCount = objectBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(self->field_004c - objectBegin) >> 2)
        : 0;

    for (std::int32_t index =
             static_cast<std::int32_t>(objectCount - 1);
         index >= 0; --index)
    {
        const std::uint32_t slotAddress =
            self->field_0048 + static_cast<std::uint32_t>(index) * 4;

        if (!CSF_AT(std::uint32_t, reinterpret_cast<void*>(slotAddress), 0))
        {
            const std::uint32_t nextAddress = slotAddress + 4;
            const std::uint32_t byteCount =
                static_cast<std::uint32_t>(
                    static_cast<std::int32_t>(
                        self->field_004c - nextAddress) >> 2) << 2;

            _memcpy(reinterpret_cast<void*>(slotAddress),
                           reinterpret_cast<const void*>(nextAddress),
                           byteCount);
            self->field_004c -= 4;
        }
    }

    const std::uint32_t remainingBegin = self->field_0034;
    std::uint32_t result = remainingBegin
        ? static_cast<std::uint32_t>(
              static_cast<std::int32_t>(
                  self->field_0038 - remainingBegin) >> 3)
        : 0;

    for (std::int32_t index = static_cast<std::int32_t>(result - 1);
         index >= 0; --index)
    {
        result = self->field_0034 + static_cast<std::uint32_t>(index) * 8;
        if (CSF_AT(std::uint32_t, reinterpret_cast<void*>(result), 0)
            == UINT32_MAX)
        {
            const std::uint32_t endAddress = self->field_0038;
            result += 8;

            if (result != endAddress)
            {
                std::uint32_t destination = result - 8;
                do
                {
                    CSF_AT(std::uint32_t,
                           reinterpret_cast<void*>(destination), 0)
                        = CSF_AT(std::uint32_t,
                                 reinterpret_cast<void*>(result), 0);
                    CSF_AT(std::uint32_t,
                           reinterpret_cast<void*>(destination), 4)
                        = CSF_AT(std::uint32_t,
                                 reinterpret_cast<void*>(result), 4);
                    result += 8;
                    destination += 8;
                }
                while (result != endAddress);
            }

            self->field_0038 -= 8;
        }
    }

    return static_cast<std::int32_t>(result);
}

HOOK(0x0071B2A0, C_00864508_ProcessPendingRemovals, "auto");

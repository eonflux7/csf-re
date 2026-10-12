#include <cstdint>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GrowObjectArray, 0x004DE8F0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, VecGrow_4DE9B0, 0x004DE9B0, (void* self, void* edx, int a2));
CSF_FUNC(bool, __fastcall, InitializeConnections, 0x004E0930, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10, (void* self, void* edx));

static char __fastcall S_004de880_LoadAndResolveConnections(
    S_004de880* self, void* /*edx*/, std::uint32_t* stream)
{
    std::uint32_t tripleCount = reinterpret_cast<std::uintptr_t>(stream);
    ((void(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, stream, 0)[2])(stream, nullptr, &tripleCount, 4);

    auto* tripleArray = reinterpret_cast<std::uint8_t*>(self) + 0x30;
    if (tripleCount)
    {
        if (tripleCount > self->field_0034)
            VecGrow_4DE9B0(tripleArray, nullptr,
                          static_cast<int>(tripleCount - self->field_0034));

        for (std::uint32_t i = 0; i < tripleCount; ++i)
        {
            alignas(8) std::uint8_t tripleBytes[0x18]{};
            ((void(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, stream, 0)[2])(stream, nullptr, tripleBytes, 12);

            if (CSF_AT(std::uint32_t, self, 0x38) >= self->field_0034)
                VecGrow_4DE9B0(
                    tripleArray, nullptr,
                    static_cast<int>(CSF_AT(std::uint32_t, self, 0x3C)));

            const std::uint32_t index = CSF_AT(std::uint32_t, self, 0x38);
            const std::uint32_t data = CSF_AT(std::uint32_t, self, 0x30);
            CSF_AT(std::uint32_t, self, 0x38) = index + 1;
            auto* destination = reinterpret_cast<std::uint8_t*>(data + 12 * index);
            CSF_AT(std::uint32_t, destination, 0) =
                CSF_AT(std::uint32_t, tripleBytes, 0);
            CSF_AT(std::uint32_t, destination, 4) =
                CSF_AT(std::uint32_t, tripleBytes, 4);
            CSF_AT(std::uint32_t, destination, 8) =
                CSF_AT(std::uint32_t, tripleBytes, 8);
        }
    }

    std::uint32_t objectCount = 0;
    ((void(__fastcall*)(void*, void*, void*, int))
        CSF_AT(void**, stream, 0)[2])(stream, nullptr, &objectCount, 4);
    if (!objectCount)
        return 0;

    auto* objectArray = reinterpret_cast<std::uint8_t*>(self) + 0x20;
    if (objectCount > self->field_0024)
        GrowObjectArray(objectArray, nullptr,
                        static_cast<int>(objectCount - self->field_0024));

    alignas(8) std::uint8_t connectionCountBytes[8]{};
    auto& connectionCount = CSF_AT(std::uint32_t, connectionCountBytes, 0);

    for (std::uint32_t i = 0; i < objectCount; ++i)
    {
        ((void(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, stream, 0)[2])(
                stream, nullptr, connectionCountBytes, 4);

        alignas(8) std::uint8_t objectBytes[0x40]{};
        std::memset(objectBytes + 16, 0, 12);
        CSF_AT(float, objectBytes, 28) = 1.0f;
        CSF_AT(std::uint32_t, objectBytes, 32) = 0;
        CSF_AT(std::uint32_t, objectBytes, 36) = 0;
        CSF_AT(std::uint8_t, objectBytes, 56) = 1;
        InitializeConnections(objectBytes, nullptr, static_cast<int>(connectionCount));

        ((void(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, stream, 0)[2])(stream, nullptr, objectBytes, 4);
        ((void(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, stream, 0)[2])(stream, nullptr, objectBytes + 4, 4);
        ((void(__fastcall*)(void*, void*, void*, int))
            CSF_AT(void**, stream, 0)[2])(stream, nullptr, objectBytes + 16, 44);

        auto* connection = static_cast<std::uint8_t*>(
            GetCachedObjectProvider(objectBytes, nullptr));
        for (std::uint32_t j = 0; j < connectionCount; ++j, connection += 16)
        {
            ((void(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, stream, 0)[2])(stream, nullptr, connection, 4);
            ((void(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, stream, 0)[2])(stream, nullptr, connection + 4, 4);
            CSF_AT(std::uint32_t, connection, 8) =
                CSF_AT(std::uint32_t, objectBytes, 0);
            ((void(__fastcall*)(void*, void*, void*, int))
                CSF_AT(void**, stream, 0)[2])(stream, nullptr, connection + 12, 4);
        }

        if (self->field_0028 >= self->field_0024)
            GrowObjectArray(objectArray, nullptr,
                            static_cast<int>(CSF_AT(std::uint32_t, self, 0x2C)));

        const std::uint32_t index = self->field_0028;
        auto* destination = reinterpret_cast<void*>(self->field_0020 + (index << 6));
        self->field_0028 = index + 1;
        std::memcpy(destination, objectBytes, 0x40);
    }

    std::uint32_t objectOffset = 0;
    for (std::uint32_t i = 0; i < self->field_0028; ++i, objectOffset += 64)
    {
        auto* object = reinterpret_cast<C_0085e7b8*>(self->field_0020 + objectOffset);
        auto* connection = static_cast<std::uint8_t*>(
            GetCachedObjectProvider(object, nullptr));

        for (std::uint32_t j = 0; j < object->field_0008; ++j, connection += 16)
        {
            const std::uint32_t ownerIndex = CSF_AT(std::uint32_t, connection, 8);
            const std::uint32_t otherIndex = CSF_AT(std::uint32_t, connection, 12);
            CSF_AT(std::uint32_t, connection, 8) =
                self->field_0020 + (ownerIndex << 6);

            if (otherIndex == static_cast<std::uint32_t>(-1))
            {
                CSF_AT(std::uint32_t, connection, 12) = 0;
            }
            else
            {
                auto* otherObject =
                    reinterpret_cast<C_0085e7b8*>(self->field_0020 + (otherIndex << 6));
                auto* candidate = static_cast<std::uint8_t*>(
                    GetCachedObjectProvider(otherObject, nullptr));
                for (std::uint32_t k = 0; k < otherObject->field_0008;
                     ++k, candidate += 16)
                {
                    if (CSF_AT(std::uint32_t, candidate, 4) ==
                            CSF_AT(std::uint32_t, connection, 0) &&
                        CSF_AT(std::uint32_t, candidate, 0) ==
                            CSF_AT(std::uint32_t, connection, 4))
                    {
                        CSF_AT(std::uint32_t, connection, 12) =
                            reinterpret_cast<std::uintptr_t>(candidate);
                        break;
                    }
                }
            }
        }
    }
    return 1;
}

HOOK(0x004DEAD0, S_004de880_LoadAndResolveConnections, "auto");

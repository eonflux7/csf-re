#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, TypeRegistry_SerializeHermiteVectorMember,
         0x0054D3B0, (void* a1, int a2));
CSF_FUNC(int, __stdcall, TypeRegistry_SerializeHermiteShapeMember,
         0x0054D5A0, (void* a1, int a2));

static int __stdcall TypeRegistry_SerializeInterpolatorHermite3D(int a1, int a2)
{
    auto* object = reinterpret_cast<std::uint8_t*>(a1);
    void* context = reinterpret_cast<void*>(a2);

    bool error = false;
    if (CSF_AT(std::uint8_t, context, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        error = !reinterpret_cast<bool(__fastcall*)(void*, void*, const char*, int)>(
            CSF_AT(void**, serializer, 0x00)[0x80 / 4])(
                serializer, nullptr,
                CSF_STR(0x008443AC, "CInterpolatorHermite3D"), a2);
    }

    CSF_AT(std::uint8_t, context, 0x00) = error;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (!error)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        CSF_AT(std::uint8_t, context, 0x00) =
            reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
                CSF_AT(void**, serializer, 0x00)[0x44 / 4])(
                    serializer, nullptr, object + 0x2C, a2) == -1;
    }

    bool ready = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        TypeRegistry_SerializeHermiteVectorMember(object + 0x30, a2);
    }

    ready = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        TypeRegistry_SerializeHermiteVectorMember(object + 0x08, a2);
    }

    ready = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        TypeRegistry_SerializeHermiteShapeMember(object + 0x18, a2);
    }

    ready = CSF_AT(std::uint8_t, context, 0x00) == 0;
    CSF_AT(std::uint32_t, context, 0x1C) = 0;
    if (ready)
    {
        void* serializer = CSF_AT(void*, context, 0x04);
        CSF_AT(std::uint8_t, context, 0x15) = 0;
        CSF_AT(std::uint8_t, context, 0x16) = 0;
        error = reinterpret_cast<int(__fastcall*)(void*, void*, void*, int)>(
            CSF_AT(void**, serializer, 0x00)[0x58 / 4])(
                serializer, nullptr, object + 0x28, a2) == -1;
        CSF_AT(std::uint8_t, context, 0x00) = error;

        if (!error)
        {
            serializer = CSF_AT(void*, context, 0x04);
            if (reinterpret_cast<bool(__fastcall*)(void*, void*, int)>(
                    CSF_AT(void**, serializer, 0x00)[0x7C / 4])(
                        serializer, nullptr, a2))
            {
                CSF_AT(std::uint8_t, context, 0x00) = 0;
                return 0;
            }
        }
    }

    CSF_AT(std::uint8_t, context, 0x00) = 1;
    return -1;
}

HOOK(0x0054D750, TypeRegistry_SerializeInterpolatorHermite3D, "auto");

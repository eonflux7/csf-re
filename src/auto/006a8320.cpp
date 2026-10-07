#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));  // NetZ heap, not the game heap
CSF_FUNC(void, __fastcall, InitializeTemporaryString, 0x0064A6B0,
         (void* self, void* edx, std::int32_t source));
CSF_FUNC(void, __fastcall, DestroyTemporaryString, 0x0064A6F0,
         (void* self, void* edx));
CSF_FUNC(void*, __fastcall, ConstructResource, 0x0066A440,
         (void* self, void* edx));
CSF_FUNC(unsigned char, __fastcall, LoadResource, 0x00798FB0,
         (void* self, void* edx, void* resource, void* name,
          std::uint32_t* descriptor, void* parameters));

CSF_GLOBAL(std::uint32_t, g_ResourceDescriptorVtable, 0x0085FF90);

static char __fastcall BeginResourceLoad(
    void* self, void* /*edx*/, std::int32_t source, std::int32_t /*unused*/)
{
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[9])(
        self, nullptr);

    std::uint32_t temporaryString;
    InitializeTemporaryString(&temporaryString, nullptr, source);

    if (CSF_AT(std::uint32_t, self, 0x08) == 2)
    {
        CSF_AT(std::uint32_t, self, 0x08) = 3;
        CSF_AT(std::uint32_t, self, 0x0C) = 1;

        void* allocation = NetZ_OperatorNew(112);
        void* resource = allocation
            ? ConstructResource(allocation, nullptr)
            : nullptr;

        void* loadContext = reinterpret_cast<void*>(
            CSF_AT(std::uint32_t, self, 0x2C));

        CSF_AT(std::uint32_t, self, 0x90) =
            reinterpret_cast<std::uintptr_t>(resource);

        std::uint32_t descriptor[3];
        descriptor[0] = reinterpret_cast<std::uintptr_t>(
            &g_ResourceDescriptorVtable);
        descriptor[1] = 0xFFFFFFFFu;
        descriptor[2] = 10;

        if (!LoadResource(loadContext, nullptr, resource, &temporaryString,
                          descriptor,
                          &CSF_AT(std::uint32_t, self, 0x40)))
        {
            void* failedResource = reinterpret_cast<void*>(
                CSF_AT(std::uint32_t, self, 0x90));

            CSF_AT(std::uint32_t, self, 0x2B0) = 0x494C3036u;

            if (failedResource)
            {
                ((void(__fastcall*)(void*, void*, int))
                    CSF_AT(void**, failedResource, 0)[0])(
                        failedResource, nullptr, 1);

                CSF_AT(std::uint32_t, self, 0x90) = 0;
            }

            CSF_AT(std::uint32_t, self, 0x08) = 2;
            CSF_AT(std::uint32_t, self, 0x0C) = 0;
            CSF_AT(std::uint8_t, self, 0x334) = 0;
        }
    }

    DestroyTemporaryString(&temporaryString, nullptr);
    return 1;
}

HOOK(0x006A8320, BeginResourceLoad, "auto");

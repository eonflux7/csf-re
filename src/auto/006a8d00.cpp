#include <cstdint>
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(void*, __cdecl, NetZ_OperatorNew, 0x00647140, (unsigned int size));  // NetZ heap, not the game heap
CSF_FUNC(void*, __fastcall, ConstructRequest, 0x0066A440,
         (void* self, void* edx));
CSF_FUNC(int, __fastcall, StartRequest, 0x00799220,
         (void* self, void* edx, void* request, void* requestData, void* parameters));

static int __fastcall BeginRequestOrUpdateMode(
    void* self, void* /*edx*/, std::int8_t mode)
{
    int result = CSF_AT(std::int32_t, self, 0x08);

    if (result == 7)
    {
        void* parameters = CSF_AT(void*, self, 0x88);
        CSF_AT(std::int8_t, parameters, 0x31) = mode;
        CSF_AT(std::int16_t, parameters, 0x12) =
            static_cast<std::int16_t>(mode);

        void* previousRequest = CSF_AT(void*, self, 0x90);
        if (previousRequest)
        {
            ((int(__fastcall*)(void*, void*, int))
                 CSF_AT(void**, previousRequest, 0)[0])(
                previousRequest, nullptr, 1);
            CSF_AT(void*, self, 0x90) = nullptr;
        }

        void* allocation = NetZ_OperatorNew(0x70);
        void* request = allocation
            ? ConstructRequest(allocation, nullptr)
            : nullptr;

        void* context = CSF_AT(void*, self, 0x2C);
        CSF_AT(void*, self, 0x90) = request;
        CSF_AT(std::uint8_t, self, 0x33C) = 0;

        result = StartRequest(
            context, nullptr, request,
            static_cast<std::uint8_t*>(self) + 0x2B4,
            static_cast<std::uint8_t*>(self) + 0x84);

        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::int32_t, self, 0x08) = 10;
        }
        else
        {
            void* failedRequest = CSF_AT(void*, self, 0x90);
            CSF_AT(std::uint32_t, self, 0x2B0) = 0x494C3934;

            if (failedRequest)
            {
                result = ((int(__fastcall*)(void*, void*, int))
                              CSF_AT(void**, failedRequest, 0)[0])(
                    failedRequest, nullptr, 1);
                CSF_AT(void*, self, 0x90) = nullptr;
            }
        }
    }
    else if (result == 10)
    {
        CSF_AT(std::int8_t, self, 0x33C) = mode;
    }

    return result;
}

HOOK(0x006A8D00, BeginRequestOrUpdateMode, "auto");

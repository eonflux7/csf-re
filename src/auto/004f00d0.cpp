#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"

CSF_FUNC(char*, __stdcall, TextDatabase_ProcessRequestEntries, 0x004EFD60,
         (uint32_t* a1, char* a2));

static int __fastcall TextDatabase_ProcessTextRequest(
    void* self, void* /*edx*/, char* request)
{
    bool blocked = false;
    if (CSF_AT(uint8_t, request, 0) == 0)
    {
        void* provider = CSF_AT(void*, request, 0x4);
        blocked = !((bool(__fastcall*)(void*, void*, uint32_t, char*))
                    CSF_AT(void**, provider, 0)[0x80 / 4])(
                        provider, nullptr, 0, request);
    }

    uint32_t savedContext = CSF_AT(uint32_t, request, 0x1C);
    CSF_AT(uint8_t, request, 0) = blocked;
    CSF_AT(uint32_t, request, 0x1C) = 0;

    if (!blocked)
    {
        CSF_AT(uint8_t, request, 0x15) = 0;
        CSF_AT(uint8_t, request, 0x16) = 0;
        TextDatabase_ProcessRequestEntries(
            reinterpret_cast<uint32_t*>(static_cast<uint8_t*>(self) + 0x4),
            request);
    }

    bool continueProcessing = CSF_AT(uint8_t, request, 0) == 0;
    CSF_AT(uint32_t, request, 0x1C) = 0;
    if (continueProcessing)
    {
        void* provider = CSF_AT(void*, request, 0x4);
        CSF_AT(uint8_t, request, 0x15) = 0;
        CSF_AT(uint8_t, request, 0x16) = 0;
        int result = ((int(__fastcall*)(void*, void*, uint32_t*, char*))
                      CSF_AT(void**, provider, 0)[0x44 / 4])(
                          provider, nullptr,
                          reinterpret_cast<uint32_t*>(
                              static_cast<uint8_t*>(self) + 0x14),
                          request);
        CSF_AT(uint8_t, request, 0) = result == -1;
    }

    continueProcessing = CSF_AT(uint8_t, request, 0) == 0;
    CSF_AT(uint32_t, request, 0x1C) = 0;
    if (continueProcessing)
    {
        void* provider = CSF_AT(void*, request, 0x4);
        CSF_AT(uint8_t, request, 0x15) = 0;
        CSF_AT(uint8_t, request, 0x16) = 0;
        int result = ((int(__fastcall*)(void*, void*, uint32_t*, char*))
                      CSF_AT(void**, provider, 0)[0x54 / 4])(
                          provider, nullptr,
                          reinterpret_cast<uint32_t*>(
                              static_cast<uint8_t*>(self) + 0x18),
                          request);
        CSF_AT(uint8_t, request, 0) = result == -1;
    }

    continueProcessing = CSF_AT(uint8_t, request, 0) == 0;
    CSF_AT(uint32_t, request, 0x1C) = savedContext;
    if (continueProcessing)
    {
        void* provider = CSF_AT(void*, request, 0x4);
        uint32_t result = ((uint32_t(__fastcall*)(void*, void*, char*))
                           CSF_AT(void**, provider, 0)[0x7C / 4])(
                               provider, nullptr, request);
        if (static_cast<uint8_t>(result) != 0)
        {
            CSF_AT(uint8_t, request, 0) = 0;
            return static_cast<int>(result);
        }
    }

    CSF_AT(uint8_t, request, 0) = 1;
    return 1;
}

HOOK(0x004F00D0, TextDatabase_ProcessTextRequest, "auto");

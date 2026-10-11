#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void**, __fastcall, C_0085fe28_HandleGatheringResponse, 0x006AA880,
         (void* self, void* edx));

static void __fastcall C_0085fe28_UpdateGatheringRequest(
    C_0085fe28* self, void* /*edx*/, int /*unused*/)
{
    if (self->field_0008 != 3 || self->field_000c != 1)
        return;

    void* request = reinterpret_cast<void*>(self->field_0090);
    const std::uint32_t requestState = CSF_AT(std::uint32_t, request, 0x0C);
    if (requestState == 1)
        return;

    if (requestState == 2)
    {
        C_0085fe28_HandleGatheringResponse(self, nullptr);
        request = reinterpret_cast<void*>(self->field_0090);
    }
    else if (requestState == 3)
    {
        const std::uint32_t adjustedResult =
            CSF_AT(std::uint32_t, request, 0x44) + 2147287039u;
        self->field_02b0 = 1229731384;
        if (adjustedResult == 1)
            self->field_02b0 = 1229731385;
    }
    else
    {
        self->field_02b0 = 1229729848;
    }

    if (request)
    {
        reinterpret_cast<void (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, request, 0)[0])(request, nullptr, 1);
        self->field_0090 = 0;
    }

    self->field_000c = 0;
    self->field_0334 = 0;
    self->field_0008 = 2;
}

HOOK(0x006AAC40, C_0085fe28_UpdateGatheringRequest, "auto");

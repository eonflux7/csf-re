#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalValue_521BB0, 0x00521BB0, ());
CSF_FUNC(char, __fastcall, OrderManagerCanAcceptOrder, 0x00521C00, (void* self, void* edx));
CSF_FUNC(int, __fastcall, SubmitOrderRequest, 0x005242B0, (void* self, void* edx, void* a2, int a3));

static char __stdcall TrySubmitUnitOrder(int /*unusedArg1*/, int /*unusedArg2*/, int* orderSources)
{
    alignas(8) uint8_t request[0x38];
    CSF_AT(int32_t, request, 0x04) = 1;
    CSF_AT(uint8_t, request, 0x35) = 1;
    CSF_AT(uint8_t, request, 0x36) = 1;

    CSF_AT(int32_t, request, 0x18) = 0;
    CSF_AT(int32_t, request, 0x1C) = 0;
    CSF_AT(int32_t, request, 0x20) = 0;
    CSF_AT(uint32_t, request, 0x24) =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    CSF_AT(int32_t, request, 0x28) = -1;
    CSF_AT(int32_t, request, 0x2C) = -1;
    CSF_AT(int32_t, request, 0x10) = 0;
    CSF_AT(int32_t, request, 0x14) = 0;
    CSF_AT(uint8_t, request, 0x34) = 0;
    CSF_AT(int32_t, request, 0x08) = -1;
    CSF_AT(int32_t, request, 0x0C) = -1;
    CSF_AT(int32_t, request, 0x30) = 0;
    CSF_AT(int32_t, request, 0x00) = 2;

    uint8_t* firstSource = reinterpret_cast<uint8_t*>(orderSources[0]);
    uint8_t* firstSourceData = reinterpret_cast<uint8_t*>(CSF_AT(int32_t, firstSource, 4));
    const int32_t firstSourceId = CSF_AT(int32_t, firstSourceData, 4);
    if (firstSourceId == -1) {
        return static_cast<char>(firstSourceId);
    }

    CSF_AT(int32_t, request, 0x08) = firstSourceId;

    uint8_t* secondSource = reinterpret_cast<uint8_t*>(orderSources[1]);
    uint8_t* secondSourceData = reinterpret_cast<uint8_t*>(CSF_AT(int32_t, secondSource, 4));
    CSF_AT(int32_t, request, 0x28) = CSF_AT(int32_t, secondSourceData, 8);
    CSF_AT(int32_t, request, 0x2C) = CSF_AT(int32_t, secondSourceData, 12);

    uint8_t* thirdSource = reinterpret_cast<uint8_t*>(orderSources[2]);
    uint8_t* thirdSourceData = reinterpret_cast<uint8_t*>(CSF_AT(int32_t, thirdSource, 4));
    CSF_AT(int32_t, request, 0x10) = CSF_AT(int32_t, thirdSourceData, 4);
    CSF_AT(int32_t, request, 0x04) = 4;

    void* orderManager = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    const char canAccept = OrderManagerCanAcceptOrder(orderManager, nullptr);
    if (canAccept == 0) {
        return canAccept;
    }

    uint8_t* fourthSource = reinterpret_cast<uint8_t*>(orderSources[3]);
    uint8_t* fourthSourceData = reinterpret_cast<uint8_t*>(CSF_AT(int32_t, fourthSource, 4));
    const float orderParameter = CSF_AT(float, fourthSourceData, 4);

    alignas(8) uint8_t requestArgs[16];
    CSF_AT(float, requestArgs, 12) = orderParameter;
    CSF_AT(int32_t, requestArgs, 4) = CSF_AT(int32_t, request, 0x28);
    CSF_AT(uint32_t, requestArgs, 0) =
        static_cast<uint32_t>(reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448)));
    CSF_AT(int32_t, requestArgs, 8) = CSF_AT(int32_t, request, 0x2C);

    void* orderManagerForSubmit = reinterpret_cast<void*>(GetGlobalValue_521BB0());
    return static_cast<char>(SubmitOrderRequest(
        orderManagerForSubmit, nullptr, request,
        static_cast<int>(reinterpret_cast<uintptr_t>(requestArgs))));
}

HOOK(0x00461EF0, TrySubmitUnitOrder, "auto");

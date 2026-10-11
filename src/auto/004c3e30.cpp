#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest7, 0x004AC450, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest5, 0x004AC4B0, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest6, 0x004AC500, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest9, 0x004AC550, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest10, 0x004AC590, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest11, 0x004AC620, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest13, 0x004AC6B0, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest14, 0x004AC700, (void* a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest15, 0x004AC7A0, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest16, 0x004AC8D0, (void* a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest31, 0x004AC980, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest8, 0x004B7230, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest2, 0x004B72D0, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest3, 0x004B73C0, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest4, 0x004B7430, (int a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest12, 0x004B74C0, (int a1));
CSF_FUNC(bool, __stdcall, AiStateFactory_HandleRequest17, 0x004B7550, (void* a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest22, 0x004B75D0, (void* a1));
CSF_FUNC(bool, __stdcall, AiStateFactory_HandleRequest18, 0x004B7650, (void* a1));
CSF_FUNC(bool, __stdcall, AiStateFactory_HandleRequest19, 0x004B7700, (void* a1));
CSF_FUNC(bool, __stdcall, AiStateFactory_HandleRequest20, 0x004B77B0, (void* a1));
CSF_FUNC(bool, __stdcall, AiStateFactory_HandleRequest21, 0x004B7860, (void* a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest25, 0x004B7910, (void* a1));
CSF_FUNC(bool, __stdcall, AiStateFactory_HandleRequest26, 0x004B79D0, (void* a1));
CSF_FUNC(bool, __stdcall, AiStateFactory_HandleRequest27, 0x004B7A50, (void* a1));
CSF_FUNC(bool, __stdcall, AiStateFactory_HandleRequest28, 0x004B7B00, (void* a1));
CSF_FUNC(bool, __stdcall, AiStateFactory_HandleRequest29, 0x004B7BB0, (void* a1));
CSF_FUNC(bool, __stdcall, AiStateFactory_HandleRequest30, 0x004B7C60, (void* a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest33, 0x004B7D10, (void* a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest32, 0x004C03E0, (void* a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest23, 0x004C3C60, (void* a1));
CSF_FUNC(char, __stdcall, AiStateFactory_HandleRequest24, 0x004C3CF0, (void* a1));
CSF_FUNC(int, __fastcall, GetLinkedObjectValue_4DB5C0, 0x004DB5C0, (void* self, void*));
CSF_FUNC(int, __fastcall, GetLinkedObjectField10, 0x004DB5D0, (void* self, void*));
CSF_FUNC(int, __fastcall, Dispatch_NoLinkedTarget, 0x004DBCB0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __cdecl, _memcpy, 0x007AEE60, (void* destination, const void* source, std::size_t byteCount));

static char __stdcall AiStateFactory_ProcessQueuedRequests(
    C_0085e760* actor, int /*unused*/)
{
    auto* linked = reinterpret_cast<std::uint8_t*>(
        GetLinkedObjectValue_4DB5C0(actor, nullptr));
    auto* owner = linked ? linked - 4 : nullptr;
    void* component = CSF_AT(void*, owner, 0x2C);
    void* controller = CSF_AT(void*, component, 0x48);

    const int controllerResult =
        reinterpret_cast<int(__fastcall*)(void*, void*)>(
            CSF_AT(void**, controller, 0)[0x40 / 4])(controller, nullptr);
    if (controllerResult != 0)
        return static_cast<char>(controllerResult);

    const int actorArgument = reinterpret_cast<int>(actor);
    while (CSF_AT(std::uint32_t,
                  reinterpret_cast<void*>(GetLinkedObjectField10(actor, nullptr)),
                  0x6C) != 0)
    {
        void* linkedState =
            reinterpret_cast<void*>(GetLinkedObjectField10(actor, nullptr));
        int* requests = CSF_AT(int*, linkedState, 0x64);
        const int request = requests[0];
        const std::uint32_t remaining =
            CSF_AT(std::uint32_t, linkedState, 0x6C) - 1;

        CSF_AT(std::uint32_t, linkedState, 0x6C) = remaining;
        if (remaining != 0)
            _memcpy(requests, requests + 1, 4 * remaining);

        char result;
        switch (request)
        {
        case 2:  result = AiStateFactory_HandleRequest2(actorArgument); break;
        case 3:  result = AiStateFactory_HandleRequest3(actorArgument); break;
        case 4:  result = AiStateFactory_HandleRequest4(actorArgument); break;
        case 5:  result = AiStateFactory_HandleRequest5(actorArgument); break;
        case 6:  result = AiStateFactory_HandleRequest6(actorArgument); break;
        case 7:  result = AiStateFactory_HandleRequest7(actorArgument); break;
        case 8:  result = AiStateFactory_HandleRequest8(actorArgument); break;
        case 9:  result = AiStateFactory_HandleRequest9(actorArgument); break;
        case 10: result = AiStateFactory_HandleRequest10(actorArgument); break;
        case 11: result = AiStateFactory_HandleRequest11(actorArgument); break;
        case 12: result = AiStateFactory_HandleRequest12(actorArgument); break;
        case 13: result = AiStateFactory_HandleRequest13(actorArgument); break;
        case 14: result = AiStateFactory_HandleRequest14(actor); break;
        case 15: result = AiStateFactory_HandleRequest15(actorArgument); break;
        case 16: result = AiStateFactory_HandleRequest16(actor); break;
        case 17: result = AiStateFactory_HandleRequest17(actor); break;
        case 18: result = AiStateFactory_HandleRequest18(actor); break;
        case 19: result = AiStateFactory_HandleRequest19(actor); break;
        case 20: result = AiStateFactory_HandleRequest20(actor); break;
        case 21: result = AiStateFactory_HandleRequest21(actor); break;
        case 22: result = AiStateFactory_HandleRequest22(actor); break;
        case 23: result = AiStateFactory_HandleRequest23(actor); break;
        case 24: result = AiStateFactory_HandleRequest24(actor); break;
        case 25: result = AiStateFactory_HandleRequest25(actor); break;
        case 26: result = AiStateFactory_HandleRequest26(actor); break;
        case 27: result = AiStateFactory_HandleRequest27(actor); break;
        case 28: result = AiStateFactory_HandleRequest28(actor); break;
        case 29: result = AiStateFactory_HandleRequest29(actor); break;
        case 30: result = AiStateFactory_HandleRequest30(actor); break;
        case 31: result = AiStateFactory_HandleRequest31(actorArgument); break;
        case 32: result = AiStateFactory_HandleRequest32(actor); break;
        case 33: result = AiStateFactory_HandleRequest33(actor); break;
        default: continue;
        }

        if (result != 0)
            return result;
    }

    return static_cast<char>(Dispatch_NoLinkedTarget(actor, nullptr, 0));
}

HOOK(0x004C3E30, AiStateFactory_ProcessQueuedRequests, "auto");

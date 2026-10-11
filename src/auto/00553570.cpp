#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(char, __stdcall, sub_550CD0, 0x00550CD0, (uint32_t* a1));
CSF_FUNC(char, __stdcall, sub_550D30, 0x00550D30, (int a1));
CSF_FUNC(char, __stdcall, sub_550D90, 0x00550D90, (int a1));
CSF_FUNC(int, __stdcall, sub_550EA0, 0x00550EA0, (uint32_t* a1));
CSF_FUNC(char, __stdcall, sub_551000, 0x00551000, (uint32_t* a1));
CSF_FUNC(int, __stdcall, sub_551050, 0x00551050, (int a1));
CSF_FUNC(char, __stdcall, sub_5511A0, 0x005511A0, (uint32_t* a2));
CSF_FUNC(int, __stdcall, sub_551220, 0x00551220, (int a1));
CSF_FUNC(int, __stdcall, sub_551260, 0x00551260, (int a1));
CSF_FUNC(char*, __fastcall, sub_552000, 0x00552000, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_5522D0, 0x005522D0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, sub_5524A0, 0x005524A0, (void* self, void* edx, int a2));
CSF_FUNC(void*, __fastcall, sub_552530, 0x00552530, (void* self, void* edx, int a2));

static uint32_t* __fastcall C_00844694_DispatchSpawnRequest(
    C_00844694* self, void* /*edx*/, uint32_t* request)
{
    const int requestAddress =
        static_cast<int>(reinterpret_cast<uintptr_t>(request));

    // Byte-returning handlers define only the low byte of the result.
    // Their unspecified upper register bits are not reconstructed.
    switch (CSF_AT(uint32_t, request, 0x04))
    {
    case 4:
        return reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(static_cast<uint8_t>(sub_550D90(requestAddress))));
    case 9:
        return reinterpret_cast<uint32_t*>(sub_552000(self, nullptr, requestAddress));
    case 0xE:
        return reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(static_cast<uint8_t>(sub_551000(request))));
    case 0x10:
        return reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(static_cast<uint8_t>(sub_550CD0(request))));
    case 0x11:
        return reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(sub_550EA0(request)));
    case 0x14:
        return reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(sub_551050(requestAddress)));
    case 0x15:
        return reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(static_cast<uint8_t>(sub_5511A0(request))));
    case 0x16:
        return reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(static_cast<uint8_t>(sub_550D30(requestAddress))));
    case 0x17:
        return reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(sub_551220(requestAddress)));
    case 0x1F:
        return reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(sub_551260(requestAddress)));
    case 0x20:
        return reinterpret_cast<uint32_t*>(
            static_cast<uintptr_t>(sub_5522D0(self, nullptr, requestAddress)));
    case 0x21:
        return static_cast<uint32_t*>(sub_5524A0(self, nullptr, requestAddress));
    case 0x22:
        return static_cast<uint32_t*>(sub_552530(self, nullptr, requestAddress));
    default:
        return request;
    }
}

HOOK(0x00553570, C_00844694_DispatchSpawnRequest, "auto");

#include <cstdint>
#include "globals.h"
#include "hook.h"
CSF_FUNC(void, __fastcall, InitSelection, 0x0073D860, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Reset, 0x0064B880, (void* self, void* edx));
CSF_FUNC(void, __fastcall, Select, 0x0064B900, (void* self, void* edx, int mode));
CSF_FUNC(void, __fastcall, Assign, 0x00651C70, (void* self, void* edx, void* value));
CSF_FUNC(void, __fastcall, Construct, 0x00651A60, (void* self, void* edx, uint32_t value));
CSF_FUNC(void, __fastcall, Copy, 0x00651D30, (void* self, void* edx, void* source));
CSF_FUNC(void, __fastcall, Destroy, 0x00651B40, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, ResolveClass, 0x0064AD00, (uint32_t type));
CSF_FUNC(void, __cdecl, Notice, 0x00646D30, (int a, int b, int status, int d));
CSF_FUNC(uint32_t*, __fastcall, BuyNode, 0x0073CEB0, (void* self, void* edx, uint32_t* next, uint32_t* prev, void* value));
CSF_FUNC(uint32_t, __fastcall, IncSize, 0x0073FE80, (void* self, void* edx, uint32_t count));
CSF_FUNC(void, __fastcall, Attach, 0x0065E130, (void* self, void* edx, uint32_t iterator));
CSF_GLOBAL(uint32_t, ClassId, 0x008BC160);
static void __stdcall Collect_0073f4f0(void* output) {
    alignas(4) uint32_t selection[9];
    alignas(4) uint32_t temporary[3];
    alignas(4) uint32_t handle[3];
    uint32_t empty = 0;
    InitSelection(selection, nullptr);
    if (selection[0]) {
        Reset(selection, nullptr);
        Select(selection, nullptr, 1);
    } else Assign(selection + 1, nullptr, &empty);
    while (selection[2]) {
        Construct(temporary, nullptr, selection[1]);
        Copy(handle, nullptr, temporary);
        Destroy(temporary, nullptr);
        void* object = reinterpret_cast<void*>(handle[0]);
        if (!object) Notice(0, 0, static_cast<int>(0xA0030004u), 0);
        else {
            void* type = ResolveClass(CSF_AT(uint32_t, object, 0x2C) >> 22);
            if (!reinterpret_cast<uint8_t(__fastcall*)(void*, void*, uint32_t)>(CSF_AT(void**, type, 0)[0x3C / 4])(type, nullptr, ClassId)) {
                Notice(0, 0, static_cast<int>(0xE003000Cu), 0);
                object = nullptr;
            }
        }
        auto* head = CSF_AT(uint32_t*, output, 4);
        auto* node = BuyNode(output, nullptr, head, reinterpret_cast<uint32_t*>(head[1]), &object);
        IncSize(output, nullptr, 1);
        head[1] = reinterpret_cast<uint32_t>(node);
        *reinterpret_cast<uint32_t*>(node[1]) = reinterpret_cast<uint32_t>(node);
        Select(selection, nullptr, 0);
        Destroy(handle, nullptr);
    }
    Attach(selection, nullptr, 0);
    empty = 0;
    Assign(selection + 1, nullptr, &empty);
    Destroy(selection + 1, nullptr);
}
HOOK(0x0073F4F0, Collect_0073f4f0, "stl");

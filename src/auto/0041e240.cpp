#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, sub_41FB10, 0x0041FB10, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_41FE60, 0x0041FE60, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_421E80, 0x00421E80, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_423FD0, 0x00423FD0, (void* self, void* edx, int a2));
CSF_FUNC(uint32_t*, __fastcall, sub_424DE0, 0x00424DE0, (void* self, void* edx, int a2));
CSF_FUNC(int*, __fastcall, sub_425050, 0x00425050, (void* self, void* edx, int a2));
CSF_FUNC(int, __fastcall, sub_425560, 0x00425560, (void* self, void* edx, int a2));

static uint32_t* __cdecl TaskState_Create(int stateType, int param)
{
    uint32_t* result = nullptr;
    void* obj;

    switch ((unsigned)stateType)
    {
    case 0:
        obj = Mem_Alloc(52);
        if (!obj) return nullptr;
        return sub_41FE60(obj, nullptr, param);
    case 1:
        obj = Mem_Alloc(712);
        if (!obj) return nullptr;
        return (uint32_t*)sub_421E80(obj, nullptr, param);
    case 2:
        obj = Mem_Alloc(120);
        if (!obj) return nullptr;
        return (uint32_t*)sub_423FD0(obj, nullptr, param);
    case 3:
        obj = Mem_Alloc(32);
        if (!obj) return nullptr;
        return sub_424DE0(obj, nullptr, param);
    case 4:
        obj = Mem_Alloc(96);
        if (!obj) return nullptr;
        return (uint32_t*)sub_425050(obj, nullptr, param);
    case 5:
        obj = Mem_Alloc(76);
        if (!obj) return nullptr;
        return (uint32_t*)sub_41FB10(obj, nullptr, param);
    case 6:
        obj = Mem_Alloc(44);
        if (!obj) return nullptr;
        return (uint32_t*)sub_425560(obj, nullptr, param);
    default:
        return result;
    }
}

HOOK(0x0041E240, TaskState_Create, "auto");

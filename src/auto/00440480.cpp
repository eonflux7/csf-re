#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(C_0082d410*, __stdcall, CreateCampaignEntry, 0x0043F8E0,
         (C_0082d410* vtblArg, uint32_t arg2, C_00833908* arg3,
          int arg4, char arg5));
CSF_FUNC(char, __fastcall, Campaign_DestroySubtree, 0x0043F310,
         (void* self, void* edx, void** Block));
CSF_FUNC(void, __stdcall, RethrowCppException, 0x007AF666,
         (void* exceptionObject, void* throwInformation));

static C_0082d410* __fastcall S_00440480_CopySubtree(
    S_00440480* self, void* /*edx*/, int sourceAddress, uint32_t parent)
{
    void* source = reinterpret_cast<void*>(
        static_cast<uintptr_t>(sourceAddress));
    C_0082d410* result =
        reinterpret_cast<C_0082d410*>(self->field_0004);

    if (!CSF_AT(uint8_t, source, 0x2D)) {
        const uint32_t sentinel = self->field_0004;
        C_0082d410* entry = CreateCampaignEntry(
            reinterpret_cast<C_0082d410*>(sentinel),
            parent,
            reinterpret_cast<C_00833908*>(sentinel),
            static_cast<int>(reinterpret_cast<uintptr_t>(
                reinterpret_cast<uint8_t*>(source) + 0x0C)),
            static_cast<char>(CSF_AT(uint8_t, source, 0x2C)));

        if (CSF_AT(uint8_t, result, 0x2D))
            result = entry;

        try {
            entry->vtbl = reinterpret_cast<void**>(
                S_00440480_CopySubtree(
                    self, nullptr, CSF_AT(int, source, 0x00),
                    reinterpret_cast<uint32_t>(entry)));
            entry->field_0008 = reinterpret_cast<uint32_t>(
                S_00440480_CopySubtree(
                    self, nullptr, CSF_AT(int, source, 0x08),
                    reinterpret_cast<uint32_t>(entry)));
        } catch (...) {
            Campaign_DestroySubtree(
                self, nullptr, reinterpret_cast<void**>(result));
            RethrowCppException(nullptr, nullptr);
            __assume(0);
        }
    }

    return result;
}

HOOK(0x00440480, S_00440480_CopySubtree, "auto");

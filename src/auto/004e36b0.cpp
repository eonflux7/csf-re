#include <cstdint>
#include "globals.h"
#include <cstddef>
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CancelOwnerLink, 0x00493270, (void* self, void* edx));
CSF_FUNC(char, __fastcall, C_0083e248_RefreshState, 0x004E2EE0, (void* self, void* edx));

static char __fastcall C_0083e248_ResetAndReschedule(C_0083e248* self, void* /*edx*/)
{
    if (self->field_0018 == 3)
    {
        void* linkedObj = reinterpret_cast<void*>(self->field_000c);
        CancelOwnerLink(linkedObj, nullptr);

        // Set bit 1 in the flags of the object referenced by linkedObj->+0x34.
        uint32_t ownerObj = CSF_AT(uint32_t, linkedObj, 0x34);
        void* owner = reinterpret_cast<void*>(ownerObj);
        CSF_AT(uint32_t, owner, 0x18) |= 2u;

        // Notify the embedded subobject at linkedObj+4 (vtable slot 0x50 / 4 = 20).
        void* subObj = static_cast<uint8_t*>(reinterpret_cast<void*>(self->field_000c)) + 4;
        char notifyFlag = 0;
        auto notify = reinterpret_cast<void(__fastcall*)(void*, void*, char*)>(
            CSF_AT(void**, subObj, 0)[0x50 / 4]);
        notify(subObj, nullptr, &notifyFlag);
    }

    void* obj = reinterpret_cast<void*>(self->field_000c);
    self->field_0018 = 3;

    // Virtual slot 0x8C / 4 = 35: configure with (50, 0, -1, 0).
    auto configure = reinterpret_cast<void(__fastcall*)(void*, void*, int, int, int, int)>(
        CSF_AT(void**, obj, 0)[0x8C / 4]);
    configure(obj, nullptr, 50, 0, -1, 0);

    return C_0083e248_RefreshState(self, nullptr);
}

HOOK(0x004E36B0, C_0083e248_ResetAndReschedule, "auto");

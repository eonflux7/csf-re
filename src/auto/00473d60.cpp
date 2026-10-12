#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, StateRecord_Prepare, 0x00471E00, (uint32_t* a1));
CSF_FUNC(int, __cdecl, AllocateStateByte, 0x004723D0, (int a1));
CSF_FUNC(unsigned int, __fastcall, ResizeEntryTable, 0x00473AE0, (void* self, void* edx, unsigned int a2, int a3));
CSF_FUNC(int, __fastcall, MissionScript_ProcessSectionTable, 0x0069B460, (void* self, void* edx));

static unsigned int __fastcall S_0069b1c0_Apply(
    S_0069b1c0* self, void* /*edx*/, unsigned int count)
{
    StateRecord_Prepare(reinterpret_cast<uint32_t*>(self));
    unsigned int result = ResizeEntryTable(&self->field_000c, nullptr, count, 0);

    for (unsigned int i = 0; i < count; ++i)
    {
        void* entry = reinterpret_cast<void*>(
            MissionScript_ProcessSectionTable(
                reinterpret_cast<void*>(CSF_ADDR_DATA(0x008B02E8)), nullptr));
        if (entry)
        {
            CSF_AT(uint32_t, entry, 0x4) = 0;
            CSF_AT(void**, entry, 0x0) = CSF_VTBL(0x008313F8);
            CSF_AT(uint32_t, entry, 0x8) = 0xFFFFFFFF;
        }

        reinterpret_cast<void**>(self->field_0010)[i] = entry;
        const uint32_t state = self->field_0004;
        void* storedEntry = reinterpret_cast<void**>(self->field_0010)[i];
        auto* stateByte = reinterpret_cast<uint8_t*>(
            AllocateStateByte(static_cast<int>(state)));
        CSF_AT(uint8_t*, storedEntry, 0x4) = stateByte;
        if (stateByte)
            *stateByte = static_cast<uint8_t>(state);
        result = i + 1;
    }
    return result;
}

HOOK(0x00473D60, S_0069b1c0_Apply, "auto");

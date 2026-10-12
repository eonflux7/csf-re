#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, S_0041e5f0_GrowArrayCapacity, 0x0041E5F0, (S_0041e5f0* self, void* , int additionalCapacity));
CSF_FUNC(char, __fastcall, C_00831074_Reset, 0x00443310, (void* self, void* edx));
CSF_FUNC(unsigned int, __stdcall, Scene_SerializeNameList, 0x004E8060, (S_0041e5f0* a1, int a2));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));

static int __fastcall C_00831074_Serialize(
    C_00831074* self, void* /*edx*/, unsigned int stateAddress)
{
    // The packet also classifies the capacity-growth address as a vtable.
    (void)CSF_VTBL(0x0041E5F0);

    auto* state = reinterpret_cast<std::uint8_t*>(stateAddress);

    bool failed = false;
    if (CSF_AT(std::uint8_t, state, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        failed = !((bool(__fastcall*)(void*, void*, int, unsigned int))
            CSF_AT(void**, serializer, 0)[0x80 / 4])(
                serializer, nullptr, 0, stateAddress);
    }

    const std::uint32_t savedContext = CSF_AT(std::uint32_t, state, 0x1C);
    const bool reading = CSF_AT(std::uint8_t, state, 0x14) == 0;
    CSF_AT(std::uint8_t, state, 0x00) = failed;
    if (reading && !failed)
        C_00831074_Reset(self, nullptr);

    std::uint32_t count = self->field_0014;
    bool active = CSF_AT(std::uint8_t, state, 0x00) == 0;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;
    if (active)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0x00) =
            ((int(__fastcall*)(void*, void*, std::uint32_t*, unsigned int))
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &count, stateAddress) == -1;
    }

    if (CSF_AT(std::uint8_t, state, 0x14) == 0 &&
        CSF_AT(std::uint8_t, state, 0x00) == 0)
    {
        self->field_0014 = 0;
        auto* entries = reinterpret_cast<S_0041e5f0*>(
            reinterpret_cast<std::uint8_t*>(self) + 0x0C);

        if (count > self->field_0010)
        {
            S_0041e5f0_GrowArrayCapacity(
                entries, nullptr, static_cast<int>(count - self->field_0010));
        }

        if (count != 0)
        {
            std::uint32_t index = 0;
            do
            {
                void* entry = Mem_Alloc(44);
                if (entry)
                {
                    CSF_AT(void**, entry, 0x00) = CSF_VTBL(0x00831058);
                    CSF_AT(int, entry, 0x04) = -1;
                    CSF_AT(int, entry, 0x08) = -1;
                    CSF_AT(std::uint32_t, entry, 0x0C) = 0;
                    CSF_AT(std::uint32_t, entry, 0x10) = 0;
                    CSF_AT(std::uint32_t, entry, 0x14) = 0;
                    CSF_AT(std::uint32_t, entry, 0x18) = 0;
                    CSF_AT(std::uint32_t, entry, 0x1C) = 0;
                    CSF_AT(std::uint8_t, entry, 0x20) = 0;
                    CSF_AT(std::uint32_t, entry, 0x24) = 0;
                    CSF_AT(std::uint32_t, entry, 0x28) = 0;
                }

                if (entries->field_0008 >= entries->field_0004)
                {
                    S_0041e5f0_GrowArrayCapacity(
                        entries, nullptr, static_cast<int>(entries->field_000c));
                }

                const std::uint32_t length = entries->field_0008;
                auto* destination =
                    reinterpret_cast<std::uint32_t*>(entries->field_0000) + length;
                entries->field_0008 = length + 1;
                *destination = reinterpret_cast<std::uint32_t>(entry);
                ++index;
            } while (index < count);
        }
    }

    active = CSF_AT(std::uint8_t, state, 0x00) == 0;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;
    if (active)
    {
        CSF_AT(std::uint8_t, state, 0x15) = 1;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        Scene_SerializeNameList(
            reinterpret_cast<S_0041e5f0*>(
                reinterpret_cast<std::uint8_t*>(self) + 0x0C),
            static_cast<int>(stateAddress));
    }

    active = CSF_AT(std::uint8_t, state, 0x00) == 0;
    CSF_AT(std::uint32_t, state, 0x1C) = 0;
    if (active)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        CSF_AT(std::uint8_t, state, 0x15) = 0;
        CSF_AT(std::uint8_t, state, 0x16) = 0;
        CSF_AT(std::uint8_t, state, 0x00) =
            ((int(__fastcall*)(void*, void*, std::uint32_t*, unsigned int))
                CSF_AT(void**, serializer, 0)[0x48 / 4])(
                    serializer, nullptr, &self->field_0008, stateAddress) == -1;
    }

    CSF_AT(std::uint32_t, state, 0x1C) = savedContext;
    if (CSF_AT(std::uint8_t, state, 0x00) == 0)
    {
        void* serializer = CSF_AT(void*, state, 0x04);
        const int result =
            ((int(__fastcall*)(void*, void*, unsigned int))
                CSF_AT(void**, serializer, 0)[0x7C / 4])(
                    serializer, nullptr, stateAddress);
        if (static_cast<std::uint8_t>(result) != 0)
        {
            CSF_AT(std::uint8_t, state, 0x00) = 0;
            return result;
        }
    }

    CSF_AT(std::uint8_t, state, 0x00) = 1;
    return 1;
}

HOOK(0x004435D0, C_00831074_Serialize, "auto");

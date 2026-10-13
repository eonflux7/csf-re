#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_488830, 0x00488830, (void* self, void* edx, void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int*, __fastcall, S_004f21d0_ReserveRangeAndGetCursor, 0x004F21D0, (S_004f21d0* self, void* , int* a2, int a3, unsigned int a4));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, SetAllocationMode, 0x006910B0, (int mode));
CSF_FUNC(void*, __fastcall, TextInputSubobject_Init, 0x00697130, (S_004f21d0* self, void* , uint8_t* a2));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static void* __fastcall CMenuEula_ResetText(CMenuEula* self, void* /*edx*/)
{
    void* owner = reinterpret_cast<void*>(self->field_0014);
    reinterpret_cast<void(__fastcall*)(void*, void*, CMenuEula*, std::uint32_t)>(
        CSF_AT(void**, owner, 0)[0x60 / 4])(owner, nullptr, self, 0);

    auto* text = reinterpret_cast<S_004f21d0*>(
        reinterpret_cast<uint8_t*>(self) + 0x3C);
    TextInputSubobject_Init(text, nullptr, &g_byte_82E65C);

    alignas(8) uint8_t savedTextBytes[0x1C];
    alignas(8) uint8_t cursorBytes[0x4];

    CSF_AT(std::uint32_t, savedTextBytes, 0x18) = 7;
    CSF_AT(std::uint32_t, savedTextBytes, 0x14) = 0;
    CSF_AT(std::uint16_t, savedTextBytes, 0x4) = 0;
    AssignStringRange_488830(savedTextBytes, nullptr, text, 0, 0xFFFFFFFF);

    const std::uint32_t capacity = CSF_AT(std::uint32_t, self, 0x54);
    auto* inlineBuffer = reinterpret_cast<uint8_t*>(self) + 0x40;
    void* endBase = capacity < 8
        ? static_cast<void*>(inlineBuffer)
        : CSF_AT(void*, self, 0x40);
    const unsigned int endAddress =
        reinterpret_cast<std::uintptr_t>(endBase) +
        2 * CSF_AT(std::uint32_t, self, 0x50);
    void* begin = capacity < 8
        ? static_cast<void*>(inlineBuffer)
        : CSF_AT(void*, self, 0x40);

    // The packet also lists this callee address as a vtable reference.
    (void)CSF_VTBL(0x004F21D0);
    S_004f21d0_ReserveRangeAndGetCursor(
        text, nullptr, reinterpret_cast<int*>(cursorBytes),
        static_cast<int>(reinterpret_cast<std::uintptr_t>(begin)), endAddress);

    SetAllocationMode(2);
    AssignStringRange_488830(text, nullptr, savedTextBytes, 0, 0xFFFFFFFF);
    void* result = reinterpret_cast<void*>(
        static_cast<std::uintptr_t>(SetAllocationMode(0)));

    void* textControl = reinterpret_cast<void*>(self->m_textControl);
    self->field_0160 = 0;
    if (textControl)
    {
        result = reinterpret_cast<void*(__fastcall*)(void*, void*)>(
            CSF_AT(void**, textControl, 0)[0x110 / 4])(textControl, nullptr);
        if (result)
        {
            const int status =
                reinterpret_cast<int(__fastcall*)(void*, void*, uint8_t*)>(
                    CSF_AT(void**, result, 0)[0x3C / 4])(
                        result, nullptr, &g_byte_82E65C);
            result = reinterpret_cast<void*>(static_cast<std::uintptr_t>(status));
        }
    }

    if (CSF_AT(std::uint32_t, savedTextBytes, 0x18) >= 8)
    {
        result = CSF_AT(void*, savedTextBytes, 0x4);
        if (result)
            return reinterpret_cast<void*>(
                static_cast<std::uintptr_t>(Mem_FreeDebug(result)));
    }
    return result;
}

HOOK(0x005338D0, CMenuEula_ResetText, "auto");

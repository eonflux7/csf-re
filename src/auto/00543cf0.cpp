#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringSubstring, 0x004109E0, (void* self, void* edx, const void* source, std::uint32_t position, std::uint32_t count));
CSF_FUNC(int, __fastcall, CompareStringSubstring, 0x0042E200, (void* string, void* edx, unsigned int position, unsigned int count, const char* other, unsigned int otherCount));
CSF_FUNC(int, __cdecl, GetGlobalValue_8B056C, 0x00506BD0, ());
CSF_FUNC(int, __fastcall, COptionsFile_ReleaseFourSubObjects, 0x0050BC30, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CMenuMain_InitializeScreen, 0x0053A9F0, (void* self, void* edx, int a2, int a3));
CSF_FUNC(int, __cdecl, Mem_FreeDebug, 0x00690E60, (void* block));
CSF_FUNC(int, __cdecl, Text_ConvertToDisplayEncoding, 0x00692580, (int text, int flag));
CSF_FUNC(void*, __fastcall, ConstructWideString, 0x00696F50, (void* self, void* edx, wchar_t* String));
CSF_FUNC(uint32_t*, __fastcall, SaveSlots_ApplyIndex, 0x00697020, (S_004f21d0* self, void* , int16_t a2));
CSF_FUNC(void*, __fastcall, AppendNarrowToWideString, 0x00697150, (void* self, void* , uint8_t* a2));
CSF_FUNC(void, __fastcall, C_00720DF0_ForwardCallToChildVtblSlot16, 0x00720DF0, (void* self, void* edx, void* text));
CSF_FUNC(int, __cdecl, GetGlobalValue_75FDF0, 0x0075FDF0, ());

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);

static int __fastcall CMenuMainPC_InitializeScreen(
    CMenuMainPC* self, void* /*edx*/, int a2, int a3)
{
    const int result = CMenuMain_InitializeScreen(self, nullptr, a2, a3);

    if (self->field_003c)
    {
        void* globals = reinterpret_cast<void*>(GetGlobalValue_8B056C());
        COptionsFile_ReleaseFourSubObjects(
            CSF_AT(void*, globals, 0x18), nullptr);
        self->field_003c = 0;
    }

    void* controls = reinterpret_cast<void*>(self->field_0018);
    void* copyrightControl =
        ((void* (__fastcall*)(void*, void*, const char*))
            CSF_AT(void**, controls, 0)[0xFC / 4])(
                controls, nullptr, CSF_STR(0x0084388C, "StaticCopyRight"));

    if (copyrightControl)
    {
        void* globals = reinterpret_cast<void*>(GetGlobalValue_75FDF0());

        alignas(8) uint8_t narrowBytes[0x1C];
        C_0082d410& narrow = *reinterpret_cast<C_0082d410*>(narrowBytes);
        CSF_AT(unsigned int, narrowBytes, 0x18) = 15;
        CSF_AT(unsigned int, narrowBytes, 0x14) = 0;
        reinterpret_cast<uint8_t*>(&narrow.field_0004)[0] = 0;

        AssignStringSubstring(
            &narrow, nullptr, static_cast<uint8_t*>(globals) + 0xF4,
            0, static_cast<std::uint32_t>(-1));

        if (CompareStringSubstring(
                &narrow, nullptr, 0,
                CSF_AT(unsigned int, narrowBytes, 0x14),
                reinterpret_cast<const char*>(&g_byte_82E65C), 0))
        {
            wchar_t* prefix = reinterpret_cast<wchar_t*>(
                Text_ConvertToDisplayEncoding(
                    reinterpret_cast<int>(CSF_STR(0x00843884, "cop0")), 1));

            alignas(8) uint8_t wideBytes[0x1C];
            ConstructWideString(wideBytes, nullptr, prefix);
            SaveSlots_ApplyIndex(
                reinterpret_cast<S_004f21d0*>(wideBytes), nullptr, 32);

            uint8_t* suffix =
                CSF_AT(unsigned int, narrowBytes, 0x18) < 0x10
                    ? reinterpret_cast<uint8_t*>(&narrow.field_0004)
                    : reinterpret_cast<uint8_t*>(narrow.field_0004);
            AppendNarrowToWideString(wideBytes, nullptr, suffix);

            void* displayText =
                CSF_AT(unsigned int, wideBytes, 0x18) < 8
                    ? static_cast<void*>(wideBytes + 0x4)
                    : CSF_AT(void*, wideBytes, 0x4);
            C_00720DF0_ForwardCallToChildVtblSlot16(
                copyrightControl, nullptr, displayText);

            if (CSF_AT(unsigned int, wideBytes, 0x18) >= 8 &&
                CSF_AT(void*, wideBytes, 0x4))
            {
                Mem_FreeDebug(CSF_AT(void*, wideBytes, 0x4));
            }
        }

        if (CSF_AT(unsigned int, narrowBytes, 0x18) >= 0x10 &&
            narrow.field_0004)
        {
            Mem_FreeDebug(reinterpret_cast<void*>(narrow.field_0004));
        }
    }

    return result;
}

HOOK(0x00543CF0, CMenuMainPC_InitializeScreen, "auto");

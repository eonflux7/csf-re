#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, Scene_SerializeFirstList, 0x00408C80,
         (uint32_t* a1, int a2));
CSF_FUNC(int, __stdcall, Scene_SerializeThirdList, 0x00408ED0,
         (int* a1, int a2));
CSF_FUNC(unsigned int, __stdcall, Scene_SerializeNameList, 0x004E8060,
         (void* a1, int a2));

static int __fastcall S_004090c0_SerializeLists(
    S_004090c0* self, void* /*edx*/, int contextAddress)
{
    void* context = reinterpret_cast<void*>(contextAddress);

    bool failed = false;
    if (CSF_AT(uint8_t, context, 0) == 0) {
        void* stream = CSF_AT(void*, context, 4);
        failed = !reinterpret_cast<bool (__fastcall*)(void*, void*, int, int)>(
            CSF_AT(void**, stream, 0)[0x80 / 4])(
                stream, nullptr, 0, contextAddress);
    }

    uint32_t savedState = CSF_AT(uint32_t, context, 0x1C);
    CSF_AT(uint8_t, context, 0) = failed;
    CSF_AT(uint32_t, context, 0x1C) = 0;

    if (!failed) {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Scene_SerializeFirstList(
            reinterpret_cast<uint32_t*>(
                reinterpret_cast<uint8_t*>(self) + 8),
            contextAddress);
    }

    bool active = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (active) {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Scene_SerializeNameList(
            reinterpret_cast<uint8_t*>(self) + 0x18, contextAddress);
    }

    active = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = 0;
    if (active) {
        CSF_AT(uint8_t, context, 0x15) = 0;
        CSF_AT(uint8_t, context, 0x16) = 0;
        Scene_SerializeThirdList(
            reinterpret_cast<int*>(
                reinterpret_cast<uint8_t*>(self) + 0x28),
            contextAddress);
    }

    active = CSF_AT(uint8_t, context, 0) == 0;
    CSF_AT(uint32_t, context, 0x1C) = savedState;
    if (active) {
        void* stream = CSF_AT(void*, context, 4);
        int result = reinterpret_cast<int (__fastcall*)(void*, void*, int)>(
            CSF_AT(void**, stream, 0)[0x7C / 4])(
                stream, nullptr, contextAddress);
        if (static_cast<uint8_t>(result) != 0) {
            CSF_AT(uint8_t, context, 0) = 0;
            return result;
        }
    }

    CSF_AT(uint8_t, context, 0) = 1;
    return 1;
}

HOOK(0x004090C0, S_004090c0_SerializeLists, "auto");

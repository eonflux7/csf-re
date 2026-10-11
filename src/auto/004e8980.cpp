#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __stdcall, SceneLoader_SerializeRecordMember, 0x00445620, (void* a1, int a2));
CSF_FUNC(int, __fastcall, sub_4E6B70, 0x004E6B70, (void* self, void* edx, int a2));
CSF_FUNC(unsigned int, __stdcall, Scene_SerializeNameList, 0x004E8060, (void* a1, int a2));

static int __fastcall S_004e6b70_SerializeAll(S_004e6b70* self, void* /*edx*/, int a2)
{
    uint8_t* base = reinterpret_cast<uint8_t*>(self);
    int savedField1C;
    bool flag;

    flag = false;
    if (CSF_AT(uint8_t, a2, 0) == 0) {
        void** obj = CSF_AT(void**, a2, 4);
        bool r = ((bool(__fastcall*)(void*, void*, int, int))CSF_AT(void**, obj, 0)[0x80 / 4])(obj, nullptr, 0, a2);
        flag = !r;
    }
    savedField1C = CSF_AT(int, a2, 0x1C);
    CSF_AT(uint8_t, a2, 0) = flag ? 1 : 0;

    sub_4E6B70(self, nullptr, a2);

    {
        bool z = CSF_AT(uint8_t, a2, 0) == 0;
        CSF_AT(int, a2, 0x1C) = 0;
        if (z) {
            CSF_AT(uint8_t, a2, 0x15) = 0;
            CSF_AT(uint8_t, a2, 0x16) = 0;
            Scene_SerializeNameList(base + 0x4C, a2);
        }
    }
    {
        bool z = CSF_AT(uint8_t, a2, 0) == 0;
        CSF_AT(int, a2, 0x1C) = 0;
        if (z) {
            CSF_AT(uint8_t, a2, 0x15) = 0;
            CSF_AT(uint8_t, a2, 0x16) = 0;
            Scene_SerializeNameList(base + 0x5C, a2);
        }
    }
    {
        bool z = CSF_AT(uint8_t, a2, 0) == 0;
        CSF_AT(int, a2, 0x1C) = 0;
        if (z) {
            CSF_AT(uint8_t, a2, 0x15) = 0;
            CSF_AT(uint8_t, a2, 0x16) = 0;
            Scene_SerializeNameList(base + 0x6C, a2);
        }
    }
    {
        bool z = CSF_AT(uint8_t, a2, 0) == 0;
        CSF_AT(int, a2, 0x1C) = 0;
        if (z) {
            CSF_AT(uint8_t, a2, 0x15) = 0;
            CSF_AT(uint8_t, a2, 0x16) = 0;
            Scene_SerializeNameList(base + 0x7C, a2);
        }
    }
    {
        bool z = CSF_AT(uint8_t, a2, 0) == 0;
        CSF_AT(int, a2, 0x1C) = 0;
        if (z) {
            CSF_AT(uint8_t, a2, 0x15) = 0;
            CSF_AT(uint8_t, a2, 0x16) = 0;
            SceneLoader_SerializeRecordMember(base + 0x8C, a2);
        }
    }
    {
        bool z = CSF_AT(uint8_t, a2, 0) == 0;
        CSF_AT(int, a2, 0x1C) = 0;
        if (z) {
            void** obj = CSF_AT(void**, a2, 4);
            CSF_AT(uint8_t, a2, 0x15) = 0;
            CSF_AT(uint8_t, a2, 0x16) = 0;
            int r = ((int(__fastcall*)(void*, void*, void*, int))CSF_AT(void**, obj, 0)[0x54 / 4])(obj, nullptr, base + 0x9C, a2);
            CSF_AT(uint8_t, a2, 0) = (r == -1) ? 1 : 0;
        }
    }
    CSF_AT(int, a2, 0x1C) = savedField1C;

    if (CSF_AT(uint8_t, a2, 0) == 0) {
        void** obj = CSF_AT(void**, a2, 4);
        int r = ((int(__fastcall*)(void*, void*, int))CSF_AT(void**, obj, 0)[0x7C / 4])(obj, nullptr, a2);
        if ((r & 0xFF) != 0) {
            CSF_AT(uint8_t, a2, 0) = 0;
            return r;
        }
    }
    CSF_AT(uint8_t, a2, 0) = 1;
    return 1;
}

HOOK(0x004E8980, S_004e6b70_SerializeAll, "auto");

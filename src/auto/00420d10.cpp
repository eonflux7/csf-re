#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, GetObjectPositionVector, 0x0048E430, (void* self, void* edx));
CSF_FUNC(int, __fastcall, TCharTask_Sub4DB440, 0x004DB440, (void* self, void* edx, int a2));

static int __fastcall C_00834fa8_Activate(C_00834fa8* self, void* /*edx*/, int a2, int a3)
{
    void* owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 8));
    void* inner = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
    void* child = reinterpret_cast<void*>(CSF_AT(uint32_t, inner, 0x48));

    void* current = ((void*(__fastcall*)(void*, void*))CSF_AT(void**, child, 0)[0x40 / 4])(child, nullptr);
    if (current == owner) {
        inner = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
        child = reinterpret_cast<void*>(CSF_AT(uint32_t, inner, 0x48));
        ((void(__fastcall*)(void*, void*, void*))CSF_AT(void**, child, 0)[0x3c / 4])(child, nullptr, owner);
    }

    owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 8));
    inner = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
    child = reinterpret_cast<void*>(CSF_AT(uint32_t, inner, 0x48));
    bool flag = ((bool(__fastcall*)(void*, void*, int))CSF_AT(void**, child, 0)[0x24 / 4])(child, nullptr, 1);
    if (flag) {
        owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 8));
        inner = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
        ((void(__fastcall*)(void*, void*, int, int, int, int))CSF_AT(void**, inner, 0)[0x8c / 4])(inner, nullptr, 0, 0, -1, 0);
    }

    owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 8));
    inner = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
    void* sub = reinterpret_cast<uint8_t*>(inner) + 4;
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, sub, 0)[0x10 / 4])(sub, nullptr);

    owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 8));
    CSF_AT(uint32_t, owner, 0x18) = CSF_AT(uint32_t, owner, 0x18) | 4u;

    owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 8));
    void* posObj = reinterpret_cast<void*>(CSF_AT(uint32_t, owner, 0x14));
    uint32_t* pos = reinterpret_cast<uint32_t*>(GetObjectPositionVector(posObj, nullptr));

    owner = reinterpret_cast<void*>(CSF_AT(uint32_t, self, 8));
    uint32_t* dst = reinterpret_cast<uint32_t*>(reinterpret_cast<uint8_t*>(owner) + 0x218);
    dst[0] = pos[0];
    dst[1] = pos[1];
    dst[2] = pos[2];

    return TCharTask_Sub4DB440(self, nullptr, CSF_AT(int, self, 0xc) + 32);
}

HOOK(0x00420D10, C_00834fa8_Activate, "auto");

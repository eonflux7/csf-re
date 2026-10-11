#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, SceneObj_IsInactive, 0x0048E100, (void* self, void* edx));
CSF_FUNC(int, __fastcall, TAutonomo_Update, 0x004D73E0, (void* self, void* edx));
CSF_FUNC(int, __fastcall, CharTaskBase_Prepare, 0x004DB110, (void* self, void* edx));
CSF_FUNC(unsigned int, __fastcall, DispatchWithArgBlock, 0x004DD060, (void* self, void* edx, uint32_t* a2));
CSF_FUNC(int, __cdecl, GetValue8B0554, 0x004E8C50, ());

static uint32_t __fastcall TCharTask_PrepareAndResetOwner(void* self, void* /*edx*/)
{
    CharTaskBase_Prepare(self, nullptr);

    uint8_t* owner = CSF_AT(uint8_t*, self, 0x8);
    if (static_cast<uint8_t>(SceneObj_IsInactive(CSF_AT(void*, owner, 0x14), nullptr)) == 0)
        TAutonomo_Update(CSF_AT(uint8_t*, self, 0x8) + 0x224, nullptr);

    uint8_t* obj = CSF_AT(uint8_t*, self, 0x8);

    void* sub30c = CSF_AT(void*, obj, 0x30C);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, sub30c, 0)[0x90 / 4])(sub30c, nullptr);

    void* sub3c = CSF_AT(void*, CSF_AT(uint8_t*, obj, 0x14), 0x3C);
    if (sub3c)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, sub3c, 0)[0x94 / 4])(sub3c, nullptr);

    void* sub40 = CSF_AT(void*, CSF_AT(uint8_t*, obj, 0x14), 0x40);
    if (sub40)
        ((void(__fastcall*)(void*, void*))CSF_AT(void**, sub40, 0)[0x80 / 4])(sub40, nullptr);

    uint8_t* pos = CSF_AT(uint8_t*, CSF_AT(uint8_t*, self, 0x8), 0x14) + 0x1C;
    uint32_t args[8];
    args[0] = 0;
    args[1] = reinterpret_cast<uintptr_t>(CSF_VTBL(0x0082D448));
    args[2] = CSF_AT(uint32_t, pos, 4);
    args[3] = CSF_AT(uint32_t, pos, 8);
    args[4] = 0;
    args[5] = 0;
    args[6] = 0;

    int global = GetValue8B0554();
    DispatchWithArgBlock(CSF_AT(uint8_t*, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(global)), 0x1FD8) + 0x54, nullptr, args);

    void* sub4 = CSF_AT(uint8_t*, CSF_AT(uint8_t*, self, 0x8), 0x14) + 4;
    char flag = 0;
    ((void(__fastcall*)(void*, void*, char*))CSF_AT(void**, sub4, 0)[0x50 / 4])(sub4, nullptr, &flag);

    uint8_t* o2 = CSF_AT(uint8_t*, self, 0x8);
    CSF_AT(uint32_t, o2, 0x18) = CSF_AT(uint32_t, o2, 0x18) | 2u;

    uint8_t* o3 = CSF_AT(uint8_t*, self, 0x8);
    CSF_AT(uint32_t, CSF_AT(uint8_t*, CSF_AT(uint8_t*, o3, 0x14), 0x30), 0x1C) = 0;
    return reinterpret_cast<uintptr_t>(o3);
}

HOOK(0x004D9060, TCharTask_PrepareAndResetOwner, "auto");

#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static int __fastcall C_008342e8_SelectPlacement(C_008342e8* self, void* /*edx*/, int a2)
{
    int v2 = 28 * a2;

    int resolvedHandle = CHandleTable_Resolve(g_pCurrentScene, nullptr, static_cast<int>(v2 + self->field_0198));
    uint32_t entryAddr = static_cast<uint32_t>(v2) + self->field_0198;
    if (resolvedHandle != 0 &&
        ((*reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(entryAddr + 24)) >> 3) & 1) != 0)
    {
        void* resolvedObj = reinterpret_cast<void*>(static_cast<uintptr_t>(static_cast<uint32_t>(resolvedHandle)));
        using NoArgFn = void(__fastcall*)(void*, void*);
        reinterpret_cast<NoArgFn>(CSF_AT(void**, resolvedObj, 0)[30])(resolvedObj, nullptr);

        entryAddr = static_cast<uint32_t>(v2) + self->field_0198;
        *reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(entryAddr + 24)) &= ~8u;
    }

    void* placementHandler = reinterpret_cast<void*>(static_cast<uintptr_t>(self->field_0034));
    if (placementHandler != nullptr)
    {
        using OneArgFn = void(__fastcall*)(void*, void*, int);
        reinterpret_cast<OneArgFn>(CSF_AT(void**, placementHandler, 0)[57])(placementHandler, nullptr, a2);
    }

    entryAddr = static_cast<uint32_t>(v2) + self->field_0198;
    uint32_t* entryWords = reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(entryAddr));
    entryWords[1] = 0xFFFFFFFFu;
    entryWords[2] = 0xFFFFFFFFu;

    int result = static_cast<int>(self->field_0198);
    entryAddr = static_cast<uint32_t>(v2) + self->field_0198;
    if ((*reinterpret_cast<uint32_t*>(static_cast<uintptr_t>(entryAddr + 24)) & 4) != 0)
    {
        self->field_0170 |= 0x200u;
    }
    return result;
}

HOOK(0x0049F220, C_008342e8_SelectPlacement, "auto");

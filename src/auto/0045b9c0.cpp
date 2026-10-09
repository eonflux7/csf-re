#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

// 0x00444940: __fastcall(self, edx, char); callee pops its one stack arg.
CSF_FUNC(int, __fastcall, NotifySceneByteChanged, 0x00444940, (void* self, void* edx, char a2));

// __thiscall(this, a2, a3, a4) with ret 12 is written as __fastcall: ecx=self, edx unused, 3 stack args.
static int __fastcall StoreSecondByteAndNotifyScene(void* self, void* /*edx*/, int a2, int a3, uint32_t a4)
{
    (void)a2;
    (void)a3;

    // v4 = *(BYTE*)( *(DWORD*)( *(DWORD*)a4 ) + 4 ) + 1 )
    void* argPtr = reinterpret_cast<void*>(a4);
    uint32_t pointee = CSF_AT(uint32_t, argPtr, 0);
    uint32_t inner = CSF_AT(uint32_t, reinterpret_cast<void*>(pointee), 4);
    uint8_t v4 = CSF_AT(uint8_t, reinterpret_cast<void*>(inner), 1);

    // this->field_0083 = v4;
    CSF_AT(uint8_t, self, 0x83) = v4;

    // ecx = g_pCurrentScene->[+0xC0]; edx still holds *(a4) from the original.
    void* currentScene = g_pCurrentScene;
    void* sceneChild = CSF_AT(void*, currentScene, 0xC0);

    return NotifySceneByteChanged(sceneChild, reinterpret_cast<void*>(pointee), static_cast<char>(v4));
}

HOOK(0x0045B9C0, StoreSecondByteAndNotifyScene, "auto");

#include "globals.h"
#include "hook.h"

// CMenuSoundOptions_ChainVtableSlot74
// Naked tail-jump thunk reproducing the original's final `jmp dword ptr [eax + 0x74]`:
// forwards `this` (ecx) and the caller's stack untouched to the function stored in
// this's vtable slot 0x74. Signature unknown; returns int per the original prototype.
static __declspec(naked) int __fastcall CMenuSoundOptions_ChainVtableSlot74(int* self)
{
    __asm {
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x74]
    }
}

// CMenuSoundOptions_ReleaseSoundAndChain
// Releases the sound sub-object held at this+0x2c: notifies it twice (vtable slots
// 0x1c with this+0x30, then 0x24 with this+0x34), runs this's own vtable slots 0x6c
// and 0x70, then tail-chains through this's vtable slot 0x74. If this+0x2c is null,
// does nothing and returns 0.
static int __fastcall CMenuSoundOptions_ReleaseSoundAndChain(int* self, void* /*edx*/)
{
    void* soundObject = CSF_AT(void*, self, 0x2c);
    if (soundObject == nullptr) {
        return 0;
    }

    int firstNotifyArg = CSF_AT(int, self, 0x30);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, soundObject, 0)[0x1c / 4])(
        soundObject, nullptr, firstNotifyArg);

    void* soundObjectAgain = CSF_AT(void*, self, 0x2c);
    int secondNotifyArg = CSF_AT(int, self, 0x34);
    ((void(__fastcall*)(void*, void*, int))CSF_AT(void**, soundObjectAgain, 0)[0x24 / 4])(
        soundObjectAgain, nullptr, secondNotifyArg);

    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x6c / 4])(self, nullptr);
    ((void(__fastcall*)(void*, void*))CSF_AT(void**, self, 0)[0x70 / 4])(self, nullptr);

    return CMenuSoundOptions_ChainVtableSlot74(self);
}

HOOK(0x0053CC70, CMenuSoundOptions_ReleaseSoundAndChain, "auto");

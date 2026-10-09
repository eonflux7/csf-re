#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void, __fastcall, ApplyGlobalValueSettings, 0x005233D0, (void* self, void* edx));
CSF_FUNC(int, __cdecl, GetGlobalValue_70DDE0, 0x0070DDE0, ());

static void* __fastcall CSoundManager_PrepareThirdManagerTarget(
    CSoundManager* self, void* /*edx*/)
{
    ApplyGlobalValueSettings(self, nullptr);
    return reinterpret_cast<void*>(GetGlobalValue_70DDE0());
}

static void* (__fastcall* g_PrepareThirdManagerTarget)(CSoundManager*, void*) =
    CSoundManager_PrepareThirdManagerTarget;

static __declspec(naked) int __fastcall CSoundManager_ResetThirdManager(
    CSoundManager* self, void* /*edx*/)
{
    __asm {
        mov eax, dword ptr [g_PrepareThirdManagerTarget]
        call eax
        mov edx, dword ptr [eax]
        mov ecx, eax
        jmp dword ptr [edx + 4]
    }
}

HOOK(0x00523640, CSoundManager_ResetThirdManager, "auto");

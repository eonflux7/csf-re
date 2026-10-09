#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(bool, __fastcall, sub_480B80, 0x00480B80, (void* self, void* edx));

static __declspec(naked) char __fastcall CCharActionFsm_TailDispatchAction(
    CCharActionFsm*, void*)
{
    __asm {
        mov eax, ecx
        mov edx, dword ptr [eax]
        mov ecx, eax
        jmp dword ptr [edx + 0x88]
    }
}

static char __fastcall CCharActionFsm_ResolveActionDispatch(
    CCharActionFsm* self, void* /*edx*/)
{
    if (static_cast<uint8_t>(self->vIsField000cNull()) == 0)
        return 0;

    int stateCode = 0;
    if (self->field_009c)
    {
        void* stateObject = reinterpret_cast<void*>(self->field_009c);
        stateCode =
            ((int(__fastcall*)(void*, void*))
                CSF_AT(void**, stateObject, 0)[0x4C / 4])(
                    stateObject, nullptr);
    }

    if (stateCode == 11)
        return 0;

    if (stateCode != 10)
        return sub_480B80(self, nullptr);

    CCharActionFsm* dispatchObject =
        reinterpret_cast<CCharActionFsm*>(self->vSlotDC());

    [[clang::musttail]] return CCharActionFsm_TailDispatchAction(
        dispatchObject, nullptr);
}

HOOK(0x004842B0, CCharActionFsm_ResolveActionDispatch, "auto");

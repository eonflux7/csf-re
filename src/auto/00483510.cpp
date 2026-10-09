#include <cstdint>
#include "hook.h"
#include "types/auto_classes.h"

static __declspec(naked) int __fastcall CCharActionFsm_ForwardResolvedObjectQuery(
    void* /*object*/, void* /*edx*/)
{
    __asm {
        mov eax, ecx
        mov edx, dword ptr [eax]
        mov ecx, eax
        jmp dword ptr [edx + 0x50]
    }
}

static int __fastcall CCharActionFsm_QueryCurrentAction(
    CCharActionFsm* self, void* /*edx*/)
{
    if (self->field_0024 == static_cast<uint32_t>(-1))
        return 0;

    return CCharActionFsm_ForwardResolvedObjectQuery(
        reinterpret_cast<void*>(self->vGetIndexedEntryOrZero_37()), nullptr);
}

HOOK(0x00483510, CCharActionFsm_QueryCurrentAction, "auto");

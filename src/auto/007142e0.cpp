#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, GetCachedObjectProvider, 0x00686C10,
         (void* self, void* edx));

static __declspec(naked) int __fastcall TailDispatchCachedObject(
    void* /*self*/, void* /*edx*/)
{
    __asm
    {
        mov ecx, dword ptr [ecx + 0x184]
        mov eax, dword ptr [ecx]
        jmp dword ptr [eax + 0x2c]
    }
}

static int __fastcall GetOrCreateCachedObjectAndTailDispatch(
    void* self, void* incomingEdx)
{
    void* dispatchEdx = incomingEdx;

    if (CSF_AT(std::uint32_t, self, 0x184) == 0)
    {
        void* provider = GetCachedObjectProvider(self, incomingEdx);
        void** providerVtable = CSF_AT(void**, provider, 0);

        // Capture EDX:EAX: EAX is the cached pointer, while EDX must
        // retain the register value left by this call for the final jump.
        const std::uint64_t returnedRegisters =
            ((std::uint64_t(__fastcall*)(void*, void*))
                 providerVtable[0x48 / 4])(provider, providerVtable);

        CSF_AT(std::uint32_t, self, 0x184) =
            static_cast<std::uint32_t>(returnedRegisters);
        dispatchEdx = reinterpret_cast<void*>(
            static_cast<std::uint32_t>(returnedRegisters >> 32));
    }

    // Mandatory tail call removes this frame without shifting caller arguments.
    [[clang::musttail]]
    return TailDispatchCachedObject(self, dispatchEdx);
}

HOOK(0x007142E0, GetOrCreateCachedObjectAndTailDispatch, "auto");

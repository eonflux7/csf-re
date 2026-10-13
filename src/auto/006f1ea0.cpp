#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

static char __fastcall C_00862490_SetTargetAndCopyMatrix(C_00862490* self, void* /*edx*/, int a2)
{
    void* notifier = reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0020));
    self->field_0024 = static_cast<std::uint32_t>(a2);

    // notifier->vtbl[15](a2), __thiscall
    using NotifyFn = void(__fastcall*)(void*, void*, int);
    ((NotifyFn)CSF_AT(void**, notifier, 0)[15])(notifier, nullptr, a2);

    // Frame object: dword at +0 (matrix pointer), byte at +4 (flag). Callee fills it.
    alignas(8) std::uint8_t outBuffer[8];

    void* target = reinterpret_cast<void*>(static_cast<std::uintptr_t>(self->field_0024));
    using GetBlockFn = std::uint32_t(__fastcall*)(void*, void*, void*);
    std::uint32_t blockRef = ((GetBlockFn)CSF_AT(void**, target, 0)[35])(target, nullptr, outBuffer);

    // Dereference the returned reference, then take the block at +0x20.
    std::uintptr_t blockAddr = static_cast<std::uintptr_t>(*reinterpret_cast<std::uint32_t*>(blockRef)) + 0x20;
    void* block = reinterpret_cast<void*>(blockAddr);

    CSF_AT(std::uint32_t, self, 0x2C) = CSF_AT(std::uint32_t, block, 0x0);
    CSF_AT(std::uint32_t, self, 0x30) = CSF_AT(std::uint32_t, block, 0x4);
    CSF_AT(std::uint32_t, self, 0x34) = CSF_AT(std::uint32_t, block, 0x8);

    char result = static_cast<char>(outBuffer[4]);
    if (result) {
        std::uint32_t matrixValue = *reinterpret_cast<std::uint32_t*>(outBuffer);
        return static_cast<char>(RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixValue)));
    }
    return result;
}

HOOK(0x006F1EA0, C_00862490_SetTargetAndCopyMatrix, "auto");

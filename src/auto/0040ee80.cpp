#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));
CSF_GLOBAL(std::uint32_t, g_MatrixRequestFlags, 0x0082D6E8);

// Requests a 0x40-byte matrix from the object at field_001c (vtable slot 0x2C) with flags 0x201,
// copies it into the buffer pointed to by field_0020, and destroys the returned matrix if the
// callee's flag byte is set.
static char __fastcall C_0082ddf8_CopyMatrixAndReleaseIfFlagged(C_0082ddf8* self, void* /*edx*/)
{
    // Stack object: [ebp-8] = returned matrix pointer, [ebp-4] = returned flag byte.
    alignas(8) std::uint8_t requestFrame[8];
    const void** matrixOut = reinterpret_cast<const void**>(requestFrame);

    void* requestSource = reinterpret_cast<void*>(self->field_001c);
    using MatrixRequestFn = void(__fastcall*)(void* self, void* edx, const void** out, std::uint32_t flags);
    reinterpret_cast<MatrixRequestFn>(CSF_AT(void**, requestSource, 0)[0x2C / 4])(
        requestSource, nullptr, matrixOut, 0x201);

    // Destination: the buffer whose first dword is stored at field_0020.
    auto* matrixBlock = reinterpret_cast<std::uint32_t*>(self->field_0020);
    void* destination = reinterpret_cast<void*>(CSF_AT(std::uint32_t, matrixBlock, 0));
    const void* source = *matrixOut;
    std::memcpy(destination, source, 0x40u);

    char flag = static_cast<char>(requestFrame[4]);
    if (flag) {
        auto* matrix = reinterpret_cast<std::uint32_t*>(const_cast<void*>(*matrixOut));
        return static_cast<char>(RwMatrixDestroy(matrix));
    }
    return flag;
}

HOOK(0x0040EE80, C_0082ddf8_CopyMatrixAndReleaseIfFlagged, "auto");

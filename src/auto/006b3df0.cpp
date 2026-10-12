#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __cdecl, RwMatrixDestroy, 0x00624D10, (std::uint32_t* matrix));

// Releases the RwMatrix held in the first dword of the record when the
// record's owner flag (byte at +4) is set. Otherwise returns the record pointer.
static int __stdcall GfxObject_DestroyOwnedMatrix(int a1) {
    void* record = reinterpret_cast<void*>(a1);
    if (CSF_AT(std::uint8_t, record, 0x04)) {
        std::uint32_t matrixHandle = CSF_AT(std::uint32_t, record, 0x00);
        return RwMatrixDestroy(reinterpret_cast<std::uint32_t*>(matrixHandle));
    }
    return a1;
}

HOOK(0x6B3DF0, GfxObject_DestroyOwnedMatrix, "auto");

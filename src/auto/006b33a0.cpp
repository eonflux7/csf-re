#include <cstdint>
#include <cstddef>
#include <cstring>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(std::uint32_t*, __cdecl, RwMatrixCreate, 0x00624D40, ());

// Creates an RwMatrix (64 bytes) copied from *srcMatrixPtr and stores it at
// the first dword of the target record; the record's flag byte at +4 is set to 1.
static void __stdcall GfxObject_CreateMatrixCopy(int targetRecord, const void** srcMatrixPtr)
{
    if (targetRecord == 0)
        return;

    std::uint8_t* record = reinterpret_cast<std::uint8_t*>(static_cast<std::uintptr_t>(targetRecord));

    std::uint32_t* newMatrix = RwMatrixCreate();
    CSF_AT(std::uint32_t, record, 0) = static_cast<std::uint32_t>(reinterpret_cast<std::uintptr_t>(newMatrix));

    // The original reloads *srcMatrixPtr after the RwMatrixCreate call.
    const void* source = *srcMatrixPtr;
    std::memcpy(newMatrix, source, 0x40);

    CSF_AT(std::uint8_t, record, 4) = 1;
}

HOOK(0x6B33A0, GfxObject_CreateMatrixCopy, "auto");

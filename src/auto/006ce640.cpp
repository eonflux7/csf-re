#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"

// Bound by address: per-shape update routine called once per 100-byte shape record.
CSF_FUNC(char*, __fastcall, GfxCollisionShapes_UpdateShapeRecord, 0x006CE3A0, (void* self, void* edx, int a2));

// Copies the dword at +0x60 of the shared source block (a3) into every 100-byte
// shape record in [a1, a2), after running the per-shape update on each record.
static int __cdecl GfxCollisionShapes_CopyTailFieldRange(int a1, int a2, void* a3)
{
    int result = 0;
    uint8_t* shape = reinterpret_cast<uint8_t*>(a1);
    uint8_t* end = reinterpret_cast<uint8_t*>(a2);

    while (shape != end) {
        GfxCollisionShapes_UpdateShapeRecord(shape, nullptr, reinterpret_cast<int>(a3));
        result = CSF_AT(int, a3, 0x60);
        CSF_AT(int, shape, 0x60) = result;
        shape += 100;
    }
    return result;
}

HOOK(0x006CE640, GfxCollisionShapes_CopyTailFieldRange, "auto");

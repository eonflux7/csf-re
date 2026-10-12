#include <cstdint>
#include "globals.h"
#include <cstddef>

#include "hook.h"
#include "types/auto_classes.h"

// Callee: returns a char status (al). Its stack arguments are the eight dwords
// pushed by the wrapper, in the order listed in its bind signature.
CSF_FUNC(char, __fastcall, sub_4E05E0, 0x004E05E0, (void* self, void* edx, float* a2, int a3, int a4, int a5, uint32_t* a6, char a7, int* a8, int a9));

// Queries the callee with this object and the given inputs. The status goes to
// out[0]. On success, the query inputs and the callee's flag are copied into the
// record at out, with this object stored at out+4.
static char __fastcall S_004dfbc0_QueryAndStoreRecord(S_004dfbc0* self, void* /*edx*/, float* a2, int* a3, int a4, int a5, uint8_t* out, char a7)
{
    char result = sub_4E05E0(self, nullptr, a2, reinterpret_cast<int>(a3), a4, a5,
                             reinterpret_cast<uint32_t*>(out + 8),
                             static_cast<char>(a7 | 6),
                             reinterpret_cast<int*>(out + 56),
                             reinterpret_cast<int>(out + 60));

    CSF_AT(char, out, 0) = result;
    if (result)
    {
        CSF_AT(S_004dfbc0*, out, 4) = self;
        CSF_AT(float, out, 28) = a2[0];
        CSF_AT(float, out, 32) = a2[1];
        CSF_AT(float, out, 36) = a2[2];
        CSF_AT(int, out, 40) = a3[0];
        CSF_AT(int, out, 44) = a3[1];
        CSF_AT(int, out, 48) = a3[2];
        CSF_AT(int, out, 52) = a5;
    }
    return result;
}

HOOK(0x004E0730, S_004dfbc0_QueryAndStoreRecord, "auto");

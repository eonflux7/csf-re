#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/vc71_vector.h"

CSF_FUNC(uint32_t, __cdecl, Copy, 0x0047EBA0, (uint32_t begin, uint32_t end, uint32_t dest, uint32_t context, uint32_t tag));
struct Operations {
    static __forceinline uint32_t copy(uint32_t begin, uint32_t end, uint32_t dest, uint32_t context, uint32_t tag) {
        return Copy(begin, end, dest, context, tag);
    }
};
static uint32_t __cdecl Replica_0047f240(uint32_t begin, uint32_t end, uint32_t dest, uint32_t context) {
    return legacy::vc71::vector_uninitialized_forward<Operations>(begin, end, dest, context);
}

HOOK(0x0047F240, Replica_0047f240, "stl");

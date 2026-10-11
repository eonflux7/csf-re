#include <cstdint>
#include <cstddef>

#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int, __cdecl, GetGlobalDword_73C020, 0x0073C020, ());
CSF_FUNC(bool, __fastcall, NetSession_IsDuplicate, 0x0064B5C0, (void* self, void* edx));
CSF_FUNC(void*, __cdecl, Mem_Alloc, 0x006910F0, (std::size_t size));
CSF_FUNC(int, __fastcall, sub_741DE0, 0x00741DE0, (void* self, void* edx, uint32_t* a2));

// Queues a 28-byte-argument message record for the net session. Returns the low byte
// of the accumulated eax value, as the original does.
static char __fastcall C_00858180_QueueNetMessage(C_00858180* self, void* /*edx*/,
                                                  int a2, int a3, int a4, int a5,
                                                  int a6, int a7, int a8)
{
    int eax = GetGlobalDword_73C020();
    uint8_t* global = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(eax));

    if (CSF_AT(uint8_t, global, 0x104)) {
        eax = NetSession_IsDuplicate(self, nullptr) ? 1 : 0;
        if (eax) {
            void* allocated = Mem_Alloc(40);
            uint32_t v11 = static_cast<uint32_t>(reinterpret_cast<uintptr_t>(allocated));
            eax = static_cast<int>(v11);
            if (allocated) {
                uint8_t* block = static_cast<uint8_t*>(allocated);
                CSF_AT(uint32_t, block, 0x18) = static_cast<uint32_t>(a2);
                CSF_AT(uint32_t, block, 0x0C) = static_cast<uint32_t>(a6);
                CSF_AT(uint32_t, block, 0x10) = static_cast<uint32_t>(a7);
                CSF_AT(uint32_t, block, 0x14) = static_cast<uint32_t>(a8);
                CSF_AT(uint32_t, block, 0x00) = static_cast<uint32_t>(a3);
                CSF_AT(uint32_t, block, 0x04) = static_cast<uint32_t>(a4);
                CSF_AT(uint32_t, block, 0x08) = static_cast<uint32_t>(a5);
                CSF_AT(uint8_t, block, 0x1C) = 0;
                CSF_AT(uint8_t, block, 0x1D) = 0xFF;
                CSF_AT(uint32_t, block, 0x20) = 0xBF800000u;
                CSF_AT(uint8_t, block, 0x24) = 0;

                int queued = sub_741DE0(
                    reinterpret_cast<void*>(reinterpret_cast<uint8_t*>(self) + 0x6D0),
                    nullptr, &v11);
                eax = queued;
            }
        }
    }
    return static_cast<char>(eax);
}

HOOK(0x741E20, C_00858180_QueueNetMessage, "auto");

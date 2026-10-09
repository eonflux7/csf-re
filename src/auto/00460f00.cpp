#include <cstdint>
#include <cstddef>
#include "globals.h"
#include "hook.h"

CSF_GLOBAL(void*, g_pCurrentScene, 0x8AFC90);

CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));

// Resolves a handle record (pointer stored in the third argument's target, record+4) through the
// current scene's handle table. If the resolved control has a child object, asks it (vtable slot 42)
// to handle request 39, then copies two record values into this+0x60/+0x64, sets this+0x18 = 5
// and clears this+0x82.
static int __fastcall CopyHandleRecordIntoState(void* self, void* /*edx*/, int /*a2*/, int /*a3*/, int a4)
{
    uint8_t* argumentRef = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(a4));
    uint8_t* outerObject = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, argumentRef, 0)));
    uint8_t* recordBase = reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(CSF_AT(uint32_t, outerObject, 4)));
    int result = CSF_AT(int, recordBase, 12);
    uint8_t* recordFields = recordBase + 4;

    if (result >= 0)
    {
        result = CHandleTable_Resolve(g_pCurrentScene, nullptr,
            static_cast<int>(reinterpret_cast<uintptr_t>(recordFields)));
        if (result)
        {
            uint32_t childObject = CSF_AT(uint32_t, reinterpret_cast<uint8_t*>(static_cast<uintptr_t>(result)), 52);
            if (childObject)
            {
                void* child = reinterpret_cast<void*>(static_cast<uintptr_t>(childObject));
                int requestType = 39;
                result = ((int(__fastcall*)(void*, void*, int*))CSF_AT(void**, child, 0)[42])(
                    child, nullptr, &requestType);

                CSF_AT(uint32_t, self, 0x60) = CSF_AT(uint32_t, recordFields, 4);
                CSF_AT(uint32_t, self, 0x64) = CSF_AT(uint32_t, recordFields, 8);
                CSF_AT(uint32_t, self, 0x18) = 5;
                CSF_AT(uint8_t, self, 0x82) = 0;
            }
        }
    }
    return result;
}

HOOK(0x460F00, CopyHandleRecordIntoState, "auto");

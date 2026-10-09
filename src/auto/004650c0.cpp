#include <cstdint>
#include <cstddef>
#include <cstring>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(void*, __fastcall, AssignStringRange_429B60, 0x00429B60, (void* self, void* edx, char* text, unsigned int length));
CSF_FUNC(int, __fastcall, CHandleTable_Resolve, 0x0044FC30, (void* self, void* edx, int handle));
CSF_FUNC(int, __fastcall, GetObjectNameCString, 0x0048FE90, (void* self, void* edx));

CSF_GLOBAL(uint8_t, g_byte_82E65C, 0x0082E65C);
CSF_GLOBAL(void*, g_pCurrentScene, 0x008AFC90);

static C_00833908* __stdcall AssignResolvedObjectNameToLabel(void* a1, void* /*a2*/, void* a3)
{
    // Clear the label text owned by (*(a1 + 4)) + 4.
    void* labelOwner = CSF_AT(void*, a1, 4);
    C_0082d410* labelText = reinterpret_cast<C_0082d410*>(static_cast<uint8_t*>(labelOwner) + 4);
    AssignStringRange_429B60(labelText, nullptr, reinterpret_cast<char*>(&g_byte_82E65C), 0);

    // Locate the handle entry: v3 = *(*(a3) + 4); its index field is at +12.
    void* handleRecord = CSF_AT(void*, a3, 0);
    uint8_t* handleEntry = static_cast<uint8_t*>(CSF_AT(void*, handleRecord, 4));
    int32_t handleIndex = CSF_AT(int32_t, handleEntry, 12);
    C_00833908* result = reinterpret_cast<C_00833908*>(handleEntry + 4);

    if (handleIndex >= 0) {
        result = reinterpret_cast<C_00833908*>(
            CHandleTable_Resolve(g_pCurrentScene, nullptr,
                                 static_cast<int>(reinterpret_cast<uintptr_t>(result))));
        if (result) {
            // The original reloads the label owner after the resolve call.
            void* labelOwnerAgain = CSF_AT(void*, a1, 4);
            int nameRaw = GetObjectNameCString(result, nullptr);
            const char* objectName = reinterpret_cast<const char*>(static_cast<uintptr_t>(nameRaw));
            return reinterpret_cast<C_00833908*>(
                AssignStringRange_429B60(static_cast<uint8_t*>(labelOwnerAgain) + 4, nullptr,
                                         const_cast<char*>(objectName),
                                         static_cast<unsigned int>(strlen(objectName))));
        }
    }
    return result;
}

HOOK(0x4650C0, AssignResolvedObjectNameToLabel, "auto");

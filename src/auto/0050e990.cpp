#include <cstdint>
#include "globals.h"

#include "hook.h"

static bool __fastcall BddLoader_IsRecordListEmpty(uint8_t* self, void* /*edx*/) {
    int beginPtr = CSF_AT(int, self, 0x4);
    if (beginPtr == 0) {
        return true;
    }
    int endPtr = CSF_AT(int, self, 0x8);
    int recordCount = (endPtr - beginPtr) / 80;
    return recordCount == 0;
}

HOOK(0x0050E990, BddLoader_IsRecordListEmpty, "auto");

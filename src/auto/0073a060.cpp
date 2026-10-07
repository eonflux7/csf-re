#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"

CSF_FUNC(int**, __fastcall, FindNodeByKey, 0x00739FA0,
         (int** container, void* edx, int** result, uint16_t* key));

static int* __fastcall C_00867444_FindValueByKey(
    C_00867444* self, void* /*edx*/, int16_t key)
{
    int* result;
    int* node = *FindNodeByKey(
        reinterpret_cast<int**>(reinterpret_cast<uint8_t*>(self) + 0x28),
        nullptr,
        &result,
        reinterpret_cast<uint16_t*>(&key));

    if (node == reinterpret_cast<int*>(self->field_002c))
        return nullptr;

    return node + 4;
}

HOOK(0x0073A060, C_00867444_FindValueByKey, "auto");

#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

CSF_FUNC(uint32_t*, __fastcall, AssignArgBlock, 0x004DC570,
         (void* self, void* edx, uint32_t* a2));
CSF_FUNC(uint32_t*, __fastcall, S_004dca10_AppendArgBlock, 0x004DCF40,
         (void* self, void* edx, uint32_t* a2, int a3));

static uint32_t __fastcall S_004dca10_DispatchWithArgBlock(
    S_004dca10* self, void* /*edx*/, uint32_t* args)
{
    const uint32_t count = self->field_0008;
    uint32_t index = 0;
    uint32_t* entries = nullptr;
    bool found = false;

    if (count != 0) {
        entries = reinterpret_cast<uint32_t*>(self->field_0000);
        uint32_t* cursor = entries;
        do {
            if (cursor[0] != 0 && cursor[2] == args[2]) {
                found = true;
                break;
            }
            ++index;
            cursor += 7;
        } while (index < count);
    }

    if (found && index != UINT32_MAX) {
        uint32_t* entry = reinterpret_cast<uint32_t*>(
            reinterpret_cast<uint32_t>(entries) + 28 * index);
        entry[0] = args[0];
        entry[2] = args[2];
        entry[3] = args[3];
        entry[4] = args[4];
        entry[5] = args[5];
        entry[6] = args[6];
        return reinterpret_cast<uint32_t>(entry + 4);
    }

    if (args[0] == 0)
        return index;

    index = 0;
    if (count != 0) {
        entries = reinterpret_cast<uint32_t*>(self->field_0000);
        uint32_t* cursor = entries;
        do {
            if (cursor[0] == 0) {
                if (index == UINT32_MAX)
                    break;

                uint32_t* entry = reinterpret_cast<uint32_t*>(
                    reinterpret_cast<uint32_t>(entries) + 28 * index);
                return reinterpret_cast<uint32_t>(
                    AssignArgBlock(entry, nullptr, args));
            }
            ++index;
            cursor += 7;
        } while (index < count);
    }

    return reinterpret_cast<uint32_t>(
        S_004dca10_AppendArgBlock(self, nullptr, args, 0));
}

HOOK(0x004DD060, S_004dca10_DispatchWithArgBlock, "auto");

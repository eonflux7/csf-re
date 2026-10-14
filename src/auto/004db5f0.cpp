#include <cstdint>
#include "globals.h"
#include "hook.h"
#include "types/auto_classes.h"

static int __fastcall AiStateFactory_HasObjectReference(
    TAiState* self, void* /*edx*/, int reference)
{
    if (CSF_AT(int, self, 0x10) == reference)
        return 1;
    auto* parent = CSF_AT(TAiState*, self, 0x0C);
    if (parent && static_cast<uint8_t>(
            AiStateFactory_HasObjectReference(parent, nullptr, reference)))
        return 1;
    return 0;
}

HOOK(0x004DB5F0, AiStateFactory_HasObjectReference, "auto");

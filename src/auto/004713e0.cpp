#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cmath>
#include <climits>
#include <cfloat>
#include "globals.h"
#include "hook.h"
#include "core/memory.h"
#include "types/auto_classes.h"
#include "legacy/string.h"

CSF_FUNC(int, __fastcall, sub_69B280, 0x0069B280, (void* self, void* edx));
CSF_FUNC(void, __fastcall, sub_69B3B0, 0x0069B3B0, (void* self, void* edx));

// Runs the first-stage routine on this object and its four embedded sub-objects
// (at +0x44, +0x88, +0xCC, +0x110), then the second-stage routine in reverse order,
// ending with the object itself as a tail call.
static void __fastcall S_004713e0_InitAndReleaseSubObjects(S_004713e0* self, void* /*edx*/)
{
    S_004713e0* subObjA = reinterpret_cast<S_004713e0*>(reinterpret_cast<uint8_t*>(self) + 0x44);
    S_004713e0* subObjB = reinterpret_cast<S_004713e0*>(reinterpret_cast<uint8_t*>(self) + 0x88);
    S_004713e0* subObjC = reinterpret_cast<S_004713e0*>(reinterpret_cast<uint8_t*>(self) + 0xCC);
    S_004713e0* subObjD = reinterpret_cast<S_004713e0*>(reinterpret_cast<uint8_t*>(self) + 0x110);

    sub_69B280(self, nullptr);
    sub_69B280(subObjA, nullptr);
    sub_69B280(subObjB, nullptr);
    sub_69B280(subObjC, nullptr);
    sub_69B280(subObjD, nullptr);

    sub_69B3B0(subObjD, nullptr);
    sub_69B3B0(subObjC, nullptr);
    sub_69B3B0(subObjB, nullptr);
    sub_69B3B0(subObjA, nullptr);
    sub_69B3B0(self, nullptr);
}

HOOK(0x004713E0, S_004713e0_InitAndReleaseSubObjects, "auto");

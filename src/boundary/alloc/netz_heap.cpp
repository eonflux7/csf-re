// Standalone NetZ allocation boundary: no original allocation headers survive
// cutover. All public NetZ alloc/free pairs use the same game heap as C++ new.
#ifdef CSF_STANDALONE
#include "core/memory.h"
#include "hook.h"

static void* __cdecl offline_node_alloc(void*, size_t n, const char*, int, int) {
    return game::Mem_Alloc(n);
}
static void __cdecl offline_node_free(void*, void* p, int) { game::Mem_Free(p); }
static void* __cdecl offline_new(size_t n) { return game::Mem_Alloc(n); }
static void* __cdecl offline_tagged_new(size_t n, const char*, int) { return game::Mem_Alloc(n); }
static void __cdecl offline_delete(void* p) { game::Mem_Free(p); }
HOOK(0x00647070, offline_node_alloc, "boundary-alloc");
HOOK(0x006470E0, offline_node_free, "boundary-alloc");
HOOK(0x00647140, offline_new, "boundary-alloc");
HOOK(0x00647160, offline_tagged_new, "boundary-alloc");
HOOK(0x006471C0, offline_delete, "boundary-alloc");
#endif

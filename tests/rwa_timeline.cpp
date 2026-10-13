#include "../src/boundary/rwa/backend.h"
#include <cassert>
int main() {
    audio::Timeline t;
    t.bytes=44100*4; t.rate=44100*4; t.play(100);
    assert(t.active && t.cursor(600)==88200);
    assert(t.cursor(1100)==176400 && !t.active);
    t.offset=0; t.loop=true; t.play(2000);
    assert(t.cursor(3500)==88200 && t.active);
    t.stop(3500); assert(!t.active && t.offset==88200);
    t.play(4000); assert(t.cursor(4250)==132300);
    t.loop=false; t.offset=0; t.play(0xffffff00);
    assert(t.cursor(0x000002e8)==176400 && !t.active);
    t.bytes=0; t.play(0); assert(!t.active);
    t.bytes=1000; t.rate=1000; t.offset=0; t.play(0);
    t.set_rate(500,2000);
    assert(t.cursor(600)==700 && t.active);
    assert(t.cursor(750)==1000 && !t.active);
    t.offset=2000; t.play(1000); assert(!t.active && t.offset==1000);
    t.loop=true; t.offset=1500; t.play(1000);
    assert(t.cursor(1000)==500 && t.active);
    t.set_loop(1750,false); // after two full cycles, exit at current cursor
    assert(t.cursor(1750)==0 && t.active);
    assert(t.cursor(2250)==1000 && !t.active);
}

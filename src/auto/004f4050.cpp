// 004f4050: no reimplementation. The queue candidate fails tools/emudiff.py only because export/calls.json
// gives the indirect call at 0x4F415C 0 pops; the original's epilogue needs 4 (see the queue triage report).
// This stub has no HOOK line, so nothing is installed. Delete this file.

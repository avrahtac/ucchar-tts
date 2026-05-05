#pragma once

enum class Sthana {
    URAS,    // chest resonance — Sa
    KANTHA,  // throat — Re, Ga
    TALU,    // palate — Ma, Pa  
    MURDHA,  // crown — Dha, Ni
    NASA     // nasal — anusvara
};

enum class Ornament {
    NONE,
    GAMAK,    // FM oscillation
    MEEND,    // frequency sweep
    ANDOLAN,  // slow AM vibrato
    KAN       // grace note
};

struct Swara {
    const char* name;   // "SA", "RE" etc
    float f0_hz;        // fundamental frequency
    float f1_hz;        // first formant
    float f2_hz;        // second formant
    float f3_hz;        // third formant
    Sthana sthana;      // resonance placement
};
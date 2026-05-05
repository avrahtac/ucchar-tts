#pragma once

#include <cstdint>
#include <string>

namespace ucchar {

// -----------------------------------------------------------------------
// Sthana — resonance placement in the vocal tract
// Derived from classical Indian phonological theory.
// Corresponds directly to formant target regions in acoustic phonetics.
// -----------------------------------------------------------------------
enum class Sthana : uint8_t {
    URAS   = 0,  // chest resonance       — Sa
    KANTHA = 1,  // throat resonance      — Re, Ga
    TALU   = 2,  // palate resonance      — Ma, Pa
    MURDHA = 3,  // crown resonance       — Dha, Ni
    NASA   = 4,  // nasal cavity          — anusvara forms
    DANTA  = 5,  // dental                — dental consonants
    OSTHA  = 6   // labial                — labial consonants
};

// -----------------------------------------------------------------------
// Matra — syllable weight
// Derived from Sanskrit chandas (metrical theory).
// LAGHU = light syllable = 1 unit of duration
// GURU  = heavy syllable = 2 units of duration
// -----------------------------------------------------------------------
enum class Matra : uint8_t {
    LAGHU = 1,
    GURU  = 2
};

// -----------------------------------------------------------------------
// Ornament — pitch and amplitude modulation type
// Each ornament maps to a signal processing operation.
// GAMAK   — frequency modulation (FM) of carrier F0
// MEEND   — frequency sweep between two swara F0 values
// ANDOLAN — low-frequency amplitude modulation (AM)
// KAN     — brief grace note onset before main swara
// -----------------------------------------------------------------------
enum class Ornament : uint8_t {
    NONE    = 0,
    GAMAK   = 1,
    MEEND   = 2,
    ANDOLAN = 3,
    KAN     = 4
};

// -----------------------------------------------------------------------
// Swara — the fundamental acoustic unit
// Characterised by fundamental frequency and three formant targets.
// F0 is the carrier frequency.
// F1, F2, F3 are the resonant formants of the vocal tract configuration.
// These values are filled from acoustic measurement of sargam recordings.
// -----------------------------------------------------------------------
struct Swara {
    const char* name;      // "SA", "RE", "GA", "MA", "PA", "DHA", "NI"
    const char* devanagari;// "स", "र", "ग", "म", "प", "ध", "नि"
    float f0_hz;           // fundamental frequency in Hz
    float f1_hz;           // first formant in Hz
    float f2_hz;           // second formant in Hz
    float f3_hz;           // third formant in Hz
    Sthana sthana;         // resonance placement
};

// -----------------------------------------------------------------------
// Akshara — a consonant-vowel unit of Devanagari
// An akshara is not a phoneme. It is a syllabic unit consisting of
// an optional consonant onset and a vowel nucleus (swara).
// -----------------------------------------------------------------------
struct Akshara {
    const char* devanagari; // the akshara character, e.g. "क"
    const char* swara_name; // which swara this vowel belongs to
    Sthana      articulation_point; // where the consonant is formed
    Matra       default_matra;      // default syllable weight
    bool        has_consonant;      // false for pure vowels
};

// -----------------------------------------------------------------------
// SynthesisRequest — input to the engine from JSON or CLI
// -----------------------------------------------------------------------
struct SynthesisRequest {
    std::string text;       // input text in Devanagari or Roman
    std::string emotion;    // "calm", "devotional", "musical"
    std::string ornament;   // "none", "gamak", "meend", "andolan"
    float       rate;       // speaking rate multiplier, 1.0 = normal
};

// -----------------------------------------------------------------------
// PhonemeUnit — output of the swara analyzer
// One unit per akshara in the input text.
// This is what gets passed to the prosody and ornament engines.
// -----------------------------------------------------------------------
struct PhonemeUnit {
    Akshara     akshara;
    Swara       swara;
    Ornament    ornament;
    Matra       matra;
    float       duration_ms;   // computed by prosody engine
    float       f0_target_hz;  // target pitch, may be modified by ornament
};

} // namespace ucchar
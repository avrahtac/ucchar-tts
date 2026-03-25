# ucchar-tts

**उच्चार** — pronunciation, utterance (Marathi / Sanskrit)

An open-source Indian Text-to-Speech engine built on the acoustic foundations of Indian classical music theory. Instead of adapting Western phonological frameworks to Indian languages, ucchar-tts derives its sound units from the swara system — the same theoretical basis that has described Indian vocal production for over two thousand years.

---

## The problem with existing Indian TTS

Every major Indian TTS engine in use today — commercial or open-source — is built on top of Western phonological theory. Text is mapped to IPA phonemes, those phonemes are passed through acoustic models designed around English speaker data, and a vocoder produces audio. The result is technically intelligible but feels wrong to Indian ears. The prosody is flat, the pitch movement is absent, and the natural ornaments that characterise Indian speech — the slight glide between syllables, the gentle vibrato on a long vowel — are entirely missing.

The root cause is not data quantity. It is architectural. When you build an Indian TTS engine on English phoneme theory, you inherit all of its assumptions: five vowel categories instead of sixteen, no concept of resonance placement, no model of pitch ornament, and a stress system derived from Germanic languages rather than from Sanskrit chandas.

---

## The ucchar-tts approach

ucchar-tts treats the swara system as the primary acoustic unit rather than IPA phonemes.

The seven swaras — Sa Re Ga Ma Pa Dha Ni — are not merely musical notes. Each corresponds to a natural resonance point in the human vocal tract, described in ancient treatises as sthanas: uras (chest), kantha (throat), talu (palate), murdha (crown), and nasa (nasal). This is a complete theory of vocal formant placement that maps directly onto modern acoustic phonetics. It was simply never used as the foundation for a TTS engine.

From this foundation, ucchar-tts builds upward:

- The sixteen Sanskrit vowels are derived from swara classes, giving the engine a richer and more accurate vowel space than any IPA-derived system
- Consonant-vowel combinations (aksharas) are modelled as articulatory onsets on top of swara resonances, not as discrete phoneme sequences
- Prosody is derived from laghu-guru syllable weight, the classical Sanskrit system for marking light and heavy syllables, which provides a natural stress and duration model
- Pitch movement is modelled using ornament types — gamak (oscillation), meend (glide), andolan (slow vibrato), kan (grace note) — as first-class components of the synthesis pipeline

No other open-source TTS engine uses any of these foundations.

---

## Comparison with the Western TTS approach

```
Western TTS                          ucchar-tts
-----------                          ----------

Text input                           Text input
    |                                    |
IPA phoneme mapping                  Swara-vyanjana analysis
(English-derived sound units)        (akshara -> swara class + sthana + matra)
    |                                    |
Acoustic model                       Prosody engine
(flat pitch, no microtones)          (laghu/guru weight -> stress and duration)
    |                                    |
Vocoder                              Pitch and ornament model
(robotic, wrong prosody)             (gamak / meend / andolan F0 curves)
    |                                    |
Audio output                         Sthana-aware acoustic model
(intelligible, unnatural)            (chest / throat / head formant targets)
                                         |
                                     Neural vocoder (HiFi-GAN via ONNX)
                                         |
                                     Audio output
                                     (natural Indian speech)
```

## Data

ucchar-tts does not require large training datasets. The core acoustic parameters — swara frequencies, formant targets, ornament curves — are derived from a small set of precisely recorded reference performances of the sargam (Sa Re Ga Ma Pa Dha Ni Sa) with andolan and meend. These measurements populate the lookup tables in `data/swaras.json` and `data/aksharas.json`.

The only neural component is the vocoder (HiFi-GAN), which is a pretrained open-source model used for waveform generation. It is called via ONNX Runtime and requires no additional training.

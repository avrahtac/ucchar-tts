# ucchar-tts

**उच्चार** — pronunciation, utterance (Marathi / Sanskrit)

ucchar-tts is an open-source Text-to-Speech engine for Indian languages, grounded in acoustic physics and classical Indian phonological theory. The engine derives its fundamental sound units from the swara system and models the speech synthesis pipeline as a signal processing problem, using the theoretical frameworks of communication systems and resonance acoustics.

---

## Motivation

Contemporary Indian TTS systems are predominantly constructed on phonological frameworks developed for Indo-European languages with different acoustic properties. Text is typically mapped to IPA phonemes, processed through acoustic models trained on corpora that underrepresent Indic phoneme distributions, and rendered through vocoders optimised for Western prosodic patterns. The resulting synthesis is intelligible but fails to reproduce the prosodic and timbral characteristics natural to Indian speech.

This limitation is architectural in origin. Indian languages carry phonological information — vowel duration, resonance placement, pitch ornament — that has no representation in IPA-derived pipelines. The sixteen Sanskrit vowel forms are collapsed into approximations. The concept of sthana, the placement of resonance within the vocal tract, is absent entirely. Pitch ornaments such as gamak and meend, which occur naturally in connected Indian speech, have no counterpart in standard prosody models.

ucchar-tts proposes an alternative foundation derived from classical Indian acoustic theory, which has formally described these phenomena for over two thousand years.

---

## Theoretical Foundation

The human vocal tract functions as a resonant cavity whose modal frequencies correspond to the vowel sounds of speech. Classical Indian music theory identifies these resonant modes as swaras — Sa Re Ga Ma Pa Dha Ni — and associates each with a specific region of vocal tract activation described as a sthana.

| Sthana | Region | Associated Swaras |
|--------|--------|-------------------|
| Uras | Chest | Sa |
| Kantha | Throat | Re, Ga |
| Talu | Palate | Ma, Pa |
| Murdha | Crown | Dha, Ni |
| NASA | Nasal cavity | Anusvara forms |

This correspondence is not metaphorical. The sthanas describe formant target regions — the same acoustic parameters that modern speech science denotes F1, F2, F3. The swara system constitutes, in effect, a formant-based phoneme theory indigenous to Indian languages.

From this basis, ucchar-tts constructs its synthesis pipeline:

**Swara table.** Each swara is characterised by its fundamental frequency F0 and formant targets F1 F2 F3, derived from acoustic measurement of sargam reference recordings. This table replaces the IPA phoneme inventory as the primary acoustic unit.

**Prosody model.** Timing and stress are derived from laghu-guru syllable classification, the metrical system of Sanskrit chandas. Laghu (light) syllables carry one matra of duration; guru (heavy) syllables carry two. This provides a physically motivated duration model that reflects the prosodic structure of Indic languages without requiring a learned component.

**Ornament model.** Pitch movement is modelled using signal modulation operations. Gamak is implemented as frequency modulation of the carrier F0, with modulation index and rate derived from reference measurements. Meend is implemented as a linear or polynomial frequency sweep between two swara F0 values. Andolan is implemented as low-frequency amplitude modulation. These operations are drawn directly from communication systems theory.

**Vocoder.** The acoustic features produced by the ucchar engine are rendered to a waveform by HiFi-GAN, a pretrained open-source neural vocoder, called via ONNX Runtime. This component operates as a fixed black-box renderer and requires no training or modification.

---

## Comparison

```
Conventional pipeline                ucchar-tts pipeline
---------------------                -------------------

Text input                           Text input
    |                                    |
IPA phoneme mapping                  Swara-vyanjana analysis
English-derived phoneme inventory    Akshara to swara class, sthana, matra
    |                                    |
Statistical acoustic model           Laghu-guru prosody model
Trained duration and pitch model     Rule-based duration from syllable weight
    |                                    |
Prosody post-processing              Ornament engine
Flat or learned pitch contour        FM/AM modulation — gamak, meend, andolan
    |                                    |
Neural vocoder                       Sthana-aware formant synthesis
Waveform from mel-spectrogram        F1 F2 F3 targets per swara class
    |                                    |
Audio output                         HiFi-GAN via ONNX Runtime
                                         |
                                     Audio output
```
## Data

ucchar-tts does not require large training datasets. The core acoustic parameters — swara frequencies, formant targets, ornament curves — are derived from a small set of precisely recorded reference performances of the sargam (Sa Re Ga Ma Pa Dha Ni Sa) with andolan and meend. These measurements populate the lookup tables in `data/swaras.json` and `data/aksharas.json`.

The only neural component is the vocoder (HiFi-GAN), which is a pretrained open-source model used for waveform generation. It is called via ONNX Runtime and requires no additional training.
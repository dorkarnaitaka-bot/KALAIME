# KALA Phonetic mapping specification

This document describes the initial KALAIME phonetic mapping strategy for the first release. The goal is an original, maintainable transliteration system designed from first principles for Bangla input.

## Core rules

- Lowercase Latin input is normalized before transliteration.
- The engine prefers longest-match behavior for known syllables and common words.
- Unknown or unsupported sequences fall back to a deterministic best-effort mapping.
- Uppercase characters are normalized to lowercase for transliteration.
- The output remains Unicode Bangla text and is not a visual glyph approximation.

## Supported example mappings

| Roman input | Bangla output |
|---|---|
| ami | আমি |
| bangla | বাংলা |
| bangladesh | বাংলাদেশ |
| bhasha | ভাষা |
| shikkha | শিক্ষা |
| bhalobasha | ভালোবাসা |
| kala | কলা |
| kalaime | কালাইম |

## Handling strategy

The transliterator keeps a small set of canonical word mappings for common high-frequency Bengali terms and a broader rule-based fallback for everyday phonetic input. This respects clean-room requirements while allowing a deterministic output path for a real product.

## Future expansion

For future releases the mapping system will evolve by adding:

- a trie-based syllable matcher,
- dictionary ranking,
- candidate ranking,
- user dictionary entries,
- extension for additional Bengali conjuncts.

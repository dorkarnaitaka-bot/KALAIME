# Bengali Unicode support notes

KALAIME follows the Unicode Standard as the source of truth for Bengali text handling. The first release targets the `Bengali` Unicode block and uses ICU for normalization and Unicode-aware conversion behavior.

## Character policy

- UTF-8 is used inside the portable shared engine.
- UTF-16 is handled at the native Windows boundary.
- Bengali vowels, consonants, marks, and digits are validated against assigned Unicode ranges.
- Normalization is performed only when needed for a text operation; the engine avoids blanket re-normalization on every keystroke.

## Representative coverage

The initial engine covers the core Bengali phonetic set used by normal Bangla typing, including vowels, dependent vowel signs, virama, consonants, conjunct-related sequences, punctuation, and digits.

## Normalization policy

- NFC is used for canonical composition when a composed form is required by the composition model.
- NFKC is used for compatibility normalization when input is believed to contain compatibility-equivalent forms.
- Invalid Unicode sequences are rejected or safely escaped rather than silently rewritten.

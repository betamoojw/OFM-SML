# Changes

## upcomming releases

* Feature: `sml infoNN` diagnose command (console + DiagnoseKo) reports the live channel status derived from the last received telegram — INACTIVE/NODATA, detected meter type (1R/2R), tariff count (T0/T2) and available measurements (P/V/A/F)
* Feature: Add `Suspendiert` channel switch
* Feature: Add an optional raw 32-bit status word KO per channel (`c%C%MeterStatus`), sent on change only; 16-bit status words are placed in the lower 16 bits
* Feature: Document the status word bit layout with two ready-to-import Konfigtransfer examples (error mask on bits 9/10/17, energy flow direction on bit 11); the transmit flag is disabled by default on the status word and all six Wh-precision counter KOs, since those are meant for in-device processing (Logic module, virtual meter), not group address transmission
* Feature: Append the raw status word as a hex suffix to debug log lines
* Feature: Apply the Typ-Variante Kanalauswahl pattern to channel select, replacing the `VisibleChannels` constant
* Fix: Harden SML telegram parsing against corrupted data — bounds-check OBIS/vendor/meter-ID octet strings, check allocation return values, fix an unsigned underflow, a sign-extension bug in the meter serial number, and a format string mismatch
* Fix: Harden SML buffer and frame parsing — fix a buffer leak on overlapping telegrams, an underflow on out-of-spec fill bytes, an off-by-one in escape removal, and recognize the end sequence via an escape state machine instead of a fixed-window pattern match that could accept a corrupted telegram as complete mid-payload
* Fix: Protect the `_smlBuffer` handover between cores on ESP32 as well (previously RP2040-only)
* Refactor: Derive the module-level status from the channels instead of duplicating the stale-timeout check; drop a VLA in `processDataPoint()`
* Update: Upgrade libsml to v1.1.5
* Doc: Add horizontal rulers between headline sections, matching OFM-VirtualButton
* Doc: Add README, AGENTS.md and CLAUDE.md

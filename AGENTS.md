# AGENTS für OFM-SML

## Ziel

Dieses Modul liest SML-Telegramme (Smart Message Language) von deutschen Smartmetern über eine serielle Schnittstelle (z. B. eHZ-Infrarot-/KNX-Adapter) und sendet die Zählerstände, Momentanwerte und optional das Statuswort auf den KNX-Bus.

## Basis: OGM-Common

Dieses OFM baut auf `../OGM-Common/AGENTS.md` auf (Modulsystem, LED-/
Flash-/Zeit-API, Logging, Embedded-Rahmenbedingungen, Code-Konventionen)
— dort beschriebene Regeln und vorhandene Claude-Skills/Agents (z. B.
das Kanalauswahl-Muster) gelten auch hier.

## Prefix und Namenskonventionen

- **Modulprefix**: `SML`
- **C++-Klassen**: `SMLModule`, `SMLChannel`
- **ETS-Anzeigename**: `Smartmeter (SML)`, **Icon**: `meter-electric-outline`

## Kanalanzahl

Die Kanalanzahl wird durch das OAM über `SML_ChannelCount` vorgegeben. Jeder Kanal ist einer physischen seriellen Schnittstelle zugeordnet — der Serial-Zeiger wird nicht im OFM, sondern vom einbindenden Projekt über `SMLModule::getChannel(i)->setSerial(...)` gesetzt.

**Wichtig für main.cpp-Integration**: Kanäle werden immer vollständig angelegt (kein Lazy-Allocation-Pattern wie bei anderen Modulen), weil `setSerial()` direkt nach `openknx.setup()` auf jeden Kanal aufgerufen wird und `getChannel()` sonst `nullptr` zurückgeben würde. `SMLChannel::setup()` sichert sich selbst über `if (ParamSML_cType > 0)` ab.

## Telegramm-Verarbeitung

- `SMLChannel::writeBuffer()` nimmt Bytes byteweise auf, erkennt Start-/Endsequenz über eine Escape-State-Machine auf dem 4-Byte-Raster des SML-Transportformats.
- `OPENKNX_SML_BUFFER` (Default 1024) begrenzt die Puffergröße pro Kanal. Läuft ein Telegramm über, wird es verworfen, nicht verschoben.
- `SMLChannel::processFile()` übergibt ein komplettes Telegramm an `libsml` und wertet die OBIS-Einträge über `processDataPoint()` aus.
- Innerhalb eines Telegramms gewinnt für das Statuswort-KO der **erste** OBIS-Eintrag mit Statusfeld — die SML-Spec erlaubt grundsätzlich pro Eintrag ein eigenes, potenziell unterschiedliches Statusfeld.

## Zählertypen

Typ 0 = Inaktiv. Typ 1–4 unterscheiden Einrichtungs-/Zweirichtungszähler jeweils mit oder ohne Tarif (HT/NT), siehe `PT-SMLType` in `SMLModule.templ.xml`.

## Statuswort

Das Statuswort wird roh (unausgewertet) als 32-Bit-Wert übertragen — die Bit-Belegung ist herstellerabhängig und nicht verbindlich spezifiziert. Auswertung einzelner Bits ist Aufgabe des Logikmoduls, nicht dieses Moduls. Details und Bit-Tabellen (unverifizierte Sekundärinformation) stehen in `doc/Applikationsbeschreibung-SML.md`.

Das Ü-Flag ist beim Statuswort-KO und den Wh-genauen Zählerstand-Varianten standardmäßig deaktiviert (`TransmitFlag="Disabled"` in `SMLModule.templ.xml`) — diese Werte sind für die geräteinterne Weiterverarbeitung (Logikmodul, virtueller Zähler) gedacht, nicht für die direkte Übertragung per Gruppenadresse.

## Status-LEDs

Modul (`SML Gesamtstatus`, Funktions-ID 200) und jeder Kanal (`SML Kanalstatus X`, ID 201+n) folgen dem Zustandsmodell der OpenKNX-Wiki-Seite "Status-LED": die LED blinkt bei jedem empfangenen Byte unabhängig von dessen Gültigkeit, leuchtet dauerhaft bei einem gültigen Telegramm und erlischt bzw. wird rot, wenn `OPENKNX_SML_STALE_TIMEOUT` (Default 5000 ms) ohne gültigen Empfang verstreicht. Der Gesamtstatus des Moduls ist aktiv, sobald mindestens ein Kanal aktiv empfängt.

## Konsolenbefehle

- **sml debug** – schaltet ausführliches Logging aller OBIS-Werte und Rohtelegramme ein/aus.
- **sml infoNN** – zeigt den erkannten Zählertyp (1R/2R, höchster Tarif, verfügbare Messgrößen P/V/A/F) für Kanal NN, auch über das Diagnose-KO abrufbar.

## Regeln für Weiterentwicklung

1. Neue OBIS-Codes bekommen einen eigenen Zweig in `SMLChannel::processDataPoint()`.
2. `SMLModule` verwaltet nur das Channel-Array und den aggregierten Gesamtstatus — keine zählerspezifische Logik dort.
3. Änderungen an der Statuswort-Bit-Tabelle in der Doku klar als unverifizierte Sekundärinformation kennzeichnen, nicht als gesicherte Spezifikation.

## Dokumentation und Hilfe

- Dokumentation liegt in `doc/Applikationsbeschreibung-SML.md`
- Jede sichtbare `ParameterRefRef` bekommt einen `HelpContext` mit Prefix `SML-` (Ausnahme: `Empty` für Parameter ohne eigene Hilfeseite)
- Jede verwendete `HelpContext`-Id muss in der Applikationsbeschreibung als `<!-- DOC HelpContext="SML-..." -->` dokumentiert sein
- Baggages werden über VS Code Task "OpenKNXproducer Documentation" erzeugt (`.vscode/tasks.json`)
- Deutsche Texte mit echten Umlauten (ä, ö, ü, ß) schreiben

## Referenzen

- [README.md](README.md) — Funktionsüberblick, Status-LEDs, Diagnose, Konsolenbefehle
- [doc/Applikationsbeschreibung-SML.md](doc/Applikationsbeschreibung-SML.md) — Statuswort-Details, Konfigtransfer-Beispiele
- [CHANGELOG.md](CHANGELOG.md) — Versionshistorie

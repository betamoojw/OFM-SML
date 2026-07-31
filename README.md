# OFM-SML

Liest SML-Telegramme (Smart Message Language) von Stromzählern über einen Optokoppler bzw. eine serielle Schnittstelle und sendet die Zählerstände, Momentanwerte und optional das Statuswort auf den KNX-Bus.

## Funktionsumfang

Unterstützt werden Einrichtungs- und Zweirichtungszähler, jeweils mit oder ohne Tarifumschaltung (HT/NT). Bist du dir beim Zählertyp unsicher, kannst du zunächst Zweirichtungszähler mit Zweitarif wählen — dann werden alle möglichen Zählerstände ausgewertet, unabhängig davon, was der angeschlossene Zähler tatsächlich unterstützt; nicht unterstützte Werte bleiben einfach auf 0 stehen.

Pro Kanal werden bereitgestellt:

- **Zählerstände** – Bezug (OBIS 1.8.0), bei Zweitarifzählern zusätzlich Hoch-/Niedertarif, bei Zweirichtungszählern zusätzlich Einspeisung (2.8.0). Für jeden Zählerstand einzeln konfigurierbar: Änderung und/oder fester Zyklus, optional eine Wh-genaue Variante für Logikmodul/virtuellen Zähler.
- **Momentanwerte** – Wirkleistung, Spannung, Strom, Frequenz, je nachdem was der Zähler liefert.
- **Statuswort** – wird roh übertragen, da die Bit-Belegung herstellerabhängig und nicht verbindlich spezifiziert ist. Details und Beispiele zur Auswertung mit dem Logikmodul stehen in [doc/Applikationsbeschreibung-SML.md](doc/Applikationsbeschreibung-SML.md).

## Status-LEDs

Modul und jeder Kanal können eine Status-LED anzeigen, nach dem Zustandsmodell der OpenKNX-Wiki-Seite "Status-LED": sie blinkt bei jedem empfangenen Byte unabhängig von dessen Gültigkeit, leuchtet dauerhaft bei einem gültigen Telegramm und erlischt bzw. wird rot, wenn längere Zeit kein gültiger Empfang mehr erfolgt.

## Diagnose

Über das Diagnose-Kommunikationsobjekt lässt sich live abfragen, was der angeschlossene Zähler tatsächlich sendet — unabhängig von der aktuellen ETS-Parametrierung. Dazu den Text `sml infoXX` an das Diagnose-KO senden (XX = zweistellige Kanalnummer, z. B. `sml info01`). Die Antwort basiert auf dem zuletzt empfangenen Telegramm:

- **NODATA** – seit mehreren Sekunden ist kein gültiges Telegramm mehr angekommen
- **INACTIVE** – der Kanal ist in ETS nicht aktiviert
- **1R** / **2R** – Einrichtungs- bzw. Zweirichtungszähler
- **T0** / **T2** – keine Tarife (Eintarifzähler) bzw. zwei Tarife (Hoch-/Niedertarif)
- **P**, **V**, **A**, **F** – der Zähler liefert zusätzlich Wirkleistung, Spannung, Strom bzw. Frequenz

So lässt sich schon vor dem Einschalten der einzelnen Funktionen live prüfen, was der angeschlossene Zähler überhaupt kann.

## Konsolenbefehle

- **sml debug** – schaltet eine fortlaufende Anzeige aller empfangenen SML-Daten ein bzw. aus
- **sml infoXX** – zeigt denselben Status wie das Diagnose-KO direkt auf der Konsole

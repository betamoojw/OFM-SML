# Changes

## 0.3.0

- **Diagnosebefehl**: Über `sml infoNN` (bzw. das Diagnose-KO) lässt sich
  direkt am Gerät ablesen, was ein angeschlossener Zähler tatsächlich
  liefert — Zählertyp, Tarife, verfügbare Messwerte. Praktisch bei der
  Inbetriebnahme, ohne vorher etwas in der ETS einstellen zu müssen.
- **Kanäle einzeln suspendierbar**, ohne die komplette Konfiguration zu
  verändern.
- **Optionales Statuswort-KO**: Das rohe Statuswort eines Zählers kann
  jetzt zusätzlich als eigener Wert übertragen werden (z. B. für die
  Auswertung im Logikmodul), inkl. Beispielen zur Fehler- und
  Energierichtungserkennung in der Dokumentation.
- Deutlich robusteres Einlesen von Zählertelegrammen — beschädigte oder
  unvollständige Telegramme führen nicht mehr zu Fehlfunktionen.
- Kanalauswahl in der ETS überarbeitet.
- libsml auf Version 1.1.5 aktualisiert.

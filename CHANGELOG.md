# Changes

## 1.1.0

- Fix: Der Parameter "Zählerstände (Wh)" wurde nie ausgewertet (Offset-Makro
  statt `Param...` verwendet), die Wh-Kommunikationsobjekte wurden dadurch
  unabhängig von der Einstellung beschrieben.
- Fix: Die Wh-Zählerstände hingen am Parameter der kWh-Zählerstände — nur Wh
  aktiviert bedeutete, dass gar nichts geschrieben wurde. Beide Varianten sind
  jetzt wie in der ETS dargestellt unabhängig schaltbar.
- Fix: Bei Zweirichtungszählern mit Tarif fehlten die Kommunikationsobjekte für
  die Einspeisung in Tarif 1/2 (kWh und Wh) in der ETS, obwohl die Firmware sie
  befüllt hat. Zusätzlich waren deren Einheiten in den KO-Texten als "kW" statt
  "Wh" ausgewiesen.

## 1.0.0

Erste offizielle Veröffentlichung. Die komplette Basisimplementierung des
Moduls ist Teil dieser Version, deshalb gibt es keine separaten
Änderungseinträge.

Vor dieser Veröffentlichung gab es mehrere Testversionen. Diese sind hier
nicht aufgeführt, eine Änderungshistorie dazu existiert nicht.

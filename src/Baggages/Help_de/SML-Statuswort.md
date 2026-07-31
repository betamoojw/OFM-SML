### Statuswort

Aktiviert das vom Zähler gemeldete Statuswort. Es handelt sich um ein optionales Feld der SML-Übertragung, das nicht jeder Zähler und nicht jedes Telegramm mitliefert — wird nichts gesendet, bleibt das Kommunikationsobjekt einfach auf seinem letzten Stand bzw. auf 0 stehen.

Das Statuswort wird unverändert als 32-Bit-Rohwert übertragen, ohne dass die einzelnen Bits ausgewertet oder interpretiert werden. Je nach Zähler ist das vom Gerät gemeldete Statuswort entweder 16 oder 32 Bit breit; im 16-Bit-Fall wird der Wert unverändert in die unteren 16 Bit des Kommunikationsobjekts eingetragen, die oberen 16 Bit bleiben dabei 0.

Gesendet wird nur bei einer Änderung des Wertes (sowie einmalig nach dem Start, sobald der erste Wert vom Zähler empfangen wurde) — es gibt keine feste Zykluszeit oder Änderungsschwelle wie bei den Zählerständen und Messwerten. Liefert der Zähler dauerhaft denselben Wert, bleibt das Kommunikationsobjekt entsprechend stumm.

Eine herstellerübergreifend verbindliche Bit-Belegung gibt es nicht, jeder Hersteller kann sie abweichend festlegen. Die folgende Zuordnung ist nicht gesichert (ungeprüfte Sekundärinformation, keine Garantie auf Vollständigkeit oder Einhaltung durch den jeweiligen Zähler), wird hier aber als möglicher Anhaltspunkt aufgeführt:

- **16-Bit-Wort:**
  - Bit 0: Fehler
  - Bit 1: metrologische Synchronisation
  - Bit 2: reserviert, immer 0
  - Bit 3: Zeit-Synchronisation
  - Bit 4: Rücklaufsperre
  - Bit 5: Energierichtung
  - Bit 6: magnetische Manipulation
  - Bit 7: Leerlauf-/Anlauferkennung
  - Bit 8–9: Tarif 1/2 aktiv
  - Bit 10–15: aktuelle Tarifinformation

- **32-Bit-Wort:**
  - Bit 0–1: immer 0
  - Bit 2: immer 1 (Versionsmarker)
  - Bit 3–7: immer 0
  - Bit 8: Leerlauf-/Anlauferkennung
  - Bit 9: magnetische Manipulation
  - Bit 10: Manipulation Klemmendeckel/Plombierstift
  - Bit 11–14: Energierichtung Summe/L1/L2/L3
  - Bit 15: Drehfeld
  - Bit 16: Rücklaufsperre
  - Bit 17: Fehler eichrelevant
  - Bit 18–20: Leiterspannung L1/L2/L3 vorhanden
  - Bit 21–31: reserviert, immer 0

Da Bit 2 beim 16-Bit-Wort reserviert (immer 0) und beim 32-Bit-Wort immer 1 ist, lässt sich anhand dieses Bits im empfangenen Rohwert erkennen, welches der beiden Formate der Zähler tatsächlich verwendet.



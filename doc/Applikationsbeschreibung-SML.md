# **SML**

<!-- DOC HelpContext="Dokumentation" -->

<!-- DOCCONTENT
Eine vollständige Applikationsbeschreibung ist unter folgendem Link verfügbar: https://github.com/openknx/OFM-SML/blob/v1/doc/Applikationsbeschreibung-SML.md
DOCCONTENT -->

Das Modul ermöglicht das Auslesen von Stromzählern in Deutschland, die das SML-Protokoll per Optokoppler unterstützen.

Die Zähler selbst arbeiten alle nach demselben Prinzip und unterscheiden sich nur darin, welche Werte sie tatsächlich liefern. Bist du dir beim Zählertyp unsicher, kannst du zunächst Zweirichtungszähler mit Zweitarif (2R, HT/NT) wählen — dann werden alle möglichen Zählerstände ausgewertet, unabhängig davon, was der angeschlossene Zähler tatsächlich unterstützt.

Über die Gerätekonsole lässt sich der Empfang live beobachten:

- **sml debug** – schaltet eine fortlaufende Anzeige aller empfangenen SML-Daten ein bzw. aus, eine erneute Eingabe schaltet sie wieder ab
- **sml infoXX** – zeigt denselben Status wie das Diagnose-Kommunikationsobjekt (siehe unten) direkt auf der Konsole, XX ist die zweistellige Kanalnummer

**Diagnose:** Über das Diagnose-Kommunikationsobjekt lässt sich live abfragen, was der angeschlossene Zähler tatsächlich sendet, unabhängig von der aktuellen ETS-Parametrierung. Dazu den Text **sml infoXX** an das Diagnose-KO senden, wobei XX die zweistellige Kanalnummer ist, zum Beispiel **sml info01** für den ersten Kanal. Die Antwort basiert auf dem zuletzt empfangenen SML-Telegramm:

- **NODATA** – seit mehreren Sekunden ist kein gültiges Telegramm mehr angekommen
- **INACTIVE** – der Kanal ist in ETS nicht aktiviert
- **1R** / **2R** – Einrichtungs- bzw. Zweirichtungszähler
- **T0** / **T2** – keine Tarife (Eintarifzähler) bzw. zwei Tarife (Hoch-/Niedertarif)
- **P**, **V**, **A**, **F** – der Zähler liefert zusätzlich Wirkleistung, Spannung, Strom bzw. Frequenz

So lässt sich schon vor dem Einschalten der einzelnen Funktionen live prüfen, was der angeschlossene Zähler überhaupt kann.
<!-- DOCEND -->

## **Allgemein**

Auf dieser Seite sieht man die in der Applikation verwendete Modulversion.

<!-- DOC -->
### **Zählertyp**

Hier musst du den Zählertyp auswählen, den du verwenden möchtest. Es wird zwischen Einrichtungs- und Zweirichtungszählern unterschieden. Der Einrichtungszähler ist der klassische Zähler, während der Zweirichtungszähler bei Photovoltaikanlagen verwendet wird, da er die erzeugte Energie erfassen kann. Um welchen Typ es sich handelt, kann am Zähler selbst abgelesen werden. Dieser zeigt bei einem Zweirichtungszähler Stände für 1.8.0 und 2.8.0 im Display an.

Zusätzlich wird zwischen Eintarifzählern (ET) und Zweitarifzählern (HT/NT) unterschieden. Ein Zweitarifzähler hat neben dem Gesamtverbrauch auch zwei weitere Zählerstände für den Hoch- und Niedertarif. Dies kommt häufig bei Nachtspeicherheizungen vor. Zähler mit Tarifumschaltung zeigen im Display neben 1.8.0 auch 1.8.1 bzw. 1.8.2 an.

Generell gilt, dass die Auswahl keine Auswirkung auf die Auswertung bzw. die Kommunikation mit dem Zähler hat. Sie dient lediglich zur Vereinfachung der Oberfläche. Werte, die vom Zähler nicht unterstützt werden, bleiben einfach auf 0 stehen.


<!-- DOC -->
### **Beschreibung**

Freitext zur Bezeichnung des Smartmeters, z. B. "Hauptzähler" oder "PV-Erzeugungszähler". Dieser Text erscheint als Name des Kanal-Reiters, in der Kanalauswahl-Tabelle sowie in den Bezeichnungen der zugehörigen Kommunikationsobjekte.


<!-- DOC -->
## **Zählerstände**

Hier werden die Zählerstände (Wirkarbeit) konfiguriert und über KNX bereitgestellt. Je nach oben gewähltem Zählertyp stehen die Zählerstände für den Bezug (OBIS 1.8.0), bei Zweitarifzählern zusätzlich getrennt nach Hoch- und Niedertarif (1.8.1/1.8.2), und bei Zweirichtungszählern zusätzlich für die Einspeisung (2.8.0, ggf. 2.8.1/2.8.2) zur Verfügung. Für jeden Zählerstand kann getrennt festgelegt werden, bei welcher Änderung und/oder in welchem festen Zyklus der aktuelle Wert gesendet wird. Zusätzlich lässt sich pro Zählerstand eine Wh-genaue Variante aktivieren, die primär zur Weiterverarbeitung mit dem Logikmodul oder dem virtuellen Zähler gedacht ist und nicht zur Übertragung per Gruppenadresse.

<!-- DOC HelpContext="AenderungUndZyklus" -->
### **Änderung und Zyklus**

Die beiden Schalter "Sende bei Änderung" und "Sende nur alle" schließen sich nicht gegenseitig aus, sondern werden mit ODER verknüpft: Der aktuelle Wert wird gesendet, sobald eine der beiden Bedingungen zutrifft — entweder die eingestellte Änderungsschwelle wurde überschritten, oder das eingestellte Zeitintervall ist abgelaufen. Sind beide Schalter deaktiviert, wird der Wert nie aktiv auf den Bus gesendet, steht aber bei einem Lesevorgang weiterhin aktuell zur Verfügung.

<!-- DOC -->
## **Messwerte**

Neben der eigentlichen Aufgabe, den Stromverbrauch zu messen, senden die Zähler auch verschiedene weitere Messwerte. Dazu gehören neben der üblichen Wirkleistung auch Werte wie Spannung, Strom und Frequenz. Um diese zusätzlichen Messwerte zu erhalten, ist in der Regel die Eingabe einer PIN erforderlich, die ihr bei eurem Messstellenbetreiber anfordern könnt. Der Umfang der verfügbaren Messwerte variiert jedoch je nach Betreiber und Zählermodell. Manche Zähler geben zum Beispiel nur die Wirkleistung aus. Wenn die Schnittstelle keine Werte liefert, bleiben die betroffenen KOs einfach bei 0 stehen.

Die Wirkleistung lässt sich getrennt als Summe und als Einzelwerte je Phase aktivieren, jeweils mit eigenem Sende-Intervall. Der Summenwert ist meist der wichtigere und wird häufiger benötigt, während die drei Phasenwerte seltener gebraucht werden. Durch die Trennung lässt sich der Summenwert zum Beispiel alle 5 Sekunden senden, ohne dass zusätzlich drei weitere Telegramme für die einzelnen Phasen den Bus belasten.

<!-- DOC -->
## **Erweitert**

Hier lassen sich zusätzliche, optionale Informationen des Zählers aktivieren, die für die eigentliche Zählerauswertung nicht notwendig sind.

<!-- DOC HelpContext="Identifikationsnummer" -->
### **Identifikationsnummer**

Aktiviert die Identifikationsnummer des Zählers (Hersteller, Sparte und Seriennummer nach OBIS 96.1.0). Ist die Option eingeschaltet, sendet das zugehörige Kommunikationsobjekt die vom Zähler gemeldete Identifikationsnummer als Text.

<!-- DOC HelpContext="Statuswort" -->
### **Statuswort**

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



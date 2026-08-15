### Statuswort

Aktiviert das vom Zähler gemeldete Statuswort. Das Feld ist allerdings optional. Außerdem sind die Bits nicht sauber spezifiziert. Ich habe aber zwei Tabellen für 16 und 32 Bit gefunden. Daher wird der Wert einfach roh ausgegeben, und man kann die Auswertung z.B. mit dem Logikmodul machen.

Zähler mit nur 16 Bit landen in den unteren 16 Bit des Kommunikationsobjekts, der Rest bleibt 0. Gesendet wird nur bei einer Änderung des Wertes.

Das Ü-Flag ist deshalb standardmäßig deaktiviert: einzelne Bits (z.B. die Leerlauf-/Anlauferkennung) können sich je nach Last auch mal sekündlich ändern, und das willst du dir nicht auf den Bus holen. Werte das Statuswort daher am besten geräteintern mit dem Logikmodul aus. Willst du den Rohwert trotzdem wirklich per Gruppenadresse verschicken, musst du das Ü-Flag am Kommunikationsobjekt manuell wieder aktivieren.

Anbei gibt es neben der Bit-Tabelle noch zwei Beispiele, die du mit dem Konfigtransfer einlesen kannst. Beide gehen vom 32-Bit-Status aus, weil die meisten Zähler den nutzen.

**Beispiel 1: Fehlerauswertung** — wertet die Bits 9, 10 und 17 aus und macht daraus ein Fehler-KO.

    OpenKNX,cv1,0xAF05:0xA3/LOG:0x44/10§f~Name=Z%C3%A4hlerstatus%20(Fehler)§f~LogicType=2§f~Logic=2§f~NameInput1=Stromz%C3%A4hler%20Statusmeldung§f~E1ConvertFloat=7§f~E1=1§f~E1Dpt=13§f~E1OtherKO:2=524§f~E1UseOtherKO=1§f~NameInput2=Konstante%3A%20Bits%209%2C10%2C17§f~E2ConvertFloat=5§f~E2=1§f~E2Dpt=13§f~E2LowDpt12Fix=132608§f~NameOutput=Ausgewerteter%20Fehlerstatus§f~OSendOnChange=1§f~OOnAll=8§f~OOnFunction=9§f~OOffAll=0§;OpenKNX

**Beispiel 2: Energieflussrichtung gesamt** — wertet Bit 11 aus und macht daraus ein KO für die Energierichtung (Bezug/Einspeisung).

    OpenKNX,cv1,0xAF05:0xA3/LOG:0x44/1§f~Name=Energierichtung%20gesamt§f~LogicType=2§f~Logic=2§f~NameInput1=Stromz%C3%A4hler%20Statusmeldung§f~E1ConvertFloat=7§f~E1=1§f~E1Dpt=13§f~E1OtherKO:2=524§f~E1UseOtherKO=1§f~NameInput2=Konstante%3A%20Bit%2011§f~E2ConvertFloat=5§f~E2=1§f~E2Dpt=13§f~E2LowDpt12Fix=2048§f~NameOutput=Ausgewertete%20Energierichtung§f~OSendOnChange=1§f~OOnAll=8§f~OOnFunction=9§f~OOffAll=0§;OpenKNX

*Bit-Tabellen*

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

Praktisch ist Bit 2: beim 16-Bit-Wort ist das immer 0, beim 32-Bit-Wort immer 1. Damit siehst du am Rohwert direkt, welches der beiden Formate dein Zähler tatsächlich nutzt.



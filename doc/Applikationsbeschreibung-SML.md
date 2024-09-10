# **SML**

<!-- DOC HelpContext="Dokumentation" -->

<!-- DOCCONTENT
Eine vollständige Applikationsbeschreibung ist unter folgendem Link verfügbar: https://github.com/openknx/OFM-SML/blob/v1/doc/Applikationsbeschreibung-SML.md
DOCCONTENT -->

Das Modul ermöglicht das Auslesen von Stromzähler in Deutschland welche das SML Protokoll per Optokoppler erlauben. 
<!-- DOCEND -->

## **Allgemein**

Auf dieser Seite sieht man die in der Applikation verwendete Modulversion und die Anzahl der Binäreingänge, die der Benutzer verwenden möchte.

### **Verfügbare Kanäle**

Um die Applikation übersichtlicher zu gestalten, kann hier ausgewählt werden, wie viele Kanäle in der Applikation verfügbar und editierbar sind. Die Maximalanzahl der Kanäle hängt von der Firmware des Gerätes ab, dass dieses Modul verwendet.

Die ETS ist auch schneller in der Anzeige, wenn sie weniger (leere) Kanäle darstellen muss. Insofern macht es Sinn, nur so viele Kanäle anzuzeigen, wie man wirklich braucht.

<!-- DOC -->
### **Zählertyp**

Hier musst du den Zählertyp auswählen, den du verwenden möchtest. Es wird zwischen Einrichtungs- und Zweirichtungszählern unterschieden. Der Einrichtungszähler ist der klassische Zähler, während der Zweirichtungszähler bei Photovoltaikanlagen verwendet wird, da er die erzeugte Energie erfassen kann. Um welchen Typ es sich handelt, kann am Zähler selbst abgelesen werden. Dieser zeigt bei einem Zweirichtungszähler Stände für 1.8.0 und 2.8.0 im Display an.

Zusätzlich wird zwischen Eintarifzählern (ET) und Zweitarifzählern (HT/NT) unterschieden. Ein Zweitarifzähler hat neben dem Gesamtverbrauch auch zwei weitere Zählerstände für den Hoch- und Niedertarif. Dies kommt häufig bei Nachtspeicherheizungen vor. Zähler mit Tarifumschaltung zeigen im Display neben 1.8.0 auch 1.8.1 bzw. 1.8.2 an.

Generell gilt, dass die Auswahl keine Auswirkung auf die Auswertung bzw. die Kommunikation mit dem Zähler hat. Sie dient lediglich zur Vereinfachung der Oberfläche. Werte, die vom Zähler nicht unterstützt werden, bleiben einfach auf 0 stehen.


<!-- DOC -->
### **Beschreibung**

Beschreibe den Smartmeter


<!-- DOC -->
## **Zählerstände**

Alles über Zählerstände

<!-- DOC HelpContext="Zaehlerstaende-Format" -->
### **Format**

Gibt an ob die Werte in kWh oder in Wh

<!-- DOC -->
## **Messwerte**

Neben der eigentlichen Aufgabe, den Stromverbrauch zu messen, senden die Zähler auch verschiedene weitere Messwerte. Dazu gehören neben der üblichen Wirkleistung auch Werte wie Spannung, Strom und Frequenz. Um diese zusätzlichen Messwerte zu erhalten, ist in der Regel die Eingabe einer PIN erforderlich, die ihr bei eurem Messstellenbetreiber anfordern könnt. Der Umfang der verfügbaren Messwerte variiert jedoch je nach Betreiber und Zählermodell. Manche Zähler geben zum Beispiel nur die Wirkleistung aus. Wenn die Schnittstelle keine Werte liefert, bleiben die betroffenen KOs einfach bei 0 stehen.

<!-- DOC -->
## **Erweitert**

Erweitert




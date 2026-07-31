### Zählerstände

Hier werden die Zählerstände (Wirkarbeit) konfiguriert und über KNX bereitgestellt. Je nach oben gewähltem Zählertyp stehen die Zählerstände für den Bezug (OBIS 1.8.0), bei Zweitarifzählern zusätzlich getrennt nach Hoch- und Niedertarif (1.8.1/1.8.2), und bei Zweirichtungszählern zusätzlich für die Einspeisung (2.8.0, ggf. 2.8.1/2.8.2) zur Verfügung. Für jeden Zählerstand kann getrennt festgelegt werden, bei welcher Änderung und/oder in welchem festen Zyklus der aktuelle Wert gesendet wird.

Zusätzlich lässt sich pro Zählerstand eine Wh-genaue Variante aktivieren, die primär zur Weiterverarbeitung mit dem Logikmodul oder dem virtuellen Zähler gedacht ist und nicht zur Übertragung per Gruppenadresse. Deshalb ist das Ü-Flag bei diesen Kommunikationsobjekten standardmäßig deaktiviert; willst du den Wh-Wert doch per Gruppenadresse senden, musst du es manuell wieder aktivieren.


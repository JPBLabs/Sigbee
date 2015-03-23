# Sigbee

Introduction : 
--------------

Cette carte permet une utilisation simple de la puce Sigfox TD1208. En effet elle embarque un régulateur 3V qui permet d'une part d'alimenter la puce et d'autre part d'adapter la tension.
Ainsi on peux travailler sur une carte mère alimenter par une tension supérieure à 3V et utiliser également la puce de communication.


Hardware : 
----------

	La carte possède 7 broches extérieures : 

		- Vbat : Tension de la carte mère (tension de référence)

		- GND : Masse

		- TXHV et RXHV : Communication série avec la puce

		- SDA et SCL : Bus I2C
		
Sofware : 
---------

La librairie " SigfoxModem " peut être utilisée avec l'IDE Arduino. Le dossier contient un exemple testé avec l'IDE Arduino v1.0.4.
	
Le code source est sous licence GNU GPL v2, la carte quant à elle est sous licence CC-by-NC-SA.

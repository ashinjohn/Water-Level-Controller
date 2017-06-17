# Water-Level-Controller
ATTiny 2313 controlled Reed Switch sensed Relay Controlled Pump 

3 Reed switches are kept inside the water tank. A magnet attached to a float is used to sense the water level inside the tank.
The reed switches may be potioned based on threshhold requirement.

The lower reed switch will trigger the turing ON of the pump.
And the higher triggers turing OFF of the pump.
An additional reed switch offers more reliability and avoids overflowing.

230v 10A Relay via a Transitor circuit is used to control the AC pump.

16x2 Alphanumeric LCD module is used for indicating errors like damaged reed switches.
Due to lack of pins LCD module will be used in 4 bit mode.

Buzzer used to indicate switch ON and OFF of pump. Allows user monitoring for malfunctions in case of Submerged pumps.

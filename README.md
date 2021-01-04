# Jeopardy Buzzer

---

This is just a quick project I made over Christmas break 2020 to accompany a Jeopardy board game. It is designed to have one "host" button that,
when pressed, arms the system to receive responses. The system then sets the LED color to the color of the player who buzzed in first and makes a short 'beep'.
Any players who hit the button too early incur a half second penalty.

---

Components:
- 1x ESP8266 NodeMCU
- 1x Piezoelectric buzzer
- 1x Prototype board
- 6x screw terminal connectors (1x power, 5x buttons)
- 5x 10k resistors for pull-down circuit on the buttons
- 5x 12mm momentary switches
- 5x 3.5" PVC pipe lengths for the button handle
- 5x 4' lengths of 2 lead stranded wire to connect the buttons to the screw terminals
- 1x project enclosure (4" x 4" x 3" should be more than sufficient)
- 3x 5 LED neopixel strips to display the color of the player who responds first
- 1x 3 AAA battery holder as the power supply (4.5v is regulated on the NodeMCU to 3.3v for the ESP8266)

# Barcode-control-and-logger-circuit
My final project for my Automation and Control course during my 7th semester at the Autonomous University of Baja California (August - December, 2021): a barcode control circuit with MySQL logger.

The idea for the system came from my professor of that course, who kept getting frustrated by students leaving the computer lab equipment turned on when they leave and thus waste a lot of electricity. He wanted a system that makes it a requirement for students to insert their student IDs into a barcode reader in order to energize the lab equipment to be able to use it; conversely, the lab equipment has to become deenergized when students remove their IDs from the barcode reader to leave the labs. Additionally, when a student inserts their ID into the reader, their ID number has to be logged into a MySQL database alongside the date and time of the insertion; when the ID is removed, it has to save the date and time of removal.

## Objective
This project is a smaller prototype of that system, where the load circuit, instead of being an AC circuit that consists of the lab equipment, is a much smaller DC circuit that only contains a small red LED. The objective of this prototype is to energize the LED when a student ID is inserted into the barcode reader (as well as log into a MySQL database the ID number and the date and time of "arrival") and to deenergize the LED when the ID is removed (as well as log into the database the date and time of "departure").

## Circuit schematic

![Circuit schematic]()

## Components and tools
A Waveshare Barcode Scanner Module.  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/5f3d9791-e6f7-430c-b6c6-8c4be2225b56 width="350">

An ESP-01S (for the Wi-Fi connection to the MySQL server).  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/429623d8-a0d4-4e0c-b2ca-70b91f2ed018 width="250">

An Arduino Mega (to receive the ID number read by the barcode reader and send it to the ESP for publishing; also to control the load circuit).  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/68c7ff6f-367b-480c-b61e-83f6f428c031 width="300">  
I chose an Arduino Mega because I already had it in my possession from an internship (I was allowed to keep it); it having multiple hardware serial ports helps a lot since both the ESP-01S and the barcode reader use serial communication. Alternatively, it's possible to make this same project with an Arduino Uno and use SoftwareSerial to make the additional needed serial ports, but my Arduino Uno at the time didn't have its ATmega328P chip inserted, so I couldn't use it as an actual microcontroller; it did, however, come in handy for programming the ESP-01S. It's also possible to replace the Arduino and the ESP-01S with either an ESP8266 microcontroller or any Arduino board with built-in Wi-Fi, although that would require bigger modifications of my code.

A SUN HOLD RAS-1215 relay (to control the flow of electricity of the load circuit with its normally open contacts; according to its [datasheet](https://www.sunhold.com/upload/prd1/109-3.pdf), it requires to be fed 12V and 30mA for its normally open circuit to close).  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/b461944b-66e9-474a-a2d0-10d03dd64ef6 width="200">

A 2N2222 transistor (used as a switch for an external power source to energize the relay, since the Arduino on its own can't output enough voltage for the relay).  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/6877c925-f9d1-4c85-b267-45507ecbb33b width="150">

A 1N4004 diode (acts as a flyback diode for the relay).  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/0720418a-98a7-4478-9cfc-33036ab0b5d9 width="200">

An LED.  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/a74c3528-21ca-495e-b624-bbf8782084aa width="100">

A 12V power source (to power the relay); my power source was an array of three 3.7V batteries connected in series, which in theory should deliver only 11.1V but measuring the actual ouput of the array with a multimeter results in a reading of 12V.  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/5dc1f16c-5e6d-4bb5-9504-c031d687ab3b width="500">

A 5V power source (to power the LED); my power source was a USB cable connected to my computer.  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/78b63a03-466f-4f73-a68a-aaf64d5b6cba width="300">

A 300Ω resistor for the LED. The LED takes in a maximum of 20mA and will be fed with 5V, so the minimum resistance the LED needs is $R=V/I=5V/20mA=250Ω$; the resistor with the closest value to 250Ω that I could find was a 300Ω one.

A 1kΩ and 2.2kΩ resistor to form a voltage divider for the ESP-01S. The ESP-01S takes in 3.3V through its RX pin for serial communication, but the Arduino Mega's TX pins are 5V pins. Thus, a voltage divider is needed to reduce the Arduino's 5V to 3.3V suitable for the ESP. To start my calculations, I first proposed a 1kΩ resistor as $R_1$, so to find $R_2$: $R_2=(V_oR_1)/(V_{in}-Vo)=(3.3V\times1kΩ)/(5V-3.3V)=1941.18Ω$. The closest value of resistor I could find was 2.2kΩ, which results in an output of $V_o=(R_2V_{in})/(R_1+R_2)=(2.2kΩ\times5V)/(1kΩ+2.2kΩ)=3.44V$, which is close enough to 3.3V.

A 1kΩ resistor for the transistor's base. Since the relay, which is connected to the transistor's collector, requires a current of 30mA ($I_C$), and a 2N2222 transistor has a max $\beta$ of 300 (according to its [datasheet](https://www.farnell.com/datasheets/296640.pdf)), then to put the transistor into saturation (and thus allow the 12V source to feed the relay) I'd need in the transistor's base a minimum current of $I_B=I_C/\beta=30mA/300=100μA$. Since the Arduino Mega's digital ports output 5V, and the base-emitter voltage of a 2N2222 is 0.7V (according to the datasheet again), then in the base of the transistor I'd need a resistor of at most 

A breadboard on which to build the system  


# Barcode-control-and-logger-circuit
My final project for my Automation and Control course during my 7th semester at the Autonomous University of Baja California (August - December, 2021): a barcode control circuit with MySQL logger.

The idea for the system came from my professor of that course, who kept getting frustrated by students leaving the computer lab equipment turned on when they leave and thus waste a lot of electricity. He wanted a system that makes it a requirement for students to insert their student IDs into a barcode reader in order to energize the lab equipment to be able to use it; conversely, the lab equipment has to become deenergized when students remove their IDs from the barcode reader to leave the labs. Additionally, when a student inserts their ID into the reader, their ID number has to be logged into a MySQL database alongside the date and time of the insertion; when the ID is removed, it has to save the date and time of removal.

## Objective
This project is a smaller prototype of that system, where the load circuit, instead of being an AC circuit that consists of the lab equipment, is a much smaller DC circuit that only contains a small red LED. The objective of this prototype is to energize the LED when a student ID is inserted into the barcode reader (as well as log into a MySQL database the ID number and the date and time of "arrival") and to deenergize the LED when the ID is removed (as well as log into the database the date and time of "departure").

## Components and tools
A Waveshare Barcode Scanner Module  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/5f3d9791-e6f7-430c-b6c6-8c4be2225b56 width="350">

An ESP-01S (for the Wi-Fi connection to the MySQL server)  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/429623d8-a0d4-4e0c-b2ca-70b91f2ed018 width="250">

An Arduino Mega (to receive the ID number read by the barcode reader and send it to the ESP for publishing; also to control the load circuit)  
<img src=https://github.com/mareyna356/Barcode-control-and-logger-circuit/assets/116867368/68c7ff6f-367b-480c-b61e-83f6f428c031 width="300">  
I chose an Arduino Mega because I already had it in my possession from an internship (I was allowed to keep it); it having multiple hardware serial ports helps a lot since both the ESP-01S and the barcode reader use serial communication. Alternatively, it's possible to make this same project with an Arduino Uno and use SoftwareSerial to make the additional needed serial ports, but my Arduino Uno at the time didn't have its ATmega328P chip inserted, so I couldn't use it as an actual microcontroller; it did, however, come in handy for programming the ESP-01S. It's also possible to replace the Arduino and the ESP-01S with either an ESP8266 microcontroller or any Arduino board with built-in Wi-Fi, but it would require doing bigger modifications to my code.

- 
- A mechanical relay

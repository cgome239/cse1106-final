# cse1106-final
Smart Classroom Comfort Monitor
This is a C program for the Raspberry Pi that monitors classroom temperatures in Fahrenheit. It checks if the temperature is inside a comfortable range, logs all readings to a CSV file, updates a status file, and turns on a red warning LED when the room gets too hot or too cold.

Hardware Setup
•	GPIO 17 pin 11 wired to one side of a resistor which connects to a leg of a red LED.
•	GND pin 6 Wired from the breadboard ground to the short leg of the LED.

Project Structure
•	src/main.c - Main C program using the pigpio library
•	docs/requirements.md - Functional requirements for the system
•	docs/design.md - Hardware and software design overview
•	docs/change_log.md - History of updates made during development
•	tests/test_plan.md - Test cases and results
•	data/temperature_log.csv - Saved log of temperature readings and statuses
•	status.txt - Current system state (NORMAL or WARNING)

How to Compile and Run
From the main cse1106-final folder, run:
gcc -o monitor src/main.c -lpigpio
sudo ./monitor

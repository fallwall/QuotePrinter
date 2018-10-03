# QuotePrinter
*An ESP8266 sketch to print inspirational quotes from a thermal printer*

Do you need a bit of inspiration or motivation from time to time? Would you like to press a button and have wisdom from really smart people printed out for you--instantly? Well, have I got the project for you...

I haven't written a step-by-step tutorial (yet!) on how to create this for yourself, but the overall picture is:

1. Put together the hardware (see below)
2. Set up your software environment
  * Download/install VS Code
  * Install the PlatformIO plugin from VS Code
  * Create a simple sketch and upload it to your ESP-12E breakout board (such as the standard `blink` sketch)
3. Get the printer working
  * The thermal printer basically prints whatever it gets from serial input. You use the Adafruit thermal printer library (distributed with this project) to send data from your sketch to the printer. See Adafruit's [tutorial](https://learn.adafruit.com/mini-thermal-receipt-printer) to get started.
4. Print quotes
  * Upload the `marker.txt` and `quotes.txt` files via the PlatformIO `Upload file system image` button
  * Upload this project's sketch
  * Once the sketch is loaded and the printer is powered on, you can push the button and get a quote. You'll want to add your own quotes so that you don't get bored with the same five quotes over and over.

Note that this project ironically does not use *any* of the ESP8266's wireless capabilities. As such, it's a good intro to the chip and its SPI flash filesystem. An industrious person could extend this to pull and print quotes from the internet without too much added effort.

## Hardware
You will need:
* Half-size breadboard (full-size recommended)
* ESP-12E breakout board (~$6 on Amazon, or $2.20 on AliExpress)
* Adafruit Mini Thermal Printer (~$50 on Adafruit)
* Level shifter ($10 for 10 on Amazon)
* 5V-2A power supply with connector (see schematic below)
* 1kOhm resistor
* Standard push button
* 470uF, >=6.3V capacitor (not strictly needed...But it's a good idea)
* A bunch of jumper wires

## Board Layout
![Schematic](/esp12e-printer.png)

## Software Tools/Libraries
* PlatformIO addon in VS Code
* Arduino-style C++
* ESP8266 SPIFFS library for file storage
* Adafruit Thermal Printer library (the version I've included has an ESP8266-specific fix that I submitted a [pull request](https://github.com/adafruit/Adafruit-Thermal-Printer-Library/pull/30) for)

### Quote File Format

See the [example](data/quotes.txt). Put a quote on one line, then an author, then a quote, etc. If you file starts with line 1, then your quotes will be on even lines, and your authors will be on odd lines.

I don't include a big file of quotes for copyright reasons, but some searching and text wrangling can help you put a good quote file together.

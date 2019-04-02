

#include "Adafruit_Thermal.h"
#include "SoftwareSerial.h"

const int TX_PIN = 6;           // Arduino transmit  YELLOW WIRE  labeled RX on printer
const int RX_PIN = 5;           // Arduino receive   GREEN WIRE   labeled TX on printer
const int BUTTON_PIN = 2;       // the number of the pushbutton pin
const int WORKSPACE_SIZE = 512; // Memory space for character manipulation.
const int LINES_AFTER = 2;      // This controls how many lines you'll print after a press.

int buttonState = LOW; // variable for reading the pushbutton status
int isPrinting = LOW;  // Variable for storing if we are still printing

SoftwareSerial mySerial(RX_PIN, TX_PIN); // Declare SoftwareSerial obj first
Adafruit_Thermal printer(&mySerial);     // Pass addr to printer constructor

/* Arduino seems to be really bad at Strings... */

char *quotes[16] = {
    "In the center of your being,\nyou know who you are.",
    "You can take a horse to a\nriver but you can't force\na horse to drink.",
    "In the end, all that mattered\nis how much you loved,\nhow gently you lived,\nand how gracefully you let go.",
    "Boldness has its genius, power, and magic.",
    "It's not the load that brings\nyou down. It's the way\nyou carry it.",
    "Why do you stay in prison,\nwhen the door is so wide open?",
    "The wound is the place where\nthe lights enter you.",
    "You were born with wings.\nYou are not meant for crawling. So don't.",
    "Not knowing when the dawn\nwill come. I open every door.",
    "What matters is\nhow quickly you\ndo what your soul wants.",
    "You are not\na drop in the ocean.\nYou are the entire ocean\nin a single drop.",
    "When you do things from\nyour soul, you felt a river\nmoving in you, a joy.",
    "Dedicate your practice\nto somebody because we\nare more likely to try\nharder for somebody else.",
    "You don't know how strong\nyou are until being strong\nis the only option.",
    "How you are being taught is\nbased on your capacity to learn.",
    "If you are irritated by\nevery rub, how will you\never be polished."};

char *workspace;

// -----------------------------------------------------------------------

void setup()
{

  pinMode(BUTTON_PIN, INPUT); // initialize the pushbutton pin as an input:
  mySerial.begin(19200);      // Initialize SoftwareSerial
  printer.begin();            // Init printer

  /* We need to reset the printer for some reason beyond me */
  printer.wake();                         // Wake the printer... you never know
  printer.setDefault();                   // Reset the printer styling
  printer.test();                         // print anything
  renderInspiration(F("THIS IS A TEST")); // Print an styled quote

  /* ------------ PRINT ALL QUOTES ----------- */

  for (int i = 0; i < 16; i++)
  {
    renderInspiration(quotes[i]);
    delay(200L);
  }

  /* ---------------------------------------- */
}

void loop()
{

  buttonState = digitalRead(BUTTON_PIN);        // read the state of the pushbutton value:
  if (buttonState == HIGH && isPrinting == LOW) // check if the pushbutton is pressed and if we are printing
  {
    beInspired(); // BE INSPIRED
  }
}

// -----------------------------------------------------------------------

void beInspired()
{
  isPrinting = HIGH;
  renderInspiration(quotes[random(16)]); // BE INSPIRED!!!!
  delay(1000L);                          // wait a second
  isPrinting = LOW;
}

void renderInspiration(String inspiration)
{

  printer.wake();                                  // Wake the printer... you never know
  printer.feed(2);                                 // Make an space between this print and the last one
  printer.underlineOn();                           // Underline ON
  printer.setSize('L');                            // LARGE TEXT
  printer.justify('C');                            // CENTER TEXT
  printer.println(F("{ REFRESH }"));               // TITTLE
  printer.underlineOff();                          // Underline OFF
  printer.feed(2);                                 // EMPTY LINES
  printer.setDefault();                            // Reset the printer styling
  printer.println(inspiration);                    // Print the quote
  printer.feed(1);                                 //  EMPTY LINES
  printer.setDefault();                            // Reset the printer styling
  printer.setSize('S');                            // Small text
  printer.println(F("      ~ Quote via"));         // Print Atribution header
  printer.println(F("        Kathryn Leary."));    // Print Atribution
  printer.feed(2);                                 // EMPTY LINES
  printer.justify('C');                            // CENTER TEXT
  printer.println(F("-------------------------")); // Print a line
  printer.feed(LINES_AFTER);                       // Make space
  printer.setDefault();                            // Reset the printer styling
  printer.sleep();                                 // Tell printer to sleep
  delay(100L);                                     // ...cuz hardware.
}

// -----------------------------------------------------------------------

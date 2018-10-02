// TODO my setup stops working after a while, and then a reset makes it work again. Fix it!

#include <Arduino.h>
#include <Adafruit_Thermal.h>
#include <FS.h>

const int BAUDRATE = 19200;
const int BUTTON_GPIO = 5;
const int MAX_COL_NUM = 31;
const int NUM_LINES_AFTER_QUOTE = 4;

const int WORKSPACE_SIZE = 2048;

Adafruit_Thermal printer(&Serial);
char* workspace;

void setup()
{
    pinMode(BUTTON_GPIO, INPUT);

    Serial.begin(BAUDRATE);

    SPIFFS.begin();

    // Set up buffer for formatter function
    workspace = (char*) malloc(WORKSPACE_SIZE); // 2k should be plenty...longest I've seen so far is less than 512
    for (int i = 0; i < WORKSPACE_SIZE; i++)
        workspace[i] = '\0';


    printer.begin();
}

String prettyFormat(String ugly)
{
    if (ugly.length() > WORKSPACE_SIZE)
    {
        return "I had something to say, but it\nwas so long that you probably\nwouldn't have read it anyway.";
        // TODO I could also just return the string without doing anything to it...or add a note at the end saying I couldn't format it
    }

    String rtn;

    strcpy(workspace, ugly.c_str());
    int curCol = 0;

    // Tokenize temp space into a pretty string, adding ' ' and '\n' as necessary
    char* curWord = strtok(workspace, " \n");
    while (curWord != NULL)
    {
        if (curCol + strlen(curWord) <= MAX_COL_NUM) // Breaks (softly) if any single word is greater than MAX_COL_NUM...Oh well.
        {
            rtn += curWord;
            curCol += strlen(curWord);

            if (curCol < MAX_COL_NUM)
            {
                rtn += " ";
                curCol++;
            }
        }
        else
        {
            rtn += "\n";
            rtn += curWord;
            curCol = strlen(curWord);

            if (curCol < MAX_COL_NUM)
            {
                rtn += " ";
                curCol++;
            }
        }
        curWord = strtok(NULL, " \n");
    }

    return rtn;
}

void loop()
{
    if (digitalRead(BUTTON_GPIO) == HIGH)
    {
        File markerFile;
        File quoteFile;
        int lineNo = 0;

        // Get line number for next quote, open quote file, scroll to that line
        markerFile = SPIFFS.open("/marker.txt", "r");
        lineNo = markerFile.parseInt();
        markerFile.close();

        quoteFile = SPIFFS.open("/quotes.txt", "r");
        if (!quoteFile)
        {
            Serial.println("Failed to open quote file!");
            // TODO panic and quit, set a light red, etc.
        }
        for (int i = 0; i < lineNo; i++)
        {
            quoteFile.readStringUntil('\n');
        }

        // TODO deal with out of paper
        // TODO troubleshoot "stops working after a while"

        String line = quoteFile.readStringUntil('\n'); // Read the quote
        if (line == "")
        {
            printer.println("I'm out of quotes! But don't\nworry, I'll start back at the\nbeginning. Press the button\nagain!\n\n\n\n");
            markerFile = SPIFFS.open("/marker.txt", "w");
            markerFile.print(0);
            markerFile.close();
            return;
        }

        printer.println(prettyFormat(line));
        line = quoteFile.readStringUntil('\n'); // Read the author
        printer.println(prettyFormat(line));
        for (int i = 0; i < NUM_LINES_AFTER_QUOTE; i++)
        {
            printer.print('\n');
        }

        lineNo += 2;
        markerFile = SPIFFS.open("/marker.txt", "w");
        markerFile.print(lineNo);
        markerFile.close();

    }
}

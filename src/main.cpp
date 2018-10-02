// TODO my setup stops working after a while, and then a reset makes it work again. Fix it!

#include <Arduino.h>
#include <Adafruit_Thermal.h>
#include <FS.h>

#define BAUDRATE 19200

int button = 5;
int lineNo = 0;
Adafruit_Thermal printer(&Serial);
File quoteFile;
File markerFile;

const int WORKSPACE_SIZE = 2048;
const int MAX_COL_NUM = 31;
char* workspace;

void setup()
{
    pinMode(button, INPUT);

    Serial.begin(BAUDRATE);

    SPIFFS.begin();
    quoteFile = SPIFFS.open("/quotes.txt", "r");
    if (!quoteFile)
    {
        Serial.println("Failed to open quote file!");
        // TODO panic and quit, set a light red, etc.
    }

    // Read the number in the marker.txt file
    // This number is the line number in quotes.txt to start printing from
    // (It gets updated after every print job)
    markerFile = SPIFFS.open("/marker.txt", "r");
    lineNo = markerFile.parseInt();
    markerFile.close();

    // Serial.print("line no: ");
    // Serial.println(lineNo);

    for (int i = 0; i < lineNo; i++)
    {
        quoteFile.readStringUntil('\n');
    }

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

    return rtn + "\n";
}

void loop()
{
    if (digitalRead(button) == HIGH)
    {
        // TODO deal with case where we've reached the end of the file

        // TODO format string to fit receipt paper (32 chars/line)

        // TODO deal with out of paper

        String line = quoteFile.readStringUntil('\n');
        printer.println(prettyFormat(line));
        line = quoteFile.readStringUntil('\n');
        printer.println(line);
        printer.println("\n\n\n\n");

        lineNo += 2;
        markerFile = SPIFFS.open("/marker.txt", "w");
        markerFile.print(lineNo);
        markerFile.close();

    }
    delay(15);
}

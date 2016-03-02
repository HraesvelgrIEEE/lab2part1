#include <xc.h>
#include "keypad.h"
#include "timer.h"

#define INPUT 1
#define OUTPUT 0

#define CLOSED 1
#define OPEN 0

#define ENABLED 1

#define MULTIPLE_KEYS_PRESSED (key - (KEYPAD_ROW1 + KEYPAD_ROW2 + KEYPAD_ROW3 + KEYPAD_ROW4) > -4)

/* Initialize the rows as ODC outputs and the columns as inputs with pull-up
 * resistors. Don't forget about other considerations...
 */
void initKeypad(void) {
    ODCDbits.ODCD10 = ENABLED;
    ODCDbits.ODCD4 = ENABLED;
    ODCCbits.ODCC1 = ENABLED;
    
    TRISEbits.TRISE3 = INPUT;
    TRISEbits.TRISE2 = INPUT;
    TRISEbits.TRISE1 = INPUT;
    TRISEbits.TRISE0 = INPUT;
    
    //Note: Pull-ups will be done externally
    
    CNENEbits.CNIEE3 = ENABLED;
    CNENEbits.CNIEE2 = ENABLED;
    CNENEbits.CNIEE1 = ENABLED;
    CNENEbits.CNIEE0 = ENABLED;
    
    CNCONEbits.ON = 1;                  // Enable overall interrupt
    IFS1bits.CNEIF = 0;                 // Put down the flag
    IPC8bits.CNIP = 7;                  // Configure interrupt priority
    IEC1bits.CNEIE = ENABLED;           // Enable interrupt for E pins
}

/* This function will be called AFTER you have determined that someone pressed
 * SOME key. This function is to figure out WHICH key has been pressed.
 * This function should return -1 if more than one key is pressed or if
 * no key is pressed at all. Otherwise, it should return the ASCII character of
 * the key that is pressed. The ascii character c programmatically is just 'c'
 */
char scanKeypad(void) {
    char key = -1;
    
    //Scan column 1
    KEYPAD_COL1 = CLOSED;
    KEYPAD_COL2 = OPEN;
    KEYPAD_COL3 = OPEN;
    
//    if (key - (KEYPAD_ROW1 + KEYPAD_ROW2 + KEYPAD_ROW3 + KEYPAD_ROW4) > -4) return -1;
    if (MULTIPLE_KEYS_PRESSED) return -1;  //Check for multiple keys pressed
    else {
        if (KEYPAD_ROW1 == 0) key = '1';
        if (KEYPAD_ROW2 == 0) key = '4';
        if (KEYPAD_ROW3 == 0) key = '7';
        if (KEYPAD_ROW4 == 0) key = '*';
    }
    
    //Scan column 2
    KEYPAD_COL1 = OPEN;
    KEYPAD_COL2 = CLOSED;
    KEYPAD_COL3 = OPEN;
    
    if (MULTIPLE_KEYS_PRESSED) return -1;  //Check for multiple keys pressed
    else {
        if (KEYPAD_ROW1 == 0) key = '2';
        if (KEYPAD_ROW2 == 0) key = '5';
        if (KEYPAD_ROW3 == 0) key = '8';
        if (KEYPAD_ROW4 == 0) key = '0';
    }
    
    //Scan column 3
    KEYPAD_COL1 = OPEN;
    KEYPAD_COL2 = OPEN;
    KEYPAD_COL3 = CLOSED;
    
    if (MULTIPLE_KEYS_PRESSED) return -1;  //Check for multiple keys pressed
    else {
        if (KEYPAD_ROW1 == 0) key = '3';
        if (KEYPAD_ROW2 == 0) key = '6';
        if (KEYPAD_ROW3 == 0) key = '9';
        if (KEYPAD_ROW4 == 0) key = '#';
    }
    
    return key;
}

void enableNSA() {
    //Mass surveillance
    KEYPAD_COL1 = CLOSED;
    KEYPAD_COL2 = CLOSED;
    KEYPAD_COL3 = CLOSED;
}

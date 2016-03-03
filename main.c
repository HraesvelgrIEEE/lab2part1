/* File: main.c
 * Author: dhung 
 */

#include <xc.h>
#include <sys/attribs.h>
#include "config.h"
#include "interrupt.h"
#include "keypad.h"
#include "lcd.h"
#include "timer.h"

typedef enum stateTypeEnum {waitPress, debounce, scan, print, waitRelease} stateType;

volatile stateType state = waitPress, nextState = waitPress;

int main(void) {
    //Initialize
    initLCD();
    initKeypad();
    initTimers();
    enableInterrupts(); 
    SYSTEMConfigPerformance(10000000);
    
    char keypadChar = 0;
    char numPrinted = 0;
    char currLine = 0; //0 indexed
    
    while (1) {
        switch (state) {
            case waitPress:
                enableNSA();
                delayMs(5);
                break;
            case debounce:
                delayMs(5);
                state = nextState;
                break;
            case scan:
                keypadChar = scanKeypad();
                if (keypadChar != -1) {
                    state = print;
                    ++numPrinted;
                    
                    if (numPrinted > 16) { //Line switch
						currLine = !currLine;
                        moveCursorLCD(0, 1 + currLine);
                        numPrinted = 0;
                    }
                }
                else state = waitRelease; 
                break;
            case print:
                printCharLCD(keypadChar);
                state = waitRelease;
                break;
            case waitRelease:
                //delayMs(5);
                state = waitPress;
                break;
        }
    }
}

__ISR(_CHANGE_NOTICE_VECTOR, IPL7SRS) _CNInterrupt() {
    IFS1bits.CNEIF = 0;
    
    PORTEbits;
    
    //FIXME: ROW4
    
    //Standard transition, debounced
    if (state == waitPress) {
        nextState = scan;
        state = debounce;
    }
    //All buttons released
    else if (state == waitRelease && (KEYPAD_ROW1 + KEYPAD_ROW2 + KEYPAD_ROW3 + KEYPAD_ROW4 == 4)) {
        nextState = waitPress;
        state = debounce;
    }
    
    state;
}

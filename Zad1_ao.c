#pragma config POSCMOD = XT             // Primary Oscillator Select (XT Oscillator mode selected)
#pragma config OSCIOFNC = ON            // Primary Oscillator Output Function (OSC2/CLKO/RC15 functions as port I/O (RC15))
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor (Clock switching and Fail-Safe Clock Monitor are disabled)
#pragma config FNOSC = PRI              // Oscillator Select (Primary Oscillator (XT, HS, EC))
#pragma config IESO = ON                // Internal External Switch Over Mode (IESO mode (Two-Speed Start-up) enabled)

// CONFIG1
#pragma config WDTPS = PS32768          // Watchdog Timer Postscaler (1:32,768)
#pragma config FWPSA = PR128            // WDT Prescaler (Prescaler ratio of 1:128)
#pragma config WINDIS = ON              // Watchdog Timer Window (Standard Watchdog Timer enabled,(Windowed-mode is disabled))
#pragma config FWDTEN = ON              // Watchdog Timer Enable (Watchdog Timer is enabled)
#pragma config ICS = PGx2               // Comm Channel Select (Emulator/debugger uses EMUC2/EMUD2)
#pragma config GWRP = OFF               // General Code Segment Write Protect (Writes to program memory are allowed)
#pragma config GCP = OFF                // General Code Segment Code Protect (Code protection is disabled)
#pragma config JTAGEN = OFF 

#include "xc.h"
#include "buttons.h"
#include "libpic30.h"
#include <stdlib.h>

#define NEXT_BUTTON PORTDbits.RD13
#define PREV_BUTTON PORTDbits.RD6

 


unsigned char gray(unsigned char x)
{
    return x ^ (x >> 1);
}

unsigned char make_bcd(unsigned char tens, unsigned char ones)
{
    return (tens << 4) | (ones & 0x0F);
}

void init(void)
{
    AD1PCFG = 0xFFFF;

    TRISA = 0x0000;
    LATA = 0x0000;

    TRISDbits.TRISD6 = 1;
    TRISDbits.TRISD13 = 1; 
}

int checkButtons(int program)
{
    if(NEXT_BUTTON == 0)
    {
        __delay32(200000);

        program++;

        if(program > 9)
            program = 1;

        while(NEXT_BUTTON == 0);
    }

    if(PREV_BUTTON == 0)
    {
        __delay32(200000);

        program--;

        if(program < 1)
            program = 9;

        while(PREV_BUTTON == 0);
    }

    return program;
}

int main(void)
{
    unsigned char licznik = 0;
    unsigned char kierunek = 0;

    unsigned char bcd_tens = 0;
    unsigned char bcd_ones = 0;

    int program = 1;
    int poprzedni_program = 1;

    int liczby[36] = {1, 2, 4, 8, 16, 32, 64, 128,
    129, 130, 132, 136, 144, 160, 192,
    193, 194, 196, 200, 208, 224,
    225, 226, 228, 232, 240,
    241, 242, 244, 248,
    249, 250, 252,
    253, 254,
    255};
    init();

    while(1)
    {

        program = checkButtons(program);

        if(program != poprzedni_program)
        {
            if(program == 1)
                licznik = 0;

            else if(program == 2)
                licznik = 255;

            else if(program == 3)
                licznik = 0;

            else if(program == 4)
                licznik = 255;

            else if(program == 5)
            {
                bcd_tens = 0;
                bcd_ones = 0;
            }

            else if(program == 6)
            {
                bcd_tens = 9;
                bcd_ones = 9;
            }
            else if(program == 7){
                licznik = 7;
                kierunek = 0;
            }
            else if(program == 8)
                licznik = 0;
            else if(program == 9)
                licznik = 0;

            poprzedni_program = program;
        }

        // =========================
        // PROGRAM 1 Licznik w gore
        // =========================
        if(program == 1)
        {
            LATA = licznik;
            licznik++;
        }

        // =========================
        // PROGRAM 2 Licznik w dol
        // =========================
        else if(program == 2)
        {
            LATA = licznik;
            licznik--;
        }

        // =========================
        // PROGRAM 3 Gray do gory
        // =========================
        else if(program == 3)
        {
            LATA = gray(licznik);
            licznik++;
        }

        // =========================
        // PROGRAM 4 Gray w dol
        // =========================
        else if(program == 4)
        {
            LATA = gray(licznik);
            licznik--;
        }

        // =========================
        // PROGRAM 5 - Bcd do gory
        // =========================
        else if(program == 5)
        {

            unsigned char bcd = make_bcd(bcd_tens, bcd_ones);
            LATA = bcd;

            bcd_ones++;
            if(bcd_ones > 9)
            {
                bcd_ones = 0;
                bcd_tens++;
                if(bcd_tens > 9)
                    bcd_tens = 0;
            }
        }

        // =========================
        // PROGRAM 6 - Bcd w dol
        // =========================
        else if(program == 6)
        {
            unsigned char bcd = make_bcd(bcd_tens, bcd_ones);
            LATA = bcd;

            if(bcd_ones == 0)
            {
                bcd_ones = 9;
                if(bcd_tens == 0)
                    bcd_tens = 9;
                else
                    bcd_tens--;
            }
            else
            {
                bcd_ones--;
            }
        }

        // =========================
        // PROGRAM 7 - Wezyk
        // =========================
        else if(program == 7)
        {
            LATA = licznik;
            if (kierunek == 0)
                licznik = licznik*2;
            else if (kierunek == 1)
                licznik = licznik/2;

            if (licznik == 224)
                kierunek = 1;
            else if (licznik == 7)
                kierunek = 0;
        }

        // =========================
        // PROGRAM 8 - Kolejka
        // =========================
        else if(program == 8)
        {
            LATA = liczby[licznik];
            licznik++;
            if (licznik == 36)
                licznik = 0;
        }
         // =========================
        // PROGRAM 9 - Generator
        // =========================
        else if(program == 9)
         {
            licznik = (licznik * 5  + 17) % 256;
            LATA = licznik;
        }

        __delay32(800000);
    }

    return 0;
}

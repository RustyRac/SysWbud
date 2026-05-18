#include <stdio.h>
#include <stdlib.h>

#include <stdio.h>
#include <conio.h>
#include <windows.h>


// wypisywanie 8 bitów
void print_binary(unsigned char x)
{
    for(int i = 7; i >= 0; i--)
    {
        printf("%d", (x >> i) & 1);
    }
}

int adc_to_five(int x) {
    if (x < 0 || x > 100) {
        return -1;
    }

    return (x / 20) + 1 - (x == 100);
}


int main()
{
    int adc_value = 4;
    unsigned char licznik = 0;
    unsigned char kierunek = 0;

    int program = 1;
    int poprzedni_program = 1;


    while(1)
    {
        // zmiana programu
        if(kbhit())
        {
            char c = getch();

            if(c == 'n')
            {
                program++;
                if(program > 2) program = 1;
            }

            if(c == 'p')
            {
                program--;
                if(program < 1) program = 2;
            }

            if(c == 'q')
                break;
        }

        // pozycje startowe programow
        if(program != poprzedni_program)
        {
            if(program == 1)
                licznik = 0;

            else if(program == 2){
                licznik = 7;
                kierunek = 0;
            }
            poprzedni_program = program;
        }

        system("cls");

        printf("PROGRAM: %d\n\n", program);

        // =========================
        // PROGRAM 1 - licznik w górę
        // =========================
        if(program == 1)
        {
            printf("TRYB: LICZNIK W GORE\n\n");
            print_binary(licznik);
            licznik++;
        }


        // =========================
        // PROGRAM 2 - WEZYK
        // =========================
        else if(program == 2)
        {
            printf("TRYB: WEZYK\n\n");
            print_binary(licznik);
            if (kierunek == 0)
                licznik = licznik*2;
            else if (kierunek == 1)
                licznik = licznik/2;

            if (licznik == 224)
                kierunek = 1;
            else if (licznik == 7)
                kierunek = 0;
        }

        printf("\n\n");
        printf("n - nastepny program\n");
        printf("p - poprzedni program\n");
        printf("q - wyjscie\n");
        printf("%d", adc_to_five(adc_value));
        Sleep(100 + (20 * adc_to_five(adc_value)));
    }


    return 0;
}


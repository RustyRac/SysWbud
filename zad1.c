#include <stdio.h>
#include <conio.h>
#include <windows.h>

// funkcja kodu Graya
unsigned char gray(unsigned char x)
{
    return x ^ (x >> 1);
}

// wypisywanie 8 bitów
void print_binary(unsigned char x)
{
    for(int i = 7; i >= 0; i--)
    {
        printf("%d", (x >> i) & 1);
    }
}

// BCD -> pakowanie 2 cyfr do 1 bajtu
unsigned char make_bcd(unsigned char tens, unsigned char ones)
{
    return (tens << 4) | (ones & 0x0F);
}

int main()
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

    while(1)
    {
        // zmiana programu
        if(kbhit())
        {
            char c = getch();

            if(c == 'n')
            {
                program++;
                if(program > 9) program = 1;
            }

            if(c == 'p')
            {
                program--;
                if(program < 1) program = 9;
            }

            if(c == 'q')
                break;
        }

        // pozycje startowe programow
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
        // PROGRAM 2 - licznik w dół
        // =========================
        else if(program == 2)
        {
            printf("TRYB: LICZNIK W DOL\n\n");
            print_binary(licznik);
            licznik--;
        }

        // =========================
        // PROGRAM 3 - Gray w górę
        // =========================
        else if(program == 3)
        {
            printf("TRYB: KOD GRAYA (GORA)\n\n");
            print_binary(gray(licznik));
            licznik++;
        }

        // =========================
        // PROGRAM 4 - Gray w dół (255..0)
        // =========================
        else if(program == 4)
        {
            printf("TRYB: KOD GRAYA (DOL)\n\n");
            print_binary(gray(licznik));
            licznik--;
        }

        // =========================
        // PROGRAM 5 - BCD w góre
        // =========================
        else if(program == 5)
        {
            printf("TRYB: BCD W GORE (00-99)\n\n");

            unsigned char bcd = make_bcd(bcd_tens, bcd_ones);
            print_binary(bcd);

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
        // PROGRAM 6 - BCD w dół
        // =========================
        else if(program == 6)
        {
            printf("TRYB: BCD W DOL (99-00)\n\n");

            unsigned char bcd = make_bcd(bcd_tens, bcd_ones);
            print_binary(bcd);

            // dekrementacja BCD
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
        // PROGRAM 7 - WEZYK
        // =========================
        else if(program == 7)
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
        // =========================
        // PROGRAM 8 - KOLEJKA
        // =========================
        if(program == 8)
        {
            printf("KOLEJKA\n\n");
            print_binary(liczby[licznik]);
            licznik++;
            if (licznik == 36)
                licznik = 0;
        }
        // =========================
        // PROGRAM 8 - GENERATOR
        // =========================
        if(program == 9)
        {
            licznik = rand() % 256;
            print_binary(licznik);
        }



        printf("\n\n");
        printf("n - nastepny program\n");
        printf("p - poprzedni program\n");
        printf("q - wyjscie\n");

        Sleep(200);
    }


    return 0;
}

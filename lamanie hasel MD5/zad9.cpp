#include <iostream>
#include <fstream>
#include <thread>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <openssl/md5.h>
#include <algorithm>
#include <mutex>
#include <csignal>

using namespace std;

// g++ -std=c++11 -pthread zad9.cpp -lcrypto //

void *prod_1(), *prod_2(), *prod_3(), *prod_4(), *consm();

///////////////////////////////////////////////////////////////////////////////////////////

mutex mut;

string *slownik;
int l_slownik;

string hasla[1000]; // hasla do zlamania

int zlamane[1000]; // nr. hasla, odznaczone znalezione
// 0-nic 1-znalezione 2-odznaczenie //

string znalezione[1000]; // slowo znalezione

string na_male,str1,na_duze,str2,str3,str4,p_duza,dwu_wyraz;

////////////////////////////////////////////////////////////////////////////////////////// SIGHUP

void signalHandler( int signum )
{
    cout << "Interrupt signal (" << signum << ") received.\n";

    exit(signum);

}


int main()
{

    char nazwa[]="slownik.txt"; // nazwa slownika

    char nazwa_2[]="hasla.txt"; // nazwa pliku z haslami

    fstream plik;

    int i=0;

    string wyraz;

    l_slownik=0;

/////////////////////////////////////////////////////////////////////////////////////////// wczytywanie slownika

    plik.open(nazwa, ios::in);

    while( !plik.eof() ) // zliczanie wyrazow w slowniku
    {
        getline(plik,wyraz);

        l_slownik++;
    }
    plik.close();

    cout<<"\n liczba wyrazow w slowniku: "<<(l_slownik-1)<<endl; // liczba slow w slowniku

    slownik = new string [l_slownik];  // dynamiczne alokowanie pamieci

    plik.open(nazwa, ios::in);

    l_slownik=0;
    while( !plik.eof() ) // wczytywanie do pamiecie globalnej wyrazow slownika
    {
        getline(plik,wyraz);

        slownik[l_slownik]=wyraz;

        l_slownik++;
    }
    plik.close();


    //cout<<"\n wczytuje slownik: \n"<<endl; // wypisanie slownika
    //for(int j=0; j<(l_slownik-1); j++)
    //cout<<j+1<<": "<<slownik[j]<<endl;


    for(int ii=0; ii<1000; ii++) // inicjacja zlamane[]
        zlamane[ii]=0;

/////////////////////////////////////////////////////////////////////////////////////////// wczytywanie hasel do zlamania

    plik.open(nazwa_2, ios::in);

    int l_hasel=0;
    i=0;
    while( !plik.eof() ) // zliczanie wyrazow w slowniku
    {
        getline(plik,hasla[i]);

        //cout<<" haslo "<<i<<": "<<haslo[i]<<endl;

        i++;
        l_hasel++;
    }
    plik.close();

    //cout<<"\n wczytuje hasla: \n"<<endl; // wypisanie hasel
    //for(int j=0; j<(i-1); j++)
    //cout<<j+1<<": "<<hasla[j]<<endl;
    //cout<<endl;

    cout<<" liczba wczytanych hasel: "<<l_hasel-1<<endl<<endl;

/////////////////////////////////////////////////////////////////////////////////////////// watki

    thread t1(prod_1), t2(prod_2), t3(prod_3), t4(prod_4), t5(consm); // watki

    t1.join(); // male litery slownika	 //  t1.detach();

    t2.join(); // duze litery slownika

    t3.join(); // pierwsza duza reszta mala

    t4.join(); // konsument

///////////////////////////////////////////////////////////////////////////////////////////

    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////

void *prod_1( )
{
    unsigned char digest[16];
    char mdString[33];
    const char* string;

    for(int x=0 ; ; x++) // liczby
    {

        for(int licz_1=0; licz_1<(l_slownik-1); licz_1++)	// same slowa //
        {

            mut.lock();
            na_male = slownik[licz_1];
            transform(na_male.begin(), na_male.end(), na_male.begin(), ::tolower); // na male litery

            mut.unlock();

            string = na_male.c_str();
            MD5_CTX ctx;
            MD5_Init(&ctx);
            MD5_Update(&ctx, string, strlen(string));
            MD5_Final(digest, &ctx);

            for (int i = 0; i < 16; i++)
                sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

            str1 = mdString;

            for(int j=0; j<1000; j++)
            {
                mut.lock();
                if( zlamane[j] != 2) // juz znalezione
                {
                    if( str1 == hasla[j] )
                    {
                        zlamane[j]=1;
                        znalezione[j]=string;
                    }
                }
                mut.unlock();
            }
//=============================================================================// slowo + liczba

            na_male += to_string(x);

            string = na_male.c_str();
            MD5_Init(&ctx);
            MD5_Update(&ctx, string, strlen(string));
            MD5_Final(digest, &ctx);

            for (int i = 0; i < 16; i++)
                sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

            str1 = mdString;

            for(int j=0; j<1000; j++)
            {
                mut.lock();
                if( zlamane[j] != 2) // juz znalezione
                {
                    if( str1 == hasla[j] )
                    {
                        zlamane[j]=1;
                        znalezione[j]=string;
                    }
                }
                mut.unlock();
            }
//=============================================================================// liczba + slowo

            na_male = to_string(x) + na_male;

            string = na_male.c_str();
            MD5_Init(&ctx);
            MD5_Update(&ctx, string, strlen(string));
            MD5_Final(digest, &ctx);

            for (int i = 0; i < 16; i++)
                sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

            str1 = mdString;

            for(int j=0; j<1000; j++)
            {
                mut.lock();
                if( zlamane[j] != 2) // juz znalezione
                {
                    if( str1 == hasla[j] )
                    {
                        zlamane[j]=1;
                        znalezione[j]=string;
                    }
                }
                mut.unlock();
            }
//=============================================================================//

        } // l_licz
    } //x
}

///////////////////////////////////////////////////////////////////////////////////////////

void *prod_2()
{
    unsigned char digest[16];
    char mdString[33];
    const char* string;

    for(int x=0 ; ; x++) // liczby
    {

        for(int licz_2=0; licz_2<(l_slownik-1); licz_2++)	// same slowa //
        {

            mut.lock();
            na_duze = slownik[licz_2];
            mut.unlock();

            transform(na_duze.begin(), na_duze.end(), na_duze.begin(), ::toupper); // zmiana na duze litery

            string = na_duze.c_str();
            MD5_CTX ctx;
            MD5_Init(&ctx);
            MD5_Update(&ctx, string, strlen(string));
            MD5_Final(digest, &ctx);
            for (int i = 0; i < 16; i++)
                sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

            str2 = mdString;

            for(int j=0; j<1000; j++)
            {
                mut.lock();
                if( zlamane[j] != 2) // juz znalezione
                {
                    if( str2 == hasla[j] )
                    {
                        zlamane[j]=1;
                        znalezione[j]=string;
                    }
                }
                mut.unlock();
            }
//=============================================================================// slowo + liczba

            string = ( na_duze + to_string(x) ).c_str();

            MD5_Init(&ctx);
            MD5_Update(&ctx, string, strlen(string));
            MD5_Final(digest, &ctx);

            for (int i = 0; i < 16; i++)
                sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

            str2 = mdString;

            for(int j=0; j<1000; j++)
            {
                mut.lock();
                if( zlamane[j] != 2) // juz znalezione
                {
                    if( str2 == hasla[j] )
                    {
                        zlamane[j]=1;
                        znalezione[j]=string;
                    }
                }
                mut.unlock();
            }
//=============================================================================// liczba + slowo

            string = (to_string(x) + na_duze).c_str();

            MD5_Init(&ctx);
            MD5_Update(&ctx, string, strlen(string));
            MD5_Final(digest, &ctx);

            for (int i = 0; i < 16; i++)
                sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

            str2 = mdString;

            for(int j=0; j<1000; j++)
            {
                mut.lock();
                if( zlamane[j] != 2) // juz znalezione
                {
                    if( str2 == hasla[j] )
                    {
                        zlamane[j]=1;
                        znalezione[j]=string;
                    }
                }
                mut.unlock();
            }
//=============================================================================//

        } // l_licz
    } //x
}
///////////////////////////////////////////////////////////////////////////////////////////

void *prod_3()
{
    unsigned char digest[16];
    char mdString[33];
    const char* string;

    for(int x=0 ; ; x++) // liczby
    {

        for(int licz_3=0; licz_3<(l_slownik-1); licz_3++)	// same slowa //
        {

            mut.lock();
            p_duza = slownik[licz_3];
            mut.unlock();

            transform(p_duza.begin(), p_duza.end(), p_duza.begin(), ::tolower); // na poczatku wszystkie na male
            p_duza[0] = toupper(p_duza[0]); // pierwsza na duza

            string = p_duza.c_str();
            MD5_CTX ctx;
            MD5_Init(&ctx);
            MD5_Update(&ctx, string, strlen(string));
            MD5_Final(digest, &ctx);
            for (int i = 0; i < 16; i++)
                sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
            str3 = mdString;

            for(int j=0; j<1000; j++)
            {
                mut.lock();
                if( zlamane[j] != 2) // juz znalezione
                {
                    if( str3 == hasla[j] )
                    {
                        zlamane[j]=1;
                        znalezione[j]=string;
                    }
                }
                mut.unlock();
            }
//=============================================================================// slowo + liczba

            string = ( p_duza + to_string(x) ).c_str();

            MD5_Init(&ctx);
            MD5_Update(&ctx, string, strlen(string));
            MD5_Final(digest, &ctx);

            for (int i = 0; i < 16; i++)
                sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

            str3 = mdString;

            for(int j=0; j<1000; j++)
            {
                mut.lock();
                if( zlamane[j] != 2) // juz znalezione
                {
                    if( str3 == hasla[j] )
                    {
                        zlamane[j]=1;
                        znalezione[j]=string;
                    }
                }
                mut.unlock();
            }
//=============================================================================// liczba + slowo

            string = ( to_string(x) + p_duza ).c_str();

            MD5_Init(&ctx);
            MD5_Update(&ctx, string, strlen(string));
            MD5_Final(digest, &ctx);

            for (int i = 0; i < 16; i++)
                sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

            str3 = mdString;

            for(int j=0; j<1000; j++)
            {
                mut.lock();
                if( zlamane[j] != 2) // juz znalezione
                {
                    if( str3 == hasla[j] )
                    {
                        zlamane[j]=1;
                        znalezione[j]=string;
                    }
                }
                mut.unlock();
            }
//=============================================================================//

        } // l_licz
    } //x
}

///////////////////////////////////////////////////////////////////////////////////////////

void *prod_4()
{
    unsigned char digest[16];
    char mdString[33];
    const char* string;

    for(int x=0 ; ; x++) // liczby
    {

        for(int licz_4=0; licz_4<(l_slownik-1); licz_4++)	// same slowa //
        {
            for(int licz_5=0; licz_5<(l_slownik-1); licz_5++)
            {
                mut.lock();
                dwu_wyraz = slownik[licz_4]+slownik[licz_5] ; // 2 wyrazowe polaczenie
                mut.unlock();

                transform(dwu_wyraz.begin(), dwu_wyraz.end(), dwu_wyraz.begin(), ::tolower); // na poczatku wszystkie na male

                string = dwu_wyraz.c_str();

                //cout<<string<<endl;

                MD5_CTX ctx;
                MD5_Init(&ctx);
                MD5_Update(&ctx, string, strlen(string));
                MD5_Final(digest, &ctx);
                for (int i = 0; i < 16; i++)
                    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);
                str4 = mdString;

                for(int j=0; j<1000; j++)
                {
                    mut.lock();
                    if( zlamane[j] != 2) // juz znalezione
                    {
                        if( str4 == hasla[j] )
                        {
                            zlamane[j]=1;
                            znalezione[j]=string;
                        }
                    }
                    mut.unlock();
                }
//=============================================================================// slowo + liczba

                string = ( dwu_wyraz + to_string(x) ).c_str();

                MD5_Init(&ctx);
                MD5_Update(&ctx, string, strlen(string));
                MD5_Final(digest, &ctx);

                for (int i = 0; i < 16; i++)
                    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

                str4 = mdString;

                for(int j=0; j<1000; j++)
                {
                    mut.lock();
                    if( zlamane[j] != 2) // juz znalezione
                    {
                        if( str4 == hasla[j] )
                        {
                            zlamane[j]=1;
                            znalezione[j]=string;
                        }
                    }
                    mut.unlock();
                }
//=============================================================================// liczba + slowo

                string = ( to_string(x) + dwu_wyraz  ).c_str();

                MD5_Init(&ctx);
                MD5_Update(&ctx, string, strlen(string));
                MD5_Final(digest, &ctx);

                for (int i = 0; i < 16; i++)
                    sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

                str4 = mdString;

                for(int j=0; j<1000; j++)
                {
                    mut.lock();
                    if( zlamane[j] != 2) // juz znalezione
                    {
                        if( str4 == hasla[j] )
                        {
                            zlamane[j]=1;
                            znalezione[j]=string;
                        }
                    }
                    mut.unlock();
                }
//=============================================================================//
            }
        } // l_licz
    } //x
}

///////////////////////////////////////////////////////////////////////////////////////////

void *consm()
{

    while(1)
    {

        mut.lock();

        for(int i=0; i<1000; i++)
        {
            if(zlamane[i] == 1)
            {
                cout<<" zlamano haslo: "<<hasla[i]<<" haslo to: "<<znalezione[i]<<endl;
                zlamane[i]=2; // odznaczenie
            }
        }

        mut.unlock();

        signal(SIGINT, signalHandler);

    }


}

///////////////////////////////////////////////////////////////////////////////////////////


/*
unsigned char digest[16];
    const char* string = "absurdism";

    printf("string length: %d\n", strlen(string));

    MD5_CTX ctx;
    MD5_Init(&ctx);
    MD5_Update(&ctx, string, strlen(string));
    MD5_Final(digest, &ctx);

    char mdString[33];
    for (int i = 0; i < 16; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    printf("md5 digest: %s\n", mdString);

*/










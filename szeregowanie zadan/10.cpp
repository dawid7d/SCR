#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <queue>

using namespace std;


/////////////////////////////////////////////////////////////////////////////////////////////////////////

class Proces
{

public:

    int chwila; // chwila pojawienia sie procesu
    int nr; // numer procesu
    int pi; // priorytet
    int ri; // czas wykonywania

};

//=================================================================//

class Procesor
{

public:

    Proces p;

    bool wolny; // 1-zajety 0-wolny

    int czas; // wykowywania sie procesu na procesorze

    Procesor()
    {
        wolny=0;
        czas=0;
    }
};

//=================================================================// 

struct porownaj2
{
    bool operator ()( const Proces & p1, const Proces & p2 )
    {
        return ( p1.ri > p2.ri );
    }
};

//=================================================================// 

struct porownaj3
{
    bool operator ()( const Proces & p1, const Proces & p2 )
    {
        if ( p1.pi > p2.pi ) return true;

	else if ( p1.pi < p2.pi ) return false;

	else { 
		return ( p1.ri > p2.ri );
	     }
    }
};

//=================================================================// 

queue <Proces> kolejka0;

priority_queue <Proces, vector <Proces>, porownaj2> kolejka2;

priority_queue <Proces, vector <Proces>, porownaj3> kolejka5; 

/////////////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[])
{

    int kod_alg;
    int l_proc;
    int k_czasu ;

    kod_alg = atoi(argv[1]);
    l_proc  = atoi(argv[2]);
    k_czasu = atoi(argv[3]);

    cout<<"\n kod algorytmu: "<<kod_alg<<endl<<" liczba procesow: "<<l_proc<<endl<<" kwant czasu: "<<k_czasu<<endl<<endl;

    Proces   pr;

    Procesor procesor[l_proc]; // tablica procesorow


    int bierzacy_czas=0; // chwila czasu proc.
    int koniec=0;
    bool wywlaszczenie=0; // 0-nie 1-tak

    switch (kod_alg)
    {

/////////////////////////////////////////////////////////////////////////////////////////////////////////

    case 0:
    {
        cout<<" 0. strategia FCFS "<<endl<<endl;

        bierzacy_czas=0; // chwila czasu proc.
        koniec=0; // wszystkie procesy obsluzone

        while ( !cin.eof() || koniec>0)
        {
            cout<<" "<<bierzacy_czas<<" ";
//=================================================================// wczytwanie linijki
            if ( !cin.eof() )
            {
                cin>>pr.chwila;

                if( cin.get() != '\n' )
                {
                    while(!cin.eof())
                    {
                        cin>> pr.nr >> pr.pi >> pr.ri; // wczytanie procesu

                        kolejka0.push(pr); // dodanie do kolejki
                        koniec++;

                        if( cin.get() == '\n' )break;
                    }
                }
            }
//=================================================================// szeregowanie
            for (int i=0; i<l_proc; i++)
            {

                if( procesor[i].wolny == 0 && kolejka0.empty() == 0) // wolny proc. i pusta kolejka
                {
                    procesor[i].p = kolejka0.front(); // dodaj proces

                    kolejka0.pop(); // usun z kolejki

                    procesor[i].wolny = 1; // procesor zajety
                }
//=================================================================// wyswietlanie wynikow
                if( procesor[i].p.ri > 0 )
                    cout<<" "<<procesor[i].p.nr; //cout<<procesor[i].p.ri<<endl;

                else
                {
                    cout<<"-1";    // brak procesu na proc.
                }

                //cout<<"#"<<procesor[i].p.chwila<<" "<<procesor[i].p.nr<<" "<<procesor[i].p.pi<<" "<<procesor[i].p.ri<<endl;

//=================================================================//
                procesor[i].p.ri-- ; 						// mozna dodac ze jak wieksze od zera*

                if ( procesor[i].p.ri == 0 ) // jesli koniec
                {
                    procesor[i].wolny = 0; // zwalnia procesor
                    koniec--;
                }

            }  //for

            cout<<endl; // wys. wynikow

            bierzacy_czas++; // kolejna chwila

        } // while

    } //case
    break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////

    case 2:

        cout<<" 2. strategia RSTF "<<endl<<endl;

        bierzacy_czas=0; // chwila czasu proc.
        koniec=0; // wszystkie procesy obsluzone

        while ( !cin.eof() || koniec>0)
        {
            cout<<" "<<bierzacy_czas<<" ";
//=================================================================// wczytwanie linijki
            if ( !cin.eof() )
            {
                cin>>pr.chwila;

                if( cin.get() != '\n' )
                {
                    while(!cin.eof())
                    {
                        cin>> pr.nr >> pr.pi >> pr.ri; // wczytanie procesu

                        kolejka2.push(pr); // dodanie do kolejki
                        koniec++;

                        if( cin.get() == '\n' )break;
                    }
                }
            }
//=================================================================// szeregowanie //
            for (int i=0; i<l_proc; i++)
            {

                wywlaszczenie=1;
                for (int j=0; j<l_proc; j++)
                {
                    if( procesor[j].wolny == 0 ) wywlaszczenie=0; // jesli jakis proc. wolny to nie ma wywlaszczenia
                }

                // jesli wykonuje sie juz kwant czasu i jest inny z mniejszym czasem w kolejce i nie ma innych wolnych proc.
                if( (procesor[i].czas >= k_czasu) && (procesor[i].p.ri > kolejka2.top().ri) && (wywlaszczenie==1) )
                {
                    kolejka2.push( procesor[i].p );
                    procesor[i].wolny = 0;
                }

//=================================================================// proces na procesor

                if( procesor[i].wolny == 0 && kolejka2.empty() == 0) // wolny proc. i nie pusta kolejka
                {
                    procesor[i].p = kolejka2.top(); // dodaj proces
                    procesor[i].czas=0;

                    kolejka2.pop(); // usun z kolejki

                    procesor[i].wolny = 1; // procesor zajety
                }
//=================================================================// wyswietlanie wynikow

                if( procesor[i].p.ri > 0 )
                    cout<<" "<<procesor[i].p.nr; //cout<<procesor[i].p.ri<<endl;

                else
                {
                    cout<<"-1";    // brak procesu na proc.
                }

                //cout<<"#"<<procesor[i].p.chwila<<" "<<procesor[i].p.nr<<" "<<procesor[i].p.pi<<" "<<procesor[i].p.ri<<endl;

//=================================================================// wykonanie procesu
                procesor[i].p.ri-- ;
                procesor[i].czas++;

                if ( procesor[i].p.ri == 0 ) // jesli koniec
                {
                    procesor[i].wolny = 0; // zwalnia procesor
                    koniec--;
                }


            }  //for

            cout<<endl; // wys. wynikow

            bierzacy_czas++; // kolejna chwila, koniec bierzacej

        }//while

        break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
    case 3:

	cout<<" 3. strategia RR "<<endl<<endl;

        bierzacy_czas=0; // chwila czasu proc.
        koniec=0; // wszystkie procesy obsluzone

        while ( !cin.eof() || koniec>0 )
        {
            cout<<" "<<bierzacy_czas<<" ";
//=================================================================// wczytwanie linijki
            if ( !cin.eof() )
            {
                cin>>pr.chwila;

                if( cin.get() != '\n' )
                {
                    while(!cin.eof())
                    {
                        cin>> pr.nr >> pr.pi >> pr.ri; // wczytanie procesu

                        kolejka0.push(pr); // dodanie do kolejki
                        koniec++;

                        if( cin.get() == '\n' )break;
                    }
                }
            }
//=================================================================// szeregowanie //
            for (int i=0; i<l_proc; i++)
            {

                wywlaszczenie=1;
                for (int j=0; j<l_proc; j++)
                {
                    if( procesor[j].wolny == 0 ) wywlaszczenie=0; // jesli jakis proc. wolny to nie ma wywlaszczenia
                }


                if( (procesor[i].czas >= k_czasu) && (wywlaszczenie==1) ) // jesli wykonuje sie juz kwant czasu i nie ma innych wolnych proc. 
                {
                    kolejka0.push( procesor[i].p );
                    procesor[i].wolny = 0;
                }

//=================================================================// proces na procesor

                if( procesor[i].wolny == 0 && kolejka0.empty() == 0) // wolny proc. i nie pusta kolejka
                {
                    procesor[i].p = kolejka0.front(); // dodaj proces
                    procesor[i].czas=0;

                    kolejka0.pop(); // usun z kolejki

                    procesor[i].wolny = 1; // procesor zajety
                }

//=================================================================// wyswietlanie wynikow

                if( procesor[i].p.ri > 0 )
                    cout<<" "<<procesor[i].p.nr; 

                else
                {
                    cout<<"-1";    // brak procesu na proc.
                }

//=================================================================// wykonanie procesu

                procesor[i].p.ri-- ;
                procesor[i].czas++;

                if ( procesor[i].p.ri == 0 ) // jesli koniec
                {
                    procesor[i].wolny = 0; // zwalnia procesor
                    koniec--;
                }


            }  //for

            cout<<endl; // wys. wynikow

            bierzacy_czas++; // kolejna chwila, koniec bierzacej

        }//while

        break;
/////////////////////////////////////////////////////////////////////////////////////////////////////////
    case 5:

cout<<" 5. szeregowanie priorytetowe (RSTF)"<<endl<<endl;

        bierzacy_czas=0; // chwila czasu proc.
        koniec=0; // wszystkie procesy obsluzone

        while ( !cin.eof() || koniec>0)
        {
            cout<<" "<<bierzacy_czas<<" ";
//=================================================================// wczytwanie linijki
            if ( !cin.eof() )
            {
                cin>>pr.chwila;

                if( cin.get() != '\n' )
                {
                    while(!cin.eof())
                    {
                        cin>> pr.nr >> pr.pi >> pr.ri; // wczytanie procesu

                        kolejka5.push(pr); // dodanie do kolejki
                        koniec++;

                        if( cin.get() == '\n' )break;
                    }
                }
            }
//=================================================================// szeregowanie //
            for (int i=0; i<l_proc; i++)
            {

                wywlaszczenie=1;
                for (int j=0; j<l_proc; j++)
                {
                    if( procesor[j].wolny == 0 ) wywlaszczenie=0; // jesli jakis proc. wolny to nie ma wywlaszczenia
                }

                // jesli wykonuje sie juz kwant czasu i jest inny z wiekszym priorytetem i nie ma innych wolnych proc.
                if( (procesor[i].czas >= k_czasu) && (procesor[i].p.pi > kolejka5.top().pi ) && (wywlaszczenie==1) )
                {
                    kolejka5.push( procesor[i].p );
                    procesor[i].wolny = 0;
                }

//=================================================================// proces na procesor

                if( procesor[i].wolny == 0 && kolejka5.empty() == 0) // wolny proc. i nie pusta kolejka
                {
                    procesor[i].p = kolejka5.top(); // dodaj proces
                    procesor[i].czas=0;

                    kolejka5.pop(); // usun z kolejki

                    procesor[i].wolny = 1; // procesor zajety
                }
//=================================================================// wyswietlanie wynikow

                if( procesor[i].p.ri > 0 )
                    cout<<" "<<procesor[i].p.nr; //cout<<procesor[i].p.ri<<endl;

                else
                {
                    cout<<"-1";    // brak procesu na proc.
                }

//=================================================================// wykonanie procesu
                procesor[i].p.ri-- ;
                procesor[i].czas++;

                if ( procesor[i].p.ri == 0 ) // jesli koniec
                {
                    procesor[i].wolny = 0; // zwalnia procesor
                    koniec--;
                }


            }  //for

            cout<<endl; // wys. wynikow

            bierzacy_czas++; // kolejna chwila, koniec bierzacej

        }//while

        break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////

    default:

        cout<<" bledna kod algorytmu !"<<endl<<" dostepne: 0, 2, 3, 5 "<<endl;

        break;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
    }










}// main


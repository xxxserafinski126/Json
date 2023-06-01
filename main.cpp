#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <map>
#include "Json.h"

using namespace std;

const string PLIK_KONFIGURACJI = "konfiguracja.json";
const string PLIK_WYNIKOW = "punkty.json";

struct Wynik
{
    string pseudonim;
    int punkty;
};


void pokazOpcje(const DynamicJsonDocument& konfiguracja)
{
    cout<< "Opcje:\n";
    cout<< "1. Ustawienia graficzne: " << konfiguracja["grafika"]["rozdzielczosc"].as<string>() << ", pelny ekran: " << (konfiguracja["grafika"]["pelny_ekran"].as<bool>() ? "Tak" : "Nie") << "\n";
    cout<< "2. Ustawienia dzwieku: muzyka: " << konfiguracja["dzwiek"]["glosnosc_muzyki"].as<int>() << ", efekty dzwiekowe: " << konfiguracja["dzwiek"]["glosnosc_efektow"].as<int>() << "\n";
    cout<< "0. Powrot do menu glownego\n";
    cout<< "-----------------------------\n";
}

void pokazMenu()
{
    cout<< "Menu:\n";
    cout<< "1. Opcje\n";
    cout<< "2. Tabela wynikow\n";
    cout<< "3. Nowa gra\n";
    cout<< "0. Wyjscie\n";
    cout<< "-----------------\n";
}

void zapiszKonfiguracje(DynamicJsonDocument& dokument)
{
    ofstream plikKonfiguracji(PLIK_KONFIGURACJI);
    if (plikKonfiguracji)
    {
        serializeJson(dokument, plikKonfiguracji);
        plikKonfiguracji.close();
        cout << "Konfiguracja zostala zapisana.\n";
    }
    else
    {
        cerr << "Blad podczas zapisu pliku konfiguracyjnego.\n";
    }
}

void wczytajKonfiguracje(DynamicJsonDocument& dokument)
{
    ifstream plikKonfiguracji(PLIK_KONFIGURACJI);
    if (plikKonfiguracji)
    {
        deserializeJson(dokument, plikKonfiguracji);
        plikKonfiguracji.close();
    }
    else
    {
        cerr << "Blad podczas odczytu pliku konfiguracyjnego.\n";
    }
}

void zapiszWyniki(const vector<Wynik>& wyniki)
{
    DynamicJsonDocument dokument(1024);
    JsonArray jsonArray = dokument.to<JsonArray>();

    for (const auto& wynik : wyniki)
    {
        JsonObject obj = jsonArray.createNestedObject();
        obj["pseudonim"] = wynik.pseudonim;
        obj["punkty"] = wynik.punkty;
    }

    ofstream plikWynikow(PLIK_WYNIKOW);
    if (plikWynikow)
    {
        serializeJson(dokument, plikWynikow);
        plikWynikow.close();
        cout << "Wyniki zostaly zapisane.\n";
    }
    else
    {
        cerr << "Blad podczas zapisu pliku wynikow.\n";
    }
}

void wczytajWyniki(vector<Wynik>& wyniki)
{
    ifstream plikWynikow(PLIK_WYNIKOW);
    if (plikWynikow)
    {
        DynamicJsonDocument dokument(1024);
        deserializeJson(dokument, plikWynikow);
        plikWynikow.close();

        wyniki.clear();
        for (const auto& obj : dokument.as<JsonArray>())
        {
            Wynik wynik;
            wynik.pseudonim = obj["pseudonim"].as<string>();
            wynik.punkty = obj["punkty"].as<int>();
            wyniki.push_back(wynik);
        }
    }
    else
    {
        cerr<< "Blad podczas odczytu pliku wynikow.\n";
    }
}


void wyswietlNajlepszeWyniki(const vector<Wynik>& wyniki)
{
    cout<< "Tabela wynikow:\n";
    cout<< "Pseudonim\tWynik\n";
    for (const auto& wynik : wyniki)
    {
        cout << wynik.pseudonim << "\t" << wynik.punkty << "\n";
    }
    cout<< "\n";
}

void zmienOpcje(DynamicJsonDocument& konfiguracja)
{
    int wybor;
    while (true)
    {
        pokazOpcje(konfiguracja);
        cout << "Wybierz opcje, ktora chcesz zmienic, lub 0, aby powrocic: ";
        cin >> wybor;
        cout << "\n";

        switch (wybor)
        {
        case 1:
        {
            cout<< "Dostepne rozdzielczosci: 720p, 1080p, 1440p, 4K\n";
            string rozdzielczosc;
            cout<< "Podaj rozdzielczosc: ";
            cin>> rozdzielczosc;
            konfiguracja["grafika"]["rozdzielczosc"] = rozdzielczosc;

            bool pelnyEkran;
            cout<< "Pelny ekran? (1 dla tak, 0 dla nie): ";
            cin>> pelnyEkran;
            konfiguracja["grafika"]["pelny_ekran"] = pelnyEkran;

            zapiszKonfiguracje(konfiguracja);
        }
        break;
        case 2:
        {
            int glosnoscMuzyki;
            cout<< "Podaj glosnosc muzyki (0-100): ";
            cin>> glosnoscMuzyki;

            konfiguracja["dzwiek"]["glosnosc_muzyki"] = glosnoscMuzyki;

            int glosnoscEfektow;
            cout << "Podaj glosnosc efektow dzwiekowych (0-100): ";
            cin >> glosnoscEfektow;
            konfiguracja["dzwiek"]["glosnosc_efektow"] = glosnoscEfektow;

            zapiszKonfiguracje(konfiguracja);
        }
        break;
        case 0:
            return;
        default:
            cout << "Nieprawidlowy wybor. Sprobuj ponownie.\n";
            break;
        }
        cout << "\n";
    }
}



void zagrajWMiniGra(vector<Wynik>& wyniki)
{

    srand(time(NULL));

    string pseudonim;
    int liczbaDoZgadniecia;

    cout << "Nowa gra\n";
    cout << "Podaj swoj pseudonim: ";
    cin >> pseudonim;



    cout << "Zgadnij liczbe! | przedział od 1 do 100\n ";

    srand((time(NULL)));
    int losowaLiczba = rand() % 100 + 1;
    cout<<losowaLiczba<<endl;
    int probki = 0;

    while (true)
    {
        int zgadnij;
        cout << "Podaj liczbe: ";
        cin >> zgadnij;
        probki++;

        if (zgadnij == losowaLiczba)
        {
            cout << "Brawo! Zgadnoles liczbe " << losowaLiczba << " po " << probki << " probach.\n";
            Wynik wynik;
            wynik.pseudonim = pseudonim;
            wynik.punkty = probki;
            wyniki.push_back(wynik);
            sort(wyniki.begin(), wyniki.end(), [](const Wynik& w1, const Wynik& w2)
            {
                return w1.punkty < w2.punkty;
            });
            if (wyniki.size() > 10)
            {
                wyniki.resize(10);
            }
            zapiszWyniki(wyniki);
            break;
        }
        else if (zgadnij < losowaLiczba)
        {
            cout<< "Za malo!\n";
        }
        else
        {

            cout<< "Za duzo!\n";
        }
    }
}

int main()
{

    DynamicJsonDocument konfiguracja(1024);
    wczytajKonfiguracje(konfiguracja);

    vector<Wynik> wyniki;
    wczytajWyniki(wyniki);

    int wybor;
    while (true)
    {
        pokazMenu();
        cout<< "Wybierz opcje: ";
        cin>> wybor;
        cout<< "\n";

        switch(wybor)
        {
        case 1:
            zmienOpcje(konfiguracja);
            break;
        case 2:
            wyswietlNajlepszeWyniki(wyniki);
            break;
        case 3:
            zagrajWMiniGra(wyniki);
            break;
        case 0:
            zapiszKonfiguracje(konfiguracja);
            return 0;
        default:
            cout<< "Nieprawidlowy wybor. Sprobuj ponownie.\n";
            break;
        }

        cout<< "\n";
    }
}

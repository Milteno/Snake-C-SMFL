#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h>
#include <time.h>
#include <sstream>
#include <SFML/Audio.hpp>

using namespace sf;
using namespace std;

int nx = 55, ny = 50;
int N = 40, M = 50; //wielkosc okna
int size = 16; //wielkosc tekstury 16x16
int w = nx * size; //szerokosc okna
int h = ny * size; //wysokosc okna
int kierunek, dlugosc = 1; //kierunek snake'a, dlugosc snake'a
float d = 0.3; //opoznienie
float delay = d; //opoznienie
int wynik = 0;

struct Snake {
    int x, y;
}
s[110]; //dlugosc snake'a maksymalna 110 pól

struct Food {
    int x, y;
} f; //lokalizacja jablka

struct Gooldenapple {
    int x, y;
} a;

struct Obj //przeszkoda
{
    int x, y;
} o;
Obj ob[100];

struct grass //trawa
{
    int x, y;
} g;
grass gr[100];

Music bite; //dzwiek ugryzienia
Music game_over; //dzwiek konca gry


int prawy_interfejs(RenderWindow &window,Text text)
{
    text.setString("Wynik: " + to_string(wynik)); //prawa strona aplikacji napisy itp

        text.setPosition(nx * size - 200, ny * size - 750);
        window.draw(text);

        text.setString("Dlugosc: " + to_string(dlugosc));

        text.setPosition(nx * size - 200, ny * size - 700);
        window.draw(text);

        text.setString("Sterowanie:");
        text.setPosition(nx * size - 220, ny * size - 650);
        window.draw(text);

        text.setString("R - Reset");
        text.setPosition(nx * size - 220, ny * size - 600);
        window.draw(text);

        text.setString("W - gora");
        text.setPosition(nx * size - 220, ny * size - 550);
        window.draw(text);

        text.setString("S - dol");
        text.setPosition(nx * size - 220, ny * size - 500);
        window.draw(text);

        text.setString("A - lewo");
        text.setPosition(nx * size - 220, ny * size - 450);
        window.draw(text);

        text.setString("D - prawo");
        text.setPosition(nx * size - 220, ny * size - 400);
        window.draw(text);

        text.setString("ESC-wyjscie");
        text.setPosition(nx * size - 220, ny * size - 350);
        window.draw(text);
}

int poczatkowa_pozycja_owocow()
{
    f.x = 5;
    f.y = 5;
    a.x = 99;
    a.y = 99;
}

int renderowanie_obiektow()
{
       for (int k = 0; k <= 70; ++k) //elementy obiekty
    {
        o.x = ((rand() % 78) + 2) / 2;
        cout << "Pozycja(" << k << ")bloku kamienia x: " << o.x << endl;
        ob[k].x = o.x;

        o.y = ((rand() % 98) + 2) / 2;
        cout << "Pozycja(" << k << ")bloku kamienia y: " << o.y << endl;
        ob[k].y = o.y;

        g.x = rand() % N;
        cout << "Pozycja(" << k << ")bloku trawy x: " << g.x << endl; //losowanie pozycji obiektow
        gr[k].x = g.x;

        g.y = rand() % M;
        cout << "Pozycja(" << k << ")bloku trawy y: " << g.y << endl;
        cout << "===================" << endl;
        gr[k].y = g.y;

    }
}

int ugryzienie_sie(RenderWindow &window,Text text)
{
                game_over.openFromFile("sound/game_over.wav"); //wczytanie dzwieku
                game_over.play(); //odtworzenie dzeiwku
                text.setString("           Ugryzles sie           \n Wcisnij R, aby zrestartowac ...");
                text.setPosition(80, 350); //pozycja tekstu px
                window.draw(text); //wyswietlenie tekstu

                window.display();
}

int uderzenie_w_kamien(RenderWindow &window,Text text)
{

        game_over.openFromFile("sound/game_over.wav");
        game_over.play();
        text.setString("     Uderzyles w kamien           \n Wcisnij R, aby zrestartowac ...");
        text.setPosition(80, 350);
        window.draw(text);

        window.display();

}

int rysowanie_robaka(RenderWindow &window,Sprite sprite)
{
for (int i = 0; i < dlugosc; i++) {
            sprite.setPosition(s[i].x * size, s[i].y * size);
            window.draw(sprite);
        } //robak
}

int zapelnianie_trawa(RenderWindow &window,Sprite sprite)
{

        for (int i = 0; i < N; i++)
            for (int j = 0; j < M; j++) {
                sprite.setPosition(i * size, j * size);
                window.draw(sprite);
            } //petla na zapelnienie okna trawa
}

 int reset(RenderWindow &window)
    {
        window.close();
        delay = d;
        kierunek = 0;
        wynik = 0;
        dlugosc = 1;
        s[0].x = 0;
        s[0].y = 0;
    }

int Tick() //poruszanie sie
{
    bite.openFromFile("sound/bite.ogg"); //wczytanie dzwieku ugryzienia

    for (int i = dlugosc; i > 0; --i) {
        s[i].x = s[i - 1].x; //zmienia pozycje pól snake'a
        s[i].y = s[i - 1].y;
    }

    if (kierunek == 0) s[0].y += 1; //dól
    if (kierunek == 1) s[0].y -= 1; //góra
    if (kierunek == 2) s[0].x += 1; //prawo
    if (kierunek == 3) s[0].x -= 1; //lewo

    if ((s[0].x == f.x) && (s[0].y == f.y)) //zdobycie owocu
    {
        if (delay > 0.01) {
            delay -= 0.01; //zmniejszenie opoznienia po zebraniu owocu
        }
        dlugosc++;
        wynik += 1; //aktualizacja wyniku
        system("cls");
        cout << "Predkosc: " << delay << endl;
        cout << "Twoj wynik to: " << wynik << endl;

        bite.play(); //odtwarzanie dzwieku ugryzienia

        int r = rand() % 9;
        cout << r << endl;
        if (r == 0 || r == 1 || r == 2 || r == 3) {
            a.x = rand() % N;
            a.y = rand() % M;
            r = 10;
        }

        sleep(milliseconds(200)); //uspienie zeby odtworzyc dzwiek

        if (delay > 0.01) {
            delay -= 0.01; //zmniejszenie opoznienia po zebraniu owocu
        }

        f.x = rand() % N;
        f.y = rand() % M; //pozycja owocu po zjedzeniu

        if (f.x == N - 1 || f.y == M - 1 || f.x == N - N || f.y == M - M) { //owoc nie moze byc zrespiony na koncach mapy dla bezpieczenstwa
            f.x = rand() % N;
            f.y = rand() % M;
        }
        for (int k = 0; k <= 70; ++k) //relokacja owocu
        {
            if ((f.x == ob[k].x) && (f.y == ob[k].y) || (f.x == a.x) && (f.y == a.y)) {
                f.x = rand() % N;
                f.y = rand() % M;
            }
        }
    }

    if ((s[0].x == a.x) && (s[0].y == a.y)) //zdobycie owocu
    {
        if (delay > 0.01) {
            delay -= 0.01; //zmniejszenie opoznienia po zebraniu owocu
        }
        dlugosc++;

        wynik += 5; //aktualizacja wyniku
        system("cls");
        cout << "Predkosc: " << delay << endl;
        cout << "Twoj wynik to: " << wynik << endl;

        bite.play(); //odtwarzanie dzwieku ugryzienia
        sleep(milliseconds(200)); //uspienie zeby odtworzyc dzwiek
        a.x = 99;
        a.y = 99;

        if (a.x == N - 1 || a.y == M - 1 || a.x == N - N || a.y == M - M) { //owoc nie moze byc zrespiony na koncach mapy dla bezpieczenstwa
            a.x = rand() % N;
            a.y = rand() % M;
        }
        for (int k = 0; k <= 40; ++k) //relokacja owocu
        {
            if (((a.x == ob[k].x) && (a.y == ob[k].y)) || ((a.x == f.x) && (a.y == f.y))) {
                a.x = rand() % N;
                a.y = rand() % M;
            }
        }
    }

    if (s[0].x >= N)
        s[0].x = 0; //jesli dotknie sciany, czyli jesli pozycja bedzie wieksza niz szerokosc i dlugosc planszy to przenosi go na poczatek i odwrotnie

    if (s[0].x < 0)
        s[0].x = N - 1;

    if (s[0].y >= M)
        s[0].y = 0;

    if (s[0].y < 0)
        s[0].y = M - 1;
}

int main() {

    srand(time(0));
    Font font; //czcionka
    if (!font.loadFromFile("font/standard.ttf")) {
        cout << "Blad przy ladowaniu czcionki";
    }

    Text text; //czcionka
    text.setFont(font);
    text.setCharacterSize(30);
    text.setColor(Color::Red);
    text.setStyle(Text::Bold | Text::Underlined);

    RenderWindow window(VideoMode(w, h), "Snake Game!"); //renderuje okno
    window.setFramerateLimit(60); // 60 fps

    Texture t1,t2,t3,t4,t5,t6,t7,t8;

    t1.loadFromFile("images/grass.png");
    t2.loadFromFile("images/blue.png");
    t3.loadFromFile("images/apple.png");
    t4.loadFromFile("images/white.png");
    t5.loadFromFile("images/stone.png");
    t6.loadFromFile("images/grass12.png");
    t7.loadFromFile("images/restart.png"); //ladowanie textur
    t8.loadFromFile("images/golden.png");

    Sprite sprite1(t1); //sprite'y
    Sprite sprite2(t2);
    Sprite sprite3(t3);
    Sprite sprite4(t4);
    Sprite sprite5(t5);
    Sprite sprite6(t6);
    Sprite sprite7(t7);
    Sprite sprite8(t8);

    Clock clock;
    float timer = 0; //opoznienie

    poczatkowa_pozycja_owocow();

    renderowanie_obiektow();

    while (window.isOpen()) {

        float time = clock.getElapsedTime().asSeconds(); //timer
        clock.restart();
        timer += time;

        Event e;
        while (window.pollEvent(e)) //aktywacja X
        {
            if (e.type == Event::Closed)
                window.close();
        }

        if (Keyboard::isKeyPressed(Keyboard::W)) { //wcisniecie przycisku i blokada przeciwnego kierunku
            if (kierunek != 0) {
                kierunek = 1;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            if (kierunek != 3) {
                kierunek = 2;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            if (kierunek != 2) {
                kierunek = 3;
            }
        }
        if (Keyboard::isKeyPressed(Keyboard::S)) {
            if (kierunek != 1) {
                kierunek = 0;
            }

        }

        if (Keyboard::isKeyPressed(Keyboard::Escape)) { //esc
            window.close();
        }

            if (Keyboard::isKeyPressed(Keyboard::R)) //Restart
        {
            reset(window);
            main();
        }

        for (int i = 2; i < dlugosc; i++) {
            if (s[0].x == s[i].x && s[0].y == s[i].y) //koniec gry jesli dotknie sam siebie
            {
                ugryzienie_sie(window,text);

                for (;;) { //oczekiwanie na reakcje uzytkow
                    if (Keyboard::isKeyPressed(Keyboard::Escape)) //esc
                    {
                        window.close();
                    }
                    if (Keyboard::isKeyPressed(Keyboard::R)) //Reset
                    {
                        reset(window);
                        main();
                    }
                }
            }

        }

        if (timer > delay) {
            timer = 0;
            Tick();
        } //co kazde 0.05 sec odswieza ekran

        window.clear();

        zapelnianie_trawa(window, sprite1);

        rysowanie_robaka(window, sprite2);

        for (int i = 0; i < dlugosc; i++) {
            sprite4.setPosition(s[0].x * size, s[0].y * size);
            window.draw(sprite4);
        } //glowa

        for (int k = 0; k <= 40; ++k) {
            sprite5.setPosition(ob[k].x * size, ob[k].y * size);
            window.draw(sprite5); // obiekt
        }

        for (int k = 0; k <= 80; ++k) {
            sprite6.setPosition(gr[k].x * size, gr[k].y * size);
            window.draw(sprite6); // obiekt
        }

        sprite3.setPosition(f.x * size, f.y * size);
        window.draw(sprite3); //owoc

        sprite8.setPosition(a.x * size, a.y * size);
        window.draw(sprite8); //zlotyowoc

        prawy_interfejs(window,text);

        for (int k = 0; k <= 40; ++k) {
            if ((s[0].x == ob[k].x) && (s[0].y == ob[k].y)) //kolizja z kamieniem
            {

               uderzenie_w_kamien(window,text);

                {
                    for (;;) {
                        if (Keyboard::isKeyPressed(Keyboard::Escape)) {
                            window.close();
                        }
                        if (Keyboard::isKeyPressed(Keyboard::R)) {
                            reset(window);
                            main();
                        }
                    }
                }

            }
        }

        window.display(); //wyswietlanie obiektow
    }
    return 0;
}

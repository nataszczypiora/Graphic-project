// Dolaczamy plik naglowkowy naszej klasy MyWindow
#include "mywindow.h"
#include <tgmath.h>
// Dolaczamy plik naglowkowy zawierajacy definicje GUI
// Plik ten jest generowany automatycznie
// z pliku XML "mywindow.ui"
#include "ui_mywindow.h"

// Definicja konstruktora, wywolujemy najpierw
// konstruktor klasy nadrzednej, nastepnie tworzymy
// obiekt klasy Ui_MyWindow reprezentujacy GUI
MyWindow::MyWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    // Wywolujemy funkcje tworzaca elementy GUI
    // Jej definicja znajduje sie w pliku "ui_mywindow.h"
    ui->setupUi(this);

    // Pobieramy wymiary i wspolrzedne lewego gornego naroznika ramki
    // i ustawiamy wartosci odpowiednich pol
    // Uwaga: ramke "rysujFrame" wykorzystujemy tylko do
    // wygodnego ustaiwenia tych wymiarow. Rysunek bedziemy wyswietlac
    // bezposrednio w glownym oknie aplikacji.
    szer = ui->rysujFrame->width();
    wys = ui->rysujFrame->height();
    poczX = ui->rysujFrame->x();
    poczY = ui->rysujFrame->y();
    // Tworzymy obiekt klasy QImage, o odpowiedniej szerokosci
    // i wysokosci. Ustawiamy format bitmapy na 32 bitowe RGB
    // (0xffRRGGBB).
    img = new QImage(szer,wys,QImage::Format_RGB32);

}

// Definicja destruktora
MyWindow::~MyWindow()
{
    delete ui;
}

// Funkcja (slot) wywolywana po nacisnieciu przycisku "Wyjscie" (exitButton)
// Uwaga: polaczenie tej funkcji z sygnalem "clicked"
// emitowanym przez przycisk jest realizowane
// za pomoca funkcji QMetaObject::connectSlotsByName(MyWindow)
// znajdujacej sie w automatycznie generowanym pliku "ui_mywindow.h"
// Nie musimy wiec sami wywolywac funkcji "connect"
void MyWindow::on_exitButton_clicked()
{
    // qApp to globalny wskaznik do obiektu reprezentujacego aplikacje
    // quit() to funkcja (slot) powodujaca zakonczenie aplikacji z kodem 0 (brak bledu)
    qApp->quit();
}


// Funkcja "odmalowujaca" komponent
void MyWindow::paintEvent(QPaintEvent*)
{
    // Obiekt klasy QPainter pozwala nam rysowac na komponentach
    QPainter p(this);

    // Rysuje obrazek "img" w punkcie (poczX,poczY)
    // (tu bedzie lewy gorny naroznik)
    p.drawImage(poczX,poczY,*img);
}


// Funkcja (slot) wywolywana po nacisnieciu przycisku "Czysc" (cleanButton)
void MyWindow::on_cleanButton_clicked()
{
    // Funkcja czysci (zamalowuje na bialo) obszar rysowania
    // definicja znajduje sie ponizej
    czysc();

    // Funkcja "update()" powoduje ponowne "namalowanie" calego komponentu
    // Wywoluje funkcje "paintEvent"
    update();
}


// Funkcja powoduje wyczyszczenie (zamalowanie na bialo)
// obszaru rysowania
void MyWindow::czysc()
{
    // Wskaznik za pomoca, ktorego bedziemy modyfikowac obraz
    unsigned char *ptr;

    // Funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
    ptr = img->bits();

    int i,j;

    // Przechodzimy po wszystkich wierszach obrazu
    for(i=0; i<wys; i++)
    {
        // Przechodzimy po pikselach danego wiersza
        // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
        for(j=0; j<szer; j++)
        {
            ptr[szer*4*i + 4*j]= 0; // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
        }
    }
}

// Funkcja (slot) wywolywana po nacisnieciu przycisku myszy (w glownym oknie)
void MyWindow::mousePressEvent(QMouseEvent *event)
{
    // Pobieramy wspolrzedne punktu klikniecia
    int x = event->x();
    int y = event->y();

    // Sa to wspolrzedne wzgledem glownego okna,
    // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku


    startX = x;
    startY = y;
    update();
    }
    void MyWindow::mouseReleaseEvent(QMouseEvent *event)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        int x = event->x();
        int y = event->y();

        // Sa to wspolrzedne wzgledem glownego okna,
        // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku

        konX = x;
        konY = y;
        obliczenia();
    // Odswiezamy komponent
    update();
}
    void MyWindow::obliczenia()
    {
        double a;
        double b;
        int pom;
        if(startX > konX)
        {
            pom = startX;
            startX = konX;
            konX = pom;
            pom = startY;
            startY = konY;
            konY = pom;
        }
        a = 1.0 * (konY - startY)/(konX - startX);
        b = startY - a * startX;
        int i;
        double y, x;
        // Przechodzimy po wszystkich wierszach obrazu
        if(startX == konX && startY == konY)
        {
            rysujpiksel(startX, startY);
        }else if(startX == konX && startY != konY)
        {
            for(i = startY; i< konY; i++)
            {
                rysujpiksel(startX, i);
            }
        }else if(startX != konX && startY == konY)
        {
            for(i = startX; i< konX; i++)
            {
                rysujpiksel(i, startY);
            }
        }
        else if (startX != konX && startY != konY && (a <= 1 && a >= -1))
        {
            for(i = startX; i< konX; i++)
            {
                y = a * i + b;
                rysujpiksel(i, round(y));
            }

        }else
        {
            if(startY > konY)
            {
                pom = startX;
                startX = konX;
                konX = pom;
                pom = startY;
                startY = konY;
                konY = pom;
            }
            for(i = startY; i < konY; i++)
            {
                x =(i - b)/a ;
                rysujpiksel(round(x), i);
            }
        }
     }


    void MyWindow::rysujpiksel(int x, int y)
    {
            int kolor = 255;
            unsigned char *ptr;
            ptr = img->bits();
            ptr[szer*4*y + 4*x] = kolor;
            ptr[szer*4*y + 4*x + 1] = kolor;
            ptr[szer*4*y + 4*x + 2] = kolor;
    }


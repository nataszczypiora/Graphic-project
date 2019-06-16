// Dolaczamy plik naglowkowy naszej klasy MyWindow
#include "mywindow.h"

// Dolaczamy plik naglowkowy zawierajacy definicje GUI
// Plik ten jest generowany automatycznie
// z pliku XML "mywindow.ui"
#include "ui_mywindow.h"
#include <math.h>
#include <stdio.h>
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
    img = new QImage(":/p1.png");
    baza =  QImage(":/p1.png");
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


// Funkcja (slot) wywolywana po nacisnieciu przycisku "Rysuj 1" (draw1Button)
void MyWindow::on_draw1Button_clicked()
{
    rysuj1();
    update();
}

// Funkcja (slot) wywolywana po nacisnieciu przycisku "Rysuj 2" (draw2Button)
void MyWindow::on_draw2Button_clicked()
{
    img = new QImage(":/p1.png");
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
            ptr[szer*4*i + 4*j]=255; // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = 255; // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = 255; // Skladowa RED
        }
    }
}


// Funkcja powoduje zamalowanie obszaru rysowania pewnym wzorem
void MyWindow::rysuj1()
{

}

// Funkcja powoduje zamalowanie obszaru rysowania pewnym wzorem
void MyWindow::rysuj2()
{

}

// Funkcja (slot) wywolywana po nacisnieciu przycisku myszy (w glownym oknie)
void MyWindow::mousePressEvent(QMouseEvent *event)
{

    update();

}



void MyWindow::on_horizontalSlider_valueChanged(int value) //wysokosc
{
    wysokosc = value;
    obliczanie_pozycji();
}

void MyWindow::on_horizontalSlider_2_valueChanged(int value)
{
    szerokosc = value;
    obliczanie_pozycji();
}

void MyWindow::obliczanie_pozycji()
{
    czysc();
    // Wskaznik za pomoca, ktorego bedziemy modyfikowac obraz
    double kat = alfa * 1.0 / 100;
    unsigned char *ptr, * pom;
    double pomek1 = (skalax * 1.0) / 100;
    double pomek2 =  (skalay * 1.0) / 100;
    // Funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
    ptr = img->bits();
    pom = baza.bits();
    int i,j;
    long i1, j1;
    int zap;
    // Przechodzimy po wszystkich wierszach obrazu
    for(i=0; i<wys; i++)
    {
        QRgb pixColor;
        // Przechodzimy po pikselach danego wiersza
        // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)

        for(j=0; j<szer; j++)
        {
                pixColor = baza.pixel(i, j);
                i1 = (i + wysokosc) * pomek1;
                j1 = (j + szerokosc) * pomek2;
                zap = j1;
                j1 = j1 * cos(kat) - i1 * sin(kat);
                i1 = zap * sin(kat) + i1 * cos(kat);
                if(x == true && y != true)
                {
                    j1 = j1 + i1 * sx;
                }else if(x == false && y == true)
                {
                    i1 = i1 + j1 * sx;
                }
                if(i1 >= 0 && i1 < 600 && j1 >= 0 && j1 < 600)
                {
                    ptr[600*4*j1+ 4*i1]=qBlue(pixColor); // Skladowa BLUE
                    ptr[600*4*j1 + 4*i1 + 1] = qGreen(pixColor); // Skladowa GREEN
                    ptr[600*4*j1 + 4*i1 + 2] = qRed(pixColor); // Skladowa RED
                }
        }
        for(j = 0; j < 600; j++)
        {
            for(i = 0; i < 600; i++)
            {
                i1 = (i - wysokosc - 300) * 1.0 / pomek1;
                j1 = (j - szerokosc - 300) * 1.0 / pomek2;
                zap = j1;
                j1 = j1 * cos(-kat) - i1 * sin(-kat);
                i1 = zap * sin(-kat) + i1 * cos(-kat);
                if(x == true && y != true)
                {
                    j1 = j1 + i1 * sx;
                }else if(x == false && y == true)
                {
                    i1 = i1 + j1 * sx;
                }
                i1+=300;
                j1+=300;
                if(i1 >= 0 && i1 < 600 && j1 >= 0 && j1 < 600)
                {
                    pixColor = baza.pixel(i1, j1);
                    ptr[600*4*j+ 4*i]=qBlue(pixColor); // Skladowa BLUE
                    ptr[600*4*j + 4*i + 1] = qGreen(pixColor); // Skladowa GREEN
                    ptr[600*4*j + 4*i + 2] = qRed(pixColor); // Skladowa RED
                }
            }
        }
    }
    update();
}

void MyWindow::on_verticalSlider_valueChanged(int value) //skala wys
{
    skalax = value;
    obliczanie_pozycji();
}


void MyWindow::on_verticalSlider_2_valueChanged(int value)
{

    skalay = value;
    obliczanie_pozycji();
}

void MyWindow::on_horizontalSlider_3_valueChanged(int value)
{
    alfa = value;
    obliczanie_pozycji();
}

void MyWindow::on_verticalSlider_3_valueChanged(int value)
{
    sx = value;
    obliczanie_pozycji();
}

void MyWindow::on_checkBox_clicked(bool checked)
{
    x = checked;
}

void MyWindow::on_checkBox_2_toggled(bool checked)
{
    y = checked;
}

bool MyWindow::czy_biale(QRgb pix)
{
    if(qBlue(pix) == 255 && qGreen(pix) == 255 && qRed(pix) == 255)
    {
        return true;
    }
    else
        return false;
}

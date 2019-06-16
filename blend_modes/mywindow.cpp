// Dolaczamy plik naglowkowy naszej klasy MyWindow
#include "mywindow.h"

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

    img= new QImage(500,500,QImage::Format_RGB32);
    jamniki[0]=new QImage(":/1.jpeg");
    jamniki[1]=new QImage(":/2.jpg");
    jamniki[2]=new QImage(":/3.jpg");

    // Pobieramy wymiary i wspolrzedne lewego gornego naroznika ramki
    // i ustawiamy wartosci odpowiednich pol
    // Uwaga: ramke "rysujFrame" wykorzystujemy tylko do
    // wygodnego ustaiwenia tych wymiarow. Rysunek bedziemy wyswietlac
    // bezposrednio w glownym oknie aplikacji.
    szer = ui->rysujFrame->width();
    wys = ui->rysujFrame->height();
    poczX = ui->rysujFrame->x();
    poczY = ui->rysujFrame->y();
    wypelnienie();
    // Tworzymy obiekt klasy QImage, o odpowiedniej szerokosci
    // i wysokosci. Ustawiamy format bitmapy na 32 bitowe RGB
    // (0xffRRGGBB).

}

void MyWindow::wypelnienie()
{
    unsigned char *ptr;
    // Funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
    unsigned char *bits;
    int i,j;
    int r, g, b;
    int poczr, poczg, poczb;
    czysc();
    for (int k = 0; k < 3; k++) {

        if(wybor[k] == true)
        {
            double l1, l2;
            l1 = 1.0 * widocznosc[k] / 100;
            l2 = 1.0 - l1;
            for(i=0; i<500; i++)
            {
                bits = jamniki[k]->scanLine(i);
                ptr = img->scanLine(i);
                // Przechodzimy po pikselach danego wiersza
                // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
                for(j=0; j<500; j++)
                {
                    r = bits[4*j];
                    g = bits[4*j+1];
                    b = bits[4*j+2];
                    /*ptr[4*j] = r;
                    ptr[4*j+1] = g;
                    ptr[4 * j + 2] = b; */
                    poczr = ptr[4*j]; // ten o ktory mi chodzi
                    poczg = ptr[4*j+1];
                    poczb = ptr[4 * j + 2];
                    switch(tryb[k])
                    {
                        case 1:
                            r = multiply(poczr, r);
                            g = multiply(poczg, g);
                            b = multiply(poczb, b);
                            break;
                        case 2:
                            r = screen(poczr, r);
                            g = screen(poczg, g);
                            b = screen(poczb, b);
                            break;
                        case 3:
                            r = darken(poczr, r);
                            g = darken(poczg, g);
                            b = darken(poczb, b);
                            break;
                        case 4:
                            r = lighten(poczr, r);
                            g = lighten(poczg, g);
                            b = lighten(poczb, b);
                            break;
                    }
                    ptr[4*j] = r * l1 + l2 * ptr[4*j];
                    ptr[4*j+1] = g * l1 + l2 * ptr[4*j +1];
                    ptr[4 * j + 2] = b * l1 + l2 * ptr[4*j + 2];
                }
            }
        }

    }
    update();
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
    rysuj2();
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
        unsigned char *ptr;
        ptr = img->bits();
        int szer = img->width();
        int wys = img->height();

        int i,j;
        for(i=0;i<wys;i++)
        {
                for(j=0;j<szer ;j++)
                {
                        ptr[szer*4*i + 4*j]=i;
                        ptr[szer*4*i + 4*j + 1] = j;
                        ptr[szer*4*i + 4*j + 2] = i*j;
                }
        }
}

// Funkcja powoduje zamalowanie obszaru rysowania pewnym wzorem
void MyWindow::rysuj2()
{
        unsigned char *ptr;
        ptr = img->bits();
        int szer = img->width();
        int wys = img->height();

        int i,j;
        for(i=0;i<wys;i++)
        {
                for(j=0;j<szer ;j++)
                {
                        ptr[szer*4*i + 4*j]=i;
                        ptr[szer*4*i + 4*j + 1] = j;
                        ptr[szer*4*i + 4*j + 2] = i+j;
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

    x -= poczX;
    y -= poczY;

    int kolor = 0;
    unsigned char *ptr;
    ptr = img->bits();

    // Sprawdzamy ktory przycisk myszy zostal klkniety
    if(event->button() == Qt::LeftButton)
    {
        // jezeli lewy to ustawiamy kolor na czarny
        kolor = 0;

    }
    else
    {
        // jezeli prawy to ustawiamy kolor na bialy
        kolor = 255;
    }

    // Sprawdzamy czy klikniecie nastapilo w granicach rysunku
    if ((x>=0)&&(y>=0)&&(x<szer)&&(y<wys))
    {

        // Ustawiamy kolor kliknietego piksela na bialy lub czarny
        ptr[szer*4*y + 4*x] = kolor;
        ptr[szer*4*y + 4*x + 1] = kolor;
        ptr[szer*4*y + 4*x + 2] = kolor;
    }

    // Odswiezamy komponent
    update();

}



void MyWindow::on_horizontalSlider_valueChanged(int value) // jamniki 3
{
    widocznosc[2] = value;
    wypelnienie();
}

void MyWindow::on_horizontalSlider_2_valueChanged(int value) // jamniki 2
{
    widocznosc[1] = value;
    wypelnienie();
}

void MyWindow::on_horizontalSlider_3_valueChanged(int value) // jamniki 1
{
    widocznosc[0] = value;
    wypelnienie();
}

void MyWindow::on_checkBox_clicked(bool checked) // jamniki 3
{
    wybor[2] = checked;
    wypelnienie();
}

void MyWindow::on_checkBox_2_clicked(bool checked)
{
    wybor[1] = checked;
    wypelnienie();
}

void MyWindow::on_checkBox_3_clicked(bool checked)
{
    wybor[0] = checked;
    wypelnienie();
}

void MyWindow::on_comboBox_currentIndexChanged(int index) // jamnik 3
{
    tryb[2] = index;
    wypelnienie();
}

void MyWindow::on_comboBox_2_currentIndexChanged(int index) //jamnik 2
{
    tryb[1] = index;
    wypelnienie();
}

void MyWindow::on_comboBox_3_currentIndexChanged(int index) //jamnik 1
{
    tryb[0] = index;
    wypelnienie();
}

int MyWindow::multiply(int obecny, int stary)
{
    return (obecny * stary) >> 8;
}

int MyWindow::screen(int obecny, int stary)
{
    return 255 - (((255- stary) * (255-obecny)) >> 8);
}


int MyWindow::darken(int obecny, int stary)
{
    if(obecny > stary)
    {
        return stary;
    }
    return obecny;
}

int MyWindow::lighten(int obecny, int stary)
{
    if(obecny < stary)
    {
        return stary;
    }
    return obecny;
}

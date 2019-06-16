// Dolaczamy plik naglowkowy naszej klasy MyWindow
#include "mywindow.h"
#include <math.h>
// Dolaczamy plik naglowkowy zawierajacy definicje GUI
// Plik ten jest generowany automatycznie
// z pliku XML "mywindow.ui"
#include "ui_mywindow.h"
#include<iostream>

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


// Funkcja "odmalowujaca" komponent
void MyWindow::paintEvent(QPaintEvent*)
{
    // Obiekt klasy QPainter pozwala nam rysowac na komponentach
    QPainter p(this);

    // Rysuje obrazek "img" w punkcie (poczX,poczY)
    // (tu bedzie lewy gorny naroznik)
    p.drawImage(poczX,poczY,*img);
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



void MyWindow::on_radioButton_clicked()
{
    spr = 1;
    unsigned char *ptr;
    ptr = img->bits();
    int i,j;

    for(i=0; i<wys; i++)
    {
          // Przechodzimy po pikselach danego wiersza
          // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
          for(j=0; j<szer; j++)
          {
              ptr[szer*4*i + 4*j]=i/2; // Skladowa BLUE
              ptr[szer*4*i + 4*j + 1] = j/2; // Skladowa GREEN
              ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
          }
    }
    update();
}

void MyWindow::on_radioButton_2_clicked()
{
    spr = 2;
    unsigned char *ptr;
    ptr = img->bits();
    int i,j;

    for(i=0; i<wys; i++)
    {
          // Przechodzimy po pikselach danego wiersza
          // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
          for(j=0; j<szer; j++)
          {
              ptr[szer*4*i + 4*j]=0; // Skladowa BLUE
              ptr[szer*4*i + 4*j + 1] = j/2; // Skladowa GREEN
              ptr[szer*4*i + 4*j + 2] = i/2; // Skladowa RED
          }
    }
    update();
}

void MyWindow::on_radioButton_3_clicked()
{
    spr = 3;
    unsigned char *ptr;
    ptr = img->bits();
    int i,j;

    for(i=0; i<wys; i++)
    {
          // Przechodzimy po pikselach danego wiersza
          // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
          for(j=0; j<szer; j++)
          {
              ptr[szer*4*i + 4*j]=j/2; // Skladowa BLUE
              ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
              ptr[szer*4*i + 4*j + 2] = i/2; // Skladowa RED
          }
    }
    update();
}

void MyWindow::on_verticalSlider_valueChanged(int value)
{
    unsigned char *ptr;
    ptr = img->bits();
    int i,j;

    switch(spr)
    {
    case 1:
        for(i=0; i<wys; i++)
        {
              // Przechodzimy po pikselach danego wiersza
              // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
              for(j=0; j<szer; j++)
              {
                  ptr[szer*4*i + 4*j]=i/2; // Skladowa BLUE
                  ptr[szer*4*i + 4*j + 1] = j/2; // Skladowa GREEN
                  ptr[szer*4*i + 4*j + 2] = value; // Skladowa RED
              }
        }
        update();
        break;
    case 2:
        for(i=0; i<wys; i++)
        {
              // Przechodzimy po pikselach danego wiersza
              // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
              for(j=0; j<szer; j++)
              {
                  ptr[szer*4*i + 4*j]=value; // Skladowa BLUE
                  ptr[szer*4*i + 4*j + 1] = j/2; // Skladowa GREEN
                  ptr[szer*4*i + 4*j + 2] = i/2; // Skladowa RED
              }
        }
        update();
        break;
    case 3:
        for(i=0; i<wys; i++)
        {
              // Przechodzimy po pikselach danego wiersza
              // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
              for(j=0; j<szer; j++)
              {
                  ptr[szer*4*i + 4*j]=j/2; // Skladowa BLUE
                  ptr[szer*4*i + 4*j + 1] = value; // Skladowa GREEN
                  ptr[szer*4*i + 4*j + 2] = i/2; // Skladowa RED
              }
        }
        update();
        break;
    }
}

void MyWindow::kolorowanie(int r, int g, int b)
{
    int red = abs(r);
    int green = abs(g);
    int blue = abs(b);
    unsigned char *ptr;
    ptr = img->bits();
    int i,j;
    std::cout<<red<<" "<<green<<" "<<blue<<std::endl;
    for(i=0; i<wys; i++)
    {
          // Przechodzimy po pikselach danego wiersza
          // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
        for(j=0; j<szer; j++)
        {
            ptr[szer*4*i + 4*j] = blue; // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = green; // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = red; // Skladowa RED
        }
    }
    update();
}




void MyWindow::on_verticalSlider_2_valueChanged(int value) //hue
{
    hue = value;
    hue2 = hue / 60.0;
    int i, j;
    unsigned char *ptr;
    ptr = img->bits();
    double czer, ziel, nieb;

    for(i=0; i<wys; i++) //sat
    {
        sat = 1.0 * i/wys;
        // Przechodzimy po pikselach danego wiersza
        // W kazdym wierszu jest "szer" pikseli (tzn. 4 * "szer" bajtow)
        for(j=0; j<szer; j++)
        {
            val = 1.0 * j/szer;
            c = val * sat;
            x = c * (1 - abs(fmod(hue2, 2.0) - 1));

            if(hue2 >= 0 && hue2 <= 1)
            {
                czer = c;
                ziel = x;
                nieb = 0;
            }else if(hue2 > 1 && hue2 <= 2)
            {
                czer = x;
                ziel = c;
                nieb = 0;
            }else if(hue2 > 2 && hue2 <= 3)
            {
                czer = 0;
                ziel = c;
                nieb = x;
            }else if(hue2 > 3 && hue2 <= 4)
            {
                czer = 0;
                ziel = x;
                nieb = c;
            }else if(hue2 > 4 && hue2 <= 5)
            {
                czer = x;
                ziel = 0;
                nieb = c;
            }else if(hue2 > 5 && hue2 <= 6)
            {
                czer = c;
                ziel = 0;
                nieb = x;
            }

            m = val - c;

            ptr[szer*4*i + 4*j] = round((nieb + m)*255); // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = round((ziel + m)*255); // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = round((czer + m)*255); // Skladowa RED
        }
    }

    update();
}

void MyWindow::on_verticalSlider_3_valueChanged(int value) //sat
{
    sat = (double)(value) * 0.01;
    int i, j;
    unsigned char *ptr;
    ptr = img->bits();
    double czer, ziel, nieb;
    for(i=0; i<wys; i++) //sat
    {
        val = 1.0 * i/wys;
        for(j=0; j<szer; j++)
        {
            hue2 = 1.0 * j/(60*1.5);
            c = val * sat;
            x = c * (1 - abs(fmod(hue2, 2.0) - 1));

            if(hue2 >= 0 && hue2 <= 1)
            {
                czer = c;
                ziel = x;
                nieb = 0;
            }else if(hue2 > 1 && hue2 <= 2)
            {
                czer = x;
                ziel = c;
                nieb = 0;
            }else if(hue2 > 2 && hue2 <= 3)
            {
                czer = 0;
                ziel = c;
                nieb = x;
            }else if(hue2 > 3 && hue2 <= 4)
            {
                czer = 0;
                ziel = x;
                nieb = c;
            }else if(hue2 > 4 && hue2 <= 5)
            {
                czer = x;
                ziel = 0;
                nieb = c;
            }else if(hue2 > 5 && hue2 <= 6)
            {
                czer = c;
                ziel = 0;
                nieb = x;
            }

            m = val - c;

            ptr[szer*4*i + 4*j] = round((nieb + m)*255); // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = round((ziel + m)*255); // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = round((czer + m)*255); // Skladowa RED
        }
    }
    update();
}

void MyWindow::on_verticalSlider_4_valueChanged(int value) //val
{
    val = (double)(value) * 0.01;
    int i, j;
    unsigned char *ptr;
    ptr = img->bits();
    double czer, ziel, nieb;
    for(i=0; i<wys; i++) //sat
    {
        sat = 1.0 * i/wys;
        for(j=0; j<szer; j++)
        {
            hue2 = 1.0 * j/(60*1.5);
            c = val * sat;
            x = c * (1 - abs(fmod(hue2, 2.0) - 1));

            if(hue2 >= 0 && hue2 <= 1)
            {
                czer = c;
                ziel = x;
                nieb = 0;
            }else if(hue2 > 1 && hue2 <= 2)
            {
                czer = x;
                ziel = c;
                nieb = 0;
            }else if(hue2 > 2 && hue2 <= 3)
            {
                czer = 0;
                ziel = c;
                nieb = x;
            }else if(hue2 > 3 && hue2 <= 4)
            {
                czer = 0;
                ziel = x;
                nieb = c;
            }else if(hue2 > 4 && hue2 <= 5)
            {
                czer = x;
                ziel = 0;
                nieb = c;
            }else if(hue2 > 5 && hue2 <= 6)
            {
                czer = c;
                ziel = 0;
                nieb = x;
            }

            m = val - c;

            ptr[szer*4*i + 4*j] = round((nieb + m)*255); // Skladowa BLUE
            ptr[szer*4*i + 4*j + 1] = round((ziel + m)*255); // Skladowa GREEN
            ptr[szer*4*i + 4*j + 2] = round((czer + m)*255); // Skladowa RED
        }
    }
    update();
}

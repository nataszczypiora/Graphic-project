// Dolaczamy plik naglowkowy naszej klasy MyWindow
#include "mywindow.h"
#include <tgmath.h>
#include <math.h>
#include <QBoxLayout>
#include <vector>



// Dolaczamy plik naglowkowy zawierajacy definicje GUI
// Plik ten jest generowany automatycznie
// z pliku XML "mywindow.ui"
#include "ui_mywindow.h"

// Definicja konstruktora, wywolujemy najpierw
// konstruktor klasy nadrzednej, nastepnie tworzymy
// obiekt klasy Ui_MyWindow reprezentujacy GUI
MyWindow::MyWindow(QWidget *parent) :QMainWindow(parent),
    ui(new Ui::MyWindow)
{
    // Wywolujemy funkcje tworzaca elementy GUI
    // Jej definicja znajduje sie w pliku "ui_mywindow.h"
    startX = 0;
    startY = 0;
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
    setWindowTitle("Rysowanie pikseli w Qt");
    draw1Button = new QPushButton("Rysuj 1");
    draw2Button = new QPushButton("Rysuj 2");
    cleanButton = new QPushButton("Czysc");
    exitButton = new QPushButton("Wyjscie");
    // Laczymy sygnal emitowany po nacisnieciu przycisku "Wyjscie"
    // ze slotem quit(). qApp to globalny wskaznik reprezentujacy aplikacje
    connect(exitButton,SIGNAL(clicked()),qApp,SLOT(quit()));

    // Laczymy sygnaly emitowane po nacisnieciu przyciskow z odpowiednimi slotami

    // Tworzymy obiekt klasy QImage, o odpowiedniej szerokosci
    // i wysokosci. Ustawiamy format bitmapy na 32 bitowe RGBaziera();

    // (0xffRRGGBB).
    img = new QImage(szer,wys,QImage::Format_RGB32);
    lab = new QLabel(this);
    lab->setText("3");
    lab->setMinimumWidth(20);
    lab->setMaximumWidth(20);
    lab->setMinimumHeight(20);
    lab->setMaximumHeight(20);
    lab->move(670,223);
    lab->show();
    n = 3;
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
    punkty.clear();
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
void MyWindow::mouseMoveEvent(QMouseEvent *event)
{

    mvImg = new QImage(szer,wys,QImage::Format_RGB32);
    *mvImg = *img;
    if(startX != 0 && startY != 0)
    {
        int x = event->x();
        int y = event->y();

        konX = x - poczX;
        konY = y - poczY;

        switch(spr)
        {
            case 1:
            rysujokrag();
            break;
            case 2:
            rysujelipse();
            rysujwielokat();
            break;
       }
       repaint();
       *img = *mvImg;
    }
}

void MyWindow::mousePressEvent(QMouseEvent *event)
{
    // Pobieramy wspolrzedne punktu klikniecia
    int x = event->x();
    int y = event->y();

    // Sa to wspolrzedne wzgledem glownego okna,
    // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku


    startX = x - poczX;
    startY = y - poczY;
    if(czytrafiony(startX, startY) == -1)
    {
        rysuj_punkt(startX, startY);
    }
    else
    {
        trafiony_punkt = czytrafiony(startX, startY);
        rysuj_punkt(punkty[trafiony_punkt].x, punkty[trafiony_punkt].y, 0, 0, 255);
    }
    update();
}

void MyWindow::mouseReleaseEvent(QMouseEvent *event)
    {
        // Pobieramy wspolrzedne punktu klikniecia
        int x = event->x();
        int y = event->y();

        // Sa to wspolrzedne wzgledem glownego okna,
        // Musimy odjac od nich wpolrzedne lewego gornego naroznika rysunku
        punkt pkt;

        konX = x - poczX;
        konY = y - poczY;

        pkt.x = konX;
        pkt.y = konY;


        if(trafiony_punkt == -1)
        {
            punkty.push_back(pkt);
            licznik++;
            if(punkty.size() == 4)
            {
                krzywa_beziera();
                licznik = 0;
            }
            else
            {
               if(licznik == 3 && !(punkty.size() == 3))
               {
                   krzywa_beziera();
                   licznik = 0;
               }
            }
        }
        else
        {
            punkty[trafiony_punkt].x = pkt.x;
            punkty[trafiony_punkt].y = pkt.y;
            int ilosc = 4;
            unsigned char *ptr;
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
            for(i = 0; i < punkty.size(); i++)
            {
                rysuj_punkt(punkty[i].x, punkty[i].y);
            }
                krzywa_beziera(ilosc);
                ilosc = ilosc + 3;
            while(ilosc <= punkty.size())
            {
                   krzywa_beziera(ilosc);
                   ilosc = ilosc + 3;
            }
            trafiony_punkt = -1;
        }



        /*switch(spr)
        {
            case 1:
            rysujokrag();
            break;
            case 2:
            rysujelipse();
            rysujwielokat();
            break;
       }*/
    // Odswiezamy kompopunkty.size(nent
    repaint();
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
            rysujpiksel(startX, startY,img);
        }else if(startX == konX && startY != konY)
        {
            for(i = startY; i< konY; i++)
            {
                rysujpiksel(startX, i,img);
            }
        }else if(startX != konX && startY == konY)
        {
            for(i = startX; i< konX; i++)
            {
                rysujpiksel(i, startY,img);
            }
        }
        else if (startX != konX && startY != konY && (a <= 1 && a >= -1))
        {
            for(i = startX; i< konX; i++)
            {
                y = a * i + b;
                rysujpiksel(i, round(y),img);
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
                rysujpiksel(round(x), i,img);
            }
        }
     }


    void MyWindow::rysujpiksel(int x, int y, QImage *obrazek, int r, int g, int b)
    {
            if(!(x < 0 || y < 0 || x >= szer || y >= wys))
            {
                int kolor = 255;
                unsigned char *ptr;
                ptr = obrazek->bits();
                ptr[szer*4*y + 4*x] = b;
                ptr[szer*4*y + 4*x + 1] = g;
                ptr[szer*4*y + 4*x + 2] = r;
            }
    }

    void MyWindow::rysujokrag()
    {
        double r;
        int i;
        double j;
            r = sqrt((konX - startX)*(konX - startX)+(konY - startY)*(konY - startY));
        for(i = 0; i <= r/(sqrt(2)); i++)
        {
            j = 1.0 * sqrt(r * r - i * i);
            j = round(j);
            rysujpiksel(i+startX, j+startY,img);
            rysujpiksel(j+startX, i+startY,img);
            rysujpiksel(j+startX, -i+startY,img);
            rysujpiksel(i+startX, -j+startY,img);
            rysujpiksel(-i+startX, -j+startY,img);
            rysujpiksel(-j+startX, -i+startY,img);
            rysujpiksel(-j+startX, i+startY,img);
            rysujpiksel(-i+startX,j+startY,img);
        }
    }

    void MyWindow::rysujelipse()
    {
        double t;
        int a;
        int b;
        a = abs(startX - konX);
        b = abs(startY - konY);
        double x;
        double y;
        for(t = 0; t <= M_PI/2; t+=0.001)
        {
            x = a * cos(t);
            y = b * sin(t);
            x = round(x);
            y = round(y);
            rysujpiksel(x+startX, y+startY,img);
            rysujpiksel(x+startX, -y+startY,img);
            rysujpiksel(-x+startX, -y+startY,img);
            rysujpiksel(-x+startX, y+startY,img);
        }
    }
    void MyWindow::rysujwielokat()
    {
        int px, py;
        bool spr = false;
        double t;
        int a;
        int b;
        double x1,x2,y1,y2;
        a = abs(startX - konX);
        b = abs(startY - konY);
        double x;
        double y;
        for(t = 0; t <= 2*M_PI; t+=((2*M_PI)/n))
        {
            if(spr == false)
            {
                x1 = a * cos(t);
                y1 = b * sin(t);
                x1 = round(x1);
                y1 = round(y1);
                px = x1;
                py = x2;
                spr = true;
            }else
            {
                x2 = a * cos(t);QImage *obrazek;
                y2 = b * sin(t);
                x2 = round(x2);
                y2 = round(y2);
                obliczenia2(x1+startX,y1+startY,x2+startX,y2+startY);
                x1 = x2;
                y1 = y2;
            }
        }
        obliczenia2(x2+startX,y2+startY,px+startX,py+startY);
    }
    void MyWindow::obliczenia2(int x1, int y1, int x2, int y2)
    {
        double a;
        double b;
        int pom;
        if(x1 > x2)
        {
            pom = x1;
            x1 = x2;
            x2 = pom;
            pom = y1;
            y1 = y2;
            y2 = pom;
        }
        a = 1.0 * (y2 - y1)/(x2 - x1);
        b = y1 - a * x1;
        int i;
        double y, x;
        // Przechodzimy po wszystkich wierszach obrazu
        if(x1 == x2 && y1 == y2)
        {
            rysujpiksel(x1, y1,img);
        }else if(x1 == x2 &&  y1 != y2)
        {
            if(y1 > y2)
            {
                pom = x1;
                x1 = x2;
                x2 = pom;
                pom = y1;
                y1 = y2;
                y2 = pom;
            }
            for(i = y1; i< y2; i++)
            {
                rysujpiksel(x1, i,img);
            }
        }else if(x1 != x2 && y1 == y2)
        {
            for(i = x1; i< x2; i++)
            {
                rysujpiksel(i, y1,img);
            }
        }
        else if (x1 != x2 && y1 != y2 && (a <= 1 && a >= -1))
        {
            for(i = x1; i< x2; i++)
            {
                y = a * i + b;
                rysujpiksel(i, round(y),img);
            }

        }else
        {
            if(y1 > y2)
            {
                pom = x1;
                x1 = x2;
                x2 = pom;
                pom = y1;
                y1 = y2;
                y2 = pom;
            }
            for(i = y1; i < y2; i++)
            {
                x =(i - b)/a ;
                rysujpiksel(round(x), i,img);
            }
        }
     }



void MyWindow::on_sliderUi_valueChanged(int value)
{
    std::string s = std::to_string(value);
    QString qstr = QString::fromStdString(s);
    lab->setText(qstr);
    n = value;
}


void MyWindow::on_draw1Button_clicked()
{
    spr = 1;
}

void MyWindow::on_draw2Button_clicked()
{
    spr = 2;
}
void MyWindow::rysuj_punkt(int x, int y, int r, int g, int b)
{
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 10; j++)
        {
            rysujpiksel(x + i,y + j, img, r, g, b);
        }
    }
}
int MyWindow::czytrafiony(int x, int y)
{
    for(int i = 0; i < punkty.size(); i++)
    {
        if(x > punkty[i].x - 10  && x < punkty[i].x + 10 && y > punkty[i].y - 10  && y < (punkty[i].y + 10))
        {
            return i;
        }
    }
    return -1;
}
void MyWindow::krzywa_beziera(int zmiana)
{
   double x, y;
   if(zmiana == 0)
   {
        for(double i = 0; i < 1; i+= 0.00001)
        {
            x = (1 - i) * (1-i) *(1-i) * punkty[punkty.size()-1].x + 3*(1 - i)*(1 - i) *  i * punkty[punkty.size()-2].x + 3*(1 - i) * i * i * punkty[punkty.size()-3].x + i * i * i * punkty[punkty.size()-4].x;
            y = (1 - i)* (1-i) * (1-i) * punkty[punkty.size()-1].y + 3*(1 - i)*(1 - i) * i * punkty[punkty.size()-2].y + 3*(1 - i) * i * i * punkty[punkty.size()-3].y + i * i * i * punkty[punkty.size()-4].y;

            rysujpiksel(x, y, img);
        }
   }
   else
   {
       for(double i = 0; i < 1; i+= 0.00001)
       {
           x = (1 - i) * (1-i) *(1-i) * punkty[zmiana-1].x + 3*(1 - i)*(1 - i) *  i * punkty[zmiana-2].x + 3*(1 - i) * i * i * punkty[zmiana-3].x + i * i * i * punkty[zmiana-4].x;
           y = (1 - i)* (1-i) * (1-i) * punkty[zmiana-1].y + 3*(1 - i)*(1 - i) * i * punkty[zmiana-2].y + 3*(1 - i) * i * i * punkty[zmiana-3].y + i * i * i * punkty[zmiana-4].y;

           rysujpiksel(x, y, img);
       }
   }

}

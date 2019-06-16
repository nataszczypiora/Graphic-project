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
    jamniki=new QImage(":/2.jpg");

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
    int Y;
    czysc();
    for(i=0; i<500; i++)
    {
        bits = jamniki->scanLine(i);
        ptr = img->scanLine(i);

        for(j=0; j<500; j++)
        {
            r = bits[4*j];
            g = bits[4*j+1];
            b = bits[4*j+2];
            Y = (r + g + b)/3;
            if(Y < 40)
            {
                ptr[4*j] = 0;
                ptr[4*j+1] = 0;
                ptr[4 * j + 2] = 0;
            }else
            {
                ptr[4*j] = 255;
                ptr[4*j+1] = 255;
                ptr[4 * j + 2] = 255;
            }
        }
    }
    img2 = *img;
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

// Funkcja (slot) wywolywana po nacisnieciu przycisku myszy (w glownym oknie)
void MyWindow::mousePressEvent(QMouseEvent *event)
{

}




void MyWindow::on_comboBox_currentIndexChanged(int index)
{
    swit = index;
}

void MyWindow::dylatacja()
{
    unsigned char *ptr;
    ptr = img->bits();
    int i,j;
    int spr = 0;
    QRgb pixColor;
    double r, g, b;
    for(i=0; i<wys; i++)
    {
        for(j=0; j<szer; j++)
        {
            if(j - 1 > 0)
            {
                pixColor = img2.pixel(j - 1, i);
                r = qRed(pixColor);
                g = qGreen(pixColor);
                b = qBlue(pixColor);
                if(((r + g + b) * 85)  < 40)
                {
                    ptr[szer*4*i + 4*j] = 0; // Skladowa BLUE
                    ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
                    ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
                }
            }
            if(j + 1 < 500 && spr == 0)
            {
                pixColor = img2.pixel(j + 1, i);
                r = qRed(pixColor);
                g = qGreen(pixColor);
                b = qBlue(pixColor);
                if(((r + g + b) * 85)  < 40)
                {
                    ptr[szer*4*i + 4*j] = 0; // Skladowa BLUE
                    ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
                    ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
                }
            }
            if(i + 1 < 500 && spr == 0)
            {
                pixColor = img2.pixel(j, i + 1);
                r = qRed(pixColor);
                g = qGreen(pixColor);
                b = qBlue(pixColor);
                if(((r + g + b) * 85)  < 40)
                {
                    ptr[szer*4*i + 4*j] = 0; // Skladowa BLUE
                    ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
                    ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
                }
            }
            if(i - 1 > 0 && spr == 0)
            {
                pixColor = img2.pixel(j, i - 1);
                r = qRed(pixColor);
                g = qGreen(pixColor);
                b = qBlue(pixColor);
                if(((r + g + b) * 85)  < 40)
                {
                    ptr[szer*4*i + 4*j] = 0; // Skladowa BLUE
                    ptr[szer*4*i + 4*j + 1] = 0; // Skladowa GREEN
                    ptr[szer*4*i + 4*j + 2] = 0; // Skladowa RED
                }
            }
        }
    }
    img2 = *img;
    update();
}

void MyWindow::on_pushButton_clicked()
{
   dylatacja();
}

void MyWindow::on_pushButton_2_clicked()
{
    unsigned char *ptr;
    // Funkcja "bits()" zwraca wskaznik do pierwszego piksela danych
    unsigned char *bits;
    int i,j;
    int r, g, b;
    int Y;
    czysc();
    for(i=0; i<500; i++)
    {
        bits = jamniki->scanLine(i);
        ptr = img->scanLine(i);

        for(j=0; j<500; j++)
        {
            r = bits[4*j];
            g = bits[4*j+1];
            b = bits[4*j+2];
            Y = (r + g + b)/3;
            if(Y < 40)
            {
                ptr[4*j] = 0;
                ptr[4*j+1] = 0;
                ptr[4 * j + 2] = 0;
            }else
            {
                ptr[4*j] = 255;
                ptr[4*j+1] = 255;
                ptr[4 * j + 2] = 255;
            }
        }
    }
    img2 = *img;
    update();

}

void MyWindow::on_pushButton_3_clicked()
{
    erozja();
}

void MyWindow::erozja()
{
    unsigned char *ptr;
    ptr = img->bits();
    int i,j;
    QRgb pixColor;
    double r, g, b;
    for(i=0; i<wys; i++)
    {
        for(j=0; j<szer; j++)
        {
            if(j - 1 > 0)
            {
                pixColor = img2.pixel(j - 1, i);
                r = qRed(pixColor);
                g = qGreen(pixColor);
                b = qBlue(pixColor);
                if(((r + g + b) * 85)  > 40)
                {
                    ptr[szer*4*i + 4*j] = 255; // Skladowa BLUE
                    ptr[szer*4*i + 4*j + 1] = 255; // Skladowa GREEN
                    ptr[szer*4*i + 4*j + 2] = 255; // Skladowa RED
                }
            }
            if(j + 1 < 500)
            {
                pixColor = img2.pixel(j + 1, i);
                r = qRed(pixColor);
                g = qGreen(pixColor);
                b = qBlue(pixColor);
                if(((r + g + b) * 85)  > 40)
                {
                    ptr[szer*4*i + 4*j] = 255; // Skladowa BLUE
                    ptr[szer*4*i + 4*j + 1] = 255; // Skladowa GREEN
                    ptr[szer*4*i + 4*j + 2] = 255; // Skladowa RED
                }
            }
            if(i + 1 < 500)
            {
                pixColor = img2.pixel(j, i + 1);
                r = qRed(pixColor);
                g = qGreen(pixColor);
                b = qBlue(pixColor);
                if(((r + g + b) * 85)  > 40)
                {
                    ptr[szer*4*i + 4*j] = 255; // Skladowa BLUE
                    ptr[szer*4*i + 4*j + 1] = 255; // Skladowa GREEN
                    ptr[szer*4*i + 4*j + 2] = 255; // Skladowa RED
                }
            }
            if(i - 1 > 0)
            {
                pixColor = img2.pixel(j, i - 1);
                r = qRed(pixColor);
                g = qGreen(pixColor);
                b = qBlue(pixColor);
                if(((r + g + b) * 85)  > 40)
                {
                    ptr[szer*4*i + 4*j] = 255; // Skladowa BLUE
                    ptr[szer*4*i + 4*j + 1] = 255; // Skladowa GREEN
                    ptr[szer*4*i + 4*j + 2] = 255; // Skladowa RED
                }
            }
        }
    }
    img2 = *img;
    update();
}

void MyWindow::on_pushButton_4_clicked()
{
    dylatacja();
    erozja();
}

void MyWindow::on_pushButton_5_clicked()
{
    erozja();
    dylatacja();
}

void MyWindow::on_pushButton_6_clicked()
{
    QString filename =  QFileDialog::getOpenFileName(this, tr("Open Image"), "../", tr("Image Files (*.png *.jpg *.bmp)"));
    jamniki->load(filename);
    wypelnienie();
    update();
}

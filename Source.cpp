#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <iostream>


using namespace cv;
using namespace std;
void herr();
string formas(vector<Point> Contorno);
string colores(Mat image);

int main()
{
    Mat im, dst, gray, Gauss, abs_dst, Cont, imgHSV;
    vector<vector<Point>> Contours;
    string forma, color, nom_Figura;
    int x, y;
    im = imread("D:/Alejandro/TFG Vulkan/OpenCV_formas/shapes.jpg");
    if (im.empty())
    {
        cout << "Cannot load image!" << endl;
        return -1;
    }

    //imshow("Original", im);
    cvtColor(im, gray, COLOR_BGR2GRAY);
    cvtColor(im, imgHSV, COLOR_BGR2HSV);
    //imshow("HSV", imgHSV);
    GaussianBlur(gray, Gauss, Size(5, 5), 0, 0);
    threshold(Gauss, dst, 35, 255, THRESH_BINARY);

    findContours(dst, Contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE); 

    for (int i = 0; i < Contours.size(); i++)
    {
        Rect fig = boundingRect(Contours[i]);
        Mat drawing = Mat::zeros(dst.size(), CV_8U);
        Mat maskHSV = Mat::zeros(dst.size(), CV_8UC3);
        drawContours(drawing, Contours, i, Scalar(255,255,255), -1);
        //imshow("contorno", drawing);
        //imshow("HSV", imgHSV);
        bitwise_and(imgHSV, imgHSV, maskHSV, drawing);
        //imshow("img", maskHSV);
        forma = formas(Contours[i]);
        color = colores(maskHSV);
        nom_Figura = forma + " " + color;
        putText(im, nom_Figura, Point(fig.x, fig.y-5), 1, 0.8, Scalar(0, 255, 0), 1);
        imshow("imagen", im);

        waitKey(0);
    }

    
}

string colores(Mat HSV)
{
    Mat maskRojo1, maskRojo2, maskRojo, maskNaranja, maskAmarillo, maskVerde, maskCian, maskAzul, maskMorado, maskRosa;
    vector<vector<Point>> Rojo, Naranja, Amarillo, Verde, Cian, Azul, Morado, Rosa;
    string color;

    inRange(HSV, (0, 100, 40), (10, 255, 255), maskRojo1);
    inRange(HSV, (175, 100, 40), (180, 255, 255), maskRojo2);
    add(maskRojo1, maskRojo2, maskRojo);

    inRange(HSV, Scalar(11, 100, 40), Scalar(19, 255, 255), maskNaranja);
    inRange(HSV, Scalar(20, 100, 40), Scalar(32, 255, 255), maskAmarillo);
    inRange(HSV, Scalar(36, 100, 40), Scalar(75, 255, 255), maskVerde);
    inRange(HSV, Scalar(80, 100, 40), Scalar(95, 255, 255), maskCian);
    inRange(HSV, Scalar(100, 100, 40), Scalar(130, 255, 255), maskAzul);
    inRange(HSV, Scalar(135, 100, 40), Scalar(145, 255, 255), maskMorado);
    inRange(HSV, Scalar(145, 100, 40), Scalar(165, 255, 255), maskRosa);

    findContours(maskRojo, Rojo, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(maskNaranja, Naranja, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(maskAmarillo, Amarillo, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(maskVerde, Verde, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(maskCian, Cian, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(maskAzul, Azul, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(maskMorado, Morado, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    findContours(maskRosa, Rosa, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    if (Rojo.size() > 0)
        color = "Rojo";
    else if (Naranja.size() > 0)
        color = "Naranja";
    else if (Amarillo.size() > 0)
        color = "Amarillo";
    else if (Verde.size() > 0)
        color = "Verde";
    else if (Cian.size() > 0)
        color = "Cian";
    else if (Azul.size() > 0)
        color = "Azul";
    else if (Morado.size() > 0)
        color = "Morado";
    else if (Rosa.size() > 0)
        color = "Rosa";

    return color;
}

string formas(vector<Point> Contorno)
{
    double Per;
    vector<Point> Pol;
    string result;
    Per = arcLength(Contorno, true);

    approxPolyDP(Contorno, Pol, .03 * Per, true);

    //Mat draw = Mat::zeros(tamaño, CV_8UC3);
    //aux.push_back(Pol);
    //drawContours(draw, aux, -1, (0, 255, 255), 2, 8);

    if (Pol.size() == 3)
    {
        result = "triangulo";
    }
    else if (Pol.size() == 4)
    {
        result = "cuadrilatero";
    }
    else if (Pol.size() == 5)
    {
        result = "pentagono";
    }
    else if (Pol.size() == 6)
    {
        result = "hexagono";
    }
    else
    {
        result = "circulo";
    }

    return result;
}

void herr()
{
    Mat herr, gray, dst;
    MatND hist;
    herr = imread("D:/Alejandro/TFG Vulkan/OpenCV_Centros/shapes.png");
    if (herr.empty())
    {
        cout << "Cannot load image!" << endl;
        return;
    }

    cvtColor(herr, gray, COLOR_RGB2GRAY);
    imshow("hola", gray);

    // Initialize parameters
    int histSize = 256;    // bin size
    float range[] = { 0, 255 };
    const float* ranges[] = { range };

    // Calculate histogram
    calcHist(&gray, 1, 0, Mat(), hist, 1, &histSize, ranges, true, false);

    // Plot the histogram
    int hist_w = 512; int hist_h = 400;
    int bin_w = cvRound((double)hist_w / histSize);

    Mat histImage(hist_h, hist_w, CV_8UC1, Scalar(0, 0, 0));
    normalize(hist, hist, 0, histImage.rows, NORM_MINMAX, -1, Mat());

    for (int i = 1; i < histSize; i++)
    {
        line(histImage, Point(bin_w * (i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
            Point(bin_w * (i), hist_h - cvRound(hist.at<float>(i))),
            Scalar(255, 0, 0), 2, 8, 0);
    }

    namedWindow("Result", 1);    imshow("Result", histImage);

    threshold(gray, dst, 60, 255, 1);
    imshow("Bin", dst);
}
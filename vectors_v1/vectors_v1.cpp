///vectors V1(created)May 20 2023 Christian Rogers
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include<cmath>
#include<iostream>
#include<vector>
#include<new>
#include <cstdlib>
using namespace std;
//custom datatypes
struct point {
    int x = 0;
    int y = 0;
    int z = 0;
};
struct posVector {
    int xStart = 0;
    int yStart = 0;
    int zStart = 0;
    int m1 = 0;
    int m2 = 0;
    int m3 = 0;
};
struct pixel {
    int x = 0;
    int y = 0;
    int RGB[3] = { 0,0,0 };
    void setWhite() {
        RGB[0] = 255;
        RGB[1] = 255;
        RGB[2] = 255;
    }
    void setBlack() {
        RGB[0] = 0;
        RGB[1] = 0;
        RGB[2] = 0;
    }

};
struct pixelArray {
    pixel* array = nullptr;
    pixel* copy = nullptr;
    size_t size = 0;
    bool isFree = true;
    void deAllocate() {
        if (!isFree) {
            free(array);
            isFree = true;
        }
    }
    void initalize(size_t size) {
        //deAllocate(); uncommenting this line will solve the mem leak iddle 1.5GB --> 10MB howwever at some point sets memory outside bounds causeing undefined behavior
        // each compile is kind of broken(some work others have partialy courupted arrays that have been overwriten)
        this->array = (pixel*)malloc(size * sizeof(*array));
        this->size = size;
        this->isFree = false;
    }
    void setSize(size_t newSize) {
        this->size = newSize;
    }
    int  getSize() {
        return this->size;
    }
    void append(pixel add) {
        int baseSize = this->size;
        copy = (pixel*)malloc(size * sizeof(*copy));
        for (int i = 0; i < baseSize; i++) {
            copy[i] = array[i];
        }
        initalize(baseSize + 1);
        for (int i = 0; i < baseSize; i++) {
            array[i] = copy[i];
        }
        array[baseSize] = add;
        free(copy);
    }
};
class ArrayOpperations {
public:
    pixelArray join(pixelArray firstArray, pixelArray toJoin, size_t size1, size_t size2) {
        pixelArray out;
        out.initalize(size1 + size2);
        int i = 0;
        for (;i < size1; i++) {
            out.array[i] = firstArray.array[i];
        }
        for (int a = 0;i < (size1 + size2);i++) {
            out.array[i] = toJoin.array[a];
            a++;

        }
        ///continue with undefined behaviour due to mem  alloc going out of range
        //
        ///
        ///
        return out;

    }

};
// Global variables
#define xSIZE 900
#define ySIZE 900
ArrayOpperations arrayOpp;
int origin[] = {xSIZE / 2, ySIZE / 2};
bool isStart = true;
int lineSmoothing = 2;
int scale = 100; //(scale--> 1unit == scale(pixels)
// The main window class name.
static TCHAR szWindowClass[] = _T("vectors_V1");
// The string that appears in the application's title bar.
static TCHAR szTitle[] = _T("vectors_V1");
// Stored instance handle for use in Win32 API calls such as FindResource
HINSTANCE hInst;
// Forward declarations of functions included in this code module:
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//
class NormalizedSpace {
    private:
        pixelArray out;
        pixel draw(int x, int y) { // normalize to the origin at the center of the screen
            pixel out;
            out.x = origin[0] + x;
            out.y = origin[1] - y;
            return out;
        }
        pixelArray drawAxisX() {
            pixel pixelOut;
            pixel tempDraw;
            this->out.initalize(0);
            for (int i = 0; i < xSIZE; i++) {
                pixelOut.x = i;
                pixelOut.y  = ySIZE / 2;
                tempDraw = draw(i, 0);
                pixelOut.setWhite();
                //add ticks
                if (tempDraw.x % scale < 5) {
                    pixelOut.setBlack();
                    pixelOut.RGB[0] = 255;
                }
                //
                out.append(pixelOut);
            }
            return this->out;
        }
        pixelArray drawAxisY() {
            pixel pixelOut;
            pixel tempDraw;
            this->out.initalize(0);
            for (int i = 0; i < ySIZE; i++) {
                pixelOut.x = xSIZE / 2;
                pixelOut.y  = i;
                tempDraw = draw(i, 0);
                pixelOut.setWhite();
                //add ticks
                if (tempDraw.x % scale <5) {
                    pixelOut.setBlack();
                    pixelOut.RGB[0] = 255;
                }
                //
                out.append(pixelOut);
            }
            return this->out;
        }
        void outPutLog(int x, int y) {
            cout << "x="<<x<<endl;
            cout << "y=" << y << endl;
        }
        pixel returnPixelCordinate(double loopIterator, vector<int> vectIN, bool isZeroSlopeY, int axisPoint, bool isZeroSlopeX) {
            pixel out;
            if (isZeroSlopeY) {
                outPutLog((int)loopIterator, (int)axisPoint);
                out = draw(axisPoint, loopIterator);
            }
            else if (isZeroSlopeX) {
                outPutLog((int)loopIterator, (int)axisPoint);
                out = draw(loopIterator, axisPoint);
            }
            else {
                outPutLog((int)loopIterator, (int)(ceil((((loopIterator)-vectIN[0]) * (vectIN[3] / vectIN[2])) + vectIN[1])));
                out = draw((int)(loopIterator), (int)(ceil((((loopIterator)-vectIN[0]) * (vectIN[3] / vectIN[2])) + vectIN[1])));
            }
            out.RGB[0] = 255;
            return out;
        }
        pixelArray zeroSlope(vector<int> vectIN) { // start, stop, m1, m2
            vector<int> temp;//blank vector
            this->out.initalize(0);
            if (vectIN[2] == 0) {//is line in y plane
                if (vectIN[3] > 0) {// (pos)
                    for (double yValue = vectIN[1]; yValue < ((vectIN[3] * scale) + vectIN[1]); yValue += ((float)1 / lineSmoothing)) {
                        out.append(returnPixelCordinate(yValue, temp, true, vectIN[0], false));
                    }
                }
                else {// (neg)
                    for (double yValue = vectIN[1]; yValue > ((vectIN[3] * scale) + vectIN[1]); yValue -= ((float)1 / lineSmoothing)) {
                        out.append(returnPixelCordinate(yValue, temp, true, vectIN[0], false));
                    }
                }
            }
            else {//is line in x plane
                if (vectIN[2] > 0) {// (pos)
                    for (double xValue = vectIN[0]; xValue < ((vectIN[2] * scale) + vectIN[0]); xValue += ((float)1 / lineSmoothing)) {
                        out.append(returnPixelCordinate(xValue, temp, false, vectIN[1], true));
                    }
                }
                else {// (neg)
                    for (double xValue = vectIN[0]; xValue > ((vectIN[2] * scale) + vectIN[0]); xValue -= ((float)1 / lineSmoothing)) {
                        out.append(returnPixelCordinate(xValue, temp, false, vectIN[1], true));
                    }
                }
            }
            
            return out;
        }
    public:
        pixelArray plotVector(vector<int> vectIN) {//pass four perameters(xStart, yStart, m1,m2) calls returnPixelCordinate in loop and returns pixel cord assosiated
            vector<int> temp;
            vectIN[0] *= scale;
            vectIN[1] *= scale;
            int xDIR = 1;
            if (vectIN[2] == 0 || vectIN[3] == 0) {
                this->out.initalize(zeroSlope(vectIN).getSize());
                this->out = zeroSlope(vectIN);
            }
            else if (vectIN[2] == 0 || vectIN[3] == 0) {
                this->out.initalize(0);
                return this->out;
            }
            //negative xdirecton
            else if(vectIN[2]<0){
                this->out.initalize(0);
                for (double xValue = vectIN[0]; xValue >= ((vectIN[2] * scale) + (1 / lineSmoothing) + (vectIN[0])); xValue -= ((float)1 / lineSmoothing)) {
                    out.append(returnPixelCordinate(xValue, vectIN, false, NULL, false));
                }
            }
            //positive x direction
            else {
                this->out.initalize(0);
                for (double xValue = vectIN[0]; xValue <= ((vectIN[2] * scale) + (1 / lineSmoothing) + (vectIN[0])); xValue += ((float)1 / lineSmoothing)) {
                    out.append(returnPixelCordinate(xValue, vectIN, false, NULL, false));
                }

            }
            cout << "done\n";
            return out;
        }
        pixelArray drawAxis() {
            pixelArray axisLinesX;
            
            axisLinesX.initalize(drawAxisX().getSize());
            axisLinesX = drawAxisX();
            pixelArray axisLinesY;
            axisLinesY.initalize(drawAxisY().getSize());
            axisLinesY = drawAxisY();

            this->out.initalize(axisLinesX.getSize()+ axisLinesY.getSize());
            this->out = arrayOpp.join(axisLinesX, axisLinesY, axisLinesX.getSize(), axisLinesY.getSize());

            return this->out;
        }

};
class Command {
    private:
        string command;

        //parsing varibles for (xS,xY)[m1,m2]
        string vectComponet1 = "";
        string vectComponet2 = "";
        string xStart = "";
        string yStart = "";
        void parseVectorInput(string commandIN) {//basic vector in
            //format (xStart,yStart)[m1,m2] parsing string input
            int comma = 0;
            bool isStartPoint = true;
            if (commandIN[0] != '(') {return;}
            for (int i = 1; i < commandIN.length() - 1; i++) {
                switch (commandIN[i]) {
                    case ' ':  continue;
                    case '[': continue;
                    case ',': comma++; continue;
                    case ')': comma++;continue;
                }
                switch (comma) {
                    case 0: xStart += commandIN[i];break;
                    case 1: yStart += command[i];break;
                    case 2: vectComponet1 += commandIN[i]; break;
                    case 3: vectComponet2 += commandIN[i];
                }
            }
            return;

        }
        void help() {
            if (command == "help") {
                cout << "R^2 vector: (xStart,yStart)[x,y]\n";
                cout << "line smoothing: smoothing\n";
                cout << "scaling: scale\n";
                cout << "clear screen: clear\n";
                cout << "==================\n";
                cout << "note that invalid input will not throw error";
            }
        }
        void reset() {
            vectComponet1 = "";
            vectComponet2 = "";
            yStart = "";
            xStart = "";
        }
        void setSmoothing() {
            if (command == "smoothing") {
                string temp;
                cout << "\nenter smoothing(current ==" << lineSmoothing << ")\n";
                cin >> temp;
                lineSmoothing = stoi(temp);
            }
        }
        void setScale() {
            if (command == "scale") {
                string temp;
                cout << "\nenter scale(clear to take effect)(current ==" << scale << ")\n";
                cin >> temp;
                scale = stoi(temp);
            }
        }
    public:
        Command() {
            HWND window;
            AllocConsole();
            freopen_s((FILE**)stdout, "CONOUT$", "w", stdout);
            freopen_s((FILE**)stdin, "CONIN$", "r", stdin);
            window = FindWindowA("textWindow", NULL);
            ShowWindow(window, 0);
            if (isStart) {
                cout << "vectors V1 2023 'help' for command's";
                isStart = false;
            }
        }
        bool clear() {
            return this->command == "clear";
        }
        bool isNULL() {
            return this->command == "";
        }
        void start() {
            cout << "\n--> ";
            getline(cin, command);
            parseVectorInput(command);
            if (command == "") {
                return;
            }
            help();
            setSmoothing();
            setScale();
        }
        vector<int> vectorPerameters() {
            vector<int> out;
            if (vectComponet1 == "" || vectComponet2 =="" || yStart =="" || xStart == "") { return out; }
            out.push_back(stoi(xStart));
            out.push_back(stoi(yStart));
            out.push_back(stoi(vectComponet1));
            out.push_back(stoi(vectComponet2));
            reset();
            return out;
        }

};

class ScreenText {
    private:
        string textOut;
    public:
        ScreenText() {
            this->textOut = "(0,0)";
        }
        string update(pixel pixelCacheEnd) {
            textOut = "last position drawn(" + to_string((int)ceil((pixelCacheEnd.x - origin[0]) / scale)) + "," + to_string((int)ceil((origin[1] - pixelCacheEnd.y) / scale)) + ")";
            return textOut;
        }
};

class PixelMemory{
    private:
        NormalizedSpace grid;
        pixelArray pixelCache;
        pixelArray newLines;//helper for append

    public:
        void resetPixelCache() {
            this->pixelCache.initalize(grid.drawAxis().getSize());
            this->pixelCache = grid.drawAxis();
        }
        PixelMemory() {
            resetPixelCache();
        }
        int getSize() {
            return this->pixelCache.getSize();
        }
        pixelArray getPixelCache(vector<int> xStartyStartM1M2) {
            if (!xStartyStartM1M2.empty()) { //if there has been input add to cache
                newLines.initalize(grid.plotVector(xStartyStartM1M2).getSize());
                newLines = grid.plotVector(xStartyStartM1M2);
                for (int i = 0; i < newLines.getSize(); i++) {
                    pixelCache.append(newLines.array[i]);
                }
            }
            return this->pixelCache;
        }
};

class Interface {
    private:
        Command command;
        PixelMemory pixelMem;
        vector<int> vectorPerameters;
        pixelArray out;
        //
        ScreenText screenText;
        string tempText;
        LPSTR currentText;
        int size;
    public:
        Interface() {
            this->size = 0;
            this->currentText = NULL;
            this->out.initalize(pixelMem.getSize());
            this->out = pixelMem.getPixelCache(vectorPerameters);
        }
        int getSize() {
            return this->out.getSize();
        }
        pixelArray call() {
            command.start();
            if (command.clear()) {
                pixelMem.resetPixelCache();
            }
            if (command.isNULL()) { 
                return this->out;
            }
            vectorPerameters = command.vectorPerameters();
            out.initalize(pixelMem.getSize());
            out = pixelMem.getPixelCache(vectorPerameters);
            return this->out;
        }
        LPSTR screenTextOut() {
            tempText = screenText.update(out.array[out.getSize() - 1]);
            currentText = const_cast<char*>(tempText.c_str());
            return const_cast<char*>(tempText.c_str());
        }
        int screenTextLength() {
            this ->size = (int)tempText.length();
            return this->size;
        }

};

/// start gui
int WINAPI WinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPSTR     lpCmdLine,
    _In_ int       nCmdShow
)
{
    WNDCLASSEX wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(wcex.hInstance, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex))
    {
        MessageBox(NULL,
            _T("Call to RegisterClassEx failed!"),
            _T("thats not good"),
            NULL);

        return 1;
    }
    hInst = hInstance;
    HWND hWnd = CreateWindowEx(
        WS_EX_OVERLAPPEDWINDOW,
        szWindowClass,
        szTitle,
        WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        0, 0,
        xSIZE, ySIZE,
        NULL,
        NULL,
        hInstance,
        NULL
    );
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));// setbackground colour
    SetClassLongPtr(hWnd, GCLP_HBRBACKGROUND, (LONG_PTR)brush);

    if (!hWnd)
    {
        MessageBox(NULL,
            _T("Call to CreateWindow failed!"),
            _T("thats not good"),
            NULL);

        return 1;
    }

    // The parameters to ShowWindow explained:
    // hWnd: the value returned from CreateWindow
    // nCmdShow: the fourth parameter from WinMain
    ShowWindow(hWnd,
        nCmdShow);
    UpdateWindow(hWnd);

    // Main message loop:
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    pixelArray pixelCache;
    Interface user;
    LPSTR screenText;
    switch (message)
    {
    case WM_PAINT:
        while (true) {
            pixelCache.initalize(user.getSize());
            pixelCache = user.call();
            hdc = BeginPaint(hWnd, &ps);
            screenText = user.screenTextOut();
            TextOutA(hdc, xSIZE - 200, 2 * user.screenTextLength(), user.screenTextOut(), user.screenTextLength());
            for (int i = 0; i < pixelCache.getSize(); i++) {
                SetPixel(hdc, pixelCache.array[i].x, pixelCache.array[i].y, RGB(pixelCache.array[i].RGB[0], pixelCache.array[i].RGB[1], pixelCache.array[i].RGB[2]));
                    
            }
            
            EndPaint(hWnd, &ps);
            InvalidateRect(hWnd, 0, 1);
        }
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
        break;
    }

    return 0;
}

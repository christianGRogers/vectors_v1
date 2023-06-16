///vectors V1(created)May 20 2023 Christian Rogers
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include<cmath>
#include<iostream>
#include<vector>
using namespace std;
struct pixel {
    int x;
    int y;
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
struct XYZ {
    int x = 0;
    int y = 0;
    int z = 0;
};
struct groundedVector {
    int xStart = 0;
    int yStart = 0;
    int zStart = 0;
    int m1 = 0;
    int m2 = 0;
    int m3 = 0;
    bool empty(){
        return xStart == 0 && yStart == 0 && zStart == 0 && m1 == 0 && m2 == 0 && m3 == 0;
    }
};


// Global variables(
#define xSIZE 900
#define ySIZE 900
int origin[] = { xSIZE / 2, ySIZE / 2 };
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

class ParseVector {
private:
    XYZ parsePoint() {

    }
    XYZ parseVector() {
    }
public:

    groundedVector parseVetor(){

    }
};
class NormalizedSpace {
private:
    pixel draw(int x, int y) { // normalize to the origin at the center of the screen
        pixel out;
        out.x = origin[0] + x;
        out.y = origin[1] - y;
        return out;
    }
    vector<pixel> drawAxisX() {
        vector<pixel> out;
        pixel pixelOut;
        pixel tempDraw;
        for (int i = 0; i <= xSIZE; i++) {
            pixelOut.x = i;
            pixelOut.y = ySIZE / 2;
            tempDraw = draw(i, 0);
            pixelOut.setWhite();
            //add ticks
            if (tempDraw.x % scale < 5) {
                pixelOut.setBlack();
                pixelOut.RGB[0] = 255;
            }
            //
            out.push_back(pixelOut);
        }
        return out;
    }
    vector<pixel> drawAxisY() {
        vector<pixel> out;
        pixel pixelOut;
        pixel tempDraw;
        for (int i = 0; i <= ySIZE; i++) {
            pixelOut.x = xSIZE / 2;
            pixelOut.y = i;
            tempDraw = draw(i, 0);
            pixelOut.setWhite();
            //add ticks
            if (tempDraw.x % scale < 5) {
                pixelOut.setBlack();
                pixelOut.RGB[0] = 255;
            }
            //
            out.push_back(pixelOut);
        }
        return out;
    }
    void consoleLogCord(int x, int y) {
        cout << "x=" << x << endl;
        cout << "y=" << y << endl;
    }
    pixel returnPixelCordinate(double loopIterator, groundedVector vectIN, bool isZeroSlopeY, int axisPoint, bool isZeroSlopeX) {
        pixel out;
        if (isZeroSlopeY) {
            consoleLogCord((int)loopIterator,(int)axisPoint);
            out = draw(axisPoint, loopIterator);
        }
        else if (isZeroSlopeX) {
            consoleLogCord((int)axisPoint, (int)loopIterator);
            out = draw(loopIterator, axisPoint);
        }
        else {
            consoleLogCord((int)(loopIterator), (int)(ceil((((loopIterator)-vectIN.xStart) * (vectIN.m2 / vectIN.m1)) + vectIN.yStart)));
            out = draw((int)(loopIterator), (double)(ceil((((loopIterator)-(double)vectIN.xStart) * ((float)vectIN.m2 / (float)vectIN.m1)) + (float)vectIN.yStart)));
        }
        out.RGB[0] = 255;
        return out;
    }
    vector<pixel> zeroSlope(groundedVector vectIN) { 
        groundedVector temp;
        vector<pixel> out;
        if (vectIN.m1 == 0) {//is line in y plane
            if (vectIN.m2 > 0) {// (pos)
                for (double yValue = vectIN.yStart; yValue < ((vectIN.m2 * scale) + vectIN.yStart); yValue += ((float)1 / lineSmoothing)) {
                    out.push_back(returnPixelCordinate(yValue, temp, true, vectIN.xStart, false));
                }
            }
            else {// (neg)
                for (double yValue = vectIN.yStart; yValue > ((vectIN.m2 * scale) + vectIN.yStart); yValue -= ((float)1 / lineSmoothing)) {
                    out.push_back(returnPixelCordinate(yValue, temp, true, vectIN.xStart, false));
                }
            }
        }
        else {//is line in x plane
            if (vectIN.m1 > 0) {// (pos)
                for (double xValue = vectIN.xStart; xValue < ((vectIN.m1 * scale) + vectIN.xStart); xValue += ((float)1 / lineSmoothing)) {
                    out.push_back(returnPixelCordinate(xValue, temp, false, vectIN.yStart, true));
                }
            }
            else {// (neg)
                for (double xValue = vectIN.xStart; xValue > ((vectIN.m1 * scale) + vectIN.xStart); xValue -= ((float)1 / lineSmoothing)) {
                    out.push_back(returnPixelCordinate(xValue, temp, false, vectIN.yStart, true));
                }
            }
        }

        return out;
    }
public:
    vector<pixel> plotVector(groundedVector vectIN) {//pass four perameters(xStart, yStart, m1,m2) calls returnPixelCordinate in loop and returns pixel cord assosiated
        vector<pixel> out;
        vectIN.xStart *= scale;
        vectIN.yStart *= scale;
        int xDIR = 1;
        if (vectIN.m1 == 0 || vectIN.m2 == 0) {
            out = zeroSlope(vectIN);
        }
        else if (vectIN.m1 == 0 || vectIN.m2 == 0) {
            return out;
        }
        //negative xdirecton
        else if (vectIN.m1 < 0) {
            for (double xValue = vectIN.xStart; xValue >= ((vectIN.m1 * scale) + (1 / lineSmoothing) + (vectIN.xStart)); xValue -= ((float)1 / lineSmoothing)) {
                out.push_back(returnPixelCordinate(xValue, vectIN, false, NULL, false));
            }
        }
        //positive x direction
        else {
            for (double xValue = vectIN.xStart; xValue <= ((vectIN.m1 * scale) + (1 / lineSmoothing) + (vectIN.xStart)); xValue += ((float)1 / lineSmoothing)) {
                out.push_back(returnPixelCordinate(xValue, vectIN, false, NULL, false));
            }

        }
        cout << "done\n";
        return out;
    }
    vector<pixel> drawAxis() {
        vector<pixel> out = drawAxisX();
        vector<pixel> temp = drawAxisY();
        out.insert(out.end(), temp.begin(), temp.end());
        return out;
    }


};
class Opperations {
private:
public:
    groundedVector addition(string command) {
        //[x,y,z]+[]
        for (int i : command) {

        }
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
        if (commandIN[0] != '(') { return; }
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
        return command == "clear";
    }
    void start() {
        cout << "\n--> ";
        getline(cin, command);
        parseVectorInput(command);
        help();
        setSmoothing();
        setScale();


    }
    groundedVector vectorPerameters() {
        groundedVector outPerameters;
        if (vectComponet1 == "" || vectComponet2 == "" || yStart == "" || xStart == "") { return outPerameters; }
        outPerameters.xStart = stoi(this->xStart);
        outPerameters.yStart = stoi(this->yStart);
        outPerameters.m1 = stoi(this->vectComponet1);
        outPerameters.m2 = stoi(this->vectComponet2);
        reset();
        return outPerameters;
    }

};

class ScreenText {
private:
    string textOut;
public:
    ScreenText() {
        textOut = "(0,0)";
    }
    string update(pixel pixelCacheEnd) {
        textOut = "last position drawn(" + to_string((int)ceil((pixelCacheEnd.x - origin[0]) / scale)) + "," + to_string((int)ceil((origin[1] - pixelCacheEnd.y) / scale)) + ")";
        return textOut;
    }
};

class PixelMemory {
private:
    NormalizedSpace grid;
    vector<pixel> pixelCache;
    vector<pixel> temp; //helper for insert

public:
    PixelMemory() {
        pixelCache = grid.drawAxis();
    }
    void clearPixelMemory() {
        pixelCache.clear();
        pixelCache = grid.drawAxis();

    }
    vector<pixel> getPixelCache(groundedVector xStartyStartM1M2) {
        if (!xStartyStartM1M2.empty()) { //if there has been input add to cache
            temp = grid.plotVector(xStartyStartM1M2);
            pixelCache.insert(pixelCache.end(), temp.begin(), temp.end());
            temp.clear();
        }
        return pixelCache;
    }
};

class Interface {
private:
    Command command;
    PixelMemory pixelMem;
    groundedVector vectorPerameters;
    vector<pixel> out;
    //
    ScreenText screenText;
    string tempText;
    LPSTR currentText = NULL;
    int size = 0;
public:
    vector<pixel> call() {
        command.start();
        if (command.clear()) {
            pixelMem.clearPixelMemory();
        }
        vectorPerameters = command.vectorPerameters();
        out = pixelMem.getPixelCache(vectorPerameters);
        return out;
    }
    LPSTR screenTextOut() {
        tempText = screenText.update(out[out.size() - 1]);
        currentText = const_cast<char*>(tempText.c_str());
        return const_cast<char*>(tempText.c_str());
    }
    int screenTextLength() {
        size = (int)tempText.length();
        return size;
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

    // Store instance handle in our global variable
    hInst = hInstance;

    // The parameters to CreateWindowEx explained:
    // WS_EX_OVERLAPPEDWINDOW : An optional extended window style.
    // szWindowClass: the name of the application
    // szTitle: the text that appears in the title bar
    // WS_OVERLAPPEDWINDOW: the type of window to create
    // CW_USEDEFAULT, CW_USEDEFAULT: initial position (x, y)
    // 500, 100: initial size (width, length)
    // NULL: the parent of this window
    // NULL: this application does not have a menu bar
    // hInstance: the first parameter from WinMain
    // NULL: not used in this application
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
    HBRUSH brush = CreateSolidBrush(RGB(0, 0, 0));
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
    vector<pixel> pixelCache;
    Interface user;
    LPSTR screenText;
    switch (message)
    {
    case WM_PAINT:
        while (true) {
            // LPSTR screenText;
            pixelCache = user.call();
            hdc = BeginPaint(hWnd, &ps);
            screenText = user.screenTextOut();
            TextOutA(hdc, xSIZE - 200, 2 * user.screenTextLength(), screenText, user.screenTextLength());
            for (int i = 0; i < pixelCache.size(); i++) {
                SetPixel(hdc, pixelCache[i].x, pixelCache[i].y, RGB(pixelCache[i].RGB[0], pixelCache[i].RGB[1], pixelCache[i].RGB[2]));
            }
            pixelCache.clear();
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

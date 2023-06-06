///vectors V1(created)May 20 2023 Christian Rogers
#include <windows.h>
#include <stdlib.h>
#include <string>
#include <tchar.h>
#include<cmath>
#include<iostream>
#include<vector>
using namespace std;
//line 124 ish
// Global variables(
#define xSIZE 900
#define ySIZE 900
int origin[2] = { xSIZE / 2, ySIZE / 2 };
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


class Grid {
    private:
        vector<int> draw(int x, int y) {
            vector <int> out;
            out.push_back(origin[0] + x);
            out.push_back(origin[1] - y);
            return out;
        }
        vector<vector<int>> drawAxisX() {
            vector<vector<int>> out;
            vector<int> tempXY;
            vector<int> tempDraw;
            for (int i = 0; i <= xSIZE; i++) {
                tempXY.push_back(i);
                tempXY.push_back(ySIZE / 2);
                //RGB
                tempDraw = draw(i, 0);
                if (tempDraw[0] % scale < 5) {
                    tempXY.push_back(255);
                    tempXY.push_back(0);
                    tempXY.push_back(0);
                }
                else {
                    tempXY.push_back(255);
                    tempXY.push_back(255);
                    tempXY.push_back(255);
                }
                //
                out.push_back(tempXY);
                tempXY.clear();
            }
            return out;
        }
        vector<vector<int>> drawAxisY() {
            vector<vector<int>> out;
            vector<int> tempXY;
            vector<int> tempDraw;
            for (int i = 0; i <= ySIZE; i++) {
                tempXY.push_back(xSIZE / 2);
                tempXY.push_back(i);
                //RGB
                tempDraw = draw(i, 0);
                if (tempDraw[0] % scale <5) {
                    tempXY.push_back(255);
                    tempXY.push_back(0);
                    tempXY.push_back(0);
                }
                else {
                    tempXY.push_back(255);
                    tempXY.push_back(255);
                    tempXY.push_back(255);
                }
                //
                out.push_back(tempXY);
                tempXY.clear();
            }
            return out;
        }

        vector<int> returnPixelCordinate(double loopIterator, vector<int> vectIN, bool isZeroSlopeY, int axisPoint, bool isZeroSlopeX) {
            vector<int> out;
            if (isZeroSlopeY) {
                cout << "x=" << (int)loopIterator << "\n";
                cout << "y=" << (int)axisPoint << "\n";
                out = draw(axisPoint, loopIterator);
            }
            else if (isZeroSlopeX) {
                cout << "x=" << (int)axisPoint << "\n";
                cout << "y=" << (int)loopIterator << "\n";
                out = draw(loopIterator, axisPoint);
            }
            else {
                cout << "x=" << (int)(loopIterator) << "\n";
                cout << "y=" << (int)(ceil((((loopIterator)-vectIN[0]) * (vectIN[3] / vectIN[2])) + vectIN[1])) << "\n";
                out = draw((int)(loopIterator), (int)(ceil((((loopIterator)-vectIN[0]) * (vectIN[3] / vectIN[2])) + vectIN[1])));
            }
            //RGB
            out.push_back(255);
            out.push_back(0);
            out.push_back(0);
            //
            return out;
        }
        vector<vector<int>> zeroSlope(vector<int> vectIN) { // start, stop, m1, m2
            vector<int> temp;
            vector<vector<int>> out;
            if (vectIN[2] == 0) {//is line in y plane
                if (vectIN[3] > 0) {// (pos)
                    for (double yValue = vectIN[1]; yValue < ((vectIN[3] * scale) + vectIN[1]); yValue += ((float)1 / lineSmoothing)) {
                        out.push_back(returnPixelCordinate(yValue, temp, true, vectIN[0], false));
                    }
                }
                else {// (neg)
                    for (double yValue = vectIN[1]; yValue > ((vectIN[3] * scale) + vectIN[1]); yValue -= ((float)1 / lineSmoothing)) {
                        out.push_back(returnPixelCordinate(yValue, temp, true, vectIN[0], false));
                    }
                }
            }
            else {//is line in x plane
                if (vectIN[2] > 0) {// (pos)
                    for (double xValue = vectIN[0]; xValue < ((vectIN[2] * scale) + vectIN[0]); xValue += ((float)1 / lineSmoothing)) {
                        out.push_back(returnPixelCordinate(xValue, temp, false, vectIN[1], true));
                    }
                }
                else {// (neg)
                    for (double xValue = vectIN[0]; xValue > ((vectIN[2] * scale) + vectIN[0]); xValue -= ((float)1 / lineSmoothing)) {
                        out.push_back(returnPixelCordinate(xValue, temp, false, vectIN[1], true));
                    }
                }
            }
            
            return out;
        }
    public:
        vector<vector<int>> plotVector(vector<int> vectIN) {//pass four perameters(xStart, yStart, m1,m2) calls returnPixelCordinate in loop and returns pixel cord assosiated
            vector<vector<int>> out;
            vector<int> temp;
            vectIN[0] *= scale;
            vectIN[1] *= scale;
            int xDIR = 1;
            if (vectIN[2] == 0 || vectIN[3] == 0) {
                out = zeroSlope(vectIN);
            }
            else if (vectIN[2] == 0 || vectIN[3] == 0) {
                return out;
            }
            //negative xdirecton
            else if(vectIN[2]<0){
                for (double xValue = vectIN[0]; xValue >= ((vectIN[2] * scale) + (1 / lineSmoothing) + (vectIN[0])); xValue -= ((float)1 / lineSmoothing)) {
                    out.push_back(returnPixelCordinate(xValue, vectIN, false, NULL, false));
                }
            }
            //positive x direction
            else {
                for (double xValue = vectIN[0]; xValue <= ((vectIN[2] * scale) + (1 / lineSmoothing) + (vectIN[0])); xValue += ((float)1 / lineSmoothing)) {
                    out.push_back(returnPixelCordinate(xValue, vectIN, false, NULL, false));
                }

            }
            cout << "done\n";
            return out;
        }
        vector<vector<int>> drawAxis() {
            vector<vector<int>> out = drawAxisX();
            vector<vector<int>> temp = drawAxisY();
            out.insert(out.end(), temp.begin(), temp.end());
            return out;
        }


};
//todo
class Opperations { // called from commands to perform more complex vector opperations 
    private:
        vector<int> values;
        int type;
    public:
        Opperations(vector<int> values, int type) {
            this->values = values;
            this->type = type;
        }
        void drawOutput(vector<vector<int>> linesToBeDrawn) {

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
            textOut = "(0,0)";
        }
        string update(vector<int> pixelCacheEnd) {
            textOut = "last position drawn(" + to_string((int)ceil((pixelCacheEnd[0] - origin[0]) / scale)) + "," + to_string((int)ceil((origin[1] - pixelCacheEnd[1]) / scale)) + ")";
            return textOut;
        }
};

class PixelMemory{
    private:
        Grid grid;
        vector<vector<int>> pixelCache;
        vector<vector<int>> temp; //helper for insert

    public:
        PixelMemory() {
            pixelCache = grid.drawAxis();
        }
        void clearPixelMemory() {
            pixelCache.clear();
            pixelCache = grid.drawAxis();

        }
        vector<vector<int>> getPixelCache(vector<int> xStartyStartM1M2) {
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
        vector<int> vectorPerameters;
        vector<vector<int>> out;
        //
        ScreenText screenText;
        string tempText;
        LPSTR currentText = NULL;
        int size = 0;
    public:
        vector<vector<int>> call() {
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
    vector<vector<int>> pixelCache;
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
                SetPixel(hdc, pixelCache[i][0], pixelCache[i][1], RGB(pixelCache[i][2], pixelCache[i][3], pixelCache[i][4]));
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


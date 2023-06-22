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
struct XY {
    int x = 0;
    int y = 0;
};
struct groundedVector {
    int RGB[3] = { 0,0,0 };
    int xStart = 0;
    int yStart = 0;
    int m1 = 0;
    int m2 = 0;
    bool empty(){
        return xStart == 0 && yStart == 0&& m1 == 0 && m2 == 0;
    }
    void setRed() {
        RGB[0] = 255;
        RGB[1] = 0;
        RGB[2] = 0;
    }
    void setGreen() {
        RGB[0] = 0;
        RGB[1] = 255;
        RGB[2] = 0;
    }
    void setBlue() {
        RGB[0] = 0;
        RGB[1] = 0;
        RGB[2] = 255;
    }
};
struct opperation {
    int RGB[3] = { 0,0,0 };
    int x = 0;
    int y = 0;
    int opp = 0;
    bool empty() {
        return x == 0 && y == 0 && opp == 0;
    }
    void setRed() {
        RGB[0] = 255;
        RGB[1] = 0;
        RGB[2] = 0;
    }
    void setGreen() {
        RGB[0] = 0;
        RGB[1] = 255;
        RGB[2] = 0;
    }
    void setBlue() {
        RGB[0] = 0;
        RGB[1] = 0;
        RGB[2] = 255;
    }
};
class Helper {
public:
    bool isValidStoi(string in) {
        for (int i = 0; i < in.length();i++) {
            if (45 == (int)in[i]) {
                continue;
            }
            else if (48 > (int)in[i] || (int)in[i] > 57) {
                 return false;
            }
        }
        return true;
    }

};

// Global variables(
#define xSIZE 900
#define ySIZE 900
int origin[] = { xSIZE / 2, ySIZE / 2 };
bool isStart = true;
int lineSmoothing = 2;
int scale = 100; //(scale--> 1unit == scale(pixels)
Helper helper;
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
    XY parsePoint(string in) {
        XY out;
        string intermidiate;
        for (int i = 0; i < in.length(); i++) {
            if ((int)in[i] == 44) {
                if (helper.isValidStoi(intermidiate)) {
                    out.x = stoi(intermidiate);
                }
                intermidiate = "";
                continue;
            }
            else if ((int)in[i] < 48 || (int)in[i]>57) {
                if ((int)in[i] != 43 && (int)in[i] != 45) {
                    continue;
                }
            }
            intermidiate += in[i];
        }
        if (helper.isValidStoi(intermidiate)) {
            out.y = stoi(intermidiate);
        }
        return out;

    }
public:
    groundedVector parseVetorGrounded(string in) {//parse a input in the form (x,y)[m1,m2]
        groundedVector out;
        if ((int)in[0] != 40) {
            return out;
        }
        bool b1 = false;
        bool b2 = false;
        string point;
        string direction;
        for (int i = 0; i < in.length(); i++) {
            switch ((int)in[i]) {
            case 40:
                b1 = true;
                continue;
            case 41:
                b1 = false;
                continue;
            case 91:
                b2 = true;
                continue;
            case 93:
                b2 = false;
                continue;
            default:
                break;
            }
            if (b1) {
                point += in[i];
            }
            else if (b2) {
                direction += in[i];
            }
        }
        XY pointINT = parsePoint(point);
        XY directionINT = parsePoint(direction);
        switch ((int)in[in.length() - 1]) {
        case 82: out.setRed(); break;
        case 71: out.setGreen(); break;
        case 66: out.setBlue(); break;
        }
        out.xStart = pointINT.x;
        out.yStart = pointINT.y;
        out.m1 = directionINT.x;
        out.m2 = directionINT.y;
        return out;
    }
    groundedVector directional(string command) {
        string intermidiate;
        groundedVector out;
        XY mValues;
        int defualt = 2;
        if (93 == (int)command[command.length()-1]){
            defualt = 1;
        }
        if (91 == (int)command[0]) {
            for (int i = 1; i < command.length() - defualt; i++) {
                intermidiate += command[i];
            }
            mValues = parsePoint(intermidiate);
        }
        switch ((int)command[command.length() -1]) {
        case 82: out.setRed(); break;
        case 71: out.setGreen();break;
        case 66: out.setBlue();break;
        }
        out.xStart = 0;
        out.yStart = 0;
        out.m1 = mValues.x;
        out.m2 = mValues.y;
        return out;
    }
    vector<opperation> opperartionParse(string command) { //[x,y] + [x,y] + [x,y] - [x,y] + [x,y] etc returuns a vect of elemets x,y,opp if opp == 0; start/end assci ->int
        vector<opperation> out;
        if ((int)command[0] != 91) {
            return out;
        }
        vector<int> opperationType;
        bool isOPP = false;
        bool open = false;
        string BAR;
        XY FOO;
        opperation intermidiate;
        for (int i = 0; i < command.length(); i++) {
            if ((int)command[i] == 91) {
                open = true;
            }
            else if ((int)command[i] == 93) {
                open = false;
            }
            if ((int)command[i] == 43 || (int)command[i] == 45){
                if (!open) {
                    opperationType.push_back((int)command[i]);
                    isOPP = true;
                }
            }
        }
        if (!isOPP) {
            return out;
        }
        for (int i = 0; i < command.length(); i++) {
            if (!isOPP) {
                if ((int)command[i] == 82 || (int)command[i] == 71 || (int)command[i] == 66) {
                    switch ((int)command[i]) {
                    case 82: intermidiate.setRed(); break;
                    case 71: intermidiate.setGreen(); break;
                    case 66: intermidiate.setBlue(); break;
                    }
                    out.push_back(intermidiate);
                    continue;
                }
            }
            if((int)command[i] == 91){
                BAR = "";
                isOPP = true;
                continue;
            }
            if ((int)command[i] == 93) {
                isOPP = false;
                FOO = parsePoint(BAR);
                intermidiate.x = FOO.x;
                intermidiate.y = FOO.y;
                continue;
            }
            else if (isOPP) {
                BAR += command[i];
            }
        }
        for (int i = 1; i < out.size(); i++) {
            out[i].opp = opperationType[i-1];
        }
        return out;

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
    //called by all vector draw fuctions to return apropriate cords and log them to screen
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
        out.RGB[0] = vectIN.RGB[0];
        out.RGB[1] = vectIN.RGB[1];
        out.RGB[2] = vectIN.RGB[2];
        return out;
    }
    vector<pixel> zeroSlope(groundedVector vectIN) { //for special case vectors with no slope
        vector<pixel> out;
        if (vectIN.m1 == 0) {//is line in y plane
            if (vectIN.m2 > 0) {// (pos)
                for (double yValue = vectIN.yStart; yValue < ((vectIN.m2 * scale) + vectIN.yStart); yValue += ((float)1 / lineSmoothing)) {
                    out.push_back(returnPixelCordinate(yValue, vectIN, true, vectIN.xStart, false));
                }
            }
            else {// (neg)
                for (double yValue = vectIN.yStart; yValue > ((vectIN.m2 * scale) + vectIN.yStart); yValue -= ((float)1 / lineSmoothing)) {
                    out.push_back(returnPixelCordinate(yValue, vectIN, true, vectIN.xStart, false));
                }
            }
        }
        else {//is line in x plane
            if (vectIN.m1 > 0) {// (pos)
                for (double xValue = vectIN.xStart; xValue < ((vectIN.m1 * scale) + vectIN.xStart); xValue += ((float)1 / lineSmoothing)) {
                    out.push_back(returnPixelCordinate(xValue, vectIN, false, vectIN.yStart, true));
                }
            }
            else {// (neg)
                for (double xValue = vectIN.xStart; xValue > ((vectIN.m1 * scale) + vectIN.xStart); xValue -= ((float)1 / lineSmoothing)) {
                    out.push_back(returnPixelCordinate(xValue, vectIN, false, vectIN.yStart, true));
                }
            }
        }

        return out;
    }
public:
    //called regardless of vect type just given different context
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
    vector<groundedVector> returnGroundedVcetors(vector<opperation> in) { // take a vect of opperations and return their corosponding result with vectors use to get there
        vector<groundedVector> out;
        XY endPoint;
        groundedVector FOO;
        //
        //continue with math implementation
        //
        for (int i = 0; i < in.size();i++) {
            FOO.xStart = endPoint.x;
            FOO.yStart = endPoint.y;
            FOO.m1 = in[i].x;
            FOO.m2 = in[i].y;
            //
            if ((int)in[i].opp == 43 || (int)in[i].opp == 0) {//addition
                endPoint.x += in[i].x;
                endPoint.y += in[i].y;
            }
            else if ((int)in[i].opp == 45) { //subtraction
                FOO.m1 *= -1;
                FOO.m2 *= -1;
                endPoint.x -= in[i].x;
                endPoint.y -= in[i].y;
            }
            FOO.RGB[0] = in[i].RGB[0];
            FOO.RGB[1] = in[i].RGB[1];
            FOO.RGB[2] = in[i].RGB[2];

            out.push_back(FOO);
        }
        FOO.xStart = 0;
        FOO.yStart = 0;
        FOO.m1 = endPoint.x;
        FOO.m2 = endPoint.y;
        FOO.setRed();
        out.push_back(FOO);
        return out;
    }
};

class Command {
private:
    string command;
    ParseVector parse;


    void help() {
        if (command == "help") {
            cout << "R^2 vector: (xStart,yStart)[m1,m2]# (note that R,G or B in place of # will select colour{defualt to black})\n";
            cout << "R^2 vector: [m1,m2]#\n";
            cout << "R^2 vector: [x,y]#+[x,y]#-[x,y]# ...etc(result will be drawn in red)\n";
            cout << "line smoothing: smoothing\n";
            cout << "scaling: scale\n";
            cout << "clear screen: clear\n";
            cout << "==================\n";
            cout << "note that invalid input will not throw error";
        }
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
        help();
        setSmoothing();
        setScale();
    }
    //all three callled by the interface to pass a parsed comand or array of commands to the apropriate draw or calculate
    groundedVector vectorPerametersPointBased() {
        return parse.parseVetorGrounded(command);
    }
    groundedVector directionVector() {
        return parse.directional(command);
    }
    vector<opperation> opperations() {
        return parse.opperartionParse(command);
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
    groundedVector vectorPerametersPointBased;
    vector<pixel> out;
    Opperations opp;
    //
    ScreenText screenText;
    string tempText;
    LPSTR currentText = NULL;
    int size = 0;
public:
    vector<pixel> call() {
        command.start();
        out.clear();
        if (command.clear()) {
            pixelMem.clearPixelMemory();
        }
        //gather possible draw command parsed
        vectorPerametersPointBased = command.directionVector();

        if (command.opperations().size() != 0) {
            vector<groundedVector> vectorPerametersPointBased;
            vectorPerametersPointBased = opp.returnGroundedVcetors(command.opperations());
            for (int i = 0; i < vectorPerametersPointBased.size(); i++) {
                vector<pixel> temp = pixelMem.getPixelCache(vectorPerametersPointBased[i]);
                out.insert(out.end(), temp.begin(), temp.end());
            }
            if (!(out.size() == 0)) {
                return out;
            }    
        }
        if (vectorPerametersPointBased.empty()) {
            vectorPerametersPointBased = command.vectorPerametersPointBased();
        }
        //base (draw one vector)
        out = pixelMem.getPixelCache(vectorPerametersPointBased);
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


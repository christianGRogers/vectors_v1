#include<iostream>
#include<new>
#include <vector>
#include<cmath>
using namespace std;
#define xSIZE 900
#define ySIZE 900

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
    void deAllocate() {
        if (array == nullptr) {
            return;
        }
        delete[] array;
        array = nullptr;
    }
    void initalize(size_t size) {
        deAllocate();
        this->array = new pixel[size];
        this->size = size;
    }
    void setSize(size_t newSize) {
        this->size = newSize;
    }
    int  getSize() {
        return this->size;
    }
    void append(pixel add) {
        size_t baseSize = this->size;
        copy = new pixel[baseSize];
        for (int i = 0; i < baseSize; i++) {
            copy[i] = array[i];
        }
        initalize(baseSize + 1);
        for (int i = 0; i < baseSize; i++) {
            array[i] = copy[i];
        }
        array[baseSize] = add;
        delete[] copy;
        copy = nullptr;
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
        return out;

    }

};
// Global variables
ArrayOpperations arrayOpp;
int origin[] = {xSIZE / 2, ySIZE / 2};
bool isStart = true;
int lineSmoothing = 2;
int scale = 100; //(scale--> 1unit == scale(pixels)
// The main window class name.

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
int main(){
    NormalizedSpace grid;
    pixelArray axis;
    axis.initalize(grid.drawAxis().getSize());
    axis= grid.drawAxis();
    for(int i =0; i<grid.drawAxis().getSize(); i++){
        cout<<"x:"<<axis.array[i].x<<endl;
        cout<< "y:"<<axis.array[i].y<<endl;
    }
    return 0;
}

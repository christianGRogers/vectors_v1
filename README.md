# vectors_v1

Old version in which I atempted to impliment dynamic memory using the new library then memalloc which both resulted in a leak. Within the array struct the commented call to deallocate memory. Commented the app uses 1.5gb idel which is obiously compleatly unreasnobale for what its doing. Uncomented there are undefined compile behavour which according to some nerd on stack overflow is the result on deallocating a pointed block that is NULL. Undefined behaviour being the app's ability to function varies based on build and run. Spent hours trying to fix but just keeped circling around the point that I dont know how to make a mem safe app so I went back to work with the vectors library.

However at leat I actualy learnt how to use a pointer and stuff like passing by referance, and how to atempt to solve a mem leak.

only working vect opp is positional:
(xStart,yStart)[m1,m2]
///no colour specifyer ;defualts to red
eg:
(1,2)[3,4]

I did not include a exe for this one as it was functonaly the same as main; minus the extra features. At one point RAM analisis in VS said its was using 25GB, given I only have 16GB it fell back on HDD which almost crached my computer. So yeah I don't think you want to run this one.

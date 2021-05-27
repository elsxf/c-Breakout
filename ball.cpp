#include <stdio.h>
#include <math.h>

#define PI 3.1415926

class Ball{
    public:
        int x;
        int y;
        int rotation;
        int speed;
    void move(){
        x+=cos(rotation*PI/180)*speed;
        y+=sin(rotation*PI/180)*speed;
    }
    
};
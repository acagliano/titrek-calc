#include "mymath.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
const char cosLUT[64] = {127, 126, 126, 126, 126, 125, 125, 124, 123, 123, 122, 121, 120, 119, 118, 117, 116, 114, 113, 111, 110, 108, 107, 105, 103, 101, 99, 97, 95, 93, 91, 89, 87, 84, 82, 80, 77, 75, 72, 70, 67, 64, 62, 59, 56, 53, 50, 48, 45, 42, 39, 36, 33, 30, 27, 24, 21, 17, 14, 11, 8, 5, 2, 0};


long r_GetDistance(int xdiff, int ydiff, int zdiff){
    long distance = xdiff * xdiff + ydiff * ydiff + zdiff * zdiff;
    return (long)sqrt(distance);
}

signed char byteCos(unsigned char x){
    if(x < 64) return cosLUT[x];
    else if(x < 128) return -cosLUT[127 - x];
    else if(x < 192) return -cosLUT[x - 128];
    else if(x < 256) return cosLUT[255 - x];
}

signed char byteSin(unsigned char x){
    return byteCos(x-64);
}

char r_ArcTan(short numerator, short denominator){
    
}




void AnglesToVectors(Position_t *pos){
    unsigned char xzangle = pos->angles.xz, yangle = pos->angles.y;
    pos->vectors.x = byteCos(xzangle) * byteCos(yangle);
    pos->vectors.z = byteSin(xzangle) * byteCos(yangle);
    pos->vectors.y = byteSin(yangle);
    //pos->vectors[2] = z vector
}

char lcars_GetIntLength(char value){
    return 1 + (value > 9) + (value > 99);
}

unsigned int text_GetCenterX(char* text, int viewer_width){
    unsigned char length = strlen(text) + 2;
    unsigned char pixellen = length * 5 / 2;
    return (viewer_width>>1) - pixellen;
}


/*
 This is an example function using an array of ints.
 To use an array of coords, you'll need to modify the gt() function
 and update the type of 'arr' in the popheap(), heapify(), and
 heapsort() functions.
 
 Call heapsort() by passing in the array to sort and its size
 WARNING: This will sort in-place. It changes the original array passed to it.
 */
int gt(renderitem_t x,renderitem_t y){return x.distance>y.distance;}
void popheap(renderitem_t* arr,int size){
    //I'll never call this if arr is NULL or size 0
    renderitem_t x=arr[0];
    int k=0;
    int i;
    //I've saved arr[0], now I need to shift the biggest elements up
    while(k<size){
        i=k*2+1;
        if(i<size){
            if(i+1<size){i+=gt(arr[i],arr[i+1]);}
            arr[k]=arr[i];
        }
        k=i;
    }
    arr[i>>1]=arr[size-1];
    arr[size-1]=x;
    return;
}
void heapify(renderitem_t* arr,int size){
    int n=(size-1)>>1;
    int m=n*2+1;
    int i,k;
    renderitem_t j;
    if(size<2){return;}
   
    //Optimizing first iteration, since that's the only one that needs to verify bounds
    if(m+1==size){i=m;} else {i=m+gt(arr[m],arr[m+1]);};
    if(gt(arr[n],arr[i])){j=arr[n];arr[n]=arr[i];arr[i]=j;}
    n--;
    m-=2;
    while(n>=0){
        i=m+gt(arr[m],arr[m+1]);
        if(gt(arr[n],arr[i])){
            j=arr[n];arr[n]=arr[i];arr[i]=j;
            //but now I have to propogate changes up through the tree
            //have to check 2i+1 and 2i+2
            k=i*2+1;
            while(k<size){
                if(k+1!=size){k+=gt(arr[k],arr[k+1]);};
                if(gt(arr[i],arr[k])){
                    j=arr[i];arr[i]=arr[k];arr[k]=j;
                    i=k;
                    k=k*2+1;} else {k=size;}
            }
        }
        n--;
        m-=2;
    }
    return;
}
void heapsort(renderitem_t* arr,int size){
    int i;
    if(!arr){return;}
    heapify(arr,size);
    for(i=size;i>1;i--){    //omitting i=0 since it's the last item, so of course it will be the smallest of all remaining items
        popheap(arr,i);
    }
    return;
}


#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define min(a, b) ((a < 0) ? b : (b < 0 ? a : (a < b ? a : b)))

typedef struct point_ {
    float x;
    float y;
} point;

float d(point a, point b){ return sqrt( pow(a.x - b.x, 2) + pow(a.y - b.y, 2) ); }

void swapElements(point *vector, int i, int j){
    point tmp = { vector[i].x, vector[i].y };
    vector[i] = (point) { vector[j].x, vector[j].y };
    vector[j] = (point) { tmp.x, tmp.y };

    return;
}

void quickSort(point *vector, int leftIndex, int rightIndex){
    if(leftIndex < rightIndex){
        float pivot = vector[(int)((rightIndex+leftIndex)/2)].x;
        int left = leftIndex, right = rightIndex;

        while(1){
            // Selects elements from the left that are >= to the pivot
            while(vector[left].x < pivot){ left++; }

            // Selects elements from the right that are <= to the pivot
            while(vector[right].x > pivot){ right--; }

            // If the swapping the above selected elements is worth it, do it
            if(left < right){ swapElements(vector, left++, right--); } // And pass to the next element

            // If the swap is not worth it, the vector was successfully partitionted
            else{ break; }

        }

        // Sorts the partitions
        quickSort(vector, leftIndex, right);
        quickSort(vector, right+1, rightIndex);
    }

    return;
}

// Receives a vector and the position of two sorted subvectors inside it - merges both of them into a fully sorted one
void mergeSubvectors(point *vector, int firstLeftIndex, int firstRightIndex, int secondLeftIndex, int secondRightIndex){
    int vectorSize = (firstRightIndex - firstLeftIndex + 1) + (secondRightIndex - secondLeftIndex + 1);

    point *tmp = (point *)malloc(vectorSize * sizeof(point)); // Temporary vector in which the interpolation will occur
    
    int i, j, k = 0; // Auxiliar variables that'll index the for-loops

    // Loops through both subvectors, merging them
    for(i = firstLeftIndex, j = secondLeftIndex; k < vectorSize; k++){

        // If there are still elements in both subvector
        if(i <= firstRightIndex && j <= secondRightIndex){ 
            // Compares them and copies the lowest one to tmp
            if(vector[i].y < vector[j].y){ tmp[k] = vector[i++]; }
            else{ tmp[k] = vector[j++]; }
        }

        // If not, copies the remaining element to tmp
        else if(i <= firstRightIndex){ tmp[k] = vector[i++]; }
        else if(j <= secondRightIndex){ tmp[k] = vector[j++]; }

    }

    // Copies the sorted vector in tmp to the original one, replacing the subvectors
    for(i = firstLeftIndex, j = secondLeftIndex, k = 0; k < vectorSize; k++){
        if(i <= firstRightIndex){ vector[i++] = tmp[k]; }
        else{ vector[j++] = tmp[k]; }

    }

    free(tmp); tmp = NULL;

    return;
}

// MERGE SORT
void mergeSort(point *vector, int leftIndex, int rightIndex){
    int vectorSize = rightIndex - leftIndex + 1;

    if (vectorSize > 1){
        int middleIndex = (rightIndex + leftIndex) / 2;

        mergeSort(vector, leftIndex, middleIndex);
        mergeSort(vector, middleIndex + 1, rightIndex);
        mergeSubvectors(vector, leftIndex, middleIndex, middleIndex + 1, rightIndex);
    }

    return;
}

float auxGetLowestDist(point *points, int leftIndex, int rightIndex){
    if(leftIndex >= rightIndex){ return -1; }

    else if(rightIndex - leftIndex == 1){ return d(points[leftIndex], points[rightIndex]); }

    int middleIndex = (int)((rightIndex+leftIndex)/2);

    float left = auxGetLowestDist(points, leftIndex, middleIndex);
    float right = auxGetLowestDist(points, middleIndex, rightIndex);

    float distance = min(left, right);

    float pivot = points[middleIndex].x;

    int stripSize = 0;
    point *strip = (point *)malloc(sizeof(point));
    for(int i = leftIndex; i <= rightIndex && points[i].x <= pivot + distance; i++){
        if(points[i].x >= pivot - distance){
            strip = (point *)realloc(strip, ++stripSize * sizeof(point));
            strip[stripSize-1] = (point) {points[i].x, points[i].y};
        }
    }

    // quickSortY(strip, 0, stripSize-1);
    mergeSort(strip, 0, stripSize-1);

    float div = (strip[0].x > points[leftIndex].x && strip[stripSize-1].x < points[rightIndex].x) 
        ? auxGetLowestDist(strip, 0, stripSize-1) 
        : -1;

    free(strip);

    return min(distance, div);
}

float getLowestDist(point *points, int n){
    quickSort(points, 0, n-1);
    return auxGetLowestDist(points, 0, n-1);
}

int main(){
    int noPoints;

    scanf("%d", &noPoints);

    point *points = (point *)malloc(noPoints * sizeof(point));
    for(int i = 0; i < noPoints; i++){ scanf("%f %f", &points[i].x, &points[i].y); }

    float lowestDistance = getLowestDist(points, noPoints);
    printf("%.6f\n", lowestDistance);

    free(points);
    return 0;
}
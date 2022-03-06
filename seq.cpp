#include <iostream>
#include <bits/stdc++.h>
#include <time.h>
#include <math.h>
#include <chrono>
#include <vector>
#include "header.h"

//using namespace std;

using std::cout;
using std::endl;
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;

/**/


int main()
{   
    std::string   DATASET_PATH; 

    // if the centroid are not changed then the method stops
    bool isChange = true;
  
    cout << "DATASET SIZE: "<< DATASET_SIZE << " CLUSTER SIZE: " << endl;

    // declaring array for dataset point and for centroid points
    centroid_point  cpoints;
    static double  x[DATASET_SIZE];
    static double  y[DATASET_SIZE];
    static int     c[DATASET_SIZE];
    

    DATASET_PATH = "Datasets/Data/Dataset16/data1.csv";

    // initialize random seed
    srand(time(NULL));

    // Importing data from file
        

    // generating random centroid for the first step of the method
    cout << "Generating first " << CLUSTER_SIZE << " centroids.." << endl;
    randomCentroids(cpoints, x, y);
    cout << "Finish generating random centroids.." << endl;

    //printClusterPoint(cpoints);
    // funzione che calcola la distanza
        
    auto start = high_resolution_clock::now();
        
    int i = 0;
    while(isChange)
    {
        cout << "Calculating cluster cycle: " << i+1 <<"..."<<  endl;
        calculateDistanceS(x,y,cpoints,c);
        cout << "End calculating cluster cycle: " << i+1 << endl;

        cout << "Updating centroids..."<<  endl;
        isChange = updateCentroids(c,x, y, cpoints);
        isChange = updateCentroids(c, x, y ,cpoints);
        cout << "End Updating centroids..."<<  endl;
        i++;
            
    }
        
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<std::chrono::milliseconds>(stop - start);
    cout << "Tempo: "<<duration.count()<<"ms"<<endl;   
    
    // printing the centroid after the kmeans methods


    saveCSV(x, y, c);
    
    
    printClusterPoint(cpoints);
   
    
}

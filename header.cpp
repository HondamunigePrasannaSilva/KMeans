#include "header.h"
#include <fstream>
#include <math.h>
#include <string>


#include <omp.h>
using std::cout;
using std::endl;
/* ------------------------------- FUNCTIONS ------------------------------- */

void randomCentroids(centroid_point &cp, double vect_x[], double vect_y[])
{
    // calculating max and min of the points to generate random centroids
    int j = 0;
    
    for(int i = 0; i < CLUSTER_SIZE; i++)
    {   
        j = rand() % DATASET_SIZE;
        cp.x_c[i] = vect_x[j];
        cp.y_c[i] = vect_y[j];
    }
}

void printClusterPoint(centroid_point cp)
{
    for (int i = 0; i < CLUSTER_SIZE; i++)
    {
        cout<< cp.x_c[i] <<","<< cp.y_c[i] << endl; 
    }
}

void calculateDistanceS(double vect_x[], double vect_y[], centroid_point cp, int c_vect[])
{
    double dist,temp;
    int cluster_class;
    
    for (int i = 0; i < DATASET_SIZE; i++)
    {
       // calculating distance between dataset point and centroid
       // selecting the centroid with minium distance
        
        dist = distance(vect_x[i], vect_y[i], cp.x_c[0], cp.y_c[0]);
        cluster_class = 0;

        for (int j = 1; j < CLUSTER_SIZE; j++)
        {
            temp = distance(vect_x[i], vect_y[i], cp.x_c[j], cp.y_c[j]);
            if(dist > temp) // looking for the minimum distance given a point
            {
               cluster_class = j;
               dist = temp;
            }
        }
        
        // updating to the beloging cluster 
        c_vect[i] = cluster_class;

    }
}

void calculateDistance(double vect_x[], double vect_y[], centroid_point cp, int c_vect[]) 
{
    double dist,temp;
    int cluster_class;
 
    #pragma omp parallel for private(dist, temp, cluster_class) num_threads(NTHREAD)
    for (int i = 0; i < DATASET_SIZE; i++)
    {
       // calculating distance between dataset point and centroid
       // selecting the centroid with minium distance
        
        dist = distance(vect_x[i], vect_y[i], cp.x_c[0], cp.y_c[0]);
        cluster_class = 0;

        for (int j = 1; j < CLUSTER_SIZE; j++)
        {
            temp = distance(vect_x[i], vect_y[i], cp.x_c[j], cp.y_c[j]);
            if(dist > temp) // looking for the minimum distance given a point
            {
               cluster_class = j;
               dist = temp;
            }
        }
        
        // updating to the beloging cluster 
        c_vect[i] = cluster_class;
    }
}

double distance(double x1_point, double y1_point,double x2_point, double y2_point)
{
    return sqrt(pow(x1_point-x2_point, 2) + pow(y1_point-y2_point, 2));
    //return (abs(x1_point-x2_point)+abs(y1_point-y2_point));
}

bool updateCentroids(int vect_c[], double vect_x[], double vect_y[], centroid_point &cp)
{
    double update_x, update_y;
    int num_points, count = 0;

    for(int i = 0; i < CLUSTER_SIZE; i++)
    {
        update_x = update_y = num_points = 0;

        for(int j = 0; j < DATASET_SIZE; j++)
        {
            if(vect_c[j] == i)
            {
                update_x += vect_x[j];
                update_y += vect_y[j];
                num_points++; 
            }
        }    
        update_x = update_x/num_points;
        update_y = update_y/num_points;
        //counting unchange centroid
        double cond = distance(cp.x_c[i],cp.y_c[i],update_x,update_y);
        if( cond <= THRESHOLD )
            count++;

        // updating centroids
        if(num_points != 0 && cond > THRESHOLD)
        {
            cp.x_c[i] = update_x;
            cp.y_c[i] = update_y;
        }
    }
    if(count >= PERCENTAGE*CLUSTER_SIZE)
        return false;
    return true;
}

void saveCSV(double x[], double y[], int c[])
{
    std::ofstream myfile;

    myfile.open("final.csv");
    for (int i = 0; i < DATASET_SIZE; i++)
    {
        myfile << x[i] << "," << y[i] << "," << c[i] << "\n";

    }
}
void importData(double vect_x[], double vect_y[], int vect_c[], std::string dataset)
{
    std::ifstream myfile;
    
    std::string   line, x_s, y_s;
    myfile.open(dataset);

    for(int i = 0; i < DATASET_SIZE; i++)
    {
        // get line with new line delimiter
        getline(myfile, line,'\n');
        int  posC;
        // split the line in two part using the comma delimiter
        posC = line.find_first_of(',');
        x_s = line.substr(0, posC);
        y_s = line.substr(posC+1);
        //parsing from string to double and filling to the array of struct
        vect_x[i] = stod(x_s);
        vect_y[i] = stod(y_s);
        vect_c[i] = -1;

    }

    myfile.close();
}

void sumMeanCentroids(double vect_x[], double vect_y[],int vect_c[],centroid_point &cp)
{
   
    int countDataPoints[CLUSTER_SIZE];
    double cx_tmp[CLUSTER_SIZE];
    double cy_tmp[CLUSTER_SIZE];

    for(int i = 0; i < CLUSTER_SIZE; i++)
    {
        cp.x_c[i] = cp.y_c[i] = 0;
        cx_tmp[i] = cy_tmp[i] = 0;
        countDataPoints[i] = 0;
    }
     
    #pragma omp parallel for default(shared) reduction (+:countDataPoints[:CLUSTER_SIZE],cx_tmp[:CLUSTER_SIZE],cy_tmp[:CLUSTER_SIZE] )  num_threads(NTHREAD)
    for (int i = 0; i < DATASET_SIZE; i++) 
    {
        countDataPoints[ vect_c[i] ] += 1; 
        cx_tmp[vect_c[i]] += vect_x[i];
        cy_tmp[vect_c[i]] += vect_y[i];
    }
   
    for (int i = 0; i < CLUSTER_SIZE; i++)
    {
        cp.x_c[i] = cx_tmp[i]/countDataPoints[i];
        cp.y_c[i] = cy_tmp[i]/countDataPoints[i];
    }
       
}

bool updateMP(centroid_point &cp, centroid_point cp_new)
{
    double cond;
    int count = 0;
    for (int i = 0; i < CLUSTER_SIZE; i++)
    {
        cond = distance(cp.x_c[i],cp.y_c[i],cp_new.x_c[i],cp_new.y_c[i]);
        
        if( cond <= THRESHOLD )
        {
            count++;
        }   
        else
        {
            cp.x_c[i] = cp_new.x_c[i];
            cp.y_c[i] = cp_new.y_c[i];
        }
    }
     if(count >= PERCENTAGE*CLUSTER_SIZE)
        return false;
    
    return true;
}

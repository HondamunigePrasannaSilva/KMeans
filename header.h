#include<iostream>

#define DATASET_SIZE   1000000
#define CLUSTER_SIZE   16
#define THRESHOLD  1e-10
#define PERCENTAGE 1

#define NTHREAD 8


struct centroid_point
{
    double x_c[CLUSTER_SIZE];
    double y_c[CLUSTER_SIZE];
};


void randomCentroids(centroid_point &cp, double vect_x[], double vect_y[]);
bool updateCentroids(int vect_c[], double vect_x[], double vect_y[], centroid_point &cp);
void calculateDistance(double vect_x[], double vect_y[], centroid_point cp, int c_vect[]);
double distance(double x1_point, double y1_point,double x2_point, double y2_point);
void printClusterPoint(centroid_point cp);
void saveCSV(double x[], double y[], int c[]);
void importData(double vect_x[], double vect_y[],int vect_c[], std::string dataset);
void calculateDistanceS(double vect_x[], double vect_y[], centroid_point cp, int c_vect[]);

void sumMeanCentroids(double vect_x[], double vect_y[],int vect_c[],centroid_point &cp);

bool updateMP(centroid_point &cp, centroid_point cp_new);

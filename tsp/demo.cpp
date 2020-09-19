#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/time.h>
#include <time.h>
#include <math.h>

#include "tsp.h"

// CITY_NUM = 38;
CityPosition pos[CITY_NUM] = {
	{11003.611100,42102.500000}, {11108.611100,42373.888900},
    {11133.333300,42885.833300}, {11155.833300,42712.500000},
    {11183.333300,42933.333300}, {11297.500000,42853.333300},
    {11310.277800,42929.444400}, {11416.666700,42983.333300},
    {11423.888900,43000.277800}, {11438.333300,42057.222200},
    {11461.111100,43252.777800}, {11485.555600,43187.222200},
    {11503.055600,42855.277800}, {11511.388900,42106.388900},
    {11522.222200,42841.944400}, {11569.444400,43136.666700},
    {11583.333300,43150.000000}, {11595.000000,43148.055600},
    {11600.000000,43150.000000}, {11690.555600,42686.666700},
    {11715.833300,41836.111100}, {11751.111100,42814.444400},
    {11770.277800,42651.944400}, {11785.277800,42884.444400},
    {11822.777800,42673.611100}, {11846.944400,42660.555600},
    {11963.055600,43290.555600}, {11973.055600,43026.111100},
    {12058.333300,42195.555600}, {12149.444400,42477.500000},
    {12286.944400,43355.555600}, {12300.000000,42433.333300},
    {12355.833300,43156.388900}, {12363.333300,43189.166700},
    {12372.777800,42711.388900}, {12386.666700,43334.722200},
    {12421.666700,42895.555600}, {12645.000000,42973.333300}
};

/*
void mapGenerate(int n, CityPosition pos[]) {
    unsigned seed = (unsigned)time(0);
    FILE* fp = fopen("data.txt", "w");
    for (int i = 0; i < n; i++) {
        pos[i].x = 11000 + rand() / (double)(RAND_MAX/2000);
        pos[i].y = 42000 + rand() / (double)(RAND_MAX/2000);
        fprintf(fp, "{%.5lf,%.5lf}, ", pos[i].x, pos[i].y);
    }
    fclose(fp);
}
 */

int main(int argc, char* argv[]) {

    int MAX_EPOCH = strtol(argv[1], NULL, 10);
    TSP city;
    double pCross=0.5, pMutation=0.05;

    timeval start, finish;
    double duration;

    initColony(city, pos);

    gettimeofday(&start, NULL);
    for (int i = 0; i < MAX_EPOCH; i++) {
        select(city);
        cross(city, pCross);
        mutation(city, pMutation);
    }
    gettimeofday(&finish, NULL);
    duration = finish.tv_sec - start.tv_sec + 
                (finish.tv_usec - start.tv_usec) / 1000000.0;
    printf("Use time: %lf\n", duration);
    outputRes(city);
    return 0;
}
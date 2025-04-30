#pragma once

#include <cmath>

// DFM10H - dead fuel moisture
double EMC(double temp, double rh);
double EMC_corr(double temp, double rh, double rain, double snowCover);
double EMC_corr_hourly(double tempH, double rhH, double rainH, double snowCover);
double EMC_24(double dayLength, double emcMax, double emcMin);
double EMC_24(double emcMax, double emcMin);
double EMC_24(double* hourEmc);
int Pdur_24(double rain);
int Pdur_24(double* hourRain);
double D24_100H(double emc24, int pDur24);
double D24_1000H(double emc24, int pDur24);
double D24_1000H_7Davg(double* d24_1000H);
double DFM1H(double emc, double rain, double snowCover);
double DFM1H_hourly(double dfm1hPrev, double emcH, double rainH, double snowCover);
double DFM10H(double emc, double rain, double snowCover);
double DFM10H_hourly(double dfm10hPrev, double emcH, double rainH, double snowCover);
double DFM100H(double dfm100hPrev, double d24);
double DFM100H_hourly(double dfm100hPrev, double d24H);
double DFM100H_hourly(double dfm100hPrev15h, double d24H, int hoursFromPrev15h);
double HourCoef100H(int hoursFromPrev15h);
double DFM100H_hourly(double dfm100hPrev15h, double d24H, double hourCoef);
double DFM1000H(double dfm1000hPrev7, double d24Avg7);
double DFM1000H_simple(double dfm1000hPrev, double d24);
double DFM1000H_hourly(double dfm1000hPrev7, double d24Avg7H);
double DFM1000H_simple_hourly(double dfm1000hPrev, double d24H);
double DFM1000H_simple_hourly(double dfm1000hPrev15h, double d24H, int hoursFromPrev15h);
double HourCoef1000H(int hoursFromPrev15h);
double DFM1000H_simple_hourly(double dfm1000hPrev15h, double d24H, double hourCoef);
char DFM_Cat(double dfm);

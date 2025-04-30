#pragma once

#include <cmath>

const double MRT_ALPHA = 0.85;
const double MRT_ALBEDO = 0.23;

// UTCI (Universal Thermal Climate Index)
double MRT_FP(int year, int jd, double latitude);
double MRT(double temp, double srad, double mrt_fp, double alpha = MRT_ALPHA, double albedo = MRT_ALBEDO);
double MRT(int year, int jd, double latitude, double temp, double srad, double alpha = MRT_ALPHA, double albedo = MRT_ALBEDO);
double UTCI(double temp, double mrt, double wind, double vapo);
char UTCI_cat(double utci);

#pragma once

#include <cmath>

// FWI
const double DMC_DayLength46N[13] = { 0.0,  6.5,  7.5,  9.0, 12.8, 13.9, 13.9, 12.4, 10.9,  9.4,  8.0,  7.0,  6.0 };
const double DMC_DayLength20N[13] = { 0.0,  7.9,  8.4,  8.9,  9.5,  9.9, 10.2, 10.1,  9.7,  9.1,  8.6,  8.1,  7.8 };
const double DMC_DayLength20S[13] = { 0.0, 10.1,  9.6,  9.1,  8.5,  8.1,  7.8,  7.9,  8.3,  8.9,  9.4,  9.9, 10.2 };
const double DMC_DayLength40S[13] = { 0.0, 11.5, 10.5,  9.2,  7.9,  6.8,  6.2,  6.5,  7.4,  8.7, 10.0, 11.2, 11.8 };
const double DC_LfN[13] = { 0.0, -1.6, -1.6, -1.6,  0.9,  3.8,  5.8,  6.4,  5.0,  2.4,  0.4, -1.6, -1.6 };
const double DC_LfS[13] = { 0.0,  6.4,  5.0,  2.4,  0.4, -1.6, -1.6, -1.6, -1.6, -1.6,  0.9,  3.8,  5.8 };
const double FWI_BiomThr[19][5] = {
	{ 5.2, 11.2, 21.3, 38.0, 50.0 }, // 0: global (EFFIS)
	{ 5.2, 11.2, 21.3, 38.0, 50.0 }, // 1: Arctic/Alpine (Arctic 1)
	{ 5.2, 11.2, 21.3, 38.0, 50.0 }, // 2: Arctic/Alpine (Arctic 2)
	{ 5.2, 11.2, 21.3, 38.0, 50.0 }, // 3: Arctic/Alpine (Extremely cold and wet 1)
	{ 1.2,  5.1, 10.9, 16.8, 22.5 }, // 4: Arctic/Alpine (Extremely cold and wet 2)
	{ 2.0,  7.6, 15.8, 24.0, 31.0 }, // 5: Boreal/Alpine (Cold and wet)
	{ 1.8,  6.6, 14.1, 24.1, 36.7 }, // 6: Boreal/Alpine (Extremely cold and mesic)
	{ 2.5,  9.0, 18.4, 31.4, 46.9 }, // 7: Boreal/Alpine (Cold and mesic)
	{ 2.7,  9.9, 20.7, 35.1, 51.3 }, // 8: Cool temperate (Cool temperate and dry)
	{ 7.4, 20.7, 33.8, 48.6, 62.0 }, // 9: Cool temperate (Cool temperate and xeric)
	{ 4.8, 13.7, 23.3, 36.7, 52.4 }, // 10: Cool temperate (Cool temperate and moist)
	{ 6.8, 18.1, 28.0, 40.8, 55.1 }, // 11: Warm temperate (Warm temperate and mesic)
	{ 7.8, 20.9, 32.2, 46.1, 62.0 }, // 12: Warm temperate (Warm temperate and xeric)
	{ 1.6,  5.4, 11.4, 21.8, 37.9 }, // 13: Sub-tropical (Hot and mesic)
	{ 2.9, 10.3, 20.4, 35.2, 55.0 }, // 14: Drylands (Hot and dry)
	{ 5.2, 11.2, 21.3, 38.0, 50.0 }, // 15: Drylands (Hot and arid)
	{ 5.2, 11.2, 21.3, 38.0, 50.0 }, // 16: Drylands (Extremely hot and arid)
	{ 1.7,  5.9, 12.6, 30.9, 62.0 }, // 17: Drylands (Extremely hot and xeric)
	{ 2.0,  6.6, 12.9, 23.6, 39.8 } // 18: Tropical (Extremely hot and moist)
};
double FFMC(double temp, double rh, double wind, double rain, double ffmcPrev);
double FFMC_Hourly(double tempH, double rhH, double windH, double rainH, double ffmcPrev);
double DMC(double temp, double rh, double rain, double dmcPrev, double latDeg, int month);
double DC(double temp, double rain, double dcPrev, double latDeg, int month);
double ISI(double ffmc, double wind);
double BUI(double dmc, double dc);
double FWI(double isi, double bui);
char FWI_catOrig(double fwi);
char FWI_catEffis(double fwi);
char FWI_catBioms(double fwi, int biom);

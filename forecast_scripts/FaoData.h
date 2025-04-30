#pragma once

#include "InputData.h"
#include "EtoData.h"
#include "SnowData.h"

const double CN_AWC_II = 65.0; 
const double LAYER_DEPTH[4] = { 0.1, 0.3, 0.6, 1.0 };
const double LAYER_DEPTH_MAX[4] = { 0.1, 0.4, 1.0, 2.0 };
const double LAYER_IC[4] = { 0.005, 0.005, 0.005, 0.002 };
const double LAYER_P[4] = { 0.4, 0.4, 0.4, 0.4 };
const double ROOT_COEF_1_5[4] = { 0.2, 0.4, 0.3, 0.1 };
const double SOIL_WP = 0.1;
const double LAI_COEF_K = 0.7;
const double KC_MIN = 0.15;
const double KC_MAX = 1.2;
const double CROP_P = 0.5;

class FaoData
{
private:
	InputData* iData;
	EtoData* eData;
	SnowData* sData;

	double slope;

	double tew, rew;
	double taw[4], raw[4], ksDiv[4];
	double cropHeight, rootDepth;
	double runoff;
	double Kcb, Ke;
	double rootCoefs[4];
	bool isBareGround;

	void ComputeRunOff();
	void ComputeDualCropCoefficients();
	void ComputeRootCoefs();
	void ComputeRootCoefs2(); // 40-30-20-10%
public:
	// simulacni data
	double Lai;
	double Dr[4]; // soil layer deficit
	// vystupy
	double Eta, EtRatio;
	double Awr[4]; // AWR (0-10cm, 0-40cm, 0-100cm, 0-200cm)

	FaoData(InputData* i, EtoData* e, SnowData* s);
	~FaoData();
	void SetLocation(float slope, float* awc);
	void SetSimulationData(int crop, float Lai, float cropH, float rootD, float* DrPrev);
	void Compute();
};


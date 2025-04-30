#include "utci.h"

double MRT_FP(int year, int jd, double latitude)
/* Projected area factor [-]
	year - year
	jd - day of year
	latitude - latitude [rad]
*/
{
	double solDec, gamma;

	daysInYear = ((year % 4 == 0) && ((year % 100 != 0) || (year % 400 == 0))) ? 366 : 365;
	solDec = 0.409 * sin((2.0 * M_PI * jd / daysInYear) - 1.39);
	gamma = acos(fmax(-1.0, fmin(1.0, cos(solDec) * cos(latitude) + sin(solDec) * sin(latitude))));
	return 0.308 * cos(gamma * (1 - 0.067823774 * pow(gamma, 2)));
}

double MRT(double temp, double srad, double mrt_fp, double alpha, double albedo)
/* Mean radiant temperature [°C]
	temp - air temperature [°C]
	srad - sum of global radiation [MJ/m2]
	mrt_fp - Projected area factor [-]
	alpha - absorptance of the outer surface of the person [-]
	albedo - albedo (for grass =0.23) [-]
*/
{
	if (srad < 0.01)
		return temp;

	double tk, tmrt;

	tk = Celsius2Kelvin(temp);
	tmrt = pow(pow(tk, 4) + 210000000.0 * mrt_fp * alpha * ((1 + albedo) * srad), 0.25);

	return Kelvin2Celsius(tmrt);
}

double MRT(int year, int jd, double latitude, double temp, double srad, double alpha, double albedo)
/* Mean radiant temperature [°C]
	year - year
	jd - day of year
	latitude - latitude [rad]
	temp - air temperature [°C]
	srad - sum of global radiation [MJ/m2]
	alpha - absorptance of the outer surface of the person [-]
	albedo - albedo (for grass =0.23) [-]
*/
{
	if (srad < 0.01)
		return temp;

	return MRT(temp, srad, MRT_FP(year, jd, latitude), alpha, albedo);
}

double UTCI(double temp, double mrt, double wind, double vapo)
/* Universal Thermal Climate Index [°C]
	temp - air temperature [°C]
	mrt - mean radiant temperature [°C]
	wind - wind speed (10 m) [m/s]
	vapo - water vapour presure [kPa]
*/
{
	double Ta, D_Tmrt, va, Pa, UTCI_approx;
	double Ta2, Ta3, Ta4, Ta5, Ta6, va2, va3, va4, va5, va6, D_Tmrt2, D_Tmrt3, D_Tmrt4, D_Tmrt5, D_Tmrt6, Pa2, Pa3, Pa4, Pa5, Pa6;

	Ta = fmax(-50.0, fmin(50.0, temp));
	D_Tmrt = fmax(-30.0, fmin(70.0, mrt - Ta));
	va = fmax(0.5, fmin(17.0, wind));
	Pa = fmax(0.0, fmin(50.0, vapo));

	Ta2 = Ta * Ta;
	Ta3 = Ta2 * Ta;
	Ta4 = Ta3 * Ta;
	Ta5 = Ta4 * Ta;
	Ta6 = Ta5 * Ta;
	va2 = va * va;
	va3 = va2 * va;
	va4 = va3 * va;
	va5 = va4 * va;
	va6 = va5 * va;
	D_Tmrt2 = D_Tmrt * D_Tmrt;
	D_Tmrt3 = D_Tmrt2 * D_Tmrt;
	D_Tmrt4 = D_Tmrt3 * D_Tmrt;
	D_Tmrt5 = D_Tmrt4 * D_Tmrt;
	D_Tmrt6 = D_Tmrt5 * D_Tmrt;
	Pa2 = Pa * Pa;
	Pa3 = Pa2 * Pa;
	Pa4 = Pa3 * Pa;
	Pa5 = Pa4 * Pa;
	Pa6 = Pa5 * Pa;

	UTCI_approx = Ta + 0.607562052 
		- 2.27712343e-2 * Ta + 8.06470249e-4 * Ta2 - 1.54271372e-4 * Ta3 - 3.24651735e-6 * Ta4 + 7.32602852e-8 * Ta5 + 1.35959073e-9 * Ta6
		- 2.25836520 * va + 8.80326035e-2 * Ta * va + 2.16844454e-3 * Ta2 * va - 1.53347087e-5 * Ta3 * va - 5.72983704e-7 * Ta4 * va - 2.55090145e-9 * Ta5 * va
		- 0.751269505 * va2 - 4.08350271e-3 * Ta * va2 - 5.21670675e-5 * Ta2 * va2 + 1.94544667e-6 * Ta3 * va2 + 1.14099531e-8 * Ta4 * va2 
		+ 0.158137256 * va3 - 6.57263143e-5 * Ta * va3 + 2.22697524e-7 * Ta2 * va3 - 4.16117031e-8 * Ta3 * va3 - 1.27762753e-2 * va4 
		+ 9.66891875e-6 * Ta * va4 + 2.52785852e-9 * Ta2 * va4 + 4.56306672e-4 * va5 - 1.74202546e-7 * Ta * va5 - 5.91491269e-6 * va6 
		+ 0.398374029 * D_Tmrt + 1.83945314e-4 * Ta * D_Tmrt - 1.73754510e-4 * Ta2 * D_Tmrt - 7.60781159e-7 * Ta3 * D_Tmrt + 3.77830287e-8 * Ta4 * D_Tmrt 
		+ 5.43079673e-10 * Ta5 * D_Tmrt - 2.00518269e-2 * va * D_Tmrt + 8.92859837e-4 * Ta * va * D_Tmrt + 3.45433048e-6 * Ta2 * va * D_Tmrt 
		- 3.77925774e-7 * Ta3 * va * D_Tmrt - 1.69699377e-9 * Ta4 * va * D_Tmrt + 1.69992415e-4 * va2 * D_Tmrt - 4.99204314e-5 * Ta * va2 * D_Tmrt 
		+ 2.47417178e-7 * Ta2 * va2 * D_Tmrt + 1.07596466e-8 * Ta3 * va2 * D_Tmrt + 8.49242932e-5 * va3 * D_Tmrt + 1.35191328e-6 * Ta * va3 * D_Tmrt 
		- 6.21531254e-9 * Ta2 * va3 * D_Tmrt - 4.99410301e-6 * va4 * D_Tmrt - 1.89489258e-8 * Ta * va4 * D_Tmrt + 8.15300114e-8 * va5 * D_Tmrt 
		+ 7.55043090e-4 * D_Tmrt2 - 5.65095215e-5 * Ta * D_Tmrt2 - 4.52166564e-7 * Ta2 * D_Tmrt2 + 2.46688878e-8 * Ta3 * D_Tmrt2 + 2.42674348e-10 * Ta4 * D_Tmrt2 
		+ 1.54547250e-4 * va * D_Tmrt2 + 5.24110970e-6 * Ta * va * D_Tmrt2 - 8.75874982e-8 * Ta2 * va * D_Tmrt2 - 1.50743064e-9 * Ta3 * va * D_Tmrt2 
		- 1.56236307e-5 * va2 * D_Tmrt2 - 1.33895614e-7 * Ta * va2 * D_Tmrt2 + 2.49709824e-9 * Ta2 * va2 * D_Tmrt2 + 6.51711721e-7 * va3 * D_Tmrt2 
		+ 1.94960053e-9 * Ta * va3 * D_Tmrt2 - 1.00361113e-8 * va4 * D_Tmrt2 - 1.21206673e-5 * D_Tmrt3 - 2.18203660e-7 * Ta * D_Tmrt3 + 7.51269482e-9 * Ta2 * D_Tmrt3 
		+ 9.79063848e-11 * Ta3 * D_Tmrt3 + 1.25006734e-6 * va * D_Tmrt3 - 1.81584736e-9 * Ta * va * D_Tmrt3 - 3.52197671e-10 * Ta2 * va * D_Tmrt3 
		- 3.36514630e-8 * va2 * D_Tmrt3 + 1.35908359e-10 * Ta * va2 * D_Tmrt3 + 4.17032620e-10 * va3 * D_Tmrt3 - 1.30369025e-9 * D_Tmrt4 
		+ 4.13908461e-10 * Ta * D_Tmrt4 + 9.22652254e-12 * Ta2 * D_Tmrt4 - 5.08220384e-9 * va * D_Tmrt4 - 2.24730961e-11 * Ta * va * D_Tmrt4 
		+ 1.17139133e-10 * va2 * D_Tmrt4 + 6.62154879e-10 * D_Tmrt5 + 4.03863260e-13 * Ta * D_Tmrt5 + 1.95087203e-12 * va * D_Tmrt5 - 4.73602469e-12 * D_Tmrt6 
		+ 5.12733497 * Pa - 0.312788561 * Ta * Pa - 1.96701861e-2 * Ta2 * Pa + 9.99690870e-4 * Ta3 * Pa + 9.51738512e-6 * Ta4 * Pa - 4.66426341e-7 * Ta5 * Pa 
		+ 0.548050612 * va * Pa - 3.30552823e-3 * Ta * va * Pa - 1.64119440e-3 * Ta2 * va * Pa - 5.16670694e-6 * Ta3 * va * Pa + 9.52692432e-7 * Ta4 * va * Pa
		- 4.29223622e-2 * va2 * Pa + 5.00845667e-3 * Ta * va2 * Pa + 1.00601257e-6 * Ta2 * va2 * Pa - 1.81748644e-6 * Ta3 * va2 * Pa - 1.25813502e-3 * va3 * Pa
		- 1.79330391e-4 * Ta * va3 * Pa + 2.34994441e-6 * Ta2 * va3 * Pa + 1.29735808e-4 * va4 * Pa + 1.29064870e-6 * Ta * va4 * Pa - 2.28558686e-6 * va5 * Pa
		- 3.69476348e-2 * D_Tmrt * Pa + 1.62325322e-3 * Ta * D_Tmrt * Pa - 3.14279680e-5 * Ta2 * D_Tmrt * Pa + 2.59835559e-6 * Ta3 * D_Tmrt * Pa 
		- 4.77136523e-8 * Ta4 * D_Tmrt * Pa + 8.64203390e-3 * va * D_Tmrt * Pa - 6.87405181e-4 * Ta * va * D_Tmrt * Pa - 9.13863872e-6 * Ta2 * va * D_Tmrt * Pa
		+ 5.15916806e-7 * Ta3 * va * D_Tmrt * Pa - 3.59217476e-5 * va2 * D_Tmrt * Pa + 3.28696511e-5 * Ta * va2 * D_Tmrt * Pa - 7.10542454e-7 * Ta2 * va2 * D_Tmrt * Pa 
		- 1.24382300e-5 * va3 * D_Tmrt * Pa - 7.38584400e-9 * Ta * va3 * D_Tmrt * Pa + 2.20609296e-7 * va4 * D_Tmrt * Pa - 7.32469180e-4 * D_Tmrt2 * Pa
		- 1.87381964e-5 * Ta * D_Tmrt2 * Pa + 4.80925239e-6 * Ta2 * D_Tmrt2 * Pa - 8.75492040e-8 * Ta3 * D_Tmrt2 * Pa + 2.77862930e-5 * va * D_Tmrt2 * Pa
		- 5.06004592e-6 * Ta * va * D_Tmrt2 * Pa + 1.14325367e-7 * Ta2 * va * D_Tmrt2 * Pa + 2.53016723e-6 * va2 * D_Tmrt2 * Pa - 1.72857035e-8 * Ta * va2 * D_Tmrt2 * Pa
		- 3.95079398e-8 * va3 * D_Tmrt2 * Pa - 3.59413173e-7 * D_Tmrt3 * Pa + 7.04388046e-7 * Ta * D_Tmrt3 * Pa - 1.89309167e-8 * Ta2 * D_Tmrt3 * Pa
		- 4.79768731e-7 * va * D_Tmrt3 * Pa + 7.96079978e-9 * Ta * va * D_Tmrt3 * Pa + 1.62897058e-9 * va2 * D_Tmrt3 * Pa + 3.94367674e-8 * D_Tmrt4 * Pa
		- 1.18566247e-9 * Ta * D_Tmrt4 * Pa + 3.34678041e-10 * va * D_Tmrt4 * Pa - 1.15606447e-10 * D_Tmrt5 * Pa - 2.80626406 * Pa2 + 0.548712484 * Ta * Pa2
		- 3.99428410e-3 * Ta2 * Pa2 - 9.54009191e-4 * Ta3 * Pa2 + 1.93090978e-5 * Ta4 * Pa2 - 0.308806365 * va * Pa2 + 1.16952364e-2 * Ta * va * Pa2 
		+ 4.95271903e-4 * Ta2 * va * Pa2 - 1.90710882e-5 * Ta3 * va * Pa2 + 2.10787756e-3 * va2 * Pa2 - 6.98445738e-4 * Ta * va2 * Pa2 + 2.30109073e-5 * Ta2 * va2 * Pa2 
		+ 4.17856590e-4 * va3 * Pa2 - 1.27043871e-5 * Ta * va3 * Pa2 - 3.04620472e-6 * va4 * Pa2 + 5.14507424e-2 * D_Tmrt * Pa2 - 4.32510997e-3 * Ta * D_Tmrt * Pa2
		+ 8.99281156e-5 * Ta2 * D_Tmrt * Pa2 - 7.14663943e-7 * Ta3 * D_Tmrt * Pa2 - 2.66016305e-4 * va * D_Tmrt * Pa2 + 2.63789586e-4 * Ta * va * D_Tmrt * Pa2
		- 7.01199003e-6 * Ta2 * va * D_Tmrt * Pa2 - 1.06823306e-4 * va2 * D_Tmrt * Pa2 + 3.61341136e-6 * Ta * va2 * D_Tmrt * Pa2 + 2.29748967e-7 * va3 * D_Tmrt * Pa2
		+ 3.04788893e-4 * D_Tmrt2 * Pa2 - 6.42070836e-5 * Ta * D_Tmrt2 * Pa2 + 1.16257971e-6 * Ta2 * D_Tmrt2 * Pa2 + 7.68023384e-6 * va * D_Tmrt2 * Pa2
		- 5.47446896e-7 * Ta * va * D_Tmrt2 * Pa2 - 3.59937910e-8 * va2 * D_Tmrt2 * Pa2 - 4.36497725e-6 * D_Tmrt3 * Pa2 + 1.68737969e-7 * Ta * D_Tmrt3 * Pa2
		+ 2.67489271e-8 * va * D_Tmrt3 * Pa2 + 3.23926897e-9 * D_Tmrt4 * Pa2 - 3.53874123e-2 * Pa3 - 0.221201190 * Ta * Pa3 + 1.55126038e-2 * Ta2 * Pa3
		- 2.63917279e-4 * Ta3 * Pa3 + 4.53433455e-2 * va * Pa3 - 4.32943862e-3 * Ta * va * Pa3 + 1.45389826e-4 * Ta2 * va * Pa3 + 2.17508610e-4 * va2 * Pa3
		- 6.66724702e-5 * Ta * va2 * Pa3 + 3.33217140e-5 * va3 * Pa3 - 2.26921615e-3 * D_Tmrt * Pa3 + 3.80261982e-4 * Ta * D_Tmrt * Pa3 - 5.45314314e-9 * Ta2 * D_Tmrt * Pa3
		- 7.96355448e-4 * va * D_Tmrt * Pa3 + 2.53458034e-5 * Ta * va * D_Tmrt * Pa3 - 6.31223658e-6 * va2 * D_Tmrt * Pa3 + 3.02122035e-4 * D_Tmrt2 * Pa3 
		- 4.77403547e-6 * Ta * D_Tmrt2 * Pa3 + 1.73825715e-6 * va * D_Tmrt2 * Pa3 - 4.09087898e-7 * D_Tmrt3 * Pa3 + 0.614155345 * Pa4 - 6.16755931e-2 * Ta * Pa4
		+ 1.33374846e-3 * Ta2 * Pa4 + 3.55375387e-3 * va * Pa4 - 5.13027851e-4 * Ta * va * Pa4 + 1.02449757e-4 * va2 * Pa4 - 1.48526421e-3 * D_Tmrt * Pa4
		- 4.11469183e-5 * Ta * D_Tmrt * Pa4 - 6.80434415e-6 * va * D_Tmrt * Pa4 - 9.77675906e-6 * D_Tmrt2 * Pa4 + 8.82773108e-2 * Pa5 - 3.01859306e-3 * Ta * Pa5
		+ 1.04452989e-3 * va * Pa5 + 2.47090539e-4 * D_Tmrt * Pa5 + 1.48348065e-3 * Pa6;
	return UTCI_approx;
}

char UTCI_cat(double utci)
/* UTCI classification
	1 - extreme cold stress
	2 - very strong cold stress
	3 - strong cold stress
	4 - moderate cold stress
	5 - slight cold stress
	6 - no thermal stress
	7 - moderate heat stress
	8 - strong heat stress
	9 - very strong heat stress
	10 - extreme heat stress

	utci - Universal Thermal Climate Index [°C]
*/
{
	if (utci < -40.0)
		return 1;
	else if (utci < -27.0)
		return 2;
	else if (utci < -13.0)
		return 3;
	else if (utci < 0.0)
		return 4;
	else if (utci < 9.0)
		return 5;
	else if (utci < 26.0)
		return 6;
	else if (utci < 32.0)
		return 7;
	else if (utci < 38.0)
		return 8;
	else if (utci < 46.0)
		return 9;
	else
		return 10;
}

#include "fwi.h"

double FFMC(double temp, double rh, double wind, double rain, double ffmcPrev)
/* Fine Fuel Moisture Code
	temp - air temperature at 12 h [°C]
	rh - air relative humidity at 12 h [%]
	wind - wind speed (10 m) at 12 h [m/s]
	rain - sum of precipitation at 12 h during previous 24 hours [mm]
	ffmcPrev - FFMC of previous day
*/
{
	double f, mo, mr, m, rf, ed, ew, ko, kd, kw;

	f = 3.6 * wind; // m/s -> km/h
	mo = 147.2 * (101.0 - ffmcPrev) / (59.5 + ffmcPrev);
	if (rain >= 0.50001)
	{
		rf = rain - 0.5;
		if (mo <= 150.0)
			mr = mo + 42.5 * rf * exp(-100.0 / (251.0 - mo)) * (1.0 - exp(-6.93 / rf));
		else
			mr = mo + 42.5 * rf * exp(-100.0 / (251.0 - mo)) * (1.0 - exp(-6.93 / rf)) + 0.0015 * pow(mo - 150.0, 2) * sqrt(rf);
		mo = fmin(250.0, mr);
	}
	ed = 0.942 * pow(rh, 0.679) + 11.0 * exp((rh - 100.0) / 10.0) + 0.18 * (21.1 - temp) * (1.0 - exp(-0.115 * rh));
	if (mo > ed)
	{
		ko = 0.424 * (1.0 - pow(rh / 100.0, 1.7)) + 0.0694 * sqrt(f) * (1.0 - pow(rh / 100.0, 8));
		kd = ko * 0.581 * exp(0.0365 * temp);
		m = ed + (mo - ed) * pow(10.0, -kd);
	}
	else
	{
		ew = 0.618 * pow(rh, 0.753) + 10.0 * exp((rh - 100.0) / 10.0) + 0.18 * (21.1 - temp) * (1.0 - exp(-0.115 * rh));
		if (mo < ew)
		{
			ko = 0.424 * (1.0 - pow((100.0 - rh) / 100.0, 1.7)) + 0.0694 * sqrt(f) * (1.0 - pow((100.0 - rh) / 100.0, 8));
			kw = ko * 0.581 * exp(0.0365 * temp);
			m = ew - (ew - mo) * pow(10.0, -kw);
		}
		else
			m = mo;
	}
	return fmax(0.0, fmin(101.0, 59.5 * (250.0 - m) / (147.2 + m)));
}

double FFMC_Hourly(double tempH, double rhH, double windH, double rainH, double ffmcPrev)
/* Fine Fuel Moisture Code
	tempH - hour air temperature [°C]
	rhH - hour air relative humidity [%]
	windH - hour wind speed (10 m) [m/s]
	rainH - hour sum of precipitation [mm]
	ffmcPrev - FFMC of previous hour
*/
{
	double f, mo, mr, m, ed, ew, ko, kd, kw;

	f = 3.6 * windH; // m/s -> km/h
	mo = 205.2 * (101.0 - ffmcPrev) / (82.9 + ffmcPrev);
	if (rainH >= 0.00001)
	{
		if (mo <= 150.0)
			mr = mo + 42.5 * rainH * exp(-100.0 / (251.0 - mo)) * (1.0 - exp(-6.93 / rainH));
		else
			mr = mo + 42.5 * rainH * exp(-100.0 / (251.0 - mo)) * (1.0 - exp(-6.93 / rainH)) + 0.0015 * pow(mo - 150.0, 2) * sqrt(rainH);
		mo = fmin(250.0, mr);
	}
	ed = 0.942 * pow(rhH, 0.679) + 11.0 * exp((rhH - 100.0) / 10.0) + 0.18 * (21.1 - tempH) * (1.0 - exp(-0.115 * rhH));
	if (mo > ed)
	{
		ko = 0.424 * (1.0 - pow(rhH / 100.0, 1.7)) + 0.0694 * sqrt(f) * (1.0 - pow(rhH / 100.0, 8));
		kd = ko * 0.0579 * exp(0.0365 * tempH);
		m = ed + (mo - ed) * exp(-2.303 * kd);
	}
	else
	{
		ew = 0.618 * pow(rhH, 0.753) + 10.0 * exp((rhH - 100.0) / 10.0) + 0.18 * (21.1 - tempH) * (1.0 - exp(-0.115 * rhH));
		if (mo < ew)
		{
			ko = 0.424 * (1.0 - pow((100.0 - rhH) / 100.0, 1.7)) + 0.0694 * sqrt(f) * (1.0 - pow((100.0 - rhH) / 100.0, 8));
			kw = ko * 0.0579 * exp(0.0365 * tempH);
			m = ew - (ew - mo) * exp(-2.303 * kw);
		}
		else
			m = mo;
	}
	return fmax(0.0, fmin(101.0, 82.9 * (250.0 - m) / (205.2 + m)));
}

double DMC(double temp, double rh, double rain, double dmcPrev, double latDeg, int month)
/* Duff Moisture Code
	temp - air temperature at 12 h [°C]
	rh - air relative humidity at 12 h [%]
	rain - sum of precipitation at 12 h during previous 24 hours [mm]
	dmcPrev - DMC of previous day
	latDeg - latitude [°]
	month - month
*/
{
	double dl, k, pr, re, mo, mr, b;

	if (temp > -1.1)
	{
		if (latDeg > 30.0)
			dl = DMC_DayLength46N[month];
		else if (latDeg > 10.0)
			dl = DMC_DayLength20N[month];
		else if (latDeg >= -10.0)
			dl = 9;
		else if (latDeg >= -30.0)
			dl = DMC_DayLength20S[month];
		else
			dl = DMC_DayLength40S[month];
		k = 0.0001894 * (temp + 1.1) * (100.0 - rh) * dl;
	}
	else
		k = 0.0;
	if (rain <= 1.5)
		pr = dmcPrev;
	else
	{
		re = 0.92 * rain - 1.27;
		mo = 20.0 + exp(5.6348 - dmcPrev / 43.43); // totez: mo := 20 + 280 / Exp(0.023 * dmcPrev);
		if (dmcPrev <= 33.0)
			b = 100.0 / (0.5 + 0.3 * dmcPrev);
		else if (dmcPrev <= 65.0)
			b = 14.0 - 1.3 * log(dmcPrev);
		else
			b = 6.2 * log(dmcPrev) - 17.2;
		mr = mo + 1000.0 * re / (48.77 + b * re);
		pr = fmax(0.0, 43.43 * (5.6348 - log(mr - 20.0)));
	}
	return fmax(0.0, pr + k);
}

double DC(double temp, double rain, double dcPrev, double latDeg, int month)
/* Drought Code
	temp - air temperature at 12 h [°C]
	rain - sum of precipitation at 12 h during previous 24 hours [mm]
	dcPrev - DC of previous day
	latDeg - latitude [°]
	month - month
*/
{
	double rd, qo, qr, dr, dco, v, lf;

	dco = dcPrev;
	if (rain > 2.8)
	{
		rd = 0.83 * rain - 1.27;
		qo = 800.0 * exp(-dco / 400.0);
		qr = qo + 3.937 * rd;
		dr = 400.0 * log(800.0 / qr);
		dco = fmax(0.0, dr);
	}
	if (latDeg > 20.0)
		lf = DC_LfN[month];
	else if (latDeg < -20.0)
		lf = DC_LfS[month];
	else
		lf = 1.4;
	if (temp > -2.8)
		v = 0.36 * (temp + 2.8) + lf;
	else
		v = lf;
	return dco + 0.5 * fmax(0.0, v);
}

double ISI(double ffmc, double wind)
/* Initial Spread Index
	ffmc - fine fuel moisture code
	wind - wind speed (10 m) [m/s]
*/
{
	double m, fw, ff;

	m = 147.2 * (101.0 - ffmc) / (59.5 + ffmc);
	fw = exp(0.181404 * wind);
	ff = 91.9 * exp(-0.1386 * m) * (1.0 + pow(m, 5.31) / 49300000.0);
	return 0.208 * fw * ff;
}

double BUI(double dmc, double dc)
/* BuildUp Index
	dmc - duff moisture code
	dc - drought code
*/
{
	double d, bui;

	d = dmc + 0.4 * dc;
	if (d < 0.00001)
		bui = 0.0;
	else if (dmc <= (0.4 * dc))
		bui = 0.8 * dmc * dc / d;
	else
		bui = dmc - (1.0 - 0.8 * dc / d) * (0.92 + pow(0.0114 * dmc, 1.7));
	return fmax(0.0, bui);
}

double FWI(double isi, double bui)
/* Fire Weather Index
	isi - initial spread index
	bui - buildup index
*/
{
	double fd, b;

	if (bui <= 80.0)
		fd = 0.626 * pow(bui, 0.809) + 2.0;
	else
		fd = 1000.0 / (25.0 + 108.64 * exp(-0.023 * bui));
	b = 0.1 * isi * fd;
	if (b > 1.0)
		return exp(2.72 * pow(0.434 * log(b), 0.647));
	else
		return b;
}

char FWI_catOrig(double fwi)
{
	if (fwi < 2.0)
		return 1;
	else if (fwi < 5.0)
		return 2;
	else if (fwi < 9.0)
		return 3;
	else if (fwi < 17.0)
		return 4;
	else if (fwi < 30.0)
		return 5;
	else
		return 6;
}

char FWI_catEffis(double fwi)
{
	if (fwi < 5.2)
		return 1;
	else if (fwi < 11.2)
		return 2;
	else if (fwi < 21.3)
		return 3;
	else if (fwi < 38.0)
		return 4;
	else if (fwi < 50.0)
		return 5;
	else
		return 6;
}

char FWI_catBioms(double fwi, int biom)
{
	if (biom > 18)
		biom = 0;
	if (fwi < FWI_BiomThr[biom][0])
		return 1;
	else if (fwi < FWI_BiomThr[biom][1])
		return 2;
	else if (fwi < FWI_BiomThr[biom][2])
		return 3;
	else if (fwi < FWI_BiomThr[biom][3])
		return 4;
	else if (fwi < FWI_BiomThr[biom][4])
		return 5;
	else
		return 6;
}

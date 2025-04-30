# Clim4Cast Forecast Generation Toolkit

This repository provides C++ scripts used in the Clim4Cast project for the operational processing of multi-temporal forecasts of heatwaves and thermal comfort using the Universal Thermal Climate Index (UTCI). This implementation is part of Deliverable D2.2.1 and is intended for integration into regional and national DHF (Drought-Heat-Fire) monitoring and forecasting systems.

## Contents

- `utci.cpp` / `utci.h`: Calculation of MRT (Mean Radiant Temperature) and UTCI (Universal Thermal Climate Index)
- `hi.cpp` / `hi.h`: Computation of Heat Index
- `fwi.cpp` / `fwi.h`: Fire Weather Index computation
- `dfm.h`, `FaoData.h`: Supporting datasets and parameter structures

## Installation & Compilation

You will need a C++ compiler (e.g., `g++` or `clang++`). All scripts can be compiled into a single executable for custom use. Example compilation command:

```bash
g++ -O2 -std=c++11 utci.cpp hi.cpp fwi.cpp -o clim4cast_forecast
```

## How to Use

### 1. UTCI Calculation

**Function:**
```cpp
double UTCI(double temp, double mrt, double wind, double vapo);
```

**Inputs:**
- `temp`: Air temperature [°C]
- `mrt`: Mean Radiant Temperature [°C]
- `wind`: Wind speed at 10 m [m/s]
- `vapo`: Water vapor pressure [kPa]

**Helper functions:**
- `MRT(...)`: Computes Mean Radiant Temperature based on date, radiation, and surface parameters
- `UTCI_cat(...)`: Classifies UTCI values into thermal stress categories

### 2. Heat Index

**Function:**
```cpp
double HeatIndex(double temp, double rh);
```

Where:
- `temp`: Air temperature [°C]
- `rh`: Relative humidity [%]

### 3. Fire Weather Index (FWI)

**Functionality includes:**
- Fine fuel moisture calculation
- FWI and its components
- Uses meteorological parameters (temp, RH, wind, rain)

## Data Inputs

All forecast indices can be calculated from daily weather forecasts or hindcast reanalysis, typically requiring:

- Temperature
- Relative humidity
- Wind speed
- Radiation
- Precipitation

These can be obtained from numerical weather prediction models or gridded reanalysis datasets.

## Output

The scripts return numerical values that can be:
- Written to raster formats (e.g., GeoTIFF) using separate Python or GIS scripts
- Aggregated into monitoring dashboards
- Translated into thermal stress categories for visualization

## Example Integration

The outputs from these scripts are operationally integrated into the Clim4Cast regional platform:  
🔗 [https://clim4cast.czechglobe.cz](https://clim4cast.czechglobe.cz)

## License

These scripts are distributed under the MIT License. Please cite the Clim4Cast project when using this code.
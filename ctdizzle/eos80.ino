/* The following functions are used to compute depth, salinity, and density using PSS-78 and EOS80 algorithms.
 * They require inputs and do not operate as standalone functions.
 * To compute depth, you will need to first sample pressure.
 * To compute salinity, you will need to first sample conductivity, temperature, and pressure.
 * To compute density, you will need to first sample conductivity, temperature, pressure, and have computed salinity.

Examples on how to compute depth, salinity, and density anomaly.
float depth = compute_depth(p,45.00);
float salinity = compute_salinity(ec,t,p);
float density_anomaly = compute_density_anomaly(salinity,t,p);

*/

float compute_depth(float relative_pressure,float latitude){
  /* Compute the depth in meters using pressure and known latitude. 
   *  Requires a user selected latitude, unless a GPS is integrated and sampled during the setup sequence.
   *  relative_pressure -- a pressure value in decibars. This value is returned when using the get_relative_pressure() function.
   *  latitude -- the known latitude in decimal degrees.
   *  return -- the depth in meters.
   */
  float p = relative_pressure;
  float x = sin(latitude / 57.29578);
  x = x * x;
  float gr = 9.780318 * (1.0 + (5.2788e-3 + 2.36e-5 * x) * x) + 1.092e-6 * p;
  float depth = ((((-1.82e-15 * p + 2.279e-10) * p - 2.2512e-5) * p + 9.72659) * p)/gr;
  return depth;
}

float compute_salinity(float conductivity, float temperature, float relative_pressure){
  /* Compute the salinity using conductivity, temperature and pressure per the PSS-78 formula.
   * ec -- conductivity in uS or uMhos per cm.  This value is returned when using the get_conductivity() function.
   * t -- temperature in degrees Celsius. This value is returned when using the get_temperature() function.
   * p -- pressure in decibars. This value is returned when using the get_relative_pressure() function.
   * return -- the salinity in Practical Salinity Units (PSU).
   */
   float ec = conductivity;
   float t = temperature;
   float p = relative_pressure;
   float A1=2.070e-5, A2=-6.370e-10, A3=3.989e-15;
   float sB1=3.426e-2, sB2=4.464e-1, sB3=4.215e-1, sB4=-3.107e-3;  //Compiler interference requires "B" coeffiecients to be more unique.
   float c0=6.766097e-1, c1=2.00564e-2, c2=1.104259e-4, c3=-6.9698e-7, c4=1.0031e-9;
   float a0=0.0080, a1=-0.1692, a2=25.3851, a3=14.0941, a4=-7.0261, a5=2.7081;
   float b0=0.0005, b1=-0.0056, b2=-0.0066, b3=-0.0375, b4=0.0636, b5=-0.0144;
   float k=0.0162, cstandard=42.914;
  
   float R = ((ec/1000)/cstandard);   
   float rpnum = (A1*p)*(A2*pow(p,2))+(A3*pow(p,3));
   float rpdenom = 1*(sB1*t)+(sB2*pow(t,2))+(sB3*R)+(sB4*t*R);
   float Rp = 1+(rpnum/rpdenom);
   float rT = c0 +(c1*t)+(c2*pow(t,2))+(c3*pow(t,3))+(c4*pow(t,4));
   float RT = R/(rT*Rp);
   float salinity = (a0+(a1*pow(RT,0.5))+(a2*RT)+(a3*pow(RT,1.5))+(a4*pow(RT,2))+(a5*pow(RT,2.5)))+((t-15)/(1+k*(t-15)))*(b0+(b1*pow(RT,0.5))+(b2*RT)+(b3*pow(RT,1.5))+(b4*pow(RT,2))+(b5*pow(RT,2.5)));
   return salinity;
}

float compute_density_anomaly(float salinity, float temperature, float relative_pressure){
  /* Compute density using salinity, temperature, and pressure.
   * salinity -- the known salinity in PSU. This value is returned using the compute_salinity() function.
   * temperature -- the known temperature in degrees Celsius. This value is returned when using the get_temperature() function.
   * relative_pressure -- the known pressure in decibars. This value is returned when using the get_relative_pressure() function.
   * return -- density anomaly.
   */
   float s = salinity;
   float t = temperature;
   float p = relative_pressure;
   float dB0=8.24493e-1, dB1=-4.0899e-3, dB2=7.6438e-5, dB3=-8.2467e-7, dB4=5.3875e-9; //Compiler interference requires "B" coeffiecients to be more unique.
   float C0=-5.72466e-3, C1=1.0227e-4, C2=-1.6546e-6;
   float D0=4.8314e-4;
   float A0=999.842594, A1=6.793952e-2, A2=-9.095290e-3, A3=1.001685e-4, A4=-1.120083e-6, A5=6.536332e-9;
   float FQ0=54.6746, FQ1=-0.603459, FQ2=1.09987e-2, FQ3=-6.1670e-5;
   float G0=7.944e-2, G1=1.6483e-2, G2=-5.3009e-4;
   float i0=2.2838e-3, i1=-1.0981e-5, i2=-1.6078e-6;
   float J0=1.91075e-4;
   float M0=-9.9348e-7, M1=2.0816e-8, M2=9.1697e-10;
   float E0=19652.21, E1=148.4206, E2=-2.327105, E3=1.360477e-2, E4=-5.155288e-5;
   float H0=3.239908, H1=1.43713e-3, H2=1.16092e-4, H3=-5.77905e-7;
   float K0=8.50935e-5, K1=-6.12293e-6, K2=5.2787e-8;

   float t2 = t*t;
   float t3 = t*t2;
   float t4 = t*t3;
   float t5 = t*t4;
   if (s <= 0.0){s = 0.000001;}
   float s32 = pow(s,1.5);
   p = p/10; //Put pressure in bars.
   float sigma = A0 + A1*t + A2*t2 + A3*t3 + A4*t4 + A5*t5 + (dB0 + dB1*t + dB2*t2 + dB3*t3 + dB4*t4)*s + (C0 + C1*t + C2*t2)*s32 + D0*s*s;
   float kw = E0 + E1*t + E2*t2 + E3*t3 + E4*t4;
   float aw = H0 + H1*t + H2*t2 + H3*t3;
   float bw = K0 + K1*t + K2*t2;
   float k = kw + (FQ0 + FQ1*t + FQ2*t2 + FQ3*t3)*s + (G0 + G1*t + G2*t2)*s32 + (aw + (i0 + i1*t + i2*t2)*s + (J0*s32))*p + (bw + (M0 + M1*t + M2*t2)*s)*p*p;
   float val = 1 - p / k;
   if (val){sigma = sigma/val - 1000.0;}   
   return sigma;
}

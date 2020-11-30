#include <math.h>



float Interpolate(float a, float b, float x) {
    float ft = x * 3.1415927;
    float f = (1 - cos(ft)) * 0.5;
    return  abs(a * (1 - f) + b * f);
}
_inline float Noise2D(int x, int y)
{
    int n = x + y * 57;
    n = (n << 23) ^ n;
    return (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) /
        1073741824.0f);
}inline float SmoothedNoise2D(float x, float y)
{
    float corners = (Noise2D(x - 1, y - 1) + Noise2D(x + 1, y - 1) +
        Noise2D(x - 1, y + 1) + Noise2D(x + 1, y + 1)) / 16;
    float sides = (Noise2D(x - 1, y) + Noise2D(x + 1, y) +
        Noise2D(x, y - 1) + Noise2D(x, y + 1)) / 8;
    float center = Noise2D(x, y) / 4;
    return corners + sides + center;
}

//Function that returns the value of smoothed and
// interpolated noise
float CompileNoise(float x, float y) {
    float int_X = int(x);//integer part x
    float fractional_X = x - int_X;// fraction of x
    //similar to y
    float int_Y = int(y);
    float fractional_Y = y - int_Y;
    //get 4 smoothed values
    float v1 = SmoothedNoise2D(int_X, int_Y);
    float v2 = SmoothedNoise2D(int_X + 1, int_Y);
    float v3 = SmoothedNoise2D(int_X, int_Y + 1);
    float v4 = SmoothedNoise2D(int_X + 1, int_Y + 1);
    // interpolate the values of 1 and 2 pairs and perform an interpolation between them
    float i1 = Interpolate(v1, v2, fractional_X);
    float i2 = Interpolate(v3, v4, fractional_X);
    return Interpolate(i1, i2, fractional_Y);
}
int PerlinNoise_2D(float x, float y, float factor,int octaves,int ampl)
{
    float total = 0;
    float persistence = 0.5f;


    float frequency = 0.25f;
    float amplitude = 1;// amplitude, as a direct function of the persistence value


    x += (factor);
    y += (factor);

    // NUM_OCTAVES-a variable that denotes the number of octaves.
    // the more octaves, the better the noise is
    for (int i = 0; i < octaves; i++)
    {
        total += CompileNoise(x * frequency, y * frequency) * amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }

    total = fabsf(total);
    int res = int(total * ampl);
    return res;
}

void GenNoise(int pNoise[], int sizex, int sizey, int cyc, int amplitude, int seed,float scale)
{
    srand(seed);

    float fac = rand() % 156;

    for (int i = 0; i < sizey; i++)
    {
        for (int j = 0; j < sizex; j++)
        {
            pNoise[i * sizex + j] = PerlinNoise_2D(float(i / scale), float(j / scale), fac, cyc,amplitude);
        }
    }
}

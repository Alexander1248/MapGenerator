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

//‘ункци€, котора€ возвращает значение сглаженного и 
//интерполированного шума 
float CompileNoise(float x, float y) {
    float int_X = int(x);//цела€ часть х
    float fractional_X = x - int_X;//дробь от х
//аналогично у
    float int_Y = int(y);
    float fractional_Y = y - int_Y;
    //получаем 4 сглаженных значени€
    float v1 = SmoothedNoise2D(int_X, int_Y);
    float v2 = SmoothedNoise2D(int_X + 1, int_Y);
    float v3 = SmoothedNoise2D(int_X, int_Y + 1);
    float v4 = SmoothedNoise2D(int_X + 1, int_Y + 1);
    //интерполируем значени€ 1 и 2 пары и производим интерпол€цию между ними
    float i1 = Interpolate(v1, v2, fractional_X);
    float i2 = Interpolate(v3, v4, fractional_X);
    //€ использовал косинусною интерпол€цию »ћ’ќ лучше 
    //по параметрам быстрота-//качество
    return Interpolate(i1, i2, fractional_Y);
}
int PerlinNoise_2D(float x, float y, float factor,int octaves,int ampl)
{
    float total = 0;
    // это число может иметь и другие значени€ хоть cosf(sqrtf(2))*3.14f 
    // главное чтобы было красиво и результат вас устраивал
    float persistence = 0.5f;

    // экспериментируйте с этими значени€ми, попробуйте ставить 
    // например sqrtf(3.14f)*0.25f или что-то пот€желее дл€ понимани€ J)
    float frequency = 0.25f;
    float amplitude = 1;//амплитуда, в пр€мой зависимости от значени€ настойчивости

    // вводим фактор случайности, чтобы облака не были всегда одинаковыми
    // (ћы ведь помним что ф-ци€ шума когерентна?) 

    x += (factor);
    y += (factor);

    // NUM_OCTAVES - переменна€, котора€ обозначает число октав,
    // чем больше октав, тем лучше получаетс€ шум
    for (int i = 0; i < octaves; i++)
    {
        total += CompileNoise(x * frequency, y * frequency) * amplitude;
        amplitude *= persistence;
        frequency *= 2;
    }
    //здесь можно перевести значени€ цвета   по какой-то формуле
    //например:
    //total=sqrt(total);
    // total=total*total;
    // total=sqrt(1.0f/float(total)); 
    //total=255-total;-и.т.д все зависит от желаемого результата
    total = fabsf(total);
    int res = int(total * ampl);//приводим цвет к значению 0-255Е
    return res;
}

void GenNoise(int pNoise[], int sizex, int sizey, int cyc, int amplitude, int seed,float scale)
{
    srand(seed);

    // случайное число, которое призвано внести
    // случайность в нашу текстуру
    float fac = rand() % 156;

    for (int i = 0; i < sizey; i++)
    {
        for (int j = 0; j < sizex; j++)
        {
            //проходим по всем элементам массива и заполн€ем их значени€ми   
            pNoise[i * sizex + j] = PerlinNoise_2D(float(i / scale), float(j / scale), fac, cyc,amplitude);
        }
    }
}

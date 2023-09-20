#include <stdio.h>
#include <stdlib.h>

#include "../src/math.h"

int main(int argc, char *argv[])
{
    f32 x = f32_cos(F32_PI);

    printf("%f\n", x);

    return EXIT_SUCCESS;
}

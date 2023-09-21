#include <stdio.h>
#include <stdlib.h>

#include "../src/common.h"
#include "../src/math.h"

int main(void)
{
    f32 x;

    STC_DEBUG("debug info\n");
    x = f32_cos(F32_PI);
    printf("%f\n", x);

    return EXIT_SUCCESS;
}

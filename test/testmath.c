#include <stdio.h>
#include <stdlib.h>

#include <stc/common.h>
#include <stc/math.h>

int main(void)
{
    f32 x;

    STC_DEBUG("debug info\n");
    x = f32_cos(F32_PI);
    printf("%f\n", x);

    return EXIT_SUCCESS;
}

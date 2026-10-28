#ifndef __FILTER_H
#define __FILTER_H

#include <stdint.h>

#define FILTER_WINDOW_SIZE 15

typedef struct {
    int32_t buffer[FILTER_WINDOW_SIZE];
    uint8_t index;
    uint8_t count;
    int64_t sum;
} MovingAverageFilter_t;

void Filter_Init(MovingAverageFilter_t* filter);
int32_t Filter_Update(MovingAverageFilter_t* filter, int32_t input);

#endif /* __FILTER_H */
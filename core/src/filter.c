#include "filter.h"

void Filter_Init(MovingAverageFilter_t *filter) {
    uint8_t i;
    filter->index = 0;
    filter->count = 0;
    filter->sum = 0;
    for (i = 0; i < FILTER_WINDOW_SIZE; i++) {
        filter->buffer[i] = 0;
    }
}

int32_t Filter_Update(MovingAverageFilter_t *filter, int32_t new_value) {
    filter->sum -= filter->buffer[filter->index];
    filter->buffer[filter->index] = new_value;
    filter->sum += new_value;

    filter->index = (filter->index + 1) % FILTER_WINDOW_SIZE;

    if (filter->count < FILTER_WINDOW_SIZE) {
        filter->count++;
    }

    return (int32_t)(filter->sum / filter->count);
}
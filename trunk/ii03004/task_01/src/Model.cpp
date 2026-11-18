#include "Model.h"

void Model::reset(double y0)
{
    y_ = y0;
    yPrev_ = y0;
    uPrev_ = 0.0;
}

double Model::step(double u)
{
    const double next = calculateNext(u);

    yPrev_ = y_;
    y_ = next;
    uPrev_ = u;

    return y_;
}

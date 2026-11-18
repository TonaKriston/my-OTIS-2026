#pragma once

class BaseModel{
    public:
        virtual double stepForward(double u) = 0;
        virtual void clearState() = 0;
        virtual ~BaseModel() = default;
};
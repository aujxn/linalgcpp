#ifndef OPERATOR_HPP__
#define OPERATOR_HPP__

#include <memory>
#include <vector>
#include <numeric>
#include <assert.h>

#include "vector.hpp"

namespace linalgcpp
{

class Operator
{
    public:
        virtual ~Operator() noexcept = default;

        virtual size_t Rows() const = 0;
        virtual size_t Cols() const = 0;


        virtual void Mult(const Vector<double>& input, Vector<double>& output) const = 0;
        virtual void MultAT(const Vector<double>& input, Vector<double>& output) const;
};

inline
void Operator::MultAT(const Vector<double>& input, Vector<double>& output) const
{
    throw std::runtime_error("The operator MultAT not defined!\n");
}

}
#endif // OPERATOR_HPP__
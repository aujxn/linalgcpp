/*! @file */

#include "blockoperator.hpp"

namespace linalgcpp
{

BlockOperator::BlockOperator()
    : row_offsets_(1, 0), col_offsets_(1, 0)
{

}

BlockOperator::BlockOperator(std::vector<int> offsets) :
    Operator(offsets.back()),
    row_offsets_(offsets), col_offsets_(std::move(offsets)),
    A_(row_offsets_.size() - 1, std::vector<const Operator*>(col_offsets_.size() - 1, nullptr)),
    x_(col_offsets_), y_(row_offsets_)
{

}

BlockOperator::BlockOperator(std::vector<int> row_offsets, std::vector<int> col_offsets)
    : Operator(row_offsets.back(), col_offsets.back()),
      row_offsets_(std::move(row_offsets)), col_offsets_(std::move(col_offsets)),
      A_(row_offsets_.size() - 1, std::vector<const Operator*>(col_offsets_.size() - 1, nullptr)),
      x_(col_offsets_), y_(row_offsets_)
{

}

const std::vector<int>& BlockOperator::GetRowOffsets() const
{
    return row_offsets_;
}

const std::vector<int>& BlockOperator::GetColOffsets() const
{
    return col_offsets_;
}

const Operator* BlockOperator::GetBlock(int i, int j) const
{
    assert(i < static_cast<int>(row_offsets_.size()) - 1);
    assert(j < static_cast<int>(col_offsets_.size()) - 1);

    return A_[i][j];
}

void BlockOperator::SetBlock(int i, int j, const Operator& op)
{
    assert(i < static_cast<int>(row_offsets_.size()) - 1);
    assert(j < static_cast<int>(col_offsets_.size()) - 1);

    assert(op.Rows() == (row_offsets_[i + 1] - row_offsets_[i]));
    assert(op.Cols() == (col_offsets_[i + 1] - col_offsets_[i]));

    A_[i][j] = &op;
}

void BlockOperator::Mult(const VectorView<double>& input, VectorView<double>& output) const
{
    assert(input.size() == cols_);
    assert(output.size() == rows_);

    x_ = input;
    y_ = 0.0;

    const int row_blocks = row_offsets_.size() - 1;
    const int col_blocks = col_offsets_.size() - 1;

    for (int i = 0; i < row_blocks; ++i)
    {
        VectorView<double> row_y {y_.GetBlock(i)};
        Vector<double> tmp_y(row_y.size());;

        for (int j = 0; j < col_blocks; ++j)
        {
            const Operator* op = A_[i][j];

            if (op)
            {
                op->Mult(x_.GetBlock(j), tmp_y);
                row_y += tmp_y;
            }
        }
    }

    output = y_;
}

void BlockOperator::MultAT(const VectorView<double>& input, VectorView<double>& output) const
{
    assert(input.size() == rows_);
    assert(output.size() == cols_);

    y_ = input;

    x_ = 0.0;

    const int row_blocks = row_offsets_.size() - 1;
    const int col_blocks = col_offsets_.size() - 1;

    for (int j = 0; j < col_blocks; ++j)
    {
        VectorView<double> row_x {x_.GetBlock(j)};
        Vector<double> tmp_x(row_x.size());;

        for (int i = 0; i < row_blocks; ++i)
        {
            const Operator* op = A_[i][j];

            if (op)
            {
                VectorView<double> row_block = y_.GetBlock(i);

                op->MultAT(row_block, tmp_x);

                row_x += tmp_x;
            }
        }
    }

    output = x_;
}

} // namespace linalgcpp

#ifndef EULERSOLVER_H
#define EULERSOLVER_H

#include "solver.h"

class EulerSolver : public Solver
{
public:
    EulerSolver() = delete;
    EulerSolver(qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal);
    EulerSolver(const EulerSolver&) = delete;
    EulerSolver(EulerSolver&&) noexcept = delete;

    ~EulerSolver() override = default;

    EulerSolver& operator = (const EulerSolver&) = delete;
    EulerSolver& operator = (EulerSolver&&) = delete;

    QVector<QPair<qreal, qreal>> solution(qreal, qreal) override;
};

#endif // EULERSOLVER_H

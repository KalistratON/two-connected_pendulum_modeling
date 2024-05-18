#ifndef RUNGEKUTTASOLVER_H
#define RUNGEKUTTASOLVER_H

#include "solver.h"

class RungeKuttaSolver : public Solver
{
public:
    RungeKuttaSolver() = delete;
    RungeKuttaSolver(qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal);
    RungeKuttaSolver(const RungeKuttaSolver&) = delete;
    RungeKuttaSolver(RungeKuttaSolver&&) noexcept = delete;

    ~RungeKuttaSolver() override = default;

    RungeKuttaSolver& operator = (const RungeKuttaSolver&) = delete;
    RungeKuttaSolver& operator = (RungeKuttaSolver&&) = delete;

    QVector<QPair<qreal, qreal>> solution(qreal, qreal) override;

private:
    QVector<qreal> rightPartVector(qreal, qreal, qreal, qreal);
};

#endif // RUNGEKUTTASOLVER_H

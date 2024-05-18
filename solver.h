#ifndef SOLVER_H
#define SOLVER_H

#include <QtTypes>
#include <QVector>
#include <QPair>

class Solver
{
protected:
    qreal step;
    qreal m_1;
    qreal m_2;
    qreal l_1;
    qreal l_2;

    qreal phi_1;
    qreal phi_2;

    qreal omega_1;
    qreal omega_2;

public:
    Solver() = delete;
    Solver(qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal, qreal);
    Solver(const Solver&) = delete;
    Solver(Solver&&) noexcept = delete;

    virtual ~Solver() = default;

    Solver& operator = (const Solver&) = delete;
    Solver& operator = (Solver&&) = delete;

    virtual QVector<QPair<qreal, qreal>> solution(qreal, qreal) = 0;
};

#endif // SOLVER_H

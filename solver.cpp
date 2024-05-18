#include "solver.h"
#include <stdexcept>

Solver::Solver(qreal _st, qreal _m_1,
               qreal _m_2, qreal _l_1,
               qreal _l_2, qreal _phi_1, qreal _phi_2,
               qreal _omega_1, qreal _omega_2) :
    step(_st),
    m_1(_m_1),
    m_2(_m_2),
    l_1(_l_1),
    l_2(_l_2),
    phi_1(_phi_1),
    phi_2(_phi_2),
    omega_1(_omega_1),
    omega_2(_omega_2)
{


};

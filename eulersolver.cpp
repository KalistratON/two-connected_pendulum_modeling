#include "eulersolver.h"
#include <QtMath>
#include <QFile>
#include <QDebug>
#include <QTextStream>

EulerSolver::EulerSolver(qreal _st, qreal _m_1,
                         qreal _m_2, qreal _l_1,
                         qreal _l_2, qreal _phi_1, qreal _phi_2,
                         qreal _omega_1, qreal _omega_2) :
    Solver(_st, _m_1, _m_2, _l_1, _l_2, _phi_1, _phi_2, _omega_1, _omega_2)
    {};

QVector<QPair<qreal, qreal>> EulerSolver::solution(qreal t_0, qreal t_1)
{

    if(step <= 0 || m_1 <= 0 || m_2 <= 0 || l_1 <= 0 || l_2 <= 0)
        throw std::invalid_argument("Negative parameter(-s) of system");

    quint64 k = (t_1 - t_0) / step + 1;
    QVector<QPair<qreal, qreal>> points(k);

    points[0] = QPair<qreal, qreal>(phi_1, phi_2);

    auto omg_1 = omega_1;
    auto omg_2 = omega_2;

    QVector<QVector<qreal>> result(k);
    result[0] = QVector<qreal> {t_0, phi_1, phi_2, omega_1, omega_2};

    qDebug() << qCos(phi_1) << " " << qCos(0) << " " << qCos(3.14);

    qreal Energy = m_1*(l_1*omega_1) * (l_1*omega_1) / 6 + m_2 * ((l_2 * omega_2) * (l_2 * omega_2) / 6 +
                    m_2 / 2 * (l_1*l_1*omega_1*omega_1+l_1*l_2*omega_1*omega_2*qCos(phi_2-phi_1)))
                    + m_1 * 9.81 *(1 - qCos(phi_1)) * l_1 / 2 + m_2 * 9.81 * (l_1 + l_2/2 - l_1 * qCos(phi_1)-l_2*qCos(phi_2)/2);

    QFile file("output.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << t_0 << ' ' << phi_1 << ' ' << phi_2 << ' ' << omega_1 << ' ' << omega_2 << ' ' << Energy << '\n';

    for(quint64 i = 1; i < k; i++)
    {
        points[i] = QPair<qreal, qreal>(points[i-1].first + omg_1 * step,
                                        points[i-1].second + omg_2 * step);

        auto temp_1 = 1/(l_1*(9*m_2*qCos(2*(points[i-1].first - points[i-1].second))-8*m_1-15*m_2)) *
                (9 * omg_1 * omg_1 * l_1 * m_2 * qSin(2*(points[i-1].first - points[i-1].second)) +
                (9*9.81*m_2*qSin(points[i - 1].first-2*points[i-1].second)) +
                12*m_2*l_2*omg_2*omg_2*qSin(points[i-1].first - points[i-1].second) +
                12*9.81*(m_1+5*m_2/4)*qSin(points[i-1].first));

        auto temp_2 = 1/(l_2*(9*m_2*qCos(2*(points[i-1].first - points[i-1].second))-8*m_1-15*m_2)) *
                    (-9*9.81*qSin(2*points[i-1].first - points[i-1].second)*(m_1+2*m_2)-9*m_2*l_2*omg_2*omg_2*
                    qSin(2*(points[i-1].first - points[i-1].second))-12*l_1*omg_1*omg_1*(m_1+3*m_2)*qSin(points[i-1].first - points[i-1].second)+
                    3*9.81*qSin(points[i-1].second)*(m_1+6*m_2));

        omg_1 += temp_1 * step;
        omg_2 += temp_2 * step;

        result[i] = QVector<qreal> {t_0 + i * step, points[i].first, points[i].second, omg_1, omg_2};

        qreal Energy = m_1*(l_1*omg_1) * (l_1*omg_1) / 6 + m_2 * (l_2 * omg_2) * (l_2 * omg_2) / 6 +
                        m_2 / 2 * (l_1*l_1*omg_1*omg_1+l_1*l_2*omg_1*omg_2*qCos(points[i].second-points[i].first))
                       + m_1 * 9.81 *(1 - qCos(points[i].first)) * l_1 / 2 + m_2 * 9.81 * (l_1 + l_2/2 - l_1 * qCos(points[i].first)-l_2*qCos(points[i].second)/2);

        stream << t_0 + i * step << ' ' << points[i].first << ' ' << points[i].second << ' ' << omg_1 << ' ' << omg_2 << ' ' << Energy << '\n';
    }

    return points;
}

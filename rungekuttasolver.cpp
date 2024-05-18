#include "rungekuttasolver.h"
#include <QtMath>
#include <QFile>
#include <QTextStream>

RungeKuttaSolver::RungeKuttaSolver(qreal _st, qreal _m_1,
                                   qreal _m_2, qreal _l_1,
                                   qreal _l_2, qreal _phi_1, qreal _phi_2,
                                   qreal _omega_1, qreal _omega_2) :
    Solver(_st, _m_1, _m_2, _l_1, _l_2, _phi_1, _phi_2, _omega_1, _omega_2)
    {};

QVector<QPair<qreal, qreal>> RungeKuttaSolver::solution(qreal t_0, qreal t_1)
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

    qreal Energy = m_1*(l_1*omega_1) * (l_1*omega_1) / 6 + m_2 * ((l_2 * omega_2) * (l_2 * omega_2) / 6 +
                                                                        m_2 / 2 * (l_1*l_1*omega_1*omega_1+l_1*l_2*omega_1*omega_2*qCos(phi_2-phi_1)))
                   + m_1 * 9.81 *(1 - qCos(phi_1)) * l_1 / 2 + m_2 * 9.81 * (l_1 + l_2/2 - l_1 * qCos(phi_1)-l_2*qCos(phi_2)/2);

    QFile file("output.txt");
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&file);
    stream << t_0 << ' ' << phi_1 << ' ' << phi_2 << ' ' << omega_1 << ' ' << omega_2 << ' ' << Energy << '\n';

    for(quint64 i = 1; i < k; i++)
    {
        auto w_1 = rightPartVector(points[i - 1].first, points[i - 1].second, omg_1, omg_2);
        auto w_2 = rightPartVector(points[i - 1].first + 1/2 * step * w_1[0], points[i - 1].second + 1/2 * step * w_1[1],
                                   omg_1 + 1/2 * step * w_1[2], omg_2 + 1/2 * step * w_1[3]);
        auto w_3 = rightPartVector(points[i - 1].first + 1/2 * step * w_2[0], points[i - 1].second + 1/2 * step * w_2[1],
                                   omg_1 + 1/2 * step * w_2[2], omg_2 + 1/2 * step * w_2[3]);
        auto w_4 = rightPartVector(points[i - 1].first + step * w_3[0], points[i - 1].second + step * w_3[1],
                                   omg_1 + step * w_3[2], omg_2 + step * w_3[3]);

        points[i].first = points[i - 1].first + step/6*(w_1[0]+2*w_2[0]+2*w_3[0]+w_4[0]);
        points[i].second = points[i - 1].second + step/6*(w_1[1]+2*w_2[1]+2*w_3[1]+w_4[1]);
        omg_1 += step/6*(w_1[2]+2*w_2[2]+2*w_3[2]+w_4[2]);
        omg_2 += step/6*(w_1[3]+2*w_2[3]+2*w_3[3]+w_4[3]);

        result[i] = QVector<qreal> {t_0 + i * step, points[i].first, points[i].second, omg_1, omg_2};

        qreal Energy = m_1*(l_1*omg_1) * (l_1*omg_1) / 6 + m_2 * (l_2 * omg_2) * (l_2 * omg_2) / 6 +
                       m_2 / 2 * (l_1*l_1*omg_1*omg_1+l_1*l_2*omg_1*omg_2*qCos(points[i].second-points[i].first))
                       + m_1 * 9.81 *(1 - qCos(points[i].first)) * l_1 / 2 + m_2 * 9.81 * (l_1 + l_2/2 - l_1 * qCos(points[i].first)-l_2*qCos(points[i].second)/2);


        stream << t_0 + i * step << ' ' << points[i].first << ' ' << points[i].second << ' ' << omg_1 << ' ' << omg_2 << ' ' << Energy << '\n';
    }
    return points;
}

QVector<qreal> RungeKuttaSolver::rightPartVector(qreal th_1, qreal th_2, qreal omg_1, qreal omg_2)
{
    qreal temp_1 = omg_1;
    qreal temp_2 = omg_2;

    qreal temp_3 = 1/(l_1*(9*m_2*qCos(2*(th_1 - th_2))-8*m_1-15*m_2)) *
                   (9 * omg_1 * omg_1 * l_1 * m_2 * qSin(2*(th_1 - th_2)) +
                    (9*9.81*m_2*qSin(th_1-2*th_2)) +
                    12*m_2*l_2*omg_2*omg_2*qSin(th_1 - th_2) +
                    12*9.81*(m_1+5*m_2/4)*qSin(th_1));

    qreal temp_4 = 1/(l_2*(9*m_2*qCos(2*(th_1 - th_2))-8*m_1-15*m_2)) *
                   (-9*9.81*qSin(2*th_1 - th_2)*(m_1+2*m_2)-9*m_2*l_2*omg_2*omg_2*
                    qSin(2*(th_1 - th_2))-12*l_1*omg_1*omg_1*(m_1+3*m_2)*qSin(th_1 - th_2)+
                    3*9.81*qSin(th_2)*(m_1+6*m_2));

    return QVector<qreal> {temp_1, temp_2, temp_3, temp_4};
}

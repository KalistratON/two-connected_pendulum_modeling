#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QPoint>
#include <memory>


#include "brunch.h"
#include "eulersolver.h"
#include "rungekuttasolver.h"
#include "qplot.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

    int width;
    int height;

    int margin = 350;

    qreal width_of_beam = 5.0;

    qreal step = 1.0 / 60.0 * 1000.0;
    qreal start = 0.0;
    qreal finish = 10.0 * 1000.0;

    qreal lenghtBase = 250.0;
    qreal lenghtChild = 250.0;

    qreal thetaBase = 0.0;
    qreal thetaChild = 0.0;

    qreal omegaBase = 0.0;
    qreal omegaChild = 0.0;

    qreal massBase = 0.0;
    qreal massChild = 0.0;

    qreal pivot_left = 0;
    qreal pivot_top = 0;

    QString solver_type;

    QGraphicsScene* scene = nullptr;
    Brunch* brunchBase = nullptr;
    Brunch* brunchChild = nullptr;

    QTimer* frameTimer = nullptr;
    QTimer* modelingTimer = nullptr;

    std::unique_ptr<Solver> solver;

    QVector<QPair<qreal, qreal>> solution_data;

private slots:
    void startModeling();
    void generateFrame();
    void uploadFile();

public:
    MainWindow(int width, int height, QWidget *parent = nullptr);
    ~MainWindow();

public:
    void process(QVariantMap&&);
    void setStartPosition();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H

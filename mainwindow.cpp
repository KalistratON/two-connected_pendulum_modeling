#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QGraphicsEllipseItem>
#include <QMenu>
#include <QtMath>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QByteArray>
#include <QErrorMessage>
#include <QJsonObject>
#include <QJsonDocument>

MainWindow::MainWindow(int _width, int _height, QWidget *parent)
    : QMainWindow(parent)
    , width     (_width)
    , height    (_height)
    , pivot_left((_width - margin)  / 2)
    , pivot_top((_height - margin)  / 20)
    , scene(new QGraphicsScene(0, 0, width - margin, height - margin, this))
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setReadOnly(true);

    QMenu* mainMenu = new QMenu("Menu", this);
    mainMenu->addAction("&Upload Setting File", this, SLOT(uploadFile()));
    mainMenu->addSeparator();
    mainMenu->addAction("&Exit", this, &QApplication::quit);
    ui->menubar->addMenu(mainMenu);

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setSceneRect(scene->sceneRect());

    frameTimer = new QTimer(this);
    connect(frameTimer, &QTimer::timeout, this, &MainWindow::generateFrame);

    modelingTimer = new QTimer(this); 
    connect(modelingTimer, &QTimer::timeout, this, [this]() {
        frameTimer->stop();
        modelingTimer->stop();

        setStartPosition();
    });

    connect(ui->pushButton, &QPushButton::clicked, this, &MainWindow::startModeling);
}

void MainWindow::startModeling()
{
    if (brunchBase && brunchChild)
    {
        setStartPosition();

        frameTimer->start(step * 1000.0);
        modelingTimer->start( (finish - start)  * 1000.0);
    }
    else
        (new QErrorMessage(this))->showMessage("Invalid argument");
}

void MainWindow::setStartPosition()
{
    frameTimer->stop();
    modelingTimer->stop();

    if (brunchBase && brunchChild)
    {
        scene->removeItem(brunchChild);
        scene->removeItem(brunchBase);
        delete brunchChild;
        delete brunchBase;
    }
    //solver.release();
    if (solver_type == "euler")
        solver = std::make_unique<EulerSolver>(step, massBase, massChild, lenghtBase, lenghtChild,
                                                thetaBase / 180.0 * 3.14, thetaChild / 180.0 * 3.14,
                                                omegaBase, omegaChild);
    if (solver_type == "runge_kutta")
        solver = std::make_unique<RungeKuttaSolver>(step, massBase, massChild, lenghtBase, lenghtChild,
                                                    thetaBase / 180.0 * 3.14, thetaChild / 180.0 * 3.14,
                                                    omegaBase, omegaChild);

    solution_data.clear();

    try {

        solution_data = solver->solution(start, finish);

        brunchBase = new Brunch(pivot_left, pivot_top, width_of_beam, lenghtBase * 100.0);
        brunchChild = new Brunch(pivot_left, pivot_top + lenghtBase * 100.0, width_of_beam, lenghtChild * 100.0, brunchBase);

        brunchBase->setTransformOriginPoint(pivot_left, pivot_top);
        brunchChild->setTransformOriginPoint(pivot_left, pivot_top + lenghtBase * 100.0);

        brunchBase->setRotation(thetaBase);
        brunchChild->setRotation(thetaChild - thetaBase);

        scene->addItem(brunchBase);
        scene->addItem(brunchChild);
    }
    catch(const std::invalid_argument& err)
    {
        qDebug() << err.what();

        brunchBase = nullptr;
        brunchChild = nullptr;
    }

    ui->pushButton->setEnabled(true);
}

void MainWindow::generateFrame()
{
    brunchBase->setAngle(solution_data.at(0).first * 180.0 / 3.14);
    brunchChild->setAngle((solution_data.at(0).second - solution_data.at(0).first) * 180.0 / 3.14);
    solution_data.pop_front();

    scene->advance();
}

void MainWindow::uploadFile()
{
    ui->pushButton->setEnabled(false);

    QString file_path = QFileDialog::getOpenFileName(this, tr("Choose the file"), QString(), "*.json");
    QFile file(file_path);
    if(!file.open(QIODevice::ReadOnly | QFile::Text)){
        qDebug()<<"Failed to open "<<file_path;
        return;
    }

    QString jsonText(file.readAll());
    file.close();
    ui->textEdit->setText(jsonText);
    auto json_doc = QJsonDocument::fromJson(QByteArray(jsonText.toLocal8Bit()));

    if(json_doc.isNull()){
        qDebug()<<"Failed to create JSON doc.";
       return;
    }
    if(!json_doc.isObject()){
        qDebug()<<"JSON is not an object.";
        return;
    }

    auto json_obj = json_doc.object();
    if(json_obj.isEmpty()){
        qDebug()<<"JSON object is empty.";
        return;
    }

    process(json_obj.toVariantMap());
    setStartPosition();
}

void MainWindow::process(QVariantMap&& data)
{
    auto title      = data["title"].toString();

    auto segments   = data["segment"].toList();
    auto segment_1  = segments[0].toMap();
    auto segment_2  = segments[1].toMap();

    auto durance    = data["durance"].toUInt();
    auto solver     = data["solver"].toMap();

    setWindowTitle(title);

    start = 0.0;
    finish = durance;
    step = solver["step"].toReal();
    solver_type = solver["type"].toString();

    lenghtBase = segment_1["lenght"].toReal();
    lenghtChild = segment_2["lenght"].toReal() ;

    massBase = segment_1["mass"].toReal();
    massChild = segment_2["mass"].toReal();

    thetaBase = segment_1["theta"].toReal();
    thetaChild = segment_2["theta"].toReal();

    omegaBase = segment_1["omega"].toReal();
    omegaChild = segment_2["omega"].toReal();
}

MainWindow::~MainWindow()
{
    delete ui;
}




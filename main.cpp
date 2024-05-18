#include "mainwindow.h"

#include <QApplication>
#include <QScreen>
#include <QFile>

QString styleString(const QString& path)
{
    QFile file(path);

    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream stream(&file);
        return file.readAll();
    }
    return QString("");
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w(QApplication::primaryScreen()->size().width(), QApplication::primaryScreen()->size().height());
    w.setWindowState(Qt::WindowMaximized);
    QString css = styleString(":/styles/style.css");
    if (!css.isEmpty()) a.setStyleSheet(css);

    w.show();
    return a.exec();
}

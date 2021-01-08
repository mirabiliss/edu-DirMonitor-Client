#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/client.h"
#include <nlohmann/json.hpp>

using nlohmann::json;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void setupClient(const char *hostname, const size_t portno);

    void getData();

private slots:
    void on_actionAdd_triggered();

    void on_actionConnect_triggered();

private:
    Ui::MainWindow *ui;
    Client *client;
    QString currentPath = "";
    json currentData;
};
#endif // MAINWINDOW_H

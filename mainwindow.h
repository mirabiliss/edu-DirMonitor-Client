#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "src/client.h"
#include <nlohmann/json.hpp>
#include <QInputDialog>
#include <QDebug>
#include <QDialogButtonBox>
#include <QFile>
#include <QDateTime>

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
    void on_actionExit_triggered();

private slots:
    void on_actionChoose_path_triggered();

    void on_actionAdd_custom_triggered();

    void on_actionConnect_triggered();

private:
    std::string formRequest(QString dirpath, QStringList extensions);

private:
    Ui::MainWindow *ui;
    Client *client;
    QString currentPath = "";
    json currentData;
    QMap<QAction*, QString> defaultExtensionsMap;
    QStringList extensions;
};
#endif // MAINWINDOW_H

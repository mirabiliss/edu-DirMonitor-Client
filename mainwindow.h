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
#include <QDir>

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

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

    std::shared_ptr<spdlog::logger> client_logger;
    std::shared_ptr<spdlog::logger> client_warn_logger;

    const std::string basic_log_format = "[%T][%n:%l]%15v";
    const std::string errors_log_format = "[%T][%l][%@]%15v";

    std::vector<spdlog::sink_ptr> logger_sinks;
    std::vector<spdlog::sink_ptr> warn_logger_sinks;
};
#endif // MAINWINDOW_H

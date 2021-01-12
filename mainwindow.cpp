#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <chrono>

void MainWindow::setupLoggers()
{
    // QDir::tempPath().toStdString() + "/basic-log.txt"
    // logger:
    auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console_sink->set_level(spdlog::level::debug);
    console_sink->set_pattern(basic_log_format);
    logger_sinks.push_back(console_sink);

    // file sinks:
    // basic-log.txt
    auto basic_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(QDir::tempPath().toStdString() + "/logs-client/basic-log.txt", true);
    basic_file_sink->set_level(spdlog::level::trace);
    basic_file_sink->set_pattern(basic_log_format);
    logger_sinks.push_back(basic_file_sink);

    // warning-log.txt
    auto warn_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(QDir::tempPath().toStdString() + "/logs-client/warns-log.txt", true);
    warn_file_sink->set_level(spdlog::level::warn);
    warn_file_sink->set_pattern(errors_log_format);
    warn_logger_sinks.push_back(warn_file_sink);

    // errors-log.txt
    auto error_file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(QDir::tempPath().toStdString() + "/logs-client/errors-log.txt", true);
    error_file_sink->set_level(spdlog::level::err);
    error_file_sink->set_pattern(errors_log_format);
    logger_sinks.push_back(error_file_sink);
    //-----------------------------------------------------------------------------------------------------------------------------------------------------
    auto logger = std::make_shared<spdlog::logger>("client_logger", logger_sinks.begin(), logger_sinks.end());
    logger->set_level(spdlog::level::trace);

    // use warn_logger only for "warning-log.txt"
    auto warn_logger = std::make_shared<spdlog::logger>("client_warn_logger", warn_logger_sinks.begin(), warn_logger_sinks.end());
    warn_logger->set_level(spdlog::level::warn);


    spdlog::register_logger(logger);
    spdlog::register_logger(warn_logger);

    spdlog::set_default_logger(logger);

    client_logger = spdlog::get("client_logger");
    client_warn_logger = spdlog::get("client_warn_logger");

    spdlog::flush_every(std::chrono::seconds(5));
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->defaultExtensionsMap[ui->action_cpp] = "*.cpp";
    this->defaultExtensionsMap[ui->action_txt] = "*.txt";
    this->defaultExtensionsMap[ui->action_gif] = "*.gif";
    this->defaultExtensionsMap[ui->action_mp3] = "*.mp3";
    this->defaultExtensionsMap[ui->action_pdf] = "*.pdf";
    this->defaultExtensionsMap[ui->action_tmp] = "*.tmp";
    this->defaultExtensionsMap[ui->action_zip] = "*.zip";
    this->defaultExtensionsMap[ui->action_bin] = "*.bin";
    this->defaultExtensionsMap[ui->action_png] = "*.png";
    this->defaultExtensionsMap[ui->action_docx] = "*.doc | *.docx";
    this->defaultExtensionsMap[ui->action_jpg] = "*.jpg | *.jpeg";

    setupLoggers();
}

MainWindow::~MainWindow()
{
    delete ui;
    client->get("__close__");
}

void MainWindow::setupClient(const char *hostname, const size_t portno)
{
    client = new Client(hostname, portno);
    try {
        client->setupThis();
        QMessageBox::information(this, "Success.", "Connection established.");
    } catch (const std::runtime_error &e) {
        QMessageBox::warning(this, "Error.", e.what());
    }
}

void MainWindow::showData()
{
    // clear ui
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setRowCount(0);
    ui->totalSize_label->clear();

    ui->path_label->setText(currentPath);

    // total size labels
    ui->totalSize_label->setText(tr("Total size:\t") + QString::number(currentData["total"].get<quint64>() / 1e6) + tr(" Mb"));

    // table
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(currentData["files"].size());

    QStringList horHeaders;
    horHeaders << "Filename" << "Created on" << "Size, \nbytes";
    ui->tableWidget->setHorizontalHeaderLabels(horHeaders);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    int k = 0;
    for (auto &file : currentData["files"])
    {
        auto fileName = file["filename"].get<std::string>();

        size_t pos = fileName.find(currentPath.toStdString());
        if (pos != std::string::npos)
            fileName.erase(pos, currentPath.length());

        auto createdOn = file["creation_date"].get<std::string>();
        auto fileSize = file["file_size"].get<quint64>();

        ui->tableWidget->setItem(k, 0, new QTableWidgetItem(QString::fromStdString(fileName)));
        ui->tableWidget->setItem(k, 1, new QTableWidgetItem(QString::fromStdString(createdOn)));
        ui->tableWidget->setItem(k, 2, new QTableWidgetItem(QString::number(fileSize)));

        k++;
    }
}

bool MainWindow::getData()
{
    client_logger->debug("Getting the path...");

    if (currentPath.isEmpty()) {
        client_logger->warn("No path given");
        client_warn_logger->warn("No path given");
        QMessageBox::warning(this, "No path given", "Set the path before proceeding");
        return false;
    }

    client_logger->info("The path has been succesfully set to '{}'", currentPath.toStdString());
    for (auto el = defaultExtensionsMap.begin(); el != defaultExtensionsMap.end(); el++)
    {
        if (el.key()->isChecked())
        {
            this->extensions << el.value();
        }
    }



    std::string request = formRequest(this->currentPath, this->extensions);

    client_logger->info("Request has been succesfully set");
    client_logger->info("The request: {}", request);

    // this part !!!
    std::string response;
    try {
        response = client->get(request);
    } catch (std::runtime_error& e) {
        client_logger->warn("{}", e.what());
        QMessageBox::warning(this, "Server refused to answer", e.what());
    }
    // don't forget!!!

    // Parse response
    try {
        this->currentData = json::parse(response);
    } catch (...) {
        client_logger->error("Error parsing response from server.");
        QMessageBox::information(this, "Error.", "The response from server was malformed :(");
        return false;
    }

    return currentData["status"].get<bool>();
}

std::string MainWindow::formRequest(QString dirpath, QStringList extensions)
{
    client_logger->debug("Forming request...");

    QString request = QString("%1\n%2").arg(dirpath, extensions.join(" | "));
    return request.toStdString();
}

bool MainWindow::isConnected()
{
    client_logger->debug("Performing connection check.");
    if (!client) {
        client_logger->error("Client is null.");
        return false;
    }

    try {
        client->get("__test__");
        client_logger->debug("Connection is stable.");
        return true;
    } catch (const std::runtime_error &e) {
        client_logger->debug("Performing connection check");
        client_logger->error("Connection is not set up.");
    }
    return false;
}

void MainWindow::on_actionConnect_triggered()
{
    QString hostname = "";
    size_t portno;

    // input dialog
    QDialog* d = new QDialog();
    d->setWindowTitle("Connect");

    QLineEdit* hostnameLineEdit = new QLineEdit();
    hostnameLineEdit->setPlaceholderText(tr("Hostname"));

    QLineEdit* portnoLineEdit = new QLineEdit();
    portnoLineEdit->setPlaceholderText(tr("Port No"));

    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
                                                        | QDialogButtonBox::Cancel);

    QObject::connect(buttonBox, SIGNAL(accepted()), d, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), d, SLOT(reject()));

    QVBoxLayout* vbox = new QVBoxLayout();

    vbox->addWidget(hostnameLineEdit);
    vbox->addWidget(portnoLineEdit);
    vbox->addWidget(buttonBox);

    d->setLayout(vbox);

    int result = d->exec();

    if(result == QDialog::Accepted)
    {
        hostname = hostnameLineEdit->text();
        portno = portnoLineEdit->text().toUInt();

        setupClient(hostname.toLocal8Bit(), portno);
    }

}

void MainWindow::on_actionAdd_custom_triggered()
{
    QString customExtensions = QInputDialog::getText(this, tr("Add custom file extensions"),
                                                     tr("Enter extensions separating them with ' | '\n"));
    if (!customExtensions.isEmpty())
        this->extensions << customExtensions;
}

void MainWindow::on_actionChoose_path_triggered()
{
    if (!isConnected()) {
        client_logger->info("Declining Choosing path and connection is not established.");
        QMessageBox::warning(this, "Connection failure.", "Connection with server is not established.");
        return;
    }

    currentPath = QInputDialog::getText(this, tr("Choose path to be tracked"), tr("Enter path: "));

    if (getData()) {
        showData();
    } else {
        std::string reason = this->currentData["reason"].get<std::string>();
        QMessageBox::critical(this, "Error", QString::fromStdString(reason));
    }
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

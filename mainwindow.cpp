#include "mainwindow.h"
#include "./ui_mainwindow.h"

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
}

MainWindow::~MainWindow()
{
    delete ui;
    client->get("__close__");
}

void MainWindow::setupClient(const char *hostname, const size_t portno)
{
    client = new Client(hostname, portno);
    client->setupThis();
}

void MainWindow::showData()
{
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
    if (currentPath.isEmpty())
        throw std::runtime_error("No path given");

    for (auto el = defaultExtensionsMap.begin(); el != defaultExtensionsMap.end(); el++)
    {
        if (el.key()->isChecked())
        {
            this->extensions.insert(el.value());
        }
        else
        {
            this->extensions.remove(el.value());
        }
    }

    std::string request = formRequest(this->currentPath, this->extensions.values());

    std::string response;
    try {
        response = client->get(request);
    } catch (std::runtime_error& e) {
        qDebug() << e.what();
    }

    // Parse response
    this->currentData = json::parse(response);

    return currentData["status"].get<bool>();
}

std::string MainWindow::formRequest(QString dirpath, QStringList extensions)
{
    QString request = QString("%1\n%2").arg(dirpath, extensions.join(" | "));
    return request.toStdString();
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
    QStringList customExtensionsList = customExtensions.split(" | ");
    QRegularExpression re("\\*\\.\\w+");

    for (auto &ext : customExtensionsList) {
        auto match = re.match(ext);
        if (match.hasMatch()) {
            if (match.captured().size() != ext.size()) {
                QMessageBox::critical(this, "Error", QString("Invalid extension %1, aborting.").arg(ext));
                return;
            }
            this->extensions.insert(ext);
        }
    }
}

void MainWindow::on_actionChoose_path_triggered()
{
    // clear ui
    ui->tableWidget->clear();
    ui->tableWidget->setColumnCount(0);
    ui->tableWidget->setRowCount(0);
    ui->totalSize_label->clear();

    currentPath = QInputDialog::getText(this, tr("Choose path to be tracked"), tr("Enter path: "));

    ui->path_label->setText(currentPath);

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

#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->totalSize_label->setVisible(false);

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
}

void MainWindow::setupClient(const char *hostname, const size_t portno)
{
    client = new Client(hostname, portno);
}

void MainWindow::getData()
{
    if (currentPath.isEmpty())
        throw std::runtime_error("No path given");

    for (auto el = defaultExtensionsMap.begin(); el != defaultExtensionsMap.end(); el++)
    {
        if (el.key()->isChecked())
        {
            this->extensions << el.value();
        }
    }

    std::string request = formRequest(this->currentPath, this->extensions);

    std::string response;
    try {
        response = client->get(request);
    } catch (std::runtime_error& e) {
        qDebug() << e.what();
    }

    QFile file("/home/mirabilis/Downloads/Telegram Desktop/data_format.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    response = file.readAll().toStdString();

    // Parse response
    this->currentData = json::parse(response);

    // table
    ui->tableWidget->setColumnCount(3);
    ui->tableWidget->setRowCount(currentData["files"].size());

    QStringList horHeaders;
    horHeaders << "Filename" << "Created on" << "Size";
    ui->tableWidget->setHorizontalHeaderLabels(horHeaders);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    int k = 0;
    for (auto &file : currentData["files"])
    {
        auto fileName = file["filename"].get<std::string>();
        auto createdOn = file["creation_date"].get<std::string>();
        auto fileSize = file["file_size"].get<quint64>();

        ui->tableWidget->setItem(k, 0, new QTableWidgetItem(QString::fromStdString(fileName)));
        ui->tableWidget->setItem(k, 1, new QTableWidgetItem(QString::fromStdString(createdOn)));
        ui->tableWidget->setItem(k, 2, new QTableWidgetItem(QString::number(fileSize)));

        k++;
    }
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
    if (!customExtensions.isEmpty())
        this->extensions << customExtensions;
}

void MainWindow::on_actionChoose_path_triggered()
{
    currentPath = QInputDialog::getText(this, tr("Choose path to be tracked"), tr("Enter path: "));

    ui->path_label->setText(currentPath);

    getData();
}

void MainWindow::on_actionExit_triggered()
{
    this->close();
}

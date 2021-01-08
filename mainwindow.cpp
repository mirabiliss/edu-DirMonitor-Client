#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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

    std::string request = "Your fancy request";

    /* TODO handle possible errors */
    std::string response = client->get(request);

    /* Parse response
    currentData = json::parse(response);
    */
}


void MainWindow::on_actionAdd_triggered()
{
    /*
     * Fill currentPath
     */
}

void MainWindow::on_actionConnect_triggered()
{
    /*
     * Todo
     * 1. get hostname (string) and portno(size_t) from UI
     * 2. setupClient(hostname, portno);
     *
     */
}

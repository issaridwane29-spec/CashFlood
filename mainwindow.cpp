#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "comptewidget.h"
#include "comptedao.h"
#include "databasemanager.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Dans MainWindow::MainWindow(QWidget *parent) :
    auto *layout = new QVBoxLayout(ui->centralwidget);
    layout->setContentsMargins(0, 0, 0, 0);

    m_compteWidget = new CompteWidget(ui->centralwidget);
    layout->addWidget(m_compteWidget);

    // Selon la manière dont la base est ouverte dans ton projet (ex. DatabaseManager ou QSqlDatabase) :
    static CompteDao compteDao;
    m_compteWidget->setCompteDao(&compteDao);
}

MainWindow::~MainWindow()
{
    delete ui;
}


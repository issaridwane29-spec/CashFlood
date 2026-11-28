#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    auto *layoutCentral = new QVBoxLayout(ui->centralwidget);
    layoutCentral->setContentsMargins(0, 0, 0, 0);

    m_onglets = new QTabWidget(this);
    m_compteWidget = new CompteWidget(this);
    m_transactionWidget = new TransactionWidget(this);

    m_compteWidget->setCompteDao(&m_compteDao);
    m_transactionWidget->setDaos(&m_operationDao, &m_compteDao, &m_categorieDao);

    // Met à jour l'affichage des comptes dès qu'une transaction est ajoutée, modifiée ou supprimée
    connect(m_transactionWidget, &TransactionWidget::transactionsModifiees,
            m_compteWidget, &CompteWidget::rafraichirComptes);

    // Rafraîchit aussi les listes déroulantes et noms de comptes si un compte est modifié
    connect(m_onglets, &QTabWidget::currentChanged, this, [this](int index) {
        if (m_onglets->widget(index) == m_transactionWidget) {
            m_transactionWidget->rafraichirTransactions();
        } else if (m_onglets->widget(index) == m_compteWidget) {
            m_compteWidget->rafraichirComptes();
        }
    });

    m_onglets->addTab(m_compteWidget, "Mes Comptes");
    m_onglets->addTab(m_transactionWidget, "Transactions");

    layoutCentral->addWidget(m_onglets);
}

MainWindow::~MainWindow()
{
    delete ui;
}

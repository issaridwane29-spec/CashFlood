#include "comptewidget.h"
#include "ui_comptewidget.h"
#include "databasemanager.h"
#include <QHeaderView>

CompteWidget::CompteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompteWidget)
{
    ui->setupUi(this);
    ui->tableGestionCompte->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

CompteWidget::~CompteWidget()
{
    delete ui;
}

void CompteWidget::setDatabaseManager(DatabaseManager *dbManager)
{
    m_dbManager = dbManager;
    rafraichirComptes();
}

void CompteWidget::rafraichirComptes()
{
    if (!m_dbManager) {
        return;
    }

    const QList<Compte> comptes = m_dbManager->listerCompte();
    ui->tableGestionCompte->setRowCount(0);

    for (int i = 0; i < comptes.size(); ++i) {
        const Compte &cpt = comptes.at(i);
        ui->tableGestionCompte->insertRow(i);

        auto *itemNom = new QTableWidgetItem(cpt.nom);
        // On garde l'ID du compte caché dans la première cellule pour les actions Modifier / Supprimer / ↑ / ↓
        itemNom->setData(Qt::UserRole, cpt.id);

        auto *itemType = new QTableWidgetItem(cpt.type);
        auto *itemSoldeInit = new QTableWidgetItem(QString::number(cpt.soldeInitial, 'f', 2));
        auto *itemSoldeActuel = new QTableWidgetItem(QString::number(cpt.soldeActuel, 'f', 2));

        ui->tableGestionCompte->setItem(i, 0, itemNom);
        ui->tableGestionCompte->setItem(i, 1, itemType);
        ui->tableGestionCompte->setItem(i, 2, itemSoldeInit);
        ui->tableGestionCompte->setItem(i, 3, itemSoldeActuel);

        itemType->setTextAlignment(Qt::AlignCenter);
        itemSoldeInit->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemSoldeActuel->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }

    const double total = m_dbManager->calculerSoldeTotal();
    ui->labelSoldeTotal->setText(QString("🧆 Solde Total :  %1").arg(QString::number(total, 'f', 2)));
}

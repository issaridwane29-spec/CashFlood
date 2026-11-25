//comptewidget.cpp
#include "comptewidget.h"
#include "ui_comptewidget.h"
#include "comptedialog.h"
#include "compteservice.h"
#include "validateur.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidgetItem>

CompteWidget::CompteWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CompteWidget)
{
    ui->setupUi(this);
    ui->tableGestionCompte->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableGestionCompte->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableGestionCompte->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableGestionCompte->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->pushButtonewAccount, &QPushButton::clicked, this, &CompteWidget::onNouveauCompte);
    connect(ui->pushButtonAdd, &QPushButton::clicked, this, &CompteWidget::onModifierCompte);
    connect(ui->pushButtonDelete, &QPushButton::clicked, this, &CompteWidget::onSupprimerCompte);
    connect(ui->pushBtnMonter, &QPushButton::clicked, this, &CompteWidget::onMonterCompte);
    connect(ui->pushBtnDescendre, &QPushButton::clicked, this, &CompteWidget::onDescendreCompte);
}

CompteWidget::~CompteWidget()
{
    delete ui;
}

void CompteWidget::setCompteDao(CompteDao *compteDao)
{
    m_compteDao = compteDao;
    rafraichirComptes();
}

int CompteWidget::idCompteSelectionne(int *ligneCourante) const
{
    const int row = ui->tableGestionCompte->currentRow();
    if (ligneCourante) {
        *ligneCourante = row;
    }
    if (row < 0) {
        return -1;
    }
    QTableWidgetItem *item = ui->tableGestionCompte->item(row, 0);
    if (!item) {
        return -1;
    }
    return item->data(Qt::UserRole).toInt();
}

void CompteWidget::rafraichirComptes()
{
    if (!m_compteDao) {
        return;
    }

    // Dans CompteWidget::rafraichirComptes() (comptewidget.cpp)
    const QList<Compte> comptes = m_compteDao->lister();
    ui->tableGestionCompte->setRowCount(0);

    for (int i = 0; i < comptes.size(); ++i) {
        const Compte &cpt = comptes.at(i);
        const double soldeActuel = m_compteService.calculerSoldeActuel(cpt);

        ui->tableGestionCompte->insertRow(i);

        auto *itemNom = new QTableWidgetItem(cpt.nom());
        itemNom->setData(Qt::UserRole, cpt.id());
        itemNom->setData(Qt::UserRole + 1, cpt.ordre());

        auto *itemType = new QTableWidgetItem(cpt.type());
        auto *itemSoldeInit = new QTableWidgetItem(QString::number(cpt.soldeInitial(), 'f', 2));
        auto *itemSoldeActuel = new QTableWidgetItem(QString::number(soldeActuel, 'f', 2));

        ui->tableGestionCompte->setItem(i, 0, itemNom);
        ui->tableGestionCompte->setItem(i, 1, itemType);
        ui->tableGestionCompte->setItem(i, 2, itemSoldeInit);
        ui->tableGestionCompte->setItem(i, 3, itemSoldeActuel);

        itemType->setTextAlignment(Qt::AlignCenter);
        itemSoldeInit->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemSoldeActuel->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }

    const double total = m_compteService.calculerSoldeTotal(comptes);
    ui->labelSoldeTotal->setText(
        QString("🧆 Solde Total :  %1").arg(QString::number(total, 'f', 2))
    );
}

void CompteWidget::onNouveauCompte()
{
    if (!m_compteDao) {
        return;
    }

    CompteDialog dlg(this);
    if (dlg.exec() == QDialog::Accepted) {
        Compte nouveau = dlg.compteResultat();
        // Prochain ordre d'affichage basé sur les comptes existants
        const QList<Compte> existants = m_compteDao->lister();
        int maxOrdre = 0;
        for (const Compte &c : existants) {
            if (c.ordre() > maxOrdre) {
                maxOrdre = c.ordre();
            }
        }
        nouveau.setOrdre(maxOrdre + 1);

        const QString erreur = Validateur::validerCompte(nouveau); //[cite: 1, 2]
        if (!erreur.isEmpty()) {
            QMessageBox::warning(this, "Validation", erreur);
            return;
        }

        if (!m_compteDao->inserer(nouveau)) {
            QMessageBox::critical(this, "Erreur d'ajout", m_compteDao->derniereErreur());
            return;
        }
        rafraichirComptes();
    }
}

void CompteWidget::onModifierCompte()
{
    if (!m_compteDao) {
        return;
    }

    const int id = idCompteSelectionne();
    if (id <= 0) {
        QMessageBox::information(this, "Sélection requise", "Sélectionnez un compte à modifier.");
        return;
    }

    Compte courant;
    if (!m_compteDao->trouver(id, courant)) {
        QMessageBox::critical(this, "Erreur", m_compteDao->derniereErreur());
        return;
    }

    CompteDialog dlg(this);
    dlg.chargerCompte(courant);
    if (dlg.exec() == QDialog::Accepted) {
        Compte modifie = dlg.compteResultat();
        modifie.setId(courant.id());
        modifie.setOrdre(courant.ordre());

        const QString erreur = Validateur::validerCompte(modifie); //[cite: 1, 2]
        if (!erreur.isEmpty()) {
            QMessageBox::warning(this, "Validation", erreur);
            return;
        }

        if (!m_compteDao->modifier(modifie)) {
            QMessageBox::critical(this, "Erreur de modification", m_compteDao->derniereErreur());
            return;
        }
        rafraichirComptes();
    }
}

void CompteWidget::onSupprimerCompte()
{
    if (!m_compteDao) {
        return;
    }

    const int id = idCompteSelectionne();
    if (id <= 0) {
        QMessageBox::information(this, "Sélection requise", "Sélectionnez un compte à supprimer.");
        return;
    }

    const auto rep = QMessageBox::question(
        this,
        "Confirmation",
        "Supprimer ce compte entraînera aussi la suppression de ses opérations associées. Continuer ?",
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (rep == QMessageBox::Yes) {
        if (!m_compteDao->supprimer(id)) {
            QMessageBox::critical(this, "Erreur de suppression", m_compteDao->derniereErreur());
            return;
        }
        rafraichirComptes();
    }
}

void CompteWidget::onMonterCompte()
{
    if (!m_compteDao) {
        return;
    }

    int row = -1;
    const int idCourant = idCompteSelectionne(&row);
    if (idCourant <= 0 || row <= 0) {
        return;
    }

    QTableWidgetItem *itemVoisin = ui->tableGestionCompte->item(row - 1, 0);
    if (!itemVoisin) {
        return;
    }

    Compte c1, c2;
    if (!m_compteDao->trouver(idCourant, c1) || !m_compteDao->trouver(itemVoisin->data(Qt::UserRole).toInt(), c2)) {
        return;
    }

    const int ordreTemp = c1.ordre();
    c1.setOrdre(c2.ordre());
    c2.setOrdre(ordreTemp);

    const QString err1 = Validateur::validerCompte(c1); //[cite: 1, 2]
    const QString err2 = Validateur::validerCompte(c2); //[cite: 1, 2]
    if (!err1.isEmpty() || !err2.isEmpty()) {
        QMessageBox::warning(this, "Validation", !err1.isEmpty() ? err1 : err2);
        return;
    }

    if (m_compteDao->modifier(c1) && m_compteDao->modifier(c2)) {
        rafraichirComptes();
        ui->tableGestionCompte->selectRow(row - 1);
    }
}

void CompteWidget::onDescendreCompte()
{
    if (!m_compteDao) {
        return;
    }

    int row = -1;
    const int idCourant = idCompteSelectionne(&row);
    if (idCourant <= 0 || row < 0 || row >= ui->tableGestionCompte->rowCount() - 1) {
        return;
    }

    QTableWidgetItem *itemVoisin = ui->tableGestionCompte->item(row + 1, 0);
    if (!itemVoisin) {
        return;
    }

    Compte c1, c2;
    if (!m_compteDao->trouver(idCourant, c1) || !m_compteDao->trouver(itemVoisin->data(Qt::UserRole).toInt(), c2)) {
        return;
    }

    const int ordreTemp = c1.ordre();
    c1.setOrdre(c2.ordre());
    c2.setOrdre(ordreTemp);

    const QString err1 = Validateur::validerCompte(c1); //[cite: 1, 2]
    const QString err2 = Validateur::validerCompte(c2); //[cite: 1, 2]
    if (!err1.isEmpty() || !err2.isEmpty()) {
        QMessageBox::warning(this, "Validation", !err1.isEmpty() ? err1 : err2);
        return;
    }

    if (m_compteDao->modifier(c1) && m_compteDao->modifier(c2)) {
        rafraichirComptes();
        ui->tableGestionCompte->selectRow(row + 1);
    }
}

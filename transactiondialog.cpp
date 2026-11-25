#include "transactiondialog.h"
#include "ui_transactiondialog.h"
#include "validateur.h"
#include <QMessageBox>
#include <QPushButton>

TransactionDialog::TransactionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TransactionDialog)
{
    ui->setupUi(this);

    ui->dateEditOpe->setCalendarPopup(true);
    ui->dateEditOpe->setDate(QDate::currentDate());
    ui->SpinMontant->setMinimum(0.01);
    ui->SpinMontant->setMaximum(1e12);

    ui->comboType->clear();
    ui->comboType->addItem("Dépense", "depense");
    ui->comboType->addItem("Revenu", "revenu");

    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &TransactionDialog::onAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

TransactionDialog::~TransactionDialog()
{
    delete ui;
}

void TransactionDialog::setComptesEtCategories(const QList<Compte> &comptes,
                                               const QList<Categorie> &categories)
{
    ui->comboCompte->clear();
    for (const Compte &cpt : comptes) {
        ui->comboCompte->addItem(cpt.nom(), cpt.id());
    }

    ui->comboCategorie->clear();
    ui->comboCategorie->addItem("Aucune catégorie", 0);
    for (const Categorie &cat : categories) {
        ui->comboCategorie->addItem(cat.nom(), cat.id());
    }
}

void TransactionDialog::chargerOperation(const Operation &operation)
{
    m_idOperation = operation.id();

    const int idxType = ui->comboType->findData(operation.type().trimmed().toLower());
    if (idxType >= 0) {
        ui->comboType->setCurrentIndex(idxType);
    }

    const int idxCompte = ui->comboCompte->findData(operation.idCompte());
    if (idxCompte >= 0) {
        ui->comboCompte->setCurrentIndex(idxCompte);
    }

    const int idxCat = ui->comboCategorie->findData(operation.idCategorie());
    if (idxCat >= 0) {
        ui->comboCategorie->setCurrentIndex(idxCat);
    } else {
        ui->comboCategorie->setCurrentIndex(0);
    }

    ui->SpinMontant->setValue(operation.montant());
    if (operation.date().isValid()) {
        ui->dateEditOpe->setDate(operation.date());
    }
    ui->lineDescription->setText(operation.description());
}

Operation TransactionDialog::operationResultat() const
{
    return Operation(
        m_idOperation,
        ui->comboType->currentData().toString(),
        ui->SpinMontant->value(),
        ui->dateEditOpe->date(),
        ui->lineDescription->text().trimmed(),
        ui->comboCompte->currentData().toInt(),
        ui->comboCategorie->currentData().toInt()
    );
}

void TransactionDialog::onAccepted()
{
    if (ui->comboCompte->count() == 0) {
        QMessageBox::warning(this, "Compte requis",
                             "Veuillez d'abord créer un compte avant d'ajouter une opération.");
        return;
    }

    const Operation op = operationResultat();
    const QString erreur = Validateur::validerOperation(op); //
    if (!erreur.isEmpty()) {
        QMessageBox::warning(this, "Validation", erreur);
        if (erreur.contains("montant", Qt::CaseInsensitive)) {
            ui->SpinMontant->setFocus();
        }
        return;
    }

    accept();
}

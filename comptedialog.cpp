//compte dialog.cpp
#include "comptedialog.h"
#include "ui_comptedialog.h"
#include "validateur.h"
#include <QMessageBox>
#include <QPushButton>

CompteDialog::CompteDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CompteDialog)
{
    ui->setupUi(this);

    // Ajoute "Mobile Money" si absent de la liste du .ui (Espèces, Banques)
    if (ui->comboTypeCompte->findText("Mobile Money") == -1) {
        ui->comboTypeCompte->addItem("Mobile Money");
    }

    // Intercepte le clic sur OK pour valider le compte avant fermeture
    disconnect(ui->buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &CompteDialog::onAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

CompteDialog::~CompteDialog()
{
    delete ui;
}

void CompteDialog::chargerCompte(const Compte &compte)
{
    m_idCompte = compte.id();
    m_ordre = compte.ordre();

    ui->lineEditNom_Compte->setText(compte.nom());

    const int indexType = ui->comboTypeCompte->findText(compte.type(), Qt::MatchFixedString);
    if (indexType >= 0) {
        ui->comboTypeCompte->setCurrentIndex(indexType);
    } else if (!compte.type().isEmpty()) {
        ui->comboTypeCompte->addItem(compte.type());
        ui->comboTypeCompte->setCurrentIndex(ui->comboTypeCompte->count() - 1);
    }

    ui->spinSoldeInitial->setValue(compte.soldeInitial());
}

Compte CompteDialog::compteResultat() const
{
    return Compte(
        m_idCompte,
        ui->lineEditNom_Compte->text().trimmed(),
        ui->comboTypeCompte->currentText(),
        ui->spinSoldeInitial->value(),
        m_ordre
    );
}

void CompteDialog::onAccepted()
{
    const Compte cpt = compteResultat();
    const QString erreur = Validateur::validerCompte(cpt);
    if (!erreur.isEmpty()) {
        QMessageBox::warning(this, "Validation", erreur);
        if (erreur.contains("nom", Qt::CaseInsensitive)) {
            ui->lineEditNom_Compte->setFocus();
        } else if (erreur.contains("type", Qt::CaseInsensitive)) {
            ui->comboTypeCompte->setFocus();
        }
        return;
    }

    accept();
}

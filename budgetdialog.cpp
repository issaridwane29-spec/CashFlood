#include "budgetdialog.h"
#include "ui_budgetdialog.h"
#include <QColor>
#include <QListWidgetItem>
#include <QMessageBox>

BudgetDialog::BudgetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BudgetDialog)
{
    ui->setupUi(this);

    ui->comboPeriode->addItem("Hebdomadaire", "hebdomadaire");
    ui->comboPeriode->addItem("Mensuel", "mensuel");
    ui->comboPeriode->addItem("Annuel", "annuel");
    ui->comboPeriode->setCurrentIndex(1); // Mensuel par défaut

    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &BudgetDialog::onAccepted);
    connect(ui->buttonBox, &QDialogButtonBox::rejected, this, &BudgetDialog::reject);
}

BudgetDialog::~BudgetDialog()
{
    delete ui;
}

void BudgetDialog::chargerCategoriesDisponibles(const QList<Categorie> &categories)
{
    ui->listCategories->clear();
    for (const Categorie &cat : categories) {
        QListWidgetItem *item = new QListWidgetItem(cat.nom(), ui->listCategories);
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
        item->setCheckState(Qt::Unchecked);
        item->setData(Qt::UserRole, cat.id());
        const QColor couleurCat(cat.couleur());
        if (couleurCat.isValid()) {
            item->setForeground(couleurCat);
        }
    }
}

void BudgetDialog::chargerBudget(const Budget &budget)
{
    m_idBudget = budget.id();
    setWindowTitle(m_idBudget > 0 ? "Modifier le budget" : "Nouveau budget");
    ui->labelTitre->setText(m_idBudget > 0 ? "Modifier le budget" : "Définir un nouveau budget");

    ui->champNom->setText(budget.nom());
    ui->champPlafond->setValue(budget.plafond() > 0.0 ? budget.plafond() : 50000.0);

    int index = ui->comboPeriode->findData(budget.periode());
    if (index >= 0) {
        ui->comboPeriode->setCurrentIndex(index);
    }

    const QList<int> idsLies = budget.categories();
    for (int i = 0; i < ui->listCategories->count(); ++i) {
        QListWidgetItem *item = ui->listCategories->item(i);
        const int idCat = item->data(Qt::UserRole).toInt();
        item->setCheckState(idsLies.contains(idCat) ? Qt::Checked : Qt::Unchecked);
    }
}

Budget BudgetDialog::budgetResultat() const
{
    QList<int> idsSelectionnes;
    for (int i = 0; i < ui->listCategories->count(); ++i) {
        QListWidgetItem *item = ui->listCategories->item(i);
        if (item->checkState() == Qt::Checked) {
            idsSelectionnes.append(item->data(Qt::UserRole).toInt());
        }
    }

    return Budget(
        m_idBudget,
        ui->champNom->text().trimmed(),
        ui->champPlafond->value(),
        ui->comboPeriode->currentData().toString(),
        idsSelectionnes
    );
}

void BudgetDialog::onAccepted()
{
    if (ui->champNom->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Champ requis", "Veuillez saisir un nom pour ce budget.");
        return;
    }

    bool auMoinsUne = false;
    for (int i = 0; i < ui->listCategories->count(); ++i) {
        if (ui->listCategories->item(i)->checkState() == Qt::Checked) {
            auMoinsUne = true;
            break;
        }
    }

    if (!auMoinsUne) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez cocher au moins une catégorie pour ce budget.");
        return;
    }

    accept();
}

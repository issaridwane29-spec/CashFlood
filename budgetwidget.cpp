#include "budgetwidget.h"
#include "ui_budgetwidget.h"
#include "budgetdialog.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QProgressBar>
#include <QTableWidgetItem>

BudgetWidget::BudgetWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BudgetWidget)
{
    ui->setupUi(this);

    ui->comboFiltrePeriode->addItem("Toutes les périodes", "tous");
    ui->comboFiltrePeriode->addItem("Hebdomadaire", "hebdomadaire");
    ui->comboFiltrePeriode->addItem("Mensuel", "mensuel");
    ui->comboFiltrePeriode->addItem("Annuel", "annuel");

    ui->tableBudgets->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableBudgets->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableBudgets->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableBudgets->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableBudgets->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableBudgets->horizontalHeader()->setSectionResizeMode(5, QHeaderView::Stretch);
    ui->tableBudgets->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableBudgets->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableBudgets->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableBudgets->verticalHeader()->setVisible(false);

    connect(ui->btnNouveauBudget,   &QPushButton::clicked, this, &BudgetWidget::onNouveauBudget);
    connect(ui->btnModifier,        &QPushButton::clicked, this, &BudgetWidget::onModifierBudget);
    connect(ui->btnSupprimer,       &QPushButton::clicked, this, &BudgetWidget::onSupprimerBudget);
    connect(ui->comboFiltrePeriode, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BudgetWidget::onFiltreChange);
}

BudgetWidget::~BudgetWidget()
{
    delete ui;
}

void BudgetWidget::setDaos(BudgetDao *budgetDao, CategorieDao *categorieDao)
{
    m_budgetDao = budgetDao;
    m_categorieDao = categorieDao;
    rafraichirBudgets();
}

void BudgetWidget::onFiltreChange(int index)
{
    Q_UNUSED(index);
    rafraichirBudgets();
}

QString BudgetWidget::nomsCategoriesPourBudget(const Budget &budget, const QMap<int, QString> &mapNoms) const
{
    QStringList noms;
    for (int idCat : budget.categories()) {
        if (mapNoms.contains(idCat)) {
            noms.append(mapNoms.value(idCat));
        }
    }
    return noms.isEmpty() ? "Aucune" : noms.join(", ");
}

void BudgetWidget::rafraichirBudgets()
{
    if (!m_budgetDao) {
        return;
    }

    QMap<int, QString> mapNomsCategories;
    if (m_categorieDao) {
        const QList<Categorie> toutesCategories = m_categorieDao->lister();
        for (const Categorie &c : toutesCategories) {
            mapNomsCategories.insert(c.id(), c.nom());
        }
    }

    const QString filtre = ui->comboFiltrePeriode->currentData().toString();
    const QList<Budget> tous = m_budgetDao->lister();

    QList<Budget> filtres;
    for (const Budget &b : tous) {
        if (filtre == "tous" || b.periode() == filtre) {
            filtres.append(b);
        }
    }

    ui->tableBudgets->setRowCount(filtres.size());

    double totalPlafond = 0.0;
    double totalDepense = 0.0;

    for (int i = 0; i < filtres.size(); ++i) {
        const Budget &b = filtres.at(i);
        const SuiviBudget suivi = m_budgetService.calculerSuivi(b);

        totalPlafond += b.plafond();
        totalDepense += suivi.depense;

        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(b.id()));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->tableBudgets->setItem(i, 0, idItem);

        ui->tableBudgets->setItem(i, 1, new QTableWidgetItem(b.nom()));

        QString periodeLabel = b.periode();
        if (!periodeLabel.isEmpty()) {
            periodeLabel[0] = periodeLabel[0].toUpper();
        }
        QTableWidgetItem *periodeItem = new QTableWidgetItem(periodeLabel);
        periodeItem->setTextAlignment(Qt::AlignCenter);
        ui->tableBudgets->setItem(i, 2, periodeItem);

        ui->tableBudgets->setItem(i, 3, new QTableWidgetItem(nomsCategoriesPourBudget(b, mapNomsCategories)));

        QTableWidgetItem *montantItem = new QTableWidgetItem(
            QString("%1 / %2 FCFA")
                .arg(suivi.depense, 0, 'f', 0)
                .arg(b.plafond(), 0, 'f', 0)
        );
        montantItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tableBudgets->setItem(i, 4, montantItem);

        QProgressBar *barre = new QProgressBar(ui->tableBudgets);
        barre->setRange(0, 100);
        barre->setValue(qMin(static_cast<int>(suivi.pourcentage), 100));
        barre->setAlignment(Qt::AlignCenter);

        QString couleur;
        if (suivi.pourcentage < 75.0) {
            couleur = "#27ae60";
            barre->setFormat(QString("%1 %").arg(suivi.pourcentage, 0, 'f', 1));
        } else if (suivi.pourcentage <= 100.0) {
            couleur = "#e67e22";
            barre->setFormat(QString("%1 % (Attention)").arg(suivi.pourcentage, 0, 'f', 1));
        } else {
            couleur = "#c0392b";
            barre->setFormat(QString("DÉPASSÉ (%1 %)").arg(suivi.pourcentage, 0, 'f', 1));
        }

        barre->setStyleSheet(
            QString("QProgressBar { border: 1px solid #bdc3c7; border-radius: 4px; text-align: center; }"
                    "QProgressBar::chunk { background-color: %1; }").arg(couleur)
        );
        ui->tableBudgets->setCellWidget(i, 5, barre);
    }

    const double totalReste = totalPlafond - totalDepense;
    ui->labelResume->setText(
        QString("Plafond total : %1 FCFA   |   Dépensé : %2 FCFA   |   Reste : %3 FCFA")
            .arg(totalPlafond, 0, 'f', 0)
            .arg(totalDepense, 0, 'f', 0)
            .arg(totalReste, 0, 'f', 0)
    );
}

int BudgetWidget::idBudgetSelectionne(int *ligneCourante) const
{
    const int row = ui->tableBudgets->currentRow();
    if (ligneCourante) {
        *ligneCourante = row;
    }
    if (row < 0) {
        return -1;
    }
    QTableWidgetItem *item = ui->tableBudgets->item(row, 0);
    return item ? item->text().toInt() : -1;
}

void BudgetWidget::onNouveauBudget()
{
    if (!m_budgetDao || !m_categorieDao) {
        return;
    }

    const QList<Categorie> categories = m_categorieDao->lister();
    if (categories.isEmpty()) {
        QMessageBox::warning(this, "Aucune catégorie",
                             "Veuillez créer au moins une catégorie avant de définir un budget.");
        return;
    }

    BudgetDialog dialog(this);
    dialog.chargerCategoriesDisponibles(categories);
    dialog.chargerBudget(Budget(0, "", 50000.0, "mensuel", {}));

    if (dialog.exec() == QDialog::Accepted) {
        Budget nouveau = dialog.budgetResultat();
        if (m_budgetDao->inserer(nouveau)) {
            rafraichirBudgets();
            emit budgetsModifies();
        } else {
            QMessageBox::critical(this, "Erreur",
                                  "Impossible d'enregistrer le budget :\n" + m_budgetDao->derniereErreur());
        }
    }
}

void BudgetWidget::onModifierBudget()
{
    if (!m_budgetDao || !m_categorieDao) {
        return;
    }

    const int id = idBudgetSelectionne();
    if (id < 0) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner un budget à modifier.");
        return;
    }

    Budget existant;
    if (!m_budgetDao->trouver(id, existant)) {
        QMessageBox::critical(this, "Erreur", m_budgetDao->derniereErreur());
        return;
    }

    BudgetDialog dialog(this);
    dialog.chargerCategoriesDisponibles(m_categorieDao->lister());
    dialog.chargerBudget(existant);

    if (dialog.exec() == QDialog::Accepted) {
        Budget modifie = dialog.budgetResultat();
        if (m_budgetDao->modifier(modifie)) {
            rafraichirBudgets();
            emit budgetsModifies();
        } else {
            QMessageBox::critical(this, "Erreur",
                                  "Impossible de modifier le budget :\n" + m_budgetDao->derniereErreur());
        }
    }
}

void BudgetWidget::onSupprimerBudget()
{
    if (!m_budgetDao) {
        return;
    }

    const int id = idBudgetSelectionne();
    if (id < 0) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner un budget à supprimer.");
        return;
    }

    const auto reponse = QMessageBox::question(
        this,
        "Confirmation",
        "Voulez-vous vraiment supprimer ce budget ?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reponse == QMessageBox::Yes) {
        if (m_budgetDao->supprimer(id)) {
            rafraichirBudgets();
            emit budgetsModifies();
        } else {
            QMessageBox::critical(this, "Erreur",
                                  "Impossible de supprimer le budget :\n" + m_budgetDao->derniereErreur());
        }
    }
}

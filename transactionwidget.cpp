#include "transactionwidget.h"
#include "ui_transactionwidget.h"
#include "transactiondialog.h"
#include "validateur.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QCheckBox>
#include <QPushButton>
#include <QTableWidgetItem>
#include <QDate>

TransactionWidget::TransactionWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::TransactionWidget)
{
    ui->setupUi(this);

    ui->tableWidgetGestion->verticalHeader()->setDefaultSectionSize(34);
    ui->tableWidgetGestion->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetGestion->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetGestion->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetGestion->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->dateDebut->setCalendarPopup(true);
    ui->dateFin->setCalendarPopup(true);
    ui->dateDebut->setDate(QDate::currentDate().addMonths(-1));
    ui->dateFin->setDate(QDate::currentDate());

    connect(ui->BtnAjouterTransaction, &QPushButton::clicked, this, &TransactionWidget::onAjouterTransaction);
    connect(ui->BtnReset, &QPushButton::clicked, this, &TransactionWidget::onReinitialiserFiltres);

    connect(ui->lineRecherche, &QLineEdit::textChanged, this, &TransactionWidget::rafraichirTransactions);
    connect(ui->comboFiltreType, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransactionWidget::rafraichirTransactions);
    connect(ui->comboFiltreCompte, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransactionWidget::rafraichirTransactions);
    connect(ui->comboFilreCategorie, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &TransactionWidget::rafraichirTransactions);
    connect(ui->dateDebut, &QDateEdit::dateChanged, this, &TransactionWidget::rafraichirTransactions);
    connect(ui->dateFin, &QDateEdit::dateChanged, this, &TransactionWidget::rafraichirTransactions);
}

TransactionWidget::~TransactionWidget()
{
    delete ui;
}

void TransactionWidget::setDaos(OperationDao *operationDao,
                                CompteDao *compteDao,
                                CategorieDao *categorieDao)
{
    m_operationDao = operationDao;
    m_compteDao = compteDao;
    m_categorieDao = categorieDao;

    rechargerFiltresCombos();
    rafraichirTransactions();
}

void TransactionWidget::rechargerFiltresCombos()
{
    m_chargementFiltres = true;

    const int ancienCompte = ui->comboFiltreCompte->currentData().toInt();
    const int ancienneCat = ui->comboFilreCategorie->currentData().toInt();

    ui->comboFiltreCompte->clear();
    ui->comboFiltreCompte->addItem("Tous les comptes", -1);
    if (m_compteDao) {
        const QList<Compte> comptes = m_compteDao->lister();
        m_nomsComptes = m_service.construireMapComptes(comptes);
        for (const Compte &cpt : comptes) {
            ui->comboFiltreCompte->addItem(cpt.nom(), cpt.id());
        }
    } else {
        m_nomsComptes.clear();
    }

    ui->comboFilreCategorie->clear();
    ui->comboFilreCategorie->addItem("Toutes les catégories", -1);
    ui->comboFilreCategorie->addItem("Sans catégorie", 0);
    if (m_categorieDao) {
        const QList<Categorie> categories = m_categorieDao->lister();
        m_nomsCategories = m_service.construireMapCategories(categories);
        for (const Categorie &cat : categories) {
            ui->comboFilreCategorie->addItem(cat.nom(), cat.id());
        }
    } else {
        m_nomsCategories.clear();
    }

    const int idxC = ui->comboFiltreCompte->findData(ancienCompte);
    if (idxC >= 0) {
        ui->comboFiltreCompte->setCurrentIndex(idxC);
    }

    const int idxCat = ui->comboFilreCategorie->findData(ancienneCat);
    if (idxCat >= 0) {
        ui->comboFilreCategorie->setCurrentIndex(idxCat);
    }

    m_chargementFiltres = false;
}

void TransactionWidget::rafraichirTransactions()
{
    if (!m_operationDao || m_chargementFiltres) {
        return;
    }

    if (m_compteDao) {
        m_nomsComptes = m_service.construireMapComptes(m_compteDao->lister());
    }
    if (m_categorieDao) {
        m_nomsCategories = m_service.construireMapCategories(m_categorieDao->lister());
    }

    FiltreTransaction filtre;
    filtre.motCle = ui->lineRecherche->text();

    const QString filtreTypeTexte = ui->comboFiltreType->currentText();
    if (filtreTypeTexte == "Dépense") {
        filtre.typeNormalise = "depense";
    } else if (filtreTypeTexte == "Revenu") {
        filtre.typeNormalise = "revenu";
    }

    filtre.idCompte = ui->comboFiltreCompte->currentData().toInt();
    filtre.idCategorie = ui->comboFilreCategorie->currentData().toInt();
    filtre.dateDebut = ui->dateDebut->date();
    filtre.dateFin = ui->dateFin->date();
    filtre.filtrerParDate = true;

    const QList<Operation> operationsFiltrees =
        m_service.filtrerOperations(m_operationDao->lister(), filtre);

    ui->tableWidgetGestion->setRowCount(0);

    for (int ligne = 0; ligne < operationsFiltrees.size(); ++ligne) {
        const Operation &op = operationsFiltrees.at(ligne);
        const int idope = op.id();

        ui->tableWidgetGestion->insertRow(ligne);

        const QString libelleType = (op.type() == "revenu") ? "Revenu" : "Dépense";
        const QString nomCompte = m_nomsComptes.value(
            op.idCompte(),
            QString("Compte #%1").arg(op.idCompte())
        );
        const QString nomCat = (op.idCategorie() > 0)
            ? m_nomsCategories.value(op.idCategorie(), QString("Catégorie #%1").arg(op.idCategorie()))
            : QString("Aucune");

        auto *itemType = new QTableWidgetItem(libelleType);
        itemType->setData(Qt::UserRole, idope);

        auto *itemCompte = new QTableWidgetItem(nomCompte);
        auto *itemCat = new QTableWidgetItem(nomCat);
        auto *itemDesc = new QTableWidgetItem(op.description());
        auto *itemMontant = new QTableWidgetItem(QString::number(op.montant(), 'f', 2));
        auto *itemDate = new QTableWidgetItem(op.date().toString("dd/MM/yyyy"));

        itemMontant->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemDate->setTextAlignment(Qt::AlignCenter);

        ui->tableWidgetGestion->setItem(ligne, 0, itemType);
        ui->tableWidgetGestion->setItem(ligne, 1, itemCompte);
        ui->tableWidgetGestion->setItem(ligne, 2, itemCat);
        ui->tableWidgetGestion->setItem(ligne, 3, itemDesc);
        ui->tableWidgetGestion->setItem(ligne, 4, itemMontant);
        ui->tableWidgetGestion->setItem(ligne, 5, itemDate);

        auto *btnMod = new QPushButton("🖍️", ui->tableWidgetGestion);
        auto *btnSup = new QPushButton("❌", ui->tableWidgetGestion);

        ui->tableWidgetGestion->setCellWidget(ligne, 6, btnMod);
        ui->tableWidgetGestion->setCellWidget(ligne, 7, btnSup);

        connect(btnMod, &QPushButton::clicked, this, [this, idope]() {
            modifierTransaction(idope);
        });

        connect(btnSup, &QPushButton::clicked, this, [this, idope]() {
            supprimerTransaction(idope);
        });
    }
}

void TransactionWidget::onAjouterTransaction()
{
    if (!m_operationDao) {
        return;
    }

    rechargerFiltresCombos();

    TransactionDialog dlg(this);
    const QList<Compte> comptes = m_compteDao ? m_compteDao->lister() : QList<Compte>();
    const QList<Categorie> categories = m_categorieDao ? m_categorieDao->lister() : QList<Categorie>();
    dlg.setComptesEtCategories(comptes, categories);

    if (dlg.exec() == QDialog::Accepted) {
            Operation nouvelle = dlg.operationResultat();

            const QString erreur = Validateur::validerOperation(nouvelle); //
            if (!erreur.isEmpty()) {
                QMessageBox::warning(this, "Validation", erreur);
                return;
            }

            if (!m_operationDao->inserer(nouvelle)) {
                QMessageBox::critical(this, "Erreur d'ajout", m_operationDao->derniereErreur());
                return;
            }
            rafraichirTransactions();
            emit transactionsModifiees();
        }
}

void TransactionWidget::modifierTransaction(int idope)
{
    if (!m_operationDao || idope <= 0) {
        return;
    }

    Operation existante;
    if (!m_operationDao->trouver(idope, existante)) {
        QMessageBox::critical(this, "Erreur", m_operationDao->derniereErreur());
        return;
    }

    rechargerFiltresCombos();

    TransactionDialog dlg(this);
    const QList<Compte> comptes = m_compteDao ? m_compteDao->lister() : QList<Compte>();
    const QList<Categorie> categories = m_categorieDao ? m_categorieDao->lister() : QList<Categorie>();
    dlg.setComptesEtCategories(comptes, categories);
    dlg.chargerOperation(existante);

    if (dlg.exec() == QDialog::Accepted) {
            Operation modifiee = dlg.operationResultat();
            modifiee.setId(existante.id());

            const QString erreur = Validateur::validerOperation(modifiee); //[cite: 6, 7]
            if (!erreur.isEmpty()) {
                QMessageBox::warning(this, "Validation", erreur);
                return;
            }

            if (!m_operationDao->modifier(modifiee)) {
                QMessageBox::critical(this, "Erreur de modification", m_operationDao->derniereErreur());
                return;
            }
            rafraichirTransactions();
            emit transactionsModifiees();
        }
}

void TransactionWidget::supprimerTransaction(int idope)
{
    if (!m_operationDao || idope <= 0) {
        return;
    }

    if (!m_nePlusDemanderSuppression) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Confirmation");
        msgBox.setText("Voulez-vous vraiment supprimer cette transaction ?");
        msgBox.setIcon(QMessageBox::Question);

        QPushButton *btnOui = msgBox.addButton("Supprimer", QMessageBox::YesRole);
        msgBox.addButton("Annuler", QMessageBox::NoRole);

        auto *checkNePlusDemander = new QCheckBox("Ne plus jamais me demander ça", &msgBox);
        msgBox.setCheckBox(checkNePlusDemander);

        msgBox.exec();

        if (msgBox.clickedButton() != btnOui) {
            return;
        }

        if (checkNePlusDemander->isChecked()) {
            m_nePlusDemanderSuppression = true;
        }
    }

    if (!m_operationDao->supprimer(idope)) {
        QMessageBox::critical(this, "Erreur de suppression", m_operationDao->derniereErreur());
        return;
    }

    rafraichirTransactions();
    emit transactionsModifiees();
}

void TransactionWidget::onReinitialiserFiltres()
{
    m_chargementFiltres = true;
    ui->lineRecherche->clear();
    ui->comboFiltreType->setCurrentIndex(0);
    ui->comboFiltreCompte->setCurrentIndex(0);
    ui->comboFilreCategorie->setCurrentIndex(0);
    ui->dateDebut->setDate(QDate::currentDate().addMonths(-1));
    ui->dateFin->setDate(QDate::currentDate());
    m_chargementFiltres = false;

    rafraichirTransactions();
}

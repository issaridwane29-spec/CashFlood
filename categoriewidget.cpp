#include "categoriewidget.h"
#include "ui_categoriewidget.h"
#include <QColor>
#include <QColorDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidgetItem>

CategorieWidget::CategorieWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CategorieWidget)
{
    ui->setupUi(this);

    ui->tableCategories->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableCategories->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableCategories->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableCategories->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableCategories->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableCategories->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableCategories->verticalHeader()->setVisible(false);

    mettreAJourApercuCouleur();

    connect(ui->btnAjouter,       &QPushButton::clicked, this, &CategorieWidget::onAjouterCategorie);
    connect(ui->btnModifier,      &QPushButton::clicked, this, &CategorieWidget::onModifierCategorie);
    connect(ui->btnSupprimer,     &QPushButton::clicked, this, &CategorieWidget::onSupprimerCategorie);
    connect(ui->btnCouleur,       &QPushButton::clicked, this, &CategorieWidget::onChoisirCouleur);
    connect(ui->btnReinitialiser, &QPushButton::clicked, this, &CategorieWidget::onReinitialiser);
    connect(ui->tableCategories,  &QTableWidget::itemSelectionChanged, this, &CategorieWidget::onSelectionChanged);
}

CategorieWidget::~CategorieWidget()
{
    delete ui;
}

void CategorieWidget::setCategorieDao(CategorieDao *categorieDao)
{
    m_categorieDao = categorieDao;
    rafraichirCategories();
}

void CategorieWidget::rafraichirCategories()
{
    if (!m_categorieDao) {
        return;
    }

    const QList<Categorie> categories = m_categorieDao->lister();
    ui->tableCategories->setRowCount(categories.size());
    ui->labelCompteur->setText(QString("%1 catégorie(s)").arg(categories.size()));

    for (int i = 0; i < categories.size(); ++i) {
        const Categorie &cat = categories.at(i);

        QTableWidgetItem *idItem = new QTableWidgetItem(QString::number(cat.id()));
        idItem->setTextAlignment(Qt::AlignCenter);
        ui->tableCategories->setItem(i, 0, idItem);

        QTableWidgetItem *nomItem = new QTableWidgetItem(cat.nom());
        ui->tableCategories->setItem(i, 1, nomItem);

        QColor couleur(cat.couleur());
        if (!couleur.isValid()) {
            couleur = QColor("#3498db");
        }
        const QString codeCouleur = couleur.name();

        QTableWidgetItem *couleurItem = new QTableWidgetItem(codeCouleur);
        couleurItem->setBackground(couleur);
        couleurItem->setForeground(couleur.lightness() < 128 ? Qt::white : Qt::black);
        couleurItem->setTextAlignment(Qt::AlignCenter);
        ui->tableCategories->setItem(i, 2, couleurItem);
    }
}

int CategorieWidget::idCategorieSelectionnee(int *ligneCourante) const
{
    const int row = ui->tableCategories->currentRow();
    if (ligneCourante) {
        *ligneCourante = row;
    }
    if (row < 0) {
        return -1;
    }
    QTableWidgetItem *item = ui->tableCategories->item(row, 0);
    return item ? item->text().toInt() : -1;
}

void CategorieWidget::mettreAJourApercuCouleur()
{
    QColor couleur(m_couleurSelectionnee);
    if (!couleur.isValid()) {
        couleur = QColor("#3498db");
        m_couleurSelectionnee = couleur.name();
    }
    const QString texteCouleur = couleur.lightness() < 128 ? "#ffffff" : "#000000";
    ui->btnCouleur->setText(QString("%1 (Choisir...)").arg(m_couleurSelectionnee));
    ui->btnCouleur->setStyleSheet(
        QString("background-color: %1; color: %2; border: 1px solid #999999; border-radius: 4px;")
            .arg(m_couleurSelectionnee, texteCouleur)
    );
}

void CategorieWidget::onChoisirCouleur()
{
    const QColor choisie = QColorDialog::getColor(QColor(m_couleurSelectionnee), this, "Choisir une couleur");
    if (choisie.isValid()) {
        m_couleurSelectionnee = choisie.name();
        mettreAJourApercuCouleur();
    }
}

void CategorieWidget::onAjouterCategorie()
{
    if (!m_categorieDao) {
        return;
    }

    const QString nom = ui->champNom->text().trimmed();
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Champ requis", "Veuillez saisir un nom de catégorie.");
        return;
    }

    Categorie cat(0, nom, m_couleurSelectionnee);
    if (m_categorieDao->inserer(cat)) {
        rafraichirCategories();
        onReinitialiser();
        emit categoriesModifiees();
        QMessageBox::information(this, "Succès", "Catégorie ajoutée avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur",
                              "Impossible d'ajouter la catégorie :\n" + m_categorieDao->derniereErreur());
    }
}

void CategorieWidget::onModifierCategorie()
{
    if (!m_categorieDao) {
        return;
    }

    const int id = idCategorieSelectionnee();
    if (id < 0) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner une catégorie à modifier.");
        return;
    }

    const QString nom = ui->champNom->text().trimmed();
    if (nom.isEmpty()) {
        QMessageBox::warning(this, "Champ requis", "Le nom de la catégorie ne peut pas être vide.");
        return;
    }

    Categorie cat(id, nom, m_couleurSelectionnee);
    if (m_categorieDao->modifier(cat)) {
        rafraichirCategories();
        emit categoriesModifiees();
        QMessageBox::information(this, "Succès", "Catégorie modifiée avec succès.");
    } else {
        QMessageBox::critical(this, "Erreur",
                              "Impossible de modifier la catégorie :\n" + m_categorieDao->derniereErreur());
    }
}

void CategorieWidget::onSupprimerCategorie()
{
    if (!m_categorieDao) {
        return;
    }

    const int id = idCategorieSelectionnee();
    if (id < 0) {
        QMessageBox::warning(this, "Sélection requise", "Veuillez sélectionner une catégorie à supprimer.");
        return;
    }

    const auto reponse = QMessageBox::question(
        this,
        "Confirmation",
        "Voulez-vous vraiment supprimer cette catégorie ?",
        QMessageBox::Yes | QMessageBox::No
    );

    if (reponse == QMessageBox::Yes) {
        if (m_categorieDao->supprimer(id)) {
            rafraichirCategories();
            onReinitialiser();
            emit categoriesModifiees();
        } else {
            QMessageBox::critical(this, "Erreur",
                                  "Impossible de supprimer la catégorie :\n" + m_categorieDao->derniereErreur());
        }
    }
}

void CategorieWidget::onSelectionChanged()
{
    const int row = ui->tableCategories->currentRow();
    if (row < 0) {
        return;
    }
    QTableWidgetItem *nomItem = ui->tableCategories->item(row, 1);
    QTableWidgetItem *couleurItem = ui->tableCategories->item(row, 2);

    if (nomItem) {
        ui->champNom->setText(nomItem->text());
    }
    if (couleurItem) {
        const QColor couleur(couleurItem->text());
        m_couleurSelectionnee = couleur.isValid() ? couleur.name() : "#3498db";
        mettreAJourApercuCouleur();
    }
}

void CategorieWidget::onReinitialiser()
{
    ui->tableCategories->clearSelection();
    ui->champNom->clear();
    m_couleurSelectionnee = "#3498db";
    mettreAJourApercuCouleur();
}

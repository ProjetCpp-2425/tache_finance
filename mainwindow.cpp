#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "finance.h"
#include <QMessageBox>
#include <QSqlError>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tableViewtr->setModel(f.afficher());
   // void  on_pushButton_supprimer_clicked();

    // Connect buttons to respective slots
    connect(ui->projet, &QPushButton::clicked, this, &MainWindow::on_projet_clicked);
    connect(ui->employer, &QPushButton::clicked, this, &MainWindow::on_employer_clicked);
    connect(ui->materielle, &QPushButton::clicked, this, &MainWindow::on_materielle_clicked);
    connect(ui->fournisseur, &QPushButton::clicked, this, &MainWindow::on_fournisseur_clicked);
    connect(ui->facture, &QPushButton::clicked, this, &MainWindow::on_facture_clicked);
    connect(ui->ajouter_f, &QPushButton::clicked, this, &MainWindow::on_pushButton_ajouter_clicked);
    connect(ui->supprimer_f, &QPushButton::clicked, this, &MainWindow::on_pushButton_supprimer_clicked);
    connect(ui->modifier_f, &QPushButton::clicked, this, &MainWindow::on_pushButton_modifierer_clicked);


    ui->tableViewtr->setModel(f.afficher()); // Show records if necessary


}

MainWindow::~MainWindow()
{
    delete ui;
}

// Slot to switch to "projet_3" widget
void MainWindow::on_projet_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);  // Assuming "projet_3" widget is at index 0
}

// Slot to switch to "employer_3" widget
void MainWindow::on_employer_clicked()
{
    ui->stackedWidget->setCurrentIndex(1);  // Assuming "employer_3" widget is at index 1
}

// Slot to switch to "materielle_3" widget
void MainWindow::on_materielle_clicked()
{
    ui->stackedWidget->setCurrentIndex(2);  // Assuming "materielle_3" widget is at index 2
}

// Slot to switch to "fournisseur_3" widget
void MainWindow::on_fournisseur_clicked()
{
    ui->stackedWidget->setCurrentIndex(3);  // Assuming "fournisseur_3" widget is at index 3
}

// Slot to switch to "facture_3" widget
void MainWindow::on_facture_clicked()
{
    ui->stackedWidget->setCurrentIndex(4);  // Assuming "facture_3" widget is at index 4
}

void MainWindow::on_pushButton_ajouter_clicked()
{

    // Récupération des informations saisies dans les champs
    QString ID_T = ui->lineEdit_id->text();
        QString DATE_TRANSACTION = ui->lineEdit_date->text();
        int MONTANT = ui->lineEdit_montant->text().toInt();
        QString TYPE = ui->lineEdit_tp->text();
        QString CATEGORIE = ui->lineEdit_cat->text();
        QString MODE_PAIEMENT = ui->lineEdit_mode->text();




   finance f( ID_T, DATE_TRANSACTION,MONTANT,TYPE,CATEGORIE,MODE_PAIEMENT); // instancier un objet de la classe finance.
                                 // en utilisant les informations saisies dans l'interface

    bool test = f.ajouter(); // Insérer l'objet  instancié dans la table étudiant
                             // et récupérer la valeur de retour de query.exec()

    if (test) // Si requête exécutée avec succès
    {
        ui->tableViewtr->setModel(f.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("Ajout effectué\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    }
    else // Si requête non exécutée
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("Ajout non effectué.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }
}
void MainWindow::on_pushButton_supprimer_clicked(){
    QString ID_T = ui->supp->text();
    bool test=f.supprimer(ID_T) ;
    if (test) // Si requête exécutée avec succès
    {
        ui->tableViewtr->setModel(f.afficher());
        QMessageBox::information(nullptr, QObject::tr("OK"),
                                 QObject::tr("SUPP effectué\n"
                                             "Click Cancel to exit."), QMessageBox::Cancel);
    }
    else // Si requête non exécutée
    {
        QMessageBox::critical(nullptr, QObject::tr("Not OK"),
                              QObject::tr("SUPP non effectué.\n"
                                          "Click Cancel to exit."), QMessageBox::Cancel);
    }

}
void MainWindow::on_pushButton_modifierer_clicked() {
    //  Récupérer l'ID à modifier depuis le champ de texte
    QString ID_T = ui->lineEdit_2->text();
    qDebug() << "ID entré pour la modification : " << ID_T;

    // Vérification : si l'ID est vide, afficher un message d'erreur
    if (ID_T.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID.");
        return;
    }

    //   Récupérer l'attribut choisi pour la modification
    QString attribut = ui->comboBox_MOD->currentText();
    qDebug() << "Attribut sélectionné : " << attribut;

    // Récupérer la nouvelle valeur que l'utilisateur veut attribuer
    QString nouvelleValeur = ui->lineEdit_3->text();
    qDebug() << "Nouvelle valeur à attribuer : " << nouvelleValeur;

    // Vérification  si la nouvelle valeur est vide, afficher un message d'erreur
    if (nouvelleValeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une nouvelle valeur pour l'attribut.");
        return;
    }

    //  Préparer la requête SQL pour vérifier si l'ID existe dans la base de données
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM FINANCE WHERE ID_T = :id");
    query.bindValue(":id", ID_T);
    if (!query.exec() || !query.next() || query.value(0).toInt() == 0) {
        QMessageBox::warning(this, "Erreur", "ID non trouvé dans la base de données.");
        return;
    }

    // Préparer la requête SQL pour mettre à jour l'attribut avec la nouvelle valeur
    QString queryStr = QString("UPDATE FINANCE SET %1 = :valeur WHERE ID_T = :id").arg(attribut);
    query.prepare(queryStr);
    query.bindValue(":valeur", nouvelleValeur);
    query.bindValue(":id", ID_T);

    //Exécuter la requête de mise à jour et vérifier le résultat
    if (query.exec()) {
        qDebug() << "Mise à jour réussie pour l'ID" << ID_T << ", attribut" << attribut << "mis à jour avec" << nouvelleValeur;
        QMessageBox::information(this, "Succès", "La modification a été effectuée avec succès.");
        ui->tableViewtr->setModel(f.afficher());  // Mettre à jour l'affichage
    } else {
        qDebug() << "Erreur lors de la mise à jour:" << query.lastError();
        QMessageBox::critical(this, "Erreur", "La modification a échoué.");
    }
}

void MainWindow::on_pushButton_modifier_f_clicked() {
    // Récupérer l'ID depuis le champ de texte
    QString ID_T = ui->lineEdit_2->text();

    // Vérifier si l'ID est vide
    if (ID_T.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer un ID.");
        return;
    }

    // Récupérer l'attribut à modifier et la nouvelle valeur
    QString attribut = ui->comboBox_MOD->currentText();  // Attribut à modifier
    QString nouvelleValeur = ui->lineEdit_3->text();  // Nouvelle valeur

    // Vérifier si la nouvelle valeur est vide
    if (nouvelleValeur.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Veuillez entrer une nouvelle valeur.");
        return;
    }

    // Appeler la fonction modifier
    bool result = f.modifier(ID_T, attribut, nouvelleValeur);

    if (result) {
        //  modification est réussie actualiser la table
        ui->tableViewtr->setModel(f.afficher());
        QMessageBox::information(this, "Succès", "Modification effectuée avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "La modification a échoué.");
    }
}



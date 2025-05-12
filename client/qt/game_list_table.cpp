#include "game_list_table.h"

#include <QHeaderView>
#include <QTableWidgetItem>

#define ROWS 0
#define COLUMNS 3
#define ROW_HEIGHT 20

GameListTable::GameListTable(QWidget* parent): QTableWidget(ROWS, COLUMNS, parent) {
    this->verticalHeader()->hide();
    this->verticalHeader()->setDefaultSectionSize(ROW_HEIGHT);
    this->setShowGrid(false);
    this->horizontalHeader()->setHighlightSections(false);
    this->horizontalHeader()->setSectionsClickable(false);
    this->setHorizontalHeaderLabels(QStringList() << "Game Name"
                                                  << "Players"
                                                  << "Status");
    this->setSelectionMode(QAbstractItemView::SingleSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Fixed);
    this->setColumnWidth(1, 60);
    this->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Fixed);
    this->setColumnWidth(2, 100);
    // Game 1
    this->insertRow(0);
    QTableWidgetItem* game_name = new QTableWidgetItem("Game 1");
    game_name->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QTableWidgetItem* players = new QTableWidgetItem("2/16");
    players->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QTableWidgetItem* status = new QTableWidgetItem("Waiting");
    status->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    this->setItem(0, 0, game_name);
    this->setItem(0, 1, players);
    this->setItem(0, 2, status);

    // Game 2
    this->insertRow(1);
    QTableWidgetItem* game_name2 = new QTableWidgetItem("Game 2");
    game_name2->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QTableWidgetItem* players2 = new QTableWidgetItem("16/16");
    players2->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QTableWidgetItem* status2 = new QTableWidgetItem("In Progess");
    status2->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    this->setItem(1, 0, game_name2);
    this->setItem(1, 1, players2);
    this->setItem(1, 2, status2);
}

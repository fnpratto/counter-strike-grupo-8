#include "game_list_table.h"

#include <QHeaderView>
#include <QTableWidgetItem>
#include <vector>

#include "common/responses.h"

#include "lobby_window.h"

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

    connect(this, &QTableWidget::cellDoubleClicked, this, &GameListTable::on_cell_double_clicked);
}

QString GameListTable::get_selected_game() {
    int row = this->currentRow();
    if (row < 0) {
        return QString();
    }
    QTableWidgetItem* item = this->item(row, 0);
    if (item) {
        return item->text();
    }
    return QString();
}

void GameListTable::on_cell_double_clicked(int row, int column) {
    Q_UNUSED(row);
    Q_UNUSED(column);
    LobbyWindow* game_menu = qobject_cast<LobbyWindow*>(this->parent());
    if (game_menu) {
        game_menu->join_game(this->get_selected_game());
    }
}

void GameListTable::update_game_list(const std::vector<GameInfo>& game_list) {
    this->setRowCount(0);

    for (const auto& game_info: game_list) {
        int row = this->rowCount();
        this->insertRow(row);

        QTableWidgetItem* game_name_item =
                new QTableWidgetItem(QString::fromStdString(game_info.name));
        game_name_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        this->setItem(row, 0, game_name_item);
        QTableWidgetItem* players_count_item =
                new QTableWidgetItem(QString::number(game_info.players_count));
        players_count_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        this->setItem(row, 1, players_count_item);
        QTableWidgetItem* status_item = new QTableWidgetItem(
                QString::fromStdString(game_info.phase == PhaseType::WarmUp  ? "Warm Up" :
                                       game_info.phase == PhaseType::Buying  ? "Buying" :
                                       game_info.phase == PhaseType::Playing ? "Playing" :
                                                                               "Round Finished"));
        status_item->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
        this->setItem(row, 2, status_item);
    }
}

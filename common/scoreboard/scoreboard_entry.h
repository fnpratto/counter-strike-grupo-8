#pragma once

struct ScoreboardEntry {
    int money;
    int kills;
    int deaths;
    int score;

    explicit ScoreboardEntry(int money): money(money), kills(0), deaths(0), score(0) {}
};

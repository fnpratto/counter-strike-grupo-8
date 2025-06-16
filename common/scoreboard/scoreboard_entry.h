#pragma once

struct ScoreboardEntry {
    int money;
    int kills;
    int deaths;
    int score;

    ScoreboardEntry(int money, int kills, int deaths, int score):
            money(money), kills(kills), deaths(deaths), score(score) {}
};

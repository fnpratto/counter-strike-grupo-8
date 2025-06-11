#pragma once

struct ScoreboardEntry {
    int kills;
    int deaths;
    // TODO: mvp
    int score;

    ScoreboardEntry(): kills(0), deaths(0), score(0) {}
};

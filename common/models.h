#pragma once

enum class Team { TT, CT };
enum class GunType { AK47, M3, AWP, Glock };
enum class ItemSlot { Primary, Secondary, Melee, Bomb };
enum class PhaseType { WarmUp, Buying, Playing, BombPlanted, RoundEnd, GameEnd };
enum class BombPhaseType { NotPlanted, Planted, Exploded, Defused };
enum class CharacterType {
    // Terrorists
    Pheonix,
    L337_Krew,
    Arctic_Avenger,
    Guerrilla,

    // Counter-Terrorists
    Seal_Force,
    German_GSG_9,
    UK_SAS,
    French_GIGN
};

#pragma once

#include <string>

#include <yaml-cpp/yaml.h>

#include "common/models.h"
#include "common/physics/physics_config.h"

#define TICKRATE 30

struct GameConfig {
    int max_rounds;

    struct Scores {
        int kill;
        int win;
        int loss;

        Scores load(const YAML::Node& scores_node) {
            Scores scores_config;
            scores_config.kill = scores_node["kill"].as<int>();
            scores_config.win = scores_node["win"].as<int>();
            scores_config.loss = scores_node["loss"].as<int>();
            return scores_config;
        }
    } scores;

    struct Bonifications {
        int kill;
        int win;
        int loss;

        Bonifications load(const YAML::Node& bonif_node) {
            Bonifications bonif_config;
            bonif_config.kill = bonif_node["kill"].as<int>();
            bonif_config.win = bonif_node["win"].as<int>();
            bonif_config.loss = bonif_node["loss"].as<int>();
            return bonif_config;
        }
    } bonifications;

    struct PhaseTimes {
        unsigned int buying_duration;
        unsigned int round_duration;
        unsigned int round_end_duration;

        PhaseTimes load(const YAML::Node& phase_times_node) {
            PhaseTimes phase_times_config;
            phase_times_config.buying_duration =
                    phase_times_node["buying_duration"].as<unsigned int>();
            phase_times_config.round_duration =
                    phase_times_node["round_duration"].as<unsigned int>();
            phase_times_config.round_end_duration =
                    phase_times_node["round_end_duration"].as<unsigned int>();
            return phase_times_config;
        }
    } phase_times;

    struct PlayerConfig {
        int speed;
        int full_health;
        int initial_money;

        PlayerConfig load(const YAML::Node& player_config_node) {
            PlayerConfig player_conf;
            player_conf.speed = player_config_node["speed"].as<float>();
            player_conf.full_health = player_config_node["full_health"].as<int>();
            player_conf.initial_money = player_config_node["initial_money"].as<int>();
            return player_conf;
        }
    } player_config;

    struct ShopPrices {
        int ak47;
        int m3;
        int awp;
        int mag_ak47;
        int mag_m3;
        int mag_awp;
        int mag_glock;

        ShopPrices load(const YAML::Node& shop_prices_node) {
            ShopPrices shop_prices_config;
            shop_prices_config.ak47 = shop_prices_node["Guns"]["Ak47"].as<int>();
            shop_prices_config.m3 = shop_prices_node["Guns"]["M3"].as<int>();
            shop_prices_config.awp = shop_prices_node["Guns"]["Awp"].as<int>();
            shop_prices_config.mag_ak47 = shop_prices_node["Mag"]["Ak47"].as<int>();
            shop_prices_config.mag_m3 = shop_prices_node["Mag"]["M3"].as<int>();
            shop_prices_config.mag_awp = shop_prices_node["Mag"]["Awp"].as<int>();
            shop_prices_config.mag_glock = shop_prices_node["Mag"]["Glock"].as<int>();
            return shop_prices_config;
        }
    } shop_prices;

    struct ItemsConfig {
        struct GunConfig {
            int bullets_per_mag;
            int init_mag_ammo;
            int init_reserve_ammo;
            float attack_rate;
            int dir_variation_angle;
            int bullets_per_attack;
            float burst_interval;
            int min_damage;
            int max_damage;
            int max_range;
            float precision;
            float falloff;

            GunConfig load(const YAML::Node& gun_node) {
                GunConfig gun_config;
                gun_config.bullets_per_mag = gun_node["bullets_per_mag"].as<int>();
                gun_config.init_mag_ammo = gun_node["init_mag_ammo"].as<int>();
                gun_config.init_reserve_ammo = gun_node["init_reserve_ammo"].as<int>();
                gun_config.attack_rate = gun_node["attack_rate"].as<float>();
                gun_config.dir_variation_angle = gun_node["dir_variation_angle"].as<int>();
                gun_config.bullets_per_attack = gun_node["bullets_per_attack"].as<int>();
                gun_config.burst_interval = gun_node["burst_interval"].as<float>();
                gun_config.min_damage = gun_node["min_damage"].as<int>();
                gun_config.max_damage = gun_node["max_damage"].as<int>();
                gun_config.max_range = gun_node["max_range"].as<int>() * PhysicsConfig::meter_size;
                gun_config.precision = gun_node["precision"].as<float>();
                gun_config.falloff = gun_node["falloff"].as<float>();
                return gun_config;
            }
        };

        GunConfig glock;
        GunConfig ak47;
        GunConfig m3;
        GunConfig awp;

        struct KnifeConfig {
            int min_damage;
            int max_damage;
            float attack_rate;
            int max_range;
            float precision;
            float falloff;

            KnifeConfig load(const YAML::Node& knife_node) {
                KnifeConfig knife_config;
                knife_config.min_damage = knife_node["min_damage"].as<int>();
                knife_config.max_damage = knife_node["max_damage"].as<int>();
                knife_config.attack_rate = knife_node["attack_rate"].as<float>();
                knife_config.max_range =
                        knife_node["max_range"].as<int>() * PhysicsConfig::meter_size;
                knife_config.precision = knife_node["precision"].as<float>();
                knife_config.falloff = knife_node["falloff"].as<float>();
                return knife_config;
            }
        } knife;

        struct BombConfig {
            int secs_to_plant;
            int secs_to_explode;
            int secs_to_defuse;
            int damage;
            int max_range;
            float precision;
            float falloff;

            BombConfig load(const YAML::Node& bomb_node) {
                BombConfig bomb_config;
                bomb_config.secs_to_plant = bomb_node["secs_to_plant"].as<int>();
                bomb_config.secs_to_explode = bomb_node["secs_to_explode"].as<int>();
                bomb_config.secs_to_defuse = bomb_node["secs_to_defuse"].as<int>();
                bomb_config.damage = bomb_node["damage"].as<int>();
                bomb_config.max_range =
                        bomb_node["max_range"].as<int>() * PhysicsConfig::meter_size;
                bomb_config.precision = bomb_node["precision"].as<float>();
                bomb_config.falloff = bomb_node["falloff"].as<float>();
                return bomb_config;
            }
        } bomb;

        ItemsConfig load(const YAML::Node& items_config_node) {
            ItemsConfig items_conf;
            items_conf.glock = glock.load(items_config_node["Glock"]);
            items_conf.ak47 = ak47.load(items_config_node["Ak47"]);
            items_conf.m3 = m3.load(items_config_node["M3"]);
            items_conf.awp = awp.load(items_config_node["Awp"]);
            items_conf.knife = knife.load(items_config_node["Knife"]);
            items_conf.bomb = bomb.load(items_config_node["Bomb"]);
            return items_conf;
        }

        GunConfig get_gun_config(GunType gun_type) const {
            switch (gun_type) {
                case GunType::AK47:
                    return ak47;
                case GunType::M3:
                    return m3;
                case GunType::AWP:
                    return awp;
                case GunType::Glock:
                    return glock;
                default:
                    throw std::runtime_error("Unknown gun type");
            }
        }

    } items_config;

    static GameConfig load_config(const std::string& config_file) {
        YAML::Node config = YAML::LoadFile(config_file);
        GameConfig game_config;
        game_config.max_rounds = config["GameConfig"]["max_rounds"].as<int>();
        game_config.scores = game_config.scores.load(config["GameConfig"]["Scores"]);
        game_config.bonifications =
                game_config.bonifications.load(config["GameConfig"]["Bonifications"]);
        game_config.phase_times = game_config.phase_times.load(config["PhaseTimes"]);
        game_config.player_config = game_config.player_config.load(config["Player"]);
        game_config.shop_prices = game_config.shop_prices.load(config["ShopPrices"]);
        game_config.items_config = game_config.items_config.load(config["ItemsConfig"]);
        return game_config;
    }
};

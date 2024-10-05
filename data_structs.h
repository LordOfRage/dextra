#ifndef DATASTRUCTS
#define DATASTRUCTS
#include <cstdint>

enum pokemon_type {
  NORMAL,
  FIGHTING,
  FLYING,
  POISON,
  GROUND,
  ROCK,
  BIRD,  // unused in final game
  BUG,
  GHOST,
  blank0,
  blank1,
  blank2,
  blank3,
  blank4,
  blank5,
  blank6,
  blank7,
  blank8,
  blank9,
  blank10,
  FIRE,
  WATER,
  GRASS,
  ELECTRIC,
  PSYCHIC,
  ICE,
  DRAGON
};

enum growth_rate {
  MEDIUMFAST,
  blank11,
  blank12,
  MEDIUMSLOW,
  FAST,
  SLOW
};

typedef union pokemon_data_structure {
  uint8_t buffer[75];
  struct {
    uint8_t dexnum;
    uint8_t base_hp;
    uint8_t base_attack;
    uint8_t base_defence;
    uint8_t base_speed;
    uint8_t base_special;
    uint8_t type1;
    uint8_t type2;
    uint8_t catch_rate;
    uint8_t base_xp_yield;
    uint8_t frontsprite_dimension;
    uint16_t frontsprite_pointer; // pointer within ROM btw
    uint16_t backsprite_pointer; // always 4x4
    uint8_t initial_attacks[4];
    growth_rate growth_rate;
    bool tm_usage[50];
    bool hm_usage[5];
  };

} pokemon_data_structure;

#endif

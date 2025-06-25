#ifndef DATASTRUCTS
#define DATASTRUCTS
#include <cstdint>
#include <string>

enum pokemon_type : uint8_t {
  NORMAL = 0,
  FIGHTING,
  FLYING,
  POISON,
  GROUND,
  ROCK,
  BIRD,  // unused in final game
  BUG,
  GHOST,
  FIRE = 0x14,
  WATER,
  GRASS,
  ELECTRIC,
  PSYCHIC,
  ICE,
  DRAGON
};

enum growth_rate_values : uint8_t {
  MEDIUMFAST = 0,
  MEDIUMSLOW = 2,
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
    pokemon_type type1;
    pokemon_type type2;
    uint8_t catch_rate;
    uint8_t base_xp_yield;
    uint8_t frontsprite_dimension;
    uint16_t frontsprite_pointer; // pointer within ROM btw
    uint16_t backsprite_pointer; // always 4x4
    uint8_t initial_attacks[4];
    growth_rate_values growth_rate;
    bool tm_usage[50];
    bool hm_usage[5];
  };
} pokemon_data_structure;

typedef struct dexinfo_structure {
  char pkmnkind[11];
  uint8_t height_feet;
  uint8_t height_inches;
  uint16_t weight_tenthpounds;
  uint16_t dexentry_pointer;
} dexinfo_structure;

#endif

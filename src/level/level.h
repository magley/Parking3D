#pragma once

struct Entity;
struct RendTarget;

Entity* spawn_car(int car_index);
void init_level(RendTarget* rt_screen_2d);
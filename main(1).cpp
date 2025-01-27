
#include <vector>
#include <fstream>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <memory>
#include <sstream>


#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
 #include <allegro5/allegro_acodec.h>


///@@@ const int SCREEN_WIDTH = 800;
///@@@ const int SCREEN_HEIGHT = 600;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

///@@@const int MAP_WIDTH = 20;
///@@@const int MAP_HEIGHT = 15;


///const int MAP_WIDTH = 40;
const int MAP_WIDTH = 15;

const int MAP_HEIGHT = 60;

const float TILE_SIZE = 32.0f;


///-------------------------------------------------------------------------
///MENU

// Add to existing Game class or create a new MenuState class
enum GameState {
    MENU,
     LORE,
    PLAYING,
     END_SCREEN,
    EXIT
};



///--------------------------------------------------------------------------
///TILES

// Add new tile type to enum
enum TileType {
    TILE_SOLID = 1,
    TILE_TIMED = 2,
    TILE_CLOUD = 3,
     TILE_END = 4,  // New end game tile type
     TILE_COIN = 5,
     TILE_PLAYER = 'P'
};

class Tile {
public:
    float x, y;
    float width, height;
    TileType type;
    float creation_time;
    float lifetime;

    ///para mover
     float move_direction; // 1 for right, -1 for left
    float move_speed;


    /*
    Tile(float x, float y, TileType type)
        : x(x), y(y), width(TILE_SIZE), height(TILE_SIZE),
          type(type), creation_time(0), lifetime(0) {}

    */

      Tile(float x, float y, TileType type)
        : x(x), y(y), width(TILE_SIZE), height(TILE_SIZE),
          type(type), creation_time(0), lifetime(0),
          move_direction(1), move_speed(50.0f) {}

    void updateCloud(float dt) {
        if (type == TILE_CLOUD) {
            x += move_direction * move_speed * dt;

            // Reverse direction at screen edges
            if (x < 0 || x > SCREEN_WIDTH - width) {
                move_direction *= -1;
            }
        }
    }


   ///-----------------------------------

};


///--------------------------------------------------------------------------

class Particle {
public:
    float x, y;
    float vel_x, vel_y;
    float lifetime;
    float max_lifetime;

    Particle(float start_x, float start_y) :
        x(start_x), y(start_y),
        vel_x((rand() % 40 - 20) * 0.5f),  // Random horizontal velocity
        vel_y(-(rand() % 30 + 10)),        // Upward velocity
        lifetime(0),
        max_lifetime(1.0f) {}  // 1 second lifetime

    bool update(float dt) {
        x += vel_x * dt;
        y += vel_y * dt;
        vel_y += 100 * dt;  // Gravity effect
        lifetime += dt;
        return lifetime < max_lifetime;
    }
};

///-----------------------------------------------------------------------------

///imagens

class SpriteManager {
private:
    ALLEGRO_BITMAP* tile_solid_sprite=nullptr;
    ALLEGRO_BITMAP* tile_timed_sprite=nullptr;
  /// ALLEGRO_BITMAP* player_sprite;
  ///  ALLEGRO_BITMAP* bullet_sprite;

     ALLEGRO_BITMAP* player_sprite;
       ALLEGRO_BITMAP* player_sprite_sheet;

      ALLEGRO_BITMAP* player_frames[8];

       ALLEGRO_BITMAP* end_tile_sprite;


       int current_frame = 0;
    float frame_timer = 0;
    const float FRAME_DURATION = 0.1f; // 10 frames per second

      bool is_walking = false;
        bool facing_right = true;

    ALLEGRO_BITMAP* item_sprite;
    ALLEGRO_BITMAP* cloud_sprite;

public:
  ///  SpriteManager() {

  ///$$$       SpriteManager(ALLEGRO_BITMAP* solid, ALLEGRO_BITMAP* timed,
   ///$$$                 ALLEGRO_BITMAP* player, ALLEGRO_BITMAP* bullet)
     ///$$$     : tile_solid_sprite(solid), tile_timed_sprite(timed),
     ///$$$       player_sprite(player), bullet_sprite(bullet) {}

      // Default constructor
 ///   SpriteManager() :
  ///      tile_solid_sprite(nullptr),
  ///      tile_timed_sprite(nullptr)
   //     player_sprite(nullptr),
  //      bullet_sprite(nullptr)
///  {}

    ///        SpriteManager(ALLEGRO_BITMAP* solid, ALLEGRO_BITMAP* timed): tile_solid_sprite(solid), tile_timed_sprite(timed){}

     // Update constructor to explicitly set bitmaps
 ///  void setBitmaps(ALLEGRO_BITMAP* solid, ALLEGRO_BITMAP* timed) {
  ///      tile_solid_sprite = solid;
  ///      tile_timed_sprite = timed;
  ///  }


    ///@@@  void setBitmaps(ALLEGRO_BITMAP* solid, ALLEGRO_BITMAP* timed) {
    ///@@@     tile_solid_sprite = solid;
   ///@@@      tile_timed_sprite = timed;
   ///@@@  }

   /*
     void setBitmaps(ALLEGRO_BITMAP* solid, ALLEGRO_BITMAP* timed,
                    ALLEGRO_BITMAP* player, ALLEGRO_BITMAP* item,
                    ALLEGRO_BITMAP* cloud) {
            */

  void setBitmaps(ALLEGRO_BITMAP* solid, ALLEGRO_BITMAP* timed,
                    ALLEGRO_BITMAP* player, ALLEGRO_BITMAP* item,
                    ALLEGRO_BITMAP* cloud, ALLEGRO_BITMAP* end_tile){


        tile_solid_sprite = solid;
        tile_timed_sprite = timed;

        player_sprite = player;
          player_sprite_sheet = player;


          ///----------------------------------------------------------

           end_tile_sprite = end_tile;

            ///----------------------------------------------------------
/*
           // Split player sprite sheet into 8 frames
        for (int i = 0; i < 8; i++) {
            player_frames[i] = al_create_sub_bitmap(
                player_sprite_sheet,
                i * 32, 0,  // x position based on frame
                32, 32      // frame width and height
            );
        }
        */

              // Split player sprite sheet into 8 frames
        for (int i = 0; i < 8; i++) {
            player_frames[i] = al_create_sub_bitmap(
                player_sprite_sheet,
                i * 32, 0,  // x position based on frame
                32, 64      // frame width and height
            );
        }

          ///-----------------------------------------------------------

        item_sprite = item;
        cloud_sprite = cloud;
    }

void drawTile(float x, float y, TileType type, float camera_x, float camera_y) {

      ALLEGRO_BITMAP* sprite = nullptr;

     switch(type) {
        case TILE_SOLID: sprite = tile_solid_sprite; break;
        case TILE_TIMED: sprite = tile_timed_sprite; break;
        case TILE_CLOUD: sprite = cloud_sprite; break;
         case TILE_COIN: sprite = item_sprite; break;
        case TILE_END: sprite = end_tile_sprite; break;
    }

     if (sprite) {
            al_draw_bitmap(sprite, x - camera_x, y - camera_y, 0);
     }
}

///-------------------------------------------------------------

 ///@@@ void updatePlayerAnimation(float dt, bool is_moving) {
  void updatePlayerAnimation(float dt, bool is_moving, bool moving_left, bool moving_right) {
        is_walking = is_moving;

         // Update facing direction
        if (moving_left) facing_right = false;
        if (moving_right) facing_right = true;

        if (is_walking) {
            frame_timer += dt;
            if (frame_timer >= FRAME_DURATION) {
                current_frame = (current_frame + 1) % 8;
                frame_timer = 0;
            }
        } else {
            current_frame = 0;  // Reset to first frame when not walking
        }
    }

    /*
     void drawPlayer(float x, float y, float camera_x, float camera_y) {
        if (player_sprite) {
            al_draw_bitmap(player_sprite, x - camera_x, y - camera_y, 0);
        }
    }

    */

     void drawPlayer(float x, float y, float camera_x, float camera_y) {
        if (player_frames[current_frame]) {
                  int flags = facing_right ? 0 : ALLEGRO_FLIP_HORIZONTAL;
            al_draw_bitmap(player_frames[current_frame],
                           x - camera_x, y - camera_y, flags);
        }
    }

///=============================================

    void drawItem(float x, float y, float camera_x, float camera_y) {
        if (item_sprite) {
            al_draw_bitmap(item_sprite, x - camera_x, y - camera_y, 0);
        }
    }

    void drawCloud(float x, float y, float camera_x, float camera_y) {
        if (cloud_sprite) {
            al_draw_bitmap(cloud_sprite, x - camera_x, y - camera_y, 0);
        }
    }

    ~SpriteManager() {
        // Clean up sprites
        al_destroy_bitmap(tile_solid_sprite);
        al_destroy_bitmap(tile_timed_sprite);
          al_destroy_bitmap(player_sprite);
        al_destroy_bitmap(cloud_sprite);
          al_destroy_bitmap(item_sprite);

           al_destroy_bitmap(end_tile_sprite);

           for (int i = 0; i < 8; i++) {
            if (player_frames[i]) {
                al_destroy_bitmap(player_frames[i]);
            }
        }

    ///    al_destroy_bitmap(player_sprite);
    ///    al_destroy_bitmap(bullet_sprite);
    }

};



///----------------------------------------------------------------------------
///classe jogador

class Player {
public:
    float x, y;
    float width, height;
    float vel_x, vel_y;
    bool on_ground;

    bool on_parede;

  ///  Player() : x(50.0f), y(SCREEN_HEIGHT - 300.0f), width(32.0f), height(64.0f),
    Player() : x(50.0f), y(SCREEN_HEIGHT*4 - 70.0f), width(32.0f), height(64.0f),
               vel_x(0), vel_y(0), on_ground(true),on_parede(false) {}

    void update(float dt, float gravity, float player_speed) {
        vel_y += gravity * dt;  // Changed from subtraction to addition
        x += vel_x * dt;
        y += vel_y * dt;
    }
};

///-----------------------------------------------------------------------------

class Camera {
public:
    float x, y;
    float deadzone_x, deadzone_y;

    Camera() : x(0), y(0),
                 deadzone_x(100), // Pixels before camera starts moving
               deadzone_y(75)
                {}


 void update(const Player& player, float screen_width, float screen_height) {
        // Horizontal camera movement
        if (player.x < x + deadzone_x) {
            x = player.x - deadzone_x;
        }
        if (player.x + player.width > x + screen_width - deadzone_x) {
            x = player.x + player.width - (screen_width - deadzone_x);
        }

        // Vertical camera movement
        if (player.y < y + deadzone_y) {
            y = player.y - deadzone_y;
        }
        if (player.y + player.height > y + screen_height - deadzone_y) {
            y = player.y + player.height - (screen_height - deadzone_y);
        }
    }
};

///-----------------------------------------------------------------------------

///@@@ const float BULLET_SPEED = 500.0f;
const float BULLET_SPEED = 200.0f;

const float BULLET_SIZE = 8.0f;
///const float BULLET_LIFETIME = 1.0f;
const float BULLET_LIFETIME = 0.8f;

class Bullet {
public:
    float x, y;
    float vel_x, vel_y;
    float creation_time;
    bool should_transform;

    Bullet(float start_x, float start_y, float aim_x, float aim_y)
        : x(start_x), y(start_y), should_transform(false) {
        creation_time = al_get_time();

        // Calculate direction vector
        float dx = aim_x - start_x;
        float dy = aim_y - start_y;
        float length = std::sqrt(dx*dx + dy*dy);

        vel_x = (dx / length) * BULLET_SPEED;
        vel_y = (dy / length) * BULLET_SPEED;
    }

    void update(float dt) {
        x += vel_x * dt;
        y += vel_y * dt;

        // Check if bullet should transform to tile
        if (al_get_time() - creation_time >= BULLET_LIFETIME) {
            should_transform = true;
        }
    }

    bool isActive() const {
        return !should_transform;
    }
};

///-----------------------------------------------------------------------------

///==========================================================================

///+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
///-----------------------------------------------------------------------------
///classe jogo

class Game {
private:
    ALLEGRO_DISPLAY* display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_TIMER* timer;

     ALLEGRO_FONT* font;

      ///-----------

    Player player;
     // Add initial player position variables
    float initial_player_x;
    float initial_player_y;

    ///-----------

     std::string lore_text;
    float lore_timer;
   ///@@@ const float LORE_DELAY = 2.0f;
     const float LORE_DELAY = 2.0f;

     ///-----------

    std::vector<Tile> tiles;

      Camera camera;

        SpriteManager sprites;// Change this to be a member variable



  ///  std::vector<std::pair<float, float>> coin_particles;
     std::vector<Particle> coin_particles;

      std::vector<Particle> timed_tile_particles;

      ///-----------
      ///SONS
       ALLEGRO_AUDIO_STREAM* background_music;
        ALLEGRO_SAMPLE* shoot_sound;


       ///-----------

      ///------------------------------------

       ///variavel da pontuação
          int coin_count = 0;

    bool key_left = false;
    bool key_right = false;
    bool key_jump = false;

    bool lore_jump=false;

    const float PLAYER_SPEED = 200.0f;
    const float JUMP_STRENGTH = -600.0f;  // Negative for upward jump
    const float GRAVITY = 1000.0f;


    ///-------------------------------------------------------------------------
    ///menu

    /// GameState current_state;
      GameState current_state;  // Now this will be recognized

    ALLEGRO_BITMAP* menu_background;
    ALLEGRO_FONT* menu_font;
    int selected_option;

    ///--------------------------------------------------------------------------



     void loadMap(const std::string& filename) {


          // Clear existing tiles
        tiles.clear();

        std::ifstream file(filename);
        if (!file) {
            std::cerr << "Could not open map file" << std::endl;
            exit(1);
        }

        std::string line;
        for (int y = 0; y < MAP_HEIGHT && std::getline(file, line); y++) {
            for (int x = 0; x < MAP_WIDTH; x++) {
                if (line[x] == '1') {
                    tiles.emplace_back(
                        x * TILE_SIZE,
                        y * TILE_SIZE,
                        TILE_SOLID
                    );
                }

       ///---------------------------------------
                if (line[x] == '2') {
                    auto& tile = tiles.emplace_back(
                        x * TILE_SIZE,
                        y * TILE_SIZE,
                        TILE_TIMED
                    );
                    tile.creation_time = al_get_time();
                }
      ///---------------------------------------


         if (line[x] == '3') {
                    auto& tile = tiles.emplace_back(
                        x * TILE_SIZE,
                        y * TILE_SIZE,
                        TILE_CLOUD
                    );
      //              tile.creation_time = al_get_time();
                }


  ///---------------------------------------------

   if (line[x] == '4') {
        tiles.emplace_back(
            x * TILE_SIZE,
            y * TILE_SIZE,
            TILE_END
        );
    }

    ///------------------------------------------


     if (line[x] == '5') {
                    auto& tile = tiles.emplace_back(
                        x * TILE_SIZE,
                        y * TILE_SIZE,
                        TILE_COIN
                    );
      //              tile.creation_time = al_get_time();
                }
    ///------------------------------------------


  //    case 'P': // Player start position marker

    if (line[x] == 'P') {
                        initial_player_x = x * TILE_SIZE;
                        initial_player_y = (y * TILE_SIZE)-71;
                        break;

    }


///------------------------------------------



            }
        }
    }

    ///=====================================

     void loadLore() {
        std::ifstream file("lore.txt");
        if (file) {
            std::stringstream buffer;
            buffer << file.rdbuf();
            lore_text = buffer.str();
        }
    }

    void renderLore() {
        // Draw gradient background
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            float t = (float)(y - SCREEN_HEIGHT) / SCREEN_HEIGHT + 1.0f;
            unsigned char r = (unsigned char)(255 * (t-0.0f) * 0.2f);
            unsigned char g = (unsigned char)(255 * (t-0.0f) * 0.8f);
            unsigned char b = (unsigned char)(255 * (t-0.0f) * 0.2f);
            al_draw_line(0, y, SCREEN_WIDTH, y, al_map_rgb(r, g, b), 1);
        }

        // Draw lore text
        const int LINE_HEIGHT = 30;
        int y_pos = SCREEN_HEIGHT / 4;
        std::istringstream iss(lore_text);
        std::string line;

        while (std::getline(iss, line)) {
            al_draw_text(menu_font, al_map_rgb(255, 255, 255),
                SCREEN_WIDTH/2, y_pos, ALLEGRO_ALIGN_CENTER, line.c_str());
            y_pos += LINE_HEIGHT;
        }

            ///++++++++++++++++++++++++++++++++++++++++++

 ///@@   std::cout<<" tempo de tela: "<<lore_timer<<"\n";

     if (current_state == LORE) {
            lore_timer += 1.0f/60.0f;
      ///      if (lore_timer >= LORE_DELAY && (key_jump || key_shoot)) {
        ///          if (lore_timer >= LORE_DELAY ) {
                 if (lore_jump) {
                current_state = PLAYING;
            }
        }

    ///=============================================

        al_flip_display();
    }


  ///======================================


    void handleCollisions() {

        player.on_ground = false;
         player.on_parede = false;

    for (auto it = tiles.begin(); it != tiles.end();) {
           if (player.x < it->x + it->width &&
            player.x + player.width > it->x &&
            player.y + player.height > it->y &&
            player.y < it->y + it->height) {


          ///----------------------------------------------------------

          // Add end game tile handling
   ///@@@ if (it->type == TILE_END) {
     if (it->type == TILE_END && coin_count >= 5) {
        current_state = END_SCREEN;
        it = tiles.erase(it);
     ///@@@   std::cout<<" colidiu no final \n";
    }
     else if (it->type == TILE_END) {
        ++it;
    }


///--------------------------------------------------------


                // Ground collision (when falling down)
                    if (player.vel_y > 0 &&
                    player.y + player.height > it->y &&
                    player.y + player.height < it->y + it->height) {
                    player.y = it->y - player.height;
                    player.vel_y = 0;
                    player.on_ground = true;
                }


                  // Ceiling collision (when jumping up)
                if (player.vel_y < 0 &&
                    player.y < it->y + it->height &&
                    player.y > it->y) {
                    player.y = it->y + it->height;
                    player.vel_y = 0;
                }

             // Ground collision (when falling down)
                if (player.vel_x > 0 &&
            ///@@@        player.x + player.width > tile.x &&
                    player.x + player.width+16 > it->x &&
     ///              player.x + player.width < tile.x + tile.width) {
         ///@@@            player.x + player.width < tile.x + tile.width) {
                          player.x + player.width+16 < it->x + it->width) {
               //       player.x > tile.x + tile.width) {


                    player.x = it->x - player.width;
                    player.vel_x = 0;
                    player.on_parede = true;
                }

                    if (player.vel_x < 0 &&

               ///@@@       player.x + player.width > tile.x &&
                  ///@@@     player.x + player.width < tile.x + tile.width) {

                    player.x  < it->x+ it->width  &&
     ///@@@               player.x + player.width < tile.x + tile.width) {
                   player.x  > it->x && player.y >it->y ) {

                     player.x = it->x  + it->width;

                      player.vel_x = 0;
                   player.on_parede = true;
                }

 ///-----------------------------------------------------------

                     // Cloud tile collision
            if (it->type == TILE_CLOUD) {
                // Add cloud's movement to player's x velocity
                player.x += it->move_direction * it->move_speed * (1.0f/60.0f);
            }

    ///-----------------------------------------------------------


  ///--------------------------------------------

   if (it->type == TILE_COIN) {
                // Create coin particles
                for (int i = 0; i < 15; ++i) {
              ///      coin_particles.push_back({it->x, it->y});
                     coin_particles.push_back(Particle(it->x + it->width/2, it->y + it->height/2));
                }

                // Increment coin count
                coin_count++;

                // Remove coin tile
                it = tiles.erase(it);
            } else {
                ++it;
            }








  ///fim de conferir a colisao
        } else {
            ++it;
        }






    ///=============================

        }


        ///-----------------------------------------

        ///-------------------------------------------

    }


 ///==========================================

 /*
    void cleanupTimedTiles() {
        float current_time = al_get_time();
        tiles.erase(
            std::remove_if(tiles.begin(), tiles.end(),
                [current_time](Tile& tile) {
                    return tile.type == TILE_TIMED &&
                           ///@@@ current_time - tile.creation_time > 5.0f;
                           current_time - tile.creation_time > 5.0f;
                }),
            tiles.end()
        );
    }
*/

  void cleanupTimedTiles() {
        float current_time = al_get_time();
        for (auto it = tiles.begin(); it != tiles.end();) {
            if (it->type == TILE_TIMED && current_time - it->creation_time > 5.0f) {
                // Create explosion particles
                for (int i = 0; i < 15; ++i) {
                    timed_tile_particles.push_back(Particle(
                        it->x + it->width/2,
                        it->y + it->height/2
                    ));
                }
                it = tiles.erase(it);
            } else {
                ++it;
            }
        }
    }

     void drawParticles() {
        drawCoinParticles();
        // Draw timed tile particles in red color
        for (const auto& particle : timed_tile_particles) {
            float alpha = 1.0f - (particle.lifetime / particle.max_lifetime);
            ALLEGRO_COLOR color = al_map_rgba_f(0.5f, 0.5f, 1.0f, alpha);
            al_draw_filled_circle(
                particle.x - camera.x,
                particle.y - camera.y,
                2,
                color
            );
        }

     }

    ///-------------------------------------------------------

     void drawTiles() {
        for (const auto& tile : tiles) {
            sprites.drawTile(tile.x, tile.y, tile.type, camera.x, camera.y);
        }
    }

    ///+++++++++++++++++++++++++++++++++++++++
///PARTICULAS

      void updateCoinParticles(float dt) {
        coin_particles.erase(
            std::remove_if(coin_particles.begin(), coin_particles.end(),
                [dt](Particle& p) { return !p.update(dt); }),
            coin_particles.end()
        );
    }



    void drawCoinParticles() {
        for (const auto& particle : coin_particles) {
            // Fade out particles as they approach end of lifetime
            float alpha = 1.0f - (particle.lifetime / particle.max_lifetime);
            ALLEGRO_COLOR color = al_map_rgba_f(1.0f, 1.0f, 0.0f, alpha);

            al_draw_filled_circle(
                particle.x - camera.x,
                particle.y - camera.y,
                2,  // Radius of 2
                color
            );
        }
    }


 void updateParticles(float dt) {
        // Update both coin and timed tile particles
        updateCoinParticles(dt);
        timed_tile_particles.erase(
            std::remove_if(timed_tile_particles.begin(), timed_tile_particles.end(),
                [dt](Particle& p) { return !p.update(dt); }),
            timed_tile_particles.end()
        );
    }

    ///+++++++++++++++++++++++++++++++++++++++++
    ///bullets

      std::vector<std::unique_ptr<Bullet>> bullets;
    bool key_shoot = false;
    float last_shot_time = 0.0f;
    ///const float SHOOT_COOLDOWN = 0.5f;
    const float SHOOT_COOLDOWN = 0.5f;

    void handleShooting(float mouse_x, float mouse_y) {
        float current_time = al_get_time();

        // Check cooldown and shoot key
        if (key_shoot && current_time - last_shot_time >= SHOOT_COOLDOWN) {
            // Create bullet from player center
            bullets.push_back(std::make_unique<Bullet>(
                player.x + player.width/2,
                player.y + player.height/2,
                mouse_x, mouse_y
            ));


            ///-----------------------------------

             // Play shoot sound
            if (shoot_sound) {
    ///@@@            al_play_sample(shoot_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
                 al_play_sample(shoot_sound, 10.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
            }

            ///----------------------------------

            last_shot_time = current_time;
        }
    }

    void updateBullets(float dt) {
        // Update existing bullets
        for (auto it = bullets.begin(); it != bullets.end();) {
            (*it)->update(dt);

            // Check for tile transformation
            if ((*it)->should_transform) {

                // Add a timed tile where bullet was
                tiles.emplace_back((*it)->x, (*it)->y, TILE_TIMED);

               auto& new_tile = tiles.emplace_back((*it)->x, (*it)->y, TILE_TIMED);
                new_tile.creation_time = al_get_time();

                it = bullets.erase(it);
            } else {
                ++it;
            }
        }
    }

     void drawBullets() {
        for (const auto& bullet : bullets) {
            al_draw_filled_rectangle(
                bullet->x - camera.x,
                bullet->y - camera.y,
                bullet->x - camera.x + BULLET_SIZE,
                bullet->y - camera.y + BULLET_SIZE,
                al_map_rgb_f(1.0f, 0.0f, 0.0f)
            );
        }
    }


    ///-----------------------------------------------------------------------------------------------
    ///player

          void drawPlayer() {
     ///    sprites.player(tile.x, tile.y, tile.type, camera.x, camera.y);
     //     void drawPlayer(float x, float y, float camera_x, float camera_y) {
         ///       sprites.drawPlayer(player.x - camera.x, player.y - camera.y, player.x - camera.x + player.width, player.y - camera.y + player.height);
                 sprites.drawPlayer(player.x, player.y,camera.x, camera.y);
    }

    ///=========================================

    ///++++++++++++++++++++++++++++++++++++++++++++++
    ///MENU

     void initMenu() {
        menu_background = al_load_bitmap("menu_background.png");
        menu_font = al_create_builtin_font();
        selected_option = 0;
    }

    void drawMenu() {
        // Draw background
        if (menu_background) {
            al_draw_bitmap(menu_background, 0, 0, 0);
        }

        // Menu options
    ///@@@   ALLEGRO_COLOR active_color = al_map_rgb(255, 255, 0);   // Yellow
    ///@@@    ALLEGRO_COLOR inactive_color = al_map_rgb(255, 255, 255); // White

         ALLEGRO_COLOR active_color = al_map_rgb(50, 155, 50);   // Yellow
        ALLEGRO_COLOR inactive_color = al_map_rgb(255, 255, 255); // White

        al_draw_text(menu_font,
            selected_option == 0 ? active_color : inactive_color,
    ///@@@        SCREEN_WIDTH/2, SCREEN_HEIGHT/2,
             SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 200,
            ALLEGRO_ALIGN_CENTER, "PLAY");

        al_draw_text(menu_font,
            selected_option == 1 ? active_color : inactive_color,
   ///@@@         SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 30,
            SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 230,
            ALLEGRO_ALIGN_CENTER, "EXIT");
    }

    void handleMenuInput(ALLEGRO_KEYBOARD_EVENT& keyEvent) {
        switch (keyEvent.keycode) {
            case ALLEGRO_KEY_UP:
                selected_option = (selected_option - 1 + 2) % 2;
                break;
            case ALLEGRO_KEY_DOWN:
                selected_option = (selected_option + 1) % 2;
                break;
            case ALLEGRO_KEY_ENTER:
                  if (selected_option == 0) {
        ///            current_state = PLAYING;
                       current_state = LORE;
                lore_timer = 0;

                } else {
                    current_state = EXIT;
                }
            case ALLEGRO_KEY_SPACE:
                if (selected_option == 0) {
      ///              current_state = PLAYING;
         current_state = LORE;
                lore_timer = 0;

                } else {
                    current_state = EXIT;
                }
                break;
        }
    }


///==============================================

///++++++++++++++++++++++++++++++++++++++++++++++
///TELA FINAL

// Add method to render end screen
void renderEndScreen() {
    al_clear_to_color(al_map_rgb(0, 0, 0));

    // Draw menu background
    if (menu_background) {
        al_draw_bitmap(menu_background, 0, 0, 0);
    }

    // Draw "Thanks for playing" text
    al_draw_text(menu_font,
     ///@@@   al_map_rgb(255, 255, 255),
         al_map_rgb(55, 155, 55),
        SCREEN_WIDTH/2, SCREEN_HEIGHT/2+200,
    ///@@     SCREEN_WIDTH/2, SCREEN_HEIGHT/2,
        ALLEGRO_ALIGN_CENTER, "Thanks for playing!");

    al_flip_display();
}


///=================================================

///++++++++++++++++++++++++++++++++++++++++++++

 void resetGame() {
        // Reset player position
        player.x = initial_player_x;
        player.y = initial_player_y;
        player.vel_x = 0;
        player.vel_y = 0;
        player.on_ground = true;

        // Reset game state variables
        coin_count = 0;
        coin_particles.clear();
        bullets.clear();

        // Reset camera
        camera.x = 0;
        camera.y = 0;

        // Reload map
        loadMap("map.txt");
    }

///============================================

public:




    Game() {
        al_init();
        al_install_keyboard();

         // Existing initialization...
        al_install_mouse();



        ///-------------------------

//          initAudio();

       //   al_init_acodec_addon

        //  al_init

         al_install_audio();
        al_init_acodec_addon();
        al_reserve_samples(10);

        ///---------------------------

        al_init_primitives_addon();

        ///---------------------------------------

        // Initialize font addon
        al_init_font_addon();
///        al_init_ttf_addon();

         // Load a built-in font
        font = al_create_builtin_font();
        if (!font) {
            std::cerr << "Failed to create built-in font" << std::endl;
            // Handle error
        }

        ///---------------------------------------

         // Ensure image addon is initialized
  //      al_init_image_addon();

        if(! al_init_image_addon())
        {
            std::cout<<"erro ao iniciar o mecanismo de imageens \n";
        }



        display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT);
        event_queue = al_create_event_queue();
        timer = al_create_timer(1.0 / 60.0);

        al_register_event_source(event_queue, al_get_keyboard_event_source());
        al_register_event_source(event_queue, al_get_mouse_event_source());

        al_register_event_source(event_queue, al_get_display_event_source(display));
        al_register_event_source(event_queue, al_get_timer_event_source(timer));


          loadLore();

        loadMap("map.txt");


          ///--------------------------------------------------------------


          ///--------------------------------------------

     current_state = MENU;
        initMenu();

///--------------------------------------------


          // Load images here
        ALLEGRO_BITMAP* tile_solid_sprite = al_load_bitmap("chao.png");
        ALLEGRO_BITMAP* tile_timed_sprite = al_load_bitmap("bolha.png");
   ///     ALLEGRO_BITMAP* player_sprite = al_load_bitmap("player.png");
   ///     ALLEGRO_BITMAP* bullet_sprite = al_load_bitmap("bullet.png");

    ALLEGRO_BITMAP* player_sprite = al_load_bitmap("player.png");
        ALLEGRO_BITMAP* item_sprite = al_load_bitmap("item.png");
        ALLEGRO_BITMAP* cloud_sprite = al_load_bitmap("nuvem.png");


        ///-----------------------------------------

         ALLEGRO_BITMAP* end_tile_sprite = al_load_bitmap("end_tile.png");

        ///-----------------------------------------


         // Use setter method to load bitmaps
    ///    sprites.setBitmaps(tile_solid_sprite, tile_timed_sprite);


    /*
         sprites.setBitmaps(tile_solid_sprite, tile_timed_sprite,
                           player_sprite, item_sprite, cloud_sprite);
     */

         sprites.setBitmaps(tile_solid_sprite, tile_timed_sprite,
                       player_sprite, item_sprite,
                       cloud_sprite, end_tile_sprite);

// In Game constructor
///@@@ tile_solid_sprite = load_bitmap_checked("chao.png");
///@@@tile_solid_sprite = load_bitmap_checked("bolha.png");

        ///------------------------------------------------------------------

         // ... existing initialization ...
///        initAudio();


         // Load background music
        background_music = al_load_audio_stream("background.wav", 4, 2048);
        if (background_music) {
         ///   al_set_audio_stream_playing(background_music, false);
            al_set_audio_stream_playmode(background_music, ALLEGRO_PLAYMODE_LOOP);
              al_set_audio_stream_playing(background_music, false);
            al_set_audio_stream_loop_secs(background_music, 0, 0);
        }

         /*
        // Load shoot sound
        shoot_sound = al_load_sample("shoot.wav");

        */

        /*
         // Load background music with looping
    background_music = al_load_audio_stream("background.wav", 4, 2048);
    if (background_music) {
        al_set_audio_stream_playmode(background_music, ALLEGRO_PLAYMODE_LOOP);
        al_set_audio_stream_gain(background_music, 1.0); // Adjust volume
    }
    */

    // Load shoot sound with higher volume
    shoot_sound = al_load_sample("shoot.wav");
    if (!shoot_sound) {
        // Increase volume and ensure it plays clearly
     ///   sample_volume = 10.0; // Adjust as needed
   //     al_set_sample_instance_gain(shoot_sound,10);
       // al_set_sample
       std::cout<<" erro ao carregar som de tiro \n";
    }

        ///---------------------------------------------------------------------




    }

///=======================================================

    ///---------------------------------------------------------------------------

         ALLEGRO_BITMAP* load_bitmap_checked(const char* filename) {
    ALLEGRO_BITMAP* bitmap = al_load_bitmap(filename);
    if (!bitmap) {
        std::cerr << "Failed to load bitmap: " << filename << std::endl;
        char buffer[256];
        getcwd(buffer, sizeof(buffer));
        std::cerr << "Current directory: " << buffer << std::endl;
    } else {
        int width = al_get_bitmap_width(bitmap);
        int height = al_get_bitmap_height(bitmap);
        std::cerr << "Loaded " << filename << " - Dimensions: " << width << "x" << height << std::endl;
    }
    return bitmap;
}

    ///------------------------------------------------------------------------------

    void run() {
        bool running = true;
        ALLEGRO_EVENT event;
        al_start_timer(timer);

        while (running) {
            al_wait_for_event(event_queue, &event);

            switch (event.type) {
                case ALLEGRO_EVENT_DISPLAY_CLOSE:
                    running = false;
                    break;
                case ALLEGRO_EVENT_KEY_DOWN:
                    {
       ///                  if (current_state == MENU) {
      ///                  handleMenuInput(event.keyboard);
       ///             } else {
       ///                 handleKeyDown(event.keyboard);
       ///             }

       ///-----------------------------------------------------

        if (current_state == END_SCREEN) {
                    current_state = MENU;  // Return to menu after end screen
                } else if (current_state == MENU) {
                    handleMenuInput(event.keyboard);
       ///@@@         }else {

                   }else  if(current_state==PLAYING || current_state==LORE){
                    handleKeyDown(event.keyboard);
                }

       ///---------------------------------------------------------

               }
       ///@@@             handleKeyDown(event.keyboard);
                    break;
                case ALLEGRO_EVENT_KEY_UP:
             ///@@@$$$        {

        ///              if (current_state == MENU) {
       ///              handleMenuInput(event.keyboard);
      ///           }
     ///              else {
            ///@@@   if (current_state == PLAYING)
          ///@@@$$$         if (current_state == PLAYING)
                    ///   handleKeyDown(event.keyboard);
                        handleKeyUp(event.keyboard);
       ///          }

            ///@@@$$$          }
       ///@@@              handleKeyUp(event.keyboard);
                    break;

                     // Existing event types...
            case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
                handleMouseDown(event.mouse);
                break;
            case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
                handleMouseUp(event.mouse);
                break;

                case ALLEGRO_EVENT_TIMER:
    ///@@@                 update();
      ///@@@               render();

      {

          ///-----------------------------------------------------

           switch (current_state) {

                 case LORE:
                renderLore();
                break;

                    case MENU:

                        {
                             resetGame();
                               renderMenu();
                        }

                        break;
                    case PLAYING:

                        {


                        update();
                        render();


                        ///------------------
                          // Start background music if not already playing
                if (background_music && !al_get_audio_stream_playing(background_music)) {
                    al_attach_audio_stream_to_mixer(background_music, al_get_default_mixer());
                    al_set_audio_stream_playing(background_music, true);
                }

                        ///-----------------

                           }
                        break;
                    case END_SCREEN:
                        {
                              renderEndScreen();

                      ///------------------
                             // Stop music when not playing
                            if (background_music) {
                                al_set_audio_stream_playing(background_music, false);
                            }

                        ///------------------


                        }

                        break;
                    case EXIT:
                        running = false;
                        break;

                  break;
        }

          ///-----------------------------------------------------


      }

      //      }



            /*
                      if (current_state == MENU) {
                        renderMenu();
                    } else if (current_state == PLAYING) {
                        update();
                        render();
                    } else if (current_state == EXIT) {
                        running = false;
                    }

                    */

///--------------------------------------------------------
                    break;
            }
        }
    }

    void handleKeyDown(ALLEGRO_KEYBOARD_EVENT& keyEvent) {
        switch (keyEvent.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                exit(0);
            case ALLEGRO_KEY_A:
                key_left = true;
                break;
            case ALLEGRO_KEY_D:
               key_right = true;
                break;
            case ALLEGRO_KEY_W:
                key_jump = true;
                break;

   ///@@@          case ALLEGRO_KEY_SPACE:
     case ALLEGRO_KEY_ENTER:
    ///@@@              key_shoot = true;
            lore_jump=true;
            break;

        }
    }

    void handleKeyUp(ALLEGRO_KEYBOARD_EVENT& keyEvent) {
        switch (keyEvent.keycode) {
            case ALLEGRO_KEY_A:
                key_left = false;
                break;
            case ALLEGRO_KEY_D:
                key_right = false;
                break;
                case ALLEGRO_KEY_W:
                key_jump = false;
                break;


                  case ALLEGRO_KEY_ENTER:
    ///@@@              key_shoot = true;
            lore_jump=false;
            break;

        ///@@@     case ALLEGRO_KEY_SPACE:
      ///@@@            key_shoot = false;
        ///@@@         break;


        }
    }


    ///========================================================================

    // Modify input handling methods
void handleMouseDown(ALLEGRO_MOUSE_EVENT& mouseEvent) {
    if (mouseEvent.button == 1) {  // Left mouse button
        key_shoot = true;
    }
}

void handleMouseUp(ALLEGRO_MOUSE_EVENT& mouseEvent) {
    if (mouseEvent.button == 1) {  // Left mouse button
        key_shoot = false;
    }
}

    ///=========================================================================

    void update() {


      for (auto& tile : tiles) {
        tile.updateCloud(1.0f/60.0f);
    }

    //  updateParticles(1.0f/60.0f);

    /*
    ///++++++++++++++++++++++++++++++++++++++++++

    std::cout<<" tempo de tela: "<<lore_timer<<"\n";

     if (current_state == LORE) {
            lore_timer += 1.0f/60.0f;
            if (lore_timer >= LORE_DELAY && (key_jump || key_shoot)) {
                current_state = PLAYING;
            }
        }

    ///=============================================

    */


   /// al_flush_event_queue(event_queue);

          // Update camera
   ///    camera.update(player, SCREEN_WIDTH, SCREEN_HEIGHT, 1.0f/60.0f);
   ///@@@      camera.update(player, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 1.0f/60.0f);

         // Update camera
        camera.update(player, SCREEN_WIDTH, SCREEN_HEIGHT);

        ///-----------------------------------------

         // Get mouse position for shooting
        ALLEGRO_MOUSE_STATE mouse_state;
        al_get_mouse_state(&mouse_state);

    ///@@@    handleShooting(mouse_state.x, mouse_state.y);


     handleShooting(
            mouse_state.x + camera.x,
            mouse_state.y + camera.y
        );

         updateBullets(1.0f/60.0f);

        ///-------------------------------------------



        // Horizontal movement
          if(!player.on_parede)
          {
                             if (key_left) {
                        player.vel_x = -PLAYER_SPEED;
                    } else if (key_right) {
                        player.vel_x = PLAYER_SPEED;
                    } else {
                        player.vel_x = 0;
                    }
          }


        // Jumping
        if (key_jump && player.on_ground) {
            player.vel_y = JUMP_STRENGTH;
            player.on_ground = false;
        }


        // Update player
        player.update(1.0f/60.0f, GRAVITY, PLAYER_SPEED);

        // Determine if player is moving for animation
    bool is_moving = key_left || key_right;
 ///@@@   sprites.updatePlayerAnimation(1.0f/60.0f, is_moving);
     sprites.updatePlayerAnimation(1.0f/60.0f, is_moving, key_left, key_right);

        // Check collisions
        handleCollisions();

        // Clean up timed tiles
        cleanupTimedTiles();

         updateCoinParticles(1.0f/60.0f);

           updateParticles(1.0f/60.0f);


    }

  ///+++++++++++++++++++++++++++++++++++++++++++++

   void renderMenu() {
        al_clear_to_color(al_map_rgb(0, 0, 0));
        drawMenu();
        al_flip_display();
    }


 ///+++++++++++++++++++++++++++++++++++++++++++++++++++

    void render() {


  ///@@@       al_clear_to_color(al_map_rgb(200, 200, 255));

  ///------------------------------------------------------------------------

    // Create gradient from orange (bottom) to dark blue (top)
    for (int y = 0; y < SCREEN_HEIGHT; y++) {
   ///   for (int y = SCREEN_HEIGHT; y >0; y--) {
        // Calculate interpolation factor
    ///    float t = (float)y / SCREEN_HEIGHT;
          // Calculate interpolation factor, inverting with t-1.0f
        float t = (float)(y - SCREEN_HEIGHT) / SCREEN_HEIGHT + 1.0f;

        // Interpolate colors
        // Bottom: Orange (sunset)
        // Top: Dark blue (evening sky)
    ///    unsigned char r = (unsigned char)(255 * (1.0f - t) * 0.8f + (0.0f * t));    // Orange to dark
     ///   unsigned char g = (unsigned char)(255 * (1.0f - t) * 0.4f + (0.0f * t));    // Orange to dark
    ///    unsigned char b = (unsigned char)(255 * (1.0f - t) * 0.1f + (0.4f * t));    // Orange to dark blue

   //      unsigned char r = (unsigned char)(255 * (1.0f - t) * 0.1f + (0.8f * t));    // Dark to orange
   //     unsigned char g = (unsigned char)(255 * (1.0f - t) * 0.0f + (0.4f * t));    // Dark to orange
  // ///     unsigned char b = (unsigned char)(255 * (1.0f - t) * 0.4f + (0.1f * t));    // Dark blue to orange

  ///        unsigned char r = (unsigned char)(255 * (t-0.0f ) * 0.1f + (0.8f * t));    // Dark to orange
   ///     unsigned char g = (unsigned char)(255 * (t-0.0f ) * 0.0f + (0.4f * t));    // Dark to orange
  ///      unsigned char b = (unsigned char)(255 * (t-0.0f ) * 0.4f + (0.1f * t));    // Dark blue to orange

        unsigned char r = (unsigned char)(255 * (t-0.0f ) * 0.8 + (0.0f * t));    // Orange to dark
      unsigned char g = (unsigned char)(255 * (t-0.0f ) * 0.5f + (0.0f * t));    // Orange to dark
        unsigned char b = (unsigned char)(255 * (t-0.0f ) * 0.6f + (0.4f * t));    // Orange to dark blue


        ALLEGRO_COLOR color = al_map_rgb(r, g, b);

        // Draw horizontal line with gradient color
        al_draw_line(0, y, SCREEN_WIDTH, y, color, 1);
    }

  ///-------------------------------------------------------------------------

        drawTiles();

         drawBullets();

          drawPlayer();

      ///     drawCoinParticles();

            drawParticles();

        // Optional: Render coin count
        al_draw_textf(font, al_map_rgb(255,255,255), 10, 10, 0, "Itens: %d", coin_count);

          /*
        // Draw player
        al_draw_filled_rectangle(
            player.x, player.y,
            player.x + player.width,
            player.y + player.height,
            al_map_rgb_f(0.0f, 0.5f, 1.0f)
        );
        */

        al_flip_display();
    }

    ~Game() {


        ///------------------------------------

         // Cleanup audio resources
        if (background_music) {
            al_destroy_audio_stream(background_music);
        }
        if (shoot_sound) {
            al_destroy_sample(shoot_sound);
        }
        al_uninstall_audio();

        ///-------------------------------------

          // Existing destructor code. o menu
        if (menu_background) {
            al_destroy_bitmap(menu_background);
        }

         // Destroi a fonte no  desconstrutor da classe
        if (font) {
            al_destroy_font(font);
        }

        al_destroy_display(display);
        al_destroy_event_queue(event_queue);
        al_destroy_timer(timer);
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}

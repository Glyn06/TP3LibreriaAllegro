
#include <stdio.h>
#include <allegro5/allegro.h>
#include <iostream>
using namespace std;
//para una imagen:
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"

enum DIR {
	UP = -1, DOWN = 1, LEFT = -1, RIGHT = 1
};

enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

class Bullet
{
public:
	Bullet(int _posX, int _posY, DIR dir);
	~Bullet();
	void Draw();
	void Move();
	void Update();

private:
	const int speed = 20;
	int posX;
	int posY;
	int cposX;
	int cposY;
	int cposW;
	int cposH;
	DIR dir;
	ALLEGRO_BITMAP  *sprite = NULL;
};

Bullet::Bullet(int _posX, int _posY, DIR _dir)
{
	posX = _posX;
	posY = _posY;
	sprite = al_load_bitmap("Bullet.png");
	cposH = al_get_bitmap_height(sprite);
	cposW = al_get_bitmap_width(sprite);
	cposX = cposH / 2;
	cposY = cposW / 2;
	dir = _dir;
}

Bullet::~Bullet()
{
	al_destroy_bitmap(sprite);
}

void Bullet::Draw()
{
	al_draw_bitmap(sprite, posX, posY, 0);
}

void Bullet::Move()
{
	if (dir == LEFT || dir == RIGHT)
		posX += dir;
	else
		posY += dir;
}

void Bullet::Update()
{
	Draw();
	Move();
}


const int dispx = 640;
const int dispy = 480;

int main(int argc, char **argv) {

	ALLEGRO_DISPLAY *display = NULL;
	//para imagen:
	ALLEGRO_BITMAP  *image = NULL;
	ALLEGRO_BITMAP  *image2 = NULL;

	bool gameover = false;

	const int SPEED = 5;
	int posx=0;
	int posy=0;
	int player_collisionBOX_x = posx;							  
	int player_collisionBOX_y = posy;							  
	int player_collisionBOX_w = 0;										  
	int player_collisionBOX_h = 0;
	bool arrowKeys[4] = { false,false,false,false };
	DIR direction = UP;

	int ePOSx = 200;
	int ePOSy = 200;
	int enemy_collisionBOX_x = ePOSx;								 
	int enemy_collisionBOX_y = ePOSy;								 
	int enemy_collisionBOX_w = 0;										 
	int enemy_collisionBOX_h = 0;

	if (!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}//

	if (!al_install_keyboard())    //iniciacion del teclado
	{
		al_show_native_message_box(display, "Error", "Error", "Failed to load keyboard!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}

	//para imagen:
	if (!al_init_image_addon()) {
		al_show_native_message_box(display, "Error", "Error", "Failed to initialize al_init_image_addon!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		return 0;
	}//

	display = al_create_display(dispx, dispy);
	if (!display) {
		fprintf(stderr, "failed to create display!\n");
		return -1;
	}

	//para imagen:
	image = al_load_bitmap("image.png");
	if (!image) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load image!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}//
	player_collisionBOX_h = al_get_bitmap_height(image);
	player_collisionBOX_w = al_get_bitmap_width(image);

	image2 = al_load_bitmap("image2.png");
	if (!image) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load image2!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}
	enemy_collisionBOX_h = al_get_bitmap_height(image2);																  
	enemy_collisionBOX_w = al_get_bitmap_width(image2);

	 //para eventos:
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;

	event_queue = al_create_event_queue();
	if (!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	//
	//evento
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_draw_bitmap(image2, ePOSx, ePOSy, 0);
	al_draw_bitmap(image, posx, posy, 0);
	al_flip_display();
	while (!gameover)
	{
		ALLEGRO_EVENT ev;
		ALLEGRO_TIMEOUT timeout;
		al_init_timeout(&timeout, 0.06);

		al_wait_for_event_until(event_queue,&ev, &timeout);

		if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_DOWN:
				arrowKeys[KEY_DOWN] = true;
				direction = DOWN;
				break;
			case ALLEGRO_KEY_UP:
				arrowKeys[KEY_UP] = true;
				direction = UP;
				break;
			case ALLEGRO_KEY_LEFT:
				arrowKeys[KEY_LEFT] = true;
				direction = LEFT;
				break;
			case ALLEGRO_KEY_RIGHT:
				arrowKeys[KEY_RIGHT] = true;
				direction = RIGHT;
				break;
			case ALLEGRO_KEY_S:

				break;
			case ALLEGRO_KEY_ESCAPE:
				gameover = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_DOWN:
				arrowKeys[KEY_DOWN] = false;
				break;
			case ALLEGRO_KEY_UP:
				arrowKeys[KEY_UP] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				arrowKeys[KEY_LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				arrowKeys[KEY_RIGHT] = false;
				break;
			}
		}

			posx += arrowKeys[KEY_RIGHT] * SPEED;
			posx -= arrowKeys[KEY_LEFT] * SPEED;
			posy += arrowKeys[KEY_DOWN] * SPEED;
			posy -= arrowKeys[KEY_UP] * SPEED;

			player_collisionBOX_x = player_collisionBOX_w / 2;
			player_collisionBOX_y = player_collisionBOX_h / 2;

			enemy_collisionBOX_x = enemy_collisionBOX_w / 2;
			enemy_collisionBOX_y = enemy_collisionBOX_h / 2;

			if (posx + player_collisionBOX_x > ePOSx - enemy_collisionBOX_x &&
				posx - player_collisionBOX_x < ePOSx + enemy_collisionBOX_x &&
				posy + player_collisionBOX_y > ePOSy - enemy_collisionBOX_y &&
				posy - player_collisionBOX_y < ePOSy + enemy_collisionBOX_y)
			{
				gameover = true;
			}

		
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_bitmap(image2, ePOSx, ePOSy, 0);
		al_draw_bitmap(image, posx, posy, 0);
		al_flip_display();
		}
	
	//

	al_destroy_display(display);
	al_destroy_bitmap(image);
	al_destroy_bitmap(image2);
	//evento
	al_destroy_event_queue(event_queue);

	return 0;
}
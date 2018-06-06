
#include <stdio.h>
#include <allegro5/allegro.h>
#include <iostream>
using namespace std;
//para una imagen:
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"

int pete = 0;
enum DIR {
	UP, DOWN, LEFT, RIGHT
};

enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};

class Bullet
{
public:
	Bullet() {};

	const int speed = 20;
	int posX;
	int posY;
	int cposX;
	int cposY;
	int cposW;
	int cposH;
	bool fly;
};


const int dispx = 640;
const int dispy = 480;

int main(int argc, char **argv) {

	ALLEGRO_DISPLAY *display = NULL;
	//para imagen:
	ALLEGRO_BITMAP  *image = NULL;
	ALLEGRO_BITMAP  *image2 = NULL;
	ALLEGRO_BITMAP *b_image = NULL;

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
	Bullet *bullet = NULL;
	bullet = new Bullet();

	bullet->fly = false;
	bullet->posX = -16;
	bullet->posY = -16;
	bullet->cposX = bullet->posX;
	bullet->cposY = bullet->posY;
	bullet->cposW = 0;
	bullet->cposH = 0;

	const int ENEMY_SPEED = 5;
	int ePOSx = 200;
	int ePOSy = 200;
	int enemy_collisionBOX_x = ePOSx;								 
	int enemy_collisionBOX_y = ePOSy;								 
	int enemy_collisionBOX_w = 0;										 
	int enemy_collisionBOX_h = 0;
	bool enemy_alive = true;
	bool enemy_turnaround = false;

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

	b_image = al_load_bitmap("Bullet.png");
	if (!b_image) {
		al_show_native_message_box(display, "Error", "Error", "Failed to load b_image!",
			NULL, ALLEGRO_MESSAGEBOX_ERROR);
		al_destroy_display(display);
		return 0;
	}//
	bullet->cposH = al_get_bitmap_height(b_image);
	bullet->cposW = al_get_bitmap_width(b_image);

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
				if (!bullet->fly)
				{
					direction = DOWN;
				}
				break;
			case ALLEGRO_KEY_UP:
				arrowKeys[KEY_UP] = true;
				if (!bullet->fly)
				{
					direction = UP;
				}
				break;
			case ALLEGRO_KEY_LEFT:
				arrowKeys[KEY_LEFT] = true;
				if (!bullet->fly)
				{
					direction = LEFT;
				}
				break;
			case ALLEGRO_KEY_RIGHT:
				arrowKeys[KEY_RIGHT] = true;
				if (!bullet->fly)
				{
					direction = RIGHT;
				}
				break;
			case ALLEGRO_KEY_S:
				if (!bullet->fly)
				{
					bullet->posX = posx;
					bullet->posY = posy;
					bullet->fly = true;
				}
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

			if (ePOSx >= dispx - 32)
			{
				enemy_turnaround = true;
			}
			if (ePOSx <= 0)
			{
				enemy_turnaround = false;
			}
			if (!enemy_turnaround)
			{
				ePOSx += ENEMY_SPEED;
			}
			if (enemy_turnaround)
			{
				ePOSx -= ENEMY_SPEED;
			}

			if (bullet->fly){
				if (direction == RIGHT)
				{
					bullet->posX += bullet->speed;
				}
				if (direction == LEFT)
				{
					bullet->posX -= bullet->speed;
				}
				if (direction == UP)
				{
					bullet->posY -= bullet->speed;
				}
				if (direction == DOWN)
				{
					bullet->posY += bullet->speed;
				}
			}
			if (bullet->posX >= dispx || bullet->posX <= -16 ||
				bullet->posY >= dispy || bullet->posY <= -16)
			{
				bullet->fly = false;
			}
			
			posx += arrowKeys[KEY_RIGHT] * SPEED;
			posx -= arrowKeys[KEY_LEFT] * SPEED;
			posy += arrowKeys[KEY_DOWN] * SPEED;
			posy -= arrowKeys[KEY_UP] * SPEED;

			player_collisionBOX_x = player_collisionBOX_w / 2;
			player_collisionBOX_y = player_collisionBOX_h / 2;

			enemy_collisionBOX_x = enemy_collisionBOX_w / 2;
			enemy_collisionBOX_y = enemy_collisionBOX_h / 2;

			bullet->cposX = bullet->cposW / 2;
			bullet->cposY = bullet->cposH / 2;

			if (posx + player_collisionBOX_x > ePOSx - enemy_collisionBOX_x &&
				posx - player_collisionBOX_x < ePOSx + enemy_collisionBOX_x &&
				posy + player_collisionBOX_y > ePOSy - enemy_collisionBOX_y &&
				posy - player_collisionBOX_y < ePOSy + enemy_collisionBOX_y)
			{
				gameover = true;
			}

			if (bullet->posX + bullet->cposX > ePOSx - enemy_collisionBOX_x &&
				bullet->posX - bullet->cposX  < ePOSx + enemy_collisionBOX_x &&
				bullet->posY + bullet->cposY > ePOSy - enemy_collisionBOX_y &&
				bullet->posY - bullet->cposY < ePOSy + enemy_collisionBOX_y &&
											   enemy_alive == true)
			{
				bullet->fly = false;
				bullet->posX = -16;
				bullet->posY = -16;
				enemy_alive = false;
				ePOSx = dispx * 2;
				ePOSy = dispy * 2;
			}

		
		al_clear_to_color(al_map_rgb(0, 0, 0));
		if (enemy_alive)
		{
			al_draw_bitmap(image2, ePOSx, ePOSy, 0);
		}
		al_draw_bitmap(image, posx, posy, 0);
		al_draw_bitmap(b_image, bullet->posX,bullet->posY, 0);
		al_flip_display();
		}
	
	//

	al_destroy_display(display);
	al_destroy_bitmap(image);
	al_destroy_bitmap(image2);
	al_destroy_bitmap(b_image);
	//evento
	al_destroy_event_queue(event_queue);

	return 0;
}
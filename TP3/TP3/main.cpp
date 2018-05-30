
#include <stdio.h>
#include <allegro5/allegro.h>

#include <iostream>
using namespace std;

//para una imagen:
#include "allegro5/allegro_image.h"
#include "allegro5/allegro_native_dialog.h"
//
//para imput
enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
};
const int dispx = 640;
const int dispy = 480;
//
int main(int argc, char **argv) {

	ALLEGRO_DISPLAY *display = NULL;
	//para imagen:
	ALLEGRO_BITMAP  *image = NULL;

	bool gameover = false;

	const int SPEED = 5;
	int posx=0;
	int posy=0;
	bool arrowKeys[4] = { false,false,false,false };

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
				break;
			case ALLEGRO_KEY_UP:
				arrowKeys[KEY_UP] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				arrowKeys[KEY_LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				arrowKeys[KEY_RIGHT] = true;
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

		
		al_clear_to_color(al_map_rgb(0, 0, 0));
		al_draw_bitmap(image, posx, posy, 0);
		al_flip_display();
		}
	
	//





	al_destroy_display(display);
	al_destroy_bitmap(image);
	//evento
	al_destroy_event_queue(event_queue);
	return 0;
}
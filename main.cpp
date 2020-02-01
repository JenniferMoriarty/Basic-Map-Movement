#include <stdio.h>
#include<iostream>
#include <allegro5/allegro.h>
#include<allegro5\allegro_image.h>
#include<allegro5\allegro_primitives.h>
#include<allegro5\allegro_font.h>
#include<allegro5\allegro_ttf.h>
#include<ctime>
using namespace std;


const float FPS = 60;

enum MYKEYS {
	KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT, KEY_SPACE
};



int main()
{

	//set up Allegro
	al_init();
	al_init_image_addon();
	al_install_keyboard();
	al_init_primitives_addon();



	//ALLEGRO variables
	ALLEGRO_DISPLAY* display = NULL;
	ALLEGRO_EVENT_QUEUE* event_queue = NULL;
	ALLEGRO_TIMER* timer = NULL;
	ALLEGRO_BITMAP* map = al_load_bitmap("map.png");
	ALLEGRO_BITMAP* blueTre = al_load_bitmap("tre.png");
	al_convert_mask_to_alpha(blueTre, al_map_rgb(255, 255, 255)); //sets transparency so white doesn't show up

	timer = al_create_timer(1.0 / FPS);
	display = al_create_display(800,800);

	int TreX = 500;
	int TreY = 500;

	int player_x = 400; //notice these never change! the map moves behind the player, not the other way around
	int player_y = 400;

	bool key[5] = { false, false, false, false , false };
	bool redraw = true;
	bool doexit = false;

	//CAMERA STUFF------------------------------------------------------------
	// the screen size (the actual resolution of our display or window)
	int screenWidth = 800;
	int screenHeight = 800;

	// the world size
	int worldWidth = 1920;
	int worldHeight = 1200;

	// the camera's position
	int cameraX = 0;
	int cameraY = 0;
	//------------------------------------------------------------------------




	//create event queue and set it up to look for close window buttons, timer ticks, and keyboard input
	event_queue = al_create_event_queue();
	al_register_event_source(event_queue, al_get_display_event_source(display));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());


	al_start_timer(timer);


	//game loop
	while (!doexit)
	{

		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		//timer section////////////////////////////////////////////////////////////////////////////////
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (key[KEY_UP]) {
				cameraY -= 4.0;
			}
			if (key[KEY_DOWN]) {
				cameraY += 4.0;
			}
			if (key[KEY_LEFT]) {
				cameraX -= 4.0;
			}
			if (key[KEY_RIGHT]) {
				cameraX += 4.0;
			}

			//check if you've collided with tre
			if (player_x + 30 > TreX-cameraX && 
				player_x < TreX-cameraX + 100 && 
				player_y + 30> TreY-cameraY && 
				player_y < TreY-cameraY + 130)
				cout << "AAAH!"; //normally you'd do something here, like warp somewhere or stop moving



			//don't let player move off side of map
			//later we'll make it so the player *does* move once we reach the edge of the moveable map
			//so you can actually reach the edges of the screen...
			if (cameraX < 0)
				cameraX = 0;
			if (cameraY < 0)
				cameraY = 0;
			if (cameraX > worldWidth - screenWidth)
				cameraX = worldWidth - screenWidth;		
			if (cameraY > worldHeight - screenHeight)
				cameraY = worldHeight - screenHeight;
		

			redraw = true;
		}//end timer seciton////////////////////////////////////////////////////////////////////////


		//IO sections///////////////////////////////////////////////////////////////////////////////
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			break;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = true;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = true;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = true;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = true;
				break;

			case ALLEGRO_KEY_SPACE:
				key[KEY_SPACE] = true;
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP) {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				key[KEY_UP] = false;
				break;

			case ALLEGRO_KEY_DOWN:
				key[KEY_DOWN] = false;
				break;

			case ALLEGRO_KEY_LEFT:
				key[KEY_LEFT] = false;
				break;

			case ALLEGRO_KEY_RIGHT:
				key[KEY_RIGHT] = false;
				break;

			case ALLEGRO_KEY_ESCAPE:
				doexit = true;
				break;
			case ALLEGRO_KEY_SPACE:
				key[KEY_SPACE] = false;
				break;
			}
		}
		//////////////////////////////////////////////////////////////////////////////////////////


		//render section/////////////////////////////////////////////////////////////////////////
		if (redraw && al_is_event_queue_empty(event_queue)) {
			redraw = false;

			al_clear_to_color(al_map_rgb(0, 0, 0));
	
			//the map and anything stuck to it needs to have the "camera" subtracted from it
			al_draw_bitmap(map, -cameraX, -cameraY, NULL);
			al_draw_bitmap(blueTre, 500-cameraX , 500 - cameraY, NULL);

			//player
			al_draw_filled_rectangle(player_x, player_y, player_x+30, player_y+30, al_map_rgb(255, 0, 255));



			
			al_flip_display();
		}//end render///////////////////////////////////////////////////////////////////////////////
	}//end game loop

	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);

	return 0;
}

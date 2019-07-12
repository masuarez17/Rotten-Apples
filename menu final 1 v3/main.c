#include <allegro.h>
#include <stdbool.h>

void init();
/*void deinit();*/

int main() {
	init();
	
      BITMAP *fondo;
      fondo=load_bitmap("fondo.bmp",NULL);
      BITMAP *fondo1;
	  fondo1=load_bitmap("jugar.bmp",NULL);
      BITMAP *fondo2;
	  fondo2=load_bitmap("cred.bmp",NULL);
      BITMAP *fondo3;
	  fondo3=load_bitmap("ins.bmp",NULL);
      BITMAP *fondo4;
	  fondo4=load_bitmap("sal.bmp",NULL);
      BITMAP *fondo5;
	  fondo5=load_bitmap("scor.bmp",NULL);
      BITMAP *cursor;
	  cursor=load_bitmap("cursor.bmp",NULL);
      
      bool fin=false;
      while(!fin){
      	
      	if(mouse_x > 94 && mouse_x < 253 &&
		  mouse_y > 246 && mouse_y < 300){
		  	
		  	draw_sprite(screen,fondo1,0,0);
		  	
		  	if(mouse_b & 1){
		  		//jugar
		  		fin=true;
			  
			  }
		  }else if(mouse_x >416 && mouse_x <572 &&
		  mouse_y > 253 && mouse_y < 292){
		  		draw_sprite(screen,fondo2,0,0);
		  	if(mouse_b & 1){
		  		//creditos
		  		fin=true;
			  }
		  }else draw_sprite(screen,fondo,0,0);
		  
		  /***********************************************************/
		  if(mouse_x > 550 && mouse_x < 642 &&
		  mouse_y > 12 && mouse_y < 50){
		  	
		  	draw_sprite(screen,fondo4,0,0);
		  	if(mouse_b & 1){
		  		//salir
		  		fin=true;
		  	} 
		  }//else draw_sprite(screen,fondo,0,0);
		
		  /***********************************************************************/
		  if(mouse_x > 48 && mouse_x < 270 &&
		  mouse_y > 319 && mouse_y < 366){
		  	draw_sprite(screen,fondo3,0,0);
		  	if(mouse_b & 1){
		  		//instrucciones
		  		fin=true;	  
			  }
		  }else if(mouse_x >382 && mouse_x <615 &&
		  mouse_y > 314 && mouse_y < 359){
		  		draw_sprite(screen,fondo5,0,0);
		  	if(mouse_b & 1){
		  		//puntajes
		  		fin=true;
			  }
		  }//else draw_sprite(screen,fondo,0,0);
		  
		     masked_blit(cursor,screen,0,0,mouse_x,mouse_y,23,23);
	}
     readkey();
     
    destroy_bitmap(fondo);
	/*deinit();*/
	return 0;

}
END_OF_MAIN()

void init() {
	int depth, res;
	allegro_init();
	depth = desktop_color_depth();
	if (depth == 0) depth = 32;
	set_color_depth(depth);
	res = set_gfx_mode(GFX_AUTODETECT_WINDOWED, 652, 405, 0, 0);
	if (res != 0) {
		allegro_message(allegro_error);
		exit(-1);
	}

	
	install_mouse();
	
	
}

/*void deinit() {
	clear_keybuf();
	
}*/

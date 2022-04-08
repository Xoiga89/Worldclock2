/*
   Application template for Amazfit Bip BipOS
   (C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>

   		World Clock for Amazfit Bip
		(C) Tony Nguyen  2021 <https://direct.me/tonyisme>

*/

#ifndef __APP_2048_H__
#define __APP_2048_H__
#define CT1				0// Country 1
#define CT2				-6 // Country 2
#define CT3				3// Country 3
#define CT4				-15// Country 4
#define CT5				-7// Country 5
#define CT6				-4// Country 6
#define CT7				2// Country 7
#define CT8				1// Country 8
#define CT9				-10// Country 9
#define CT10			0// Country 10
#define Name_CT1		"VNM: "//Name Country 1
#define Name_CT2		"GER: "//Name Country 2
#define Name_CT3		"AUS: "//Name Country 3
#define Name_CT4		"USA: "//Name Country 4
#define Name_CT5		"GBR: "//Name Country 5
#define Name_CT6		"RUS: "//Name Country 6
#define Name_CT7		"JPN: "//Name Country 7
#define Name_CT8		"CHN: "//Name Country 8
#define Name_CT9		"ARG: "//Name Country 9
#define Name_CT10		"VNM"//Name Country 10



struct app_data_ {
  void* 	ret_f;
  struct game;
  int		col;	//the current color of the font
  short screen;
  Elf_proc_* proc;
  int        state_hash;
  int        current_screen;
};



unsigned short randint( short max );
void draw_button(int from_x, int from_y, int to_x, int to_y);

void 	show_screen (void *return_screen);
void 	key_press_screen();
int 	dispatch_screen (void *param);
void 	screen_job();
void draw_board();
void thegioi2();

/*
  small hack: decide if we should re-render screen
  we calculate state_hash and compare to the old one
*/
int should_update_status(struct app_data_ *app_data) {
  struct datetime_ dt; get_current_date_time(&dt);
  int state_hash = (dt.hour << 2) + dt.min + dt.sec;
  if (app_data->state_hash == state_hash) {
    return 0;
  } else {
    app_data->state_hash = state_hash;
    return 1;
  }
}
#endif

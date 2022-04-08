/*
   Application template for Amazfit Bip BipOS
   (C) Maxim Volkov  2019 <Maxim.N.Volkov@ya.ru>

				World Clock for Amazfit Bip
		(C) Tony Nguyen  2021 <https://direct.me/tonyisme>

*/
// Này là bản cho đồng hồ ở Viet Nam
#include "libbip.h"
#include "2048.h"
#include "BipStatusbarLib.h"

struct regmenu_ screen_data = {
  55,
  1,
  0,
  dispatch_screen,
  key_press_screen,
  screen_job,
  0,
  show_screen,
  0,
  0
};

int main(int param0, char** argv){
  show_screen((void*) param0);
}

void show_screen (void *param0) {
  struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	pointer to a pointer to screen data
  struct app_data_ *	app_data;					//	pointer to screen data
  if ( (param0 == *app_data_p) && get_var_menu_overlay()){
    app_data = *app_data_p;
    *app_data_p = NULL;
    reg_menu(&screen_data, 0);
    *app_data_p = app_data;
  } else {
    reg_menu(&screen_data, 0);
    *app_data_p = (struct app_data_ *)pvPortMalloc(sizeof(struct app_data_));
    app_data = *app_data_p;
    _memclr(app_data, sizeof(struct app_data_));
    app_data->proc = param0;
    if ( param0 && app_data->proc->ret_f )
      app_data->ret_f = app_data->proc->elf_finish;
    else
      app_data->ret_f = show_watchface;
    
   
    app_data->screen = 1;
 app_data->col=0;
 app_data->current_screen = 0;
 app_data->state_hash = -1;
	
}
draw_screen();

// đặt bộ hẹn giờ để gọi screen_job tính bằng mili giây, tùy thuộc vào việc có cần trình bảo vệ màn hình hay không
set_update_period(1,  1000);


// prevent exiting the app after a period
  set_display_state_value(8, 1);
  set_display_state_value(2, 1);

}

//Da kiem tra oke
void key_press_screen(){
  struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
  struct app_data_ *	app_data = *app_data_p;

  show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
};

void screen_job(){
  struct app_data_ **app_data_p = get_ptr_temp_buf_2(); 
  struct app_data_ *app_data = *app_data_p;
  


 if (app_data->screen == 1) {
  draw_board();
repaint_screen_lines(0, 176);
set_update_period(1, 60000);
 }   else if ( app_data->screen == 9 ) {
thegioi2();
repaint_screen_lines(0, 176);
set_update_period(1, 60000);
  }
  else {

repaint_screen_lines(0, 176);
set_update_period(1, 60000);
  }	  
}

int dispatch_screen (void *param){
  struct app_data_** 	app_data_p = get_ptr_temp_buf_2();
  struct app_data_ *	app_data = *app_data_p;
  struct gesture_ *gest = param;
  int result = 0;
 
  switch (gest->gesture){
    case GESTURE_CLICK: {
                          vibrate(1,50,0);
                          switch (app_data->screen){
                           
									case 8: {if ( app_data->screen == 8 ){ 
				 app_data->screen = 9;
				set_bg_color(COLOR_BLACK);
                fill_screen_bg();
                thegioi2();
                                        repaint_screen_lines(0, 176);
                                      break;
			                          }}
									  case 9: {if ( app_data->screen == 9 ){ 
				 app_data->screen = 1;
				set_bg_color(COLOR_BLACK);
                fill_screen_bg();
                draw_board();
                                        repaint_screen_lines(0, 176);
                                      break;
			                          }}           
                                      break;
                    
	
                          }

                          break;
                        };
    case GESTURE_SWIPE_DOWN: {
                               
                               app_data->screen = 1;
							   draw_board();
							 

                               break;
                             };
    case GESTURE_SWIPE_UP: {
                             
                             break;
                           };
    case GESTURE_SWIPE_LEFT: {
                             
                               show_menu_animate(app_data->ret_f, (unsigned int)show_screen, ANIMATE_RIGHT);
                               break;
                             };
    case GESTURE_SWIPE_RIGHT: {
                              
                                                               
								break;
                              };
    default:{
              vibrate(1,70,0);
              break;
            };

  }
  return result;
};


void draw_board(){
// Lấy giờ Úc (Giờ VN +3) , Đức (Giờ VN -6) , Los Angeles (Giờ VN -15) , London (Giờ VN -7). Ký tự nơi ở sẽ thấy kí tự theo Mã chung ISO và cách 15 px, Giờ + thì > 24, Giờ - thì < 0)
  // Vì lý dó có các nước như Đức, London là có xài thêm giờ mùa đông nên viết lại code như sau
  //Các nước có giờ mùa đông
//-	Đức
//-	Anh (London)
//-	Đức và anh xài chung 1 giờ mùa đông
//Bắt đầu: Ngày chủ nhật cuối cùng của tháng 3 (31- 6= 25 =< x =< 31) lúc 1h sáng: Tăng thêm 1h --> Giờ VN -5 (Cho Đức), Giờ VN -6 (Cho London)
//Kết thúc: Ngày chủ nhật cuối cùng của tháng 10 (31- 6= 25 =< x =< 31) lúc 2h sáng
//-	Los Angeles
//-	Xài giờ riêng
//-	Bắt đầu: Ngày chủ nhật thứ 2 của tháng 3 (8 =< x =< 8+6) lúc 2h sáng: Tăng thêm 1h --> Giờ VN -14
//-	Kết thúc: Ngày chủ nhật thứ 1 của tháng  11 (1 =< x =< 7) lúc 2h

  set_bg_color(COLOR_WHITE);
  fill_screen_bg();
  set_fg_color(COLOR_BLACK);
text_out_center("WORLD CLOCK", 88, 6);
text_out_center("TONY", 88, 30);

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 54, 172, 85);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT1, 60, 58);
  char clock_time_VN[8]; 			//	текст время		12:34
		struct datetime_ dt;
		get_current_date_time(&dt);
		_sprintf(clock_time_VN, "%02d:%02d", dt.hour, dt.min);
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_VN, 80 , 58, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     

/*
January = 0
February = 3
March = 3
April = 6
May = 1
June = 4
July = 6
August = 2
September = 5
October = 0
November = 3
December = 5
*/


  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 90, 86, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT2, 23, 104);
  int GE3;
  GE3 = 31;
   int GE10;	
  GE10 = 31;
  int CC;
  if ( ( dt.year >= 1900 ) && ( dt.year <= 2000 ) ) {
	  CC = 0;
  } else if ( ( dt.year >= 2000 ) && ( dt.year <= 2100 ) ) {
	  CC = 6;
  } else if ( ( dt.year >= 2100 ) && ( dt.year <= 2200 ) ) {
	  CC = 4;
  } else if ( ( dt.year >= 2200 ) && ( dt.year <= 2300 ) ) {
	  CC = 2;
  } else if ( ( dt.year >= 1800 ) && ( dt.year <= 1900 ) ) {
	  CC = 2;
  } else if ( ( dt.year >= 2300 ) && ( dt.year <= 2400 ) ) {
	  CC = 0;
  }
  int LY; //Vì cái này dành cho các ngày trong tháng 1 và 2 (Tại tháng 2 bị nhuận) còn các tháng còn lại không bị nên không có trừ 1 ở đây
  // If the date is in a January or February of a leap year, you have to subtract one from your total before the final step.
  if ( ( dt.year % 4 == 0 ) && ( dt.year % 100 != 0 ) ){
	   //LY = -1; Bỏ
	   LY = 0;
  } else if ( dt.year % 400 == 0 ) {
	   //LY = -1;
	   LY = 0;
  } else {
	  LY = 0;
  }
  int YC;
  if ( ( dt.year >= 1900 ) && ( dt.year <= 2000 ) ) {
	  YC = ((dt.year - 1900) / 4 + (dt.year - 1900)) % 7;
  } else if ( ( dt.year >= 2000 ) && ( dt.year <= 2100 ) ) {
	   YC = ((dt.year - 2000) / 4 + (dt.year - 2000)) % 7;
  } else if ( ( dt.year >= 2100 ) && ( dt.year <= 2200 ) ) {
	   YC = ((dt.year - 2100) / 4 + (dt.year - 2100)) % 7;
  } else if ( ( dt.year >= 2200 ) && ( dt.year <= 2300 ) ) {
	   YC = ((dt.year - 2200) / 4 + (dt.year - 2200)) % 7;
  } else if ( ( dt.year >= 1800 ) && ( dt.year <= 1900 ) ) {
	   YC = ((dt.year - 1800) / 4 + (dt.year - 1800)) % 7;
  } else if ( ( dt.year >= 2300 ) && ( dt.year <= 2400 ) ) {
	   YC = ((dt.year - 2300) / 4 + (dt.year - 2300)) % 7;
  }
  //Cho tháng 3
  for (int i = 25; i < 32 ; i++){
		if ( (3 + CC + LY + YC + i)% 7 == 0 ){ //Số 3 ở đầu là đại diện cho tháng
			GE3 = i ;
		} 
	}
	//Cho tháng 10
	 for (int z = 25; z < 32 ; z++){
		if ( (0 + CC + LY + YC + z)% 7 == 0 ){
			GE10 = z ;
		} 
	
	}
  char clock_time_GE[8]; 			//	текст время		12:34
           //Có phải trong tháng 3 đến tháng 10 không? Không thì sao: Đã làm
			 
				 //Xét tháng 3 trước		  
 	                     if (( dt.month == 3) && ( dt.day == GE3)){
					//Xét riêng trong ngày chủ nhật lúc 1h sáng mới thay đổi đồng hồ
								if ( dt.hour + CT2 >= 1){ // Giờ ở Đức từ 1h sáng chủ nhật là cộng thêm 1h
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 1, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour + CT2 < 0) {
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 24, dt.min);
												}
								else {
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
					if (( dt.month == 3) && ( dt.day > GE3)){
							if (dt.hour + CT2 + 1 < 0) {
								 _sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 24 + 1, dt.min);
								}
							else {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 1, dt.min);
								}
					}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						if (( dt.month == 3) && ( dt.day < GE3)){
						if (dt.hour + CT2 < 0) {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 24, dt.min);
							}
							else {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2, dt.min);
								}
						}
					 // Dấu kết cho tháng 3
				//Xét đến tháng 10
       
					  //Xét trong tuần cuối cùng? Còn lại thì sao: Đã làm
 	                    if (( dt.month == 10) && ( dt.day == GE10)){
					//Xét riêng trong ngày chủ nhật lúc 2h sáng mới thay đổi đồng hồ
								if ( dt.hour + CT2 + 1 >= 2){ // Giờ ở Đức từ 2h sáng chủ nhật là về lại bình thường
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour + CT2 + 1 < 0) {
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 24 + 1, dt.min);
												}
								else {
									_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 1, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
						if (( dt.month == 10) && ( dt.day > GE10)){
							if (dt.hour + CT2 < 0) {
								 _sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 24, dt.min);
								}
							else {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2, dt.min);
								}
								}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						if (( dt.month == 10) && ( dt.day < GE10)){
							if (dt.hour + CT2 + 1 < 0) {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 24 + 1, dt.min);
							}
							else {
								_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 1, dt.min);
								}
								} //Hết xét tuần cuối cùng
						 // Dấu kết cho tháng 10														  
				//Xét các tháng còn lại trong khoảng từ tháng 3 đến tháng 10
					if ( ( dt.month >= 4) && ( dt.month <= 9) ) {
						if (dt.hour + CT2 + 1 < 0) {
							_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 24 + 1, dt.min);
						}
						else {
							_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 1, dt.min);
						}
			} //Hết cho tháng 3 và tháng 10
			if ( ( dt.month >= 11) || ( dt.month <= 2) ){
				if (dt.hour + CT2 < 0) {
					_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2 + 24, dt.min);
					}
				else {
					_sprintf(clock_time_GE, "%02d:%02d", dt.hour + CT2, dt.min);
					}
			}
			 
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_GE, 38 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 90, 172, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT3, 110, 104);
    char clock_time_AU[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT3 > 24) {
  _sprintf(clock_time_AU, "%02d:%02d", dt.hour + CT3 - 24, dt.min);
 }
  else {
 _sprintf(clock_time_AU, "%02d:%02d", dt.hour + CT3, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_AU, 125 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);    
  
    set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 140, 86, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT4, 23, 147);
     int US3;
  US3 = 31;
   int US10;
  US10 = 31;
    //Cho tháng 3
  for (int v = 8; v < 15 ; v++){
		if ( (3 + CC + LY + YC + v)% 7 == 0 ){ //Số 3 ở đầu là đại diện cho tháng
			US3 = v ;
		} 
	}
	//Cho tháng 10
	 for (int b = 1; b < 8 ; b++){
		if ( (3 + CC + LY + YC + b)% 7 == 0 ){
			US10 = b ;
		} 
	}
  char clock_time_US[8]; 			//	текст время		12:34
           //Có phải trong tháng 3 đến tháng 11 không? Không thì sao: Đã làm
		   //-	Bắt đầu: Ngày chủ nhật thứ 2 của tháng 3 (8 =< x =< 8+6) lúc 2h sáng: Tăng thêm 1h --> Giờ VN -14
			//-	Kết thúc: Ngày chủ nhật thứ 1 của tháng  11 (1 =< x =< 7) lúc 2h
			
				 //Xét tháng 3 trước
					  //Xét trong tuần cuối cùng? Còn lại thì sao: Đã làm 
 	                   if (( dt.month == 3) && ( dt.day == US3)){
							 
					//Xét riêng trong ngày chủ nhật lúc 1h sáng mới thay đổi đồng hồ
								if ( dt.hour + CT4 >= 2){ // Giờ ở USA từ 2h sáng chủ nhật là cộng thêm 1h
									_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 1, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour + CT4 < 0) {
									_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 24, dt.min);
												}
								else {
									_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
						if (( dt.month == 3) && ( dt.day > US3)){
							if (dt.hour + CT4 + 1 < 0) {
								 _sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 24 + 1, dt.min);
								}
							else {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 1, dt.min);
								}
								}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						if (( dt.month == 3) && ( dt.day < US3)){
							if (dt.hour + CT4 < 0) {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 24, dt.min);
							}
							else {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4, dt.min);
								}
								} //Hết xét tuần cuối cùng
						 // Dấu kết cho tháng 3
				//Xét đến tháng 11
                 
					  //Xét trong tuần cuối cùng? Còn lại thì sao: Đã làm
					 if (( dt.month == 11) && ( dt.day == US10)){
					//Xét riêng trong ngày chủ nhật lúc 2h sáng mới thay đổi đồng hồ
								if ( dt.hour + CT4 + 1 >= 2){ // Giờ ở Đức từ 2h sáng chủ nhật là về lại bình thường
									_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour + CT4 + 1 < 0) {
									_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 24 + 1, dt.min);
												}
								else {
									_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 1, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
						if (( dt.month == 11) && ( dt.day > US10)){
							if (dt.hour + CT4 < 0) {
								 _sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 24, dt.min);
								}
							else {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4, dt.min);
								}
								}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						if (( dt.month == 11) && ( dt.day < US10)){
							if (dt.hour + CT4 + 1 < 0) {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 24 + 1, dt.min);
							}
							else {
								_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 1, dt.min);
								}
								} //Hết xét tuần cuối cùng
					 // Dấu kết cho tháng 11														  
				//Xét các tháng còn lại trong khoảng từ tháng 3 đến tháng 10
					 if ( ( dt.month >= 4) && ( dt.month <= 10) ) {
						 if (dt.hour + CT4 + 1 < 0) {
							_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 24 + 1, dt.min);
						}
						else {
							_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 1, dt.min);
						}
			} //Hết cho tháng 3 và tháng 11
			if ( ( dt.month >= 12) || ( dt.month <= 2) ){
				if (dt.hour + CT4 < 0) {
					_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4 + 24, dt.min);
					}
				else {
					_sprintf(clock_time_US, "%02d:%02d", dt.hour + CT4, dt.min);
					}   
			}
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_US, 38 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 140, 172, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT5, 110, 147); //Nước anh
        int GB3;
  GB3 = 31;
   int GB10;
  GB10 = 31;
    //Cho tháng 3
  for (int x = 25; x < 32 ; x++){
		if ( (3 + CC + LY + YC + x)% 7 == 0 ){ //Số 3 ở đầu là đại diện cho tháng
			GB3 = x ;
		} 
	}
	//Cho tháng 10
	 for (int c = 25; c < 32 ; c++){
		if ( (0 + CC + LY + YC + c)% 7 == 0 ){
			GB10 = c ;
		} 
	}
  char clock_time_GB[8]; 			//	текст время		12:34
           //Có phải trong tháng 3 đến tháng 10 không? Không thì sao: Đã làm
				 //Xét tháng 3 trước
                  
					  //Xét trong tuần cuối cùng? Còn lại thì sao: Đã làm
					  if (( dt.month == 3) && ( dt.day == GB3)){
					//Xét riêng trong ngày chủ nhật lúc 1h sáng mới thay đổi đồng hồ
								if ( dt.hour + CT5 >= 1){ // Giờ ở Đức từ 1h sáng chủ nhật là cộng thêm 1h
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 1, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour + CT5 < 0) {
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 24, dt.min);
												}
								else {
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
						if (( dt.month == 3) && ( dt.day > GB3)){
							if (dt.hour + CT5 + 1 < 0) {
								 _sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 24 + 1, dt.min);
								}
							else {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 1, dt.min);
								}
								}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						if (( dt.month == 3) && ( dt.day < GB3)){
							if (dt.hour + CT5 < 0) {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 24, dt.min);
							}
							else {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5, dt.min);
								}
								} //Hết xét tuần cuối cùng
						 // Dấu kết cho tháng 3
				//Xét đến tháng 10
                
					  //Xét trong tuần cuối cùng? Còn lại thì sao: Đã làm
					 if (( dt.month == 10) && ( dt.day == GB10)){
					//Xét riêng trong ngày chủ nhật lúc 2h sáng mới thay đổi đồng hồ
								if ( dt.hour + CT5 + 1 >= 2){ // Giờ ở Đức từ 2h sáng chủ nhật là về lại bình thường
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5, dt.min); 
								}
					//Còn lại là không bị thay đổi +1h
								else if (dt.hour + CT5 + 1 < 0) {
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 24 + 1, dt.min);
												}
								else {
									_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 1, dt.min);
									}
									}
					//Xét các ngày sau ngày chủ nhật trong tuần cuối đó (Bị thay đổi giống ngày chủ nhật: +1h)
						if (( dt.month == 10) && ( dt.day > GB10)){
							if (dt.hour + CT5 < 0) {
								 _sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 24, dt.min);
								}
							else {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5, dt.min);
								}
								}
					//Còn lại không thuộc 2 trường hợp trên là (Trước ngày chủ nhật)
						if (( dt.month == 10) && ( dt.day < GB10)){
							if (dt.hour + CT5 + 1 < 0) {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 24 + 1, dt.min);
							}
							else {
								_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 1, dt.min);
								}
								} //Hết xét tuần cuối cùng
						 // Dấu kết cho tháng 10														  
				//Xét các tháng còn lại trong khoảng từ tháng 3 đến tháng 10
					if ( ( dt.month >= 4) && ( dt.month <= 9) ) {
						if (dt.hour + CT5 + 1 < 0) {
							_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 24 + 1, dt.min);
						}
						else {
							_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 1, dt.min);
						}
			} //Hết cho tháng 3 và tháng 10
			if ( ( dt.month >= 11) || ( dt.month <= 2) ){
				if (dt.hour + CT5 < 0) {
					_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5 + 24, dt.min);
					}
				else {
					_sprintf(clock_time_GB, "%02d:%02d", dt.hour + CT5, dt.min);
					}
			}

		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_GB, 125 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 


  repaint_screen_lines(0, 176);

};

void thegioi2(){  // Lấy giờ Nga (Giờ VN -4) , Trung  (Giờ VN +1) , Nhật (Giờ VN +2) , Argentina (Giờ VN -10). Ký tự nơi ở sẽ thấy kí tự theo Mã chung ISO và cách 15 px, Giờ + thì > 24, Giờ - thì < 0)
  set_bg_color(COLOR_WHITE);
  fill_screen_bg();
  set_fg_color(COLOR_BLACK);
text_out_center("WORLD CLOCK", 88, 6);
text_out_center("TONY", 88, 30);

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 54, 172, 85);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT1, 60, 58);
  char clock_time_VN[8]; 			//	текст время		12:34
		struct datetime_ dt;
		get_current_date_time(&dt);
		_sprintf(clock_time_VN, "%02d:%02d", dt.hour, dt.min);
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_VN, 80 , 58, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     


  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 90, 86, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT6, 23, 104);
  char clock_time_RU[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT6 < 0) {
  _sprintf(clock_time_RU, "%02d:%02d", dt.hour + CT6 + 24, dt.min);
 }
  else {
 _sprintf(clock_time_RU, "%02d:%02d", dt.hour + CT6, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_RU, 38 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);     

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 90, 172, 133);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT7, 110, 104);
    char clock_time_JP[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT7 > 24) {
  _sprintf(clock_time_JP, "%02d:%02d", dt.hour + CT7 - 24, dt.min);
 }
  else {
 _sprintf(clock_time_JP, "%02d:%02d", dt.hour + CT7, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_JP, 125 , 104, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176);    
  
    set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(4, 140, 86, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT8, 23, 147);
      char clock_time_CN[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT8 > 24) {
  _sprintf(clock_time_CN, "%02d:%02d", dt.hour + CT8 - 24, dt.min);
 }
  else {
 _sprintf(clock_time_CN, "%02d:%02d", dt.hour + CT8, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_CN, 38 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 

  set_bg_color(COLOR_BLACK);
  set_fg_color(COLOR_WHITE);
  draw_filled_rect_bg(90, 140, 172, 169);
  set_bg_color(COLOR_BLACK);
  text_out_center(Name_CT9, 110, 147); //ARGENTINA
      char clock_time_AR[8]; 			//	текст время		12:34
		
		 if (dt.hour + CT9 < 0) {
  _sprintf(clock_time_AR, "%02d:%02d", dt.hour + CT9 + 24, dt.min);
 }
  else {
 _sprintf(clock_time_AR, "%02d:%02d", dt.hour + CT9, dt.min);
  }
		//text_out_center(clock_time, 125, 163); // печатаем результат(время) большими цифрами
		text_out_font(11, clock_time_AR, 125 , 147, 1); // text_out_font(Kiểu font chữ, Char (Ở đây là Clock_time roi), x , y, Khoảng cách)
		repaint_screen_lines(0, 176); 
set_update_period(1, 60000);  
repaint_screen_lines(0, 176); 


  repaint_screen_lines(0, 176);
 
}

void draw_screen(){
	struct app_data_** 	app_data_p = get_ptr_temp_buf_2(); 	//	указатель на указатель на данные экрана 
struct app_data_ *	app_data = *app_data_p;				//	указатель на данные экрана
 set_bg_color(COLOR_BLACK);
  fill_screen_bg();
  load_font();
  //set_graph_callback_to_ram_1();
  draw_board();
  
  //set_graph_callback_to_ram_1();
}



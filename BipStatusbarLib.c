/*
	BipStatusbarLibrary for Amazfit Bip BipOS

	(C) ugers 2020
	
	Библиотека статусбар для BipOS
	
*/
#include "BipStatusbarLib.h"
//#define BipEmulator
#ifdef BipEmulator
	#include "libbip.h"

#endif

//ОПЦИИ Options
#define BATTERY_TEXT
#define BATTERY_ICON
#define TIME
#define DATA
#define BLUETOOTH_TEXT
//#define BLUETOOTH
// Выберите 1 из трех ALARM
//#define ALARM_RING
//#define ALARM_CLOCK
//#define ALARM_CLOCK_RING

void show_statusbar(int y, int bgColor,int fgColor){
	set_bg_color(bgColor); // делаем фон
	set_fg_color(fgColor); // делаем фон
	draw_filled_rect_bg(0, 0, 176, y + 20);

	//БАТАРЕЯ
	#ifdef BATTERY_ICON
	set_bg_color(COLOR_BLACK); // подложка
	draw_filled_rect_bg(152, y, 172, y + 16); // сама батарейка
	draw_filled_rect_bg(170, y + 4, 174, y + 12); //кончик
	set_bg_color(COLOR_WHITE); //корпус
	draw_filled_rect_bg(153, y + 1, 171, y + 15); // сама батарейка
	draw_filled_rect_bg(171, y + 5, 173, y + 11); //кончик
	#endif
	word battery_percentage;
	#ifdef BipEmulator
			battery_percentage = 100;
	#else
			battery_percentage = get_battery_charge();
	#endif
		#ifdef BATTERY_ICON
			#ifdef BipEmulator
			char charging = 1;
			#else
			char charging = check_app_state(APP_STATE_CHARGE_PLUGGED);
			#endif
			//Цвет индикатора батареи Colors battery indicator
			char r_count = battery_percentage / 33;
			r_count = r_count > 2 ? 2 : r_count < 1 ? 0 : r_count; // if r_count > 2 = 2 elseif r_count < 1 = 0 else r_count
			if (charging) set_bg_color(COLOR_BLUE);
			else{
				if (battery_percentage > 32) { //set color
					set_bg_color(battery_percentage <= 65 ? COLOR_YELLOW : COLOR_GREEN);
				}else if (battery_percentage <= 32) {
					set_bg_color(COLOR_RED);
				}
			}
				//draw cells
				for (char i = 0; i <= r_count; i++)
				{
					draw_filled_rect_bg(155 + i * 5, y + 2, 159 + i * 5, y + 14);
				}
				if (charging) text_out_center("⚡", 160, y);
			#endif
			#ifdef BATTERY_TEXT
			//Проценты батареи Battery in percents
			char d1[4];     // переменная для перевода переменной для печати                               
			_sprintf(d1, "%01d", battery_percentage); // конвертируем
			set_bg_color(bgColor); // делаем фон
				#ifdef BATTERY_ICON
					text_out_center(d1, 138, y);
				#else
					text_out(d1, 135, y);
					text_out("%", 162, y);
				#endif
			#endif
set_bg_color(bgColor); // делаем фон
#ifdef TIME
	// ВРЕМЯ
	char clock_time[8]; 			//	текст время		12:34
	struct datetime_ dt;
	get_current_date_time(&dt);
	_sprintf(clock_time, "%02d:%02d:%02d", dt.hour, dt.min, dt.sec);
	text_out_center(clock_time, 88,y);		// печатаем результат(время)
#endif

#ifdef DATA
	char data[12]; 			//	текст дата	день.месяц     text date day.mounth
	#ifndef TIME
		struct datetime_ dt;
		get_current_date_time(&dt);
	#endif
	_sprintf(data, "%02d.%02d", dt.day, dt.month);
	text_out(data,4,y);		// печатаем print 
#endif

// РАЗДЕЛИТЕЛЬ
	set_fg_color(fgColor); // делаем фон
	draw_horizontal_line(y+19, 0, 176);
	repaint_screen_lines(0, y+19); // обновляем строки экрана
}
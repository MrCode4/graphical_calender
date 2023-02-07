#include <iostream>
#include <string>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <thread>
#include <unistd.h>
#include <atomic>
#include <mutex>
#include <winuser.h>

using namespace std;

vector<string> menus = {"[Esc] Exit", "[H] HomePgae", "[C] Month", "[B] Mode"};

std::counting_semaphore<1> print_signal(1);

std::atomic<bool> is_in_calender_menu = false;

std::atomic<int> MODE = 1;

void background_text_color(int textColor, int backgroundColor) 
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor + (backgroundColor * 16)));
}

void text_color(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
} 

void defualt_color(int x)
{
	if (x==1) text_color(14);
	if (x==2) background_text_color(0 , 15);
}

void weekend_color(int x)
{
	if (x==1) text_color(12);
	if (x==2) background_text_color(12 , 15);
}

void gotoxy(short x, short y)
{
    COORD pos = { x, y };

    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int wherex()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    return csbi.dwCursorPosition.X;
}

int wherey()
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    return csbi.dwCursorPosition.Y;
}

int consolesizex()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;
   
    return columns;
}

int consolesizey()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns, rows;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
   
    rows = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    return rows;
}


int key_menu(string a)
{
	int k=getch();
	if (k==104 || k==72) return 1;
	else if (k==99 || k==67) return 0;
	else if (k==66 || k==98) return 3;
	else if(k==27) exit(0);
	else if(k==13)//enter
	{
		if(MODE == 1)
			exit(0);
		else if(MODE == 2)
			return 1;
		else if(MODE == 3)
			return 0;
		else if(MODE == 4)
			return 3;
		else 
		{
			return key_menu(a);
		}
	}
	else if(k == 50)//2
	{
		MODE++;

		if(MODE == 5)
			MODE = 1;

		return key_menu(a);
	}
	else if(k == 56)//8
	{
		MODE--;
		if(MODE == 0)
			MODE = 4;

		return key_menu(a);
	}
	else
	{
		return key_menu(a);
	}
}

pair<SHORT, SHORT> get_left_top_corner_pos()
{
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);

    return {csbi.srWindow.Left, csbi.srWindow.Top};
}

void print_menu_bar()
{
	while(true)
	{
		if(is_in_calender_menu)
		{				
			print_signal.acquire();

			pair<SHORT, SHORT> left_top_pos = get_left_top_corner_pos();

			int x = static_cast<int>(left_top_pos.first);
			int y = static_cast<int>(left_top_pos.second);

			gotoxy(x, y);
			cout << "----------------\n";
			cout << "|     MENU     |\n";
			cout << "|--------------|\n";
			cout << "|              |";
			y+=4;

			for(int i=0; i<menus.size(); i++)
			{
				gotoxy(x, y);

				cout << "|";
				x++;

				gotoxy(x, y);

				if(MODE == i+1)
					text_color(25);

				cout << menus[i];

				if(MODE == i+1)
					defualt_color(1);

				for(int j=0;j<16-(menus[i].size());j++) 
					cout  << " ";

				x = 15;
				gotoxy(x, y);
				cout << "|";

				x = 0;
				y++;

				gotoxy(x, y);
				cout << "|              |";
				y++;

			}

			gotoxy(x, y);
			for(int i=0;i<16;i++)
				cout << "-";
			
			y++;

			gotoxy(x,y);

			for(int p = 0;p <consolesizey() - (menus.size()*2+10);p++)
				cout << "                " << endl;

			print_signal.release();

			this_thread::sleep_for(std::chrono::milliseconds(200));

		}
		else
		{
			this_thread::sleep_for(std::chrono::milliseconds(400));			
		}
	}
}

void fullscreen()
{
	SetConsoleDisplayMode(GetStdHandle(STD_OUTPUT_HANDLE),CONSOLE_FULLSCREEN_MODE,0);
}

string month_name(int a)
{
	string month;
	switch (a)
	{
		case 1 :
		month="             FARVARDIN             "; break;
		case 2 :
		month="            ORDIBEHESHT            "; break;
		case 3 :
		month="              KHORDAD              "; break;
		case 4 :
		month="                TIR                "; break;
		case 5 :
		month="              MORDAD               "; break;
		case 6 :
		month="             SHAHRIVAR             "; break;
		case 7 :
		month="                MEHR               "; break;
		case 8 :
		month="                ABAN               "; break;
		case 9 :
		month="                AZAR               "; break;
		case 10 :
		month="                DAYE               "; break;
		case 11 :
		month="               BAHMAN              "; break;
		case 12 :
		month="               ESFAND              "; break;
	}
	
	return month;
}

void menu ()
{
	print_signal.acquire();

	cout<<"-------------------------------------------------\n";
	cout<<"                Solar Calender"<<endl;
	cout<<"-------------------------------------------------\n\n\n";
	cout<<"           Choose First Day Of The Year: "; 
	cout<<"\n-------------------------------------------------\n";
	cout<<"                [1] Saturday"<<endl;
	cout<<"                [2] Sunday"<<endl;
	cout<<"                [3] Monday"<<endl;
	cout<<"                [4] Tuesday"<<endl;
	cout<<"                [5] Wednesday"<<endl;
	cout<<"                [6] Thursday"<<endl;
	cout<<"                [7] Friday"<<endl;
	cout<<"_________________________________________________\n";
	cout<<"Choose a menu option, or press ESC to exit:";

	print_signal.release();	
}

int getfirstday()
{
	int n=getch();
	if (n==27) exit(0);
	while(n<49 || n>55)
	{
		cout<<"\n\nEntered Key Is Wrong!\n";
		cout<<"_________________________________________________\n";
		cout<<"Choose a menu option, or press ESC to exit:";

		n=getch();
		if (n==27) exit(0);
	}
	system("cls");
	
	int answer=n - '0' ;
	return answer;
}

void leapmenu()
{
	cout<<"-------------------------------------------------\n";
	cout<<"                Solar Calender"<<endl;
	cout<<"-------------------------------------------------\n\n\n";
	cout<<"             Are we in a leap year? "; 
	cout<<"\n-------------------------------------------------\n";
	cout<<"                [y] Yes"<<endl;
	cout<<"                [n] No "<<endl;
	cout<<"_________________________________________________\n";
	cout<<"Choose a menu option, or press ESC to exit:";
}

int leapcheck()
{
	leapmenu();
	getk:	
	int k=getch();
	if (k==27) exit(0);
	if (k==121 || k==89) 
	{
		system("cls");
		return 1;
	}
	else if (k==110 || k==78) 
	{
		system("cls");
		return 0;
	}
	else 
	{
		cout<<"\n\nEntered Key Is Wrong!\n";
		cout<<"_________________________________________________\n";
		cout<<"Choose a menu option, or press ESC to exit:";
		goto getk;	
	}
}

void set_menu_size()
{
	system("MODE CON COLS=50")  ;
}

void set_calendar_size()
{
	size_menu:
	cout<<"-------------------------------------------------\n";
	cout<<"                Solar Calender"<<endl;
	cout<<"-------------------------------------------------\n\n\n";
	cout<<"      Enter the calendar size you want: "; 
	cout<<"\n-------------------------------------------------\n";
	cout<<"                [1] 4*3"<<endl;
	cout<<"                [2] 3*4"<<endl;
	cout<<"                [3] 2*6"<<endl;
	cout<<"                [4] 1*12"<<endl;
	cout<<"_________________________________________________\n";
	cout<<"Choose a menu option, or press ESC to exit:";
	
	getsize:	
	int s=getch();
	if (s==27) exit(0);
	else if (s==49) 
	{
	system("cls");
	system("MODE CON COLS=168") ;
		if(consolesizex()<152) 
		{
			set_menu_size();
			cout<<"-------------------------------------------------\n";
			cout<<"                Solar Calender"<<endl;
			cout<<"-------------------------------------------------\n";
			cout<<"\n\nThis size does not work on your monitor! ";
			cout<<"\n-------------------------------------------------\n";
			cout<<"\n\n Press a key to try another number... ";
			getch();
			system("cls");
			goto size_menu;
		}
		
		else return;
	}
	
	else if (s==50) 
	{
		system("cls");
		system("MODE CON COLS=130") ;
		if(consolesizex()<114) 
		{
			set_menu_size();
			cout<<"-------------------------------------------------\n";
			cout<<"                Solar Calender"<<endl;
			cout<<"-------------------------------------------------\n";
			cout<<"\n\nThis size does not work on your monitor! ";
			cout<<"\n-------------------------------------------------\n";
			cout<<"\n\n Press a key to try another number... ";
			getch();
			system("cls");
			goto size_menu;
		}
		
		else return;
	}
	
	else if (s==51) 
	{
		system("cls");
		system("MODE CON COLS=92") ;
		return;
	}
	else if (s==52)
	{
		system("cls");
		system("MODE CON COLS=54") ;
		return;
	}
	else 
	{
	cout<<"\n\nEntered Key Is Wrong!\n";
	cout<<"_________________________________________________\n";
	cout<<"Choose a menu option, or press ESC to exit:";
	goto getsize;	
	}
}

void weekendcheck(int monthnumber , int mday , int wday , int theme )
{
	if (monthnumber==1)
	if (mday==1 || mday==2 || mday==3 ||mday==4 ||mday==5 || mday==12 ||mday==13) weekend_color(theme);
	if (monthnumber==12 && mday==29) weekend_color(theme);
	if (wday==7) weekend_color(theme);
}

int month_calendar(int firstday , int monthnumber , int new_x , int new_y , int kabise , int theme)
{	
	new_x += 16;
	if (monthnumber<=3) text_color(43);
	else if (monthnumber<=6) text_color(91) ;
	else if (monthnumber<=9) text_color(27) ; 
	else if (monthnumber<=12) text_color(75) ; 
	
	print_signal.acquire();

	gotoxy(new_x,new_y);
	cout<<month_name(monthnumber);
	
	new_y++ ; gotoxy(new_x,new_y);
	cout<<"-----------------------------------";

	if (theme==1) text_color(13);
	if (theme==2) background_text_color(9 , 15);
	
	new_y++ ; gotoxy(new_x,new_y);
	cout<<" Saturday";
	int x=wherex();
	int y=wherey();
	
	new_y++ ; gotoxy(new_x,new_y);
	cout<<"   Sunday";
	
	new_y++ ; gotoxy(new_x,new_y);
	cout<<"   Monday";
	
	new_y++ ; gotoxy(new_x,new_y);
	cout<<"  Tuesday";
	
	new_y++ ; gotoxy(new_x,new_y);
	cout<<"Wednesday";
	
	new_y++ ; gotoxy(new_x,new_y);
	cout<<" Thursday";
	
	new_y++ ; gotoxy(new_x,new_y);
	cout<<"   Friday";
	int endline=wherey();
	
	defualt_color(theme);
	
	int c=31;
	if (monthnumber>6) c=30;
	if (monthnumber==12 && kabise==0)  c=29;

	int startcolumns=x+4;
	int startline=y+(firstday-1);
	
	for(int i=1 ; i<=c ; i++ , firstday=(firstday%7)+1)
	{
	
		gotoxy(startcolumns , startline);
		weekendcheck(monthnumber , i , firstday , theme);
		cout<<i;	
		startline++;
		
		defualt_color(theme);
		
		if (startline-1==endline)
		{
			startline=y;
			startcolumns+=4;
		}
	}

	print_signal.release();

return firstday;
}

int month_firstday (int year_firstday , int monthnumber)
{
	
int c;
for(int mah=1 ; mah<monthnumber ; mah++)
{
	if (mah<=6) c=31;
	else if (mah>6) c=30;
	
	for(int rooz=1 ; rooz<=c ; rooz++)
	{
		year_firstday=(year_firstday%7)+1;
	}
	
}
return year_firstday;

}

void monthmenu(string a)
{
	cout<<"-------------------------------------------------\n";
	cout<<"                Solar Calender"<<endl;
	cout<<"-------------------------------------------------\n\n\n";
	cout<<"           Select the "<<a<<" month you want: "; 
	cout<<"\n-------------------------------------------------\n";
	cout<<"                [a] Farvardin"<<endl;
	cout<<"                [b] Ordibehesht"<<endl;
	cout<<"                [c] Khordad"<<endl;
	cout<<"                [d] Tir"<<endl;
	cout<<"                [e] Mordad"<<endl;
	cout<<"                [f] Shahrivar"<<endl;
	cout<<"                [g] Mehr"<<endl;
	cout<<"                [h] Aban"<<endl;
	cout<<"                [i] Azar"<<endl;
	cout<<"                [j] Daye"<<endl;
	cout<<"                [k] Bahman"<<endl;
	cout<<"                [l] Esfand"<<endl;
	cout<<"_________________________________________________\n";
	cout<<"Choose a menu option, or press ESC to exit:";
	
}

int choosemonth()
{
getmonth:	
	int k=getch();
	if (k==27) exit(0);
	if (k==97 || k==65) 
	{
		system("cls");
		return 1;
	}
	else if (k==66 || k==98) 
	{
		system("cls");
		return 2;
	}
	else if (k==99 || k==67) 
	{
		system("cls");
		return 3;
	}
	else if (k==68 || k==100) 
	{
		system("cls");
		return 4;
	}
	else if (k==69 || k==101) 
	{
		system("cls");
		return 5;
	}
	else if (k==70 || k==102) 
	{
		system("cls");
		return 6;
	}
	else if (k==71 || k==103) 
	{
		system("cls");
		return 7;
	}
	else if (k==72 || k==104) 
	{
		system("cls");
		return 8;
	}
	else if (k==73 || k==105) 
	{
		system("cls");
		return 9;
	}
	else if (k==74 || k==106) 
	{
		system("cls");
		return 10;
	}
	else if (k==75 || k==107) 
	{
		system("cls");
		return 11;
	}
		else if (k==76 || k==108) 
	{
		system("cls");
		return 12;
	}
	else
	{
		cout<<"\n\nEntered Key Is Wrong!\n";
		cout<<"_________________________________________________\n";
		cout<<"Choose a menu option, or press ESC to exit:";
		goto getmonth;	
	}
}

void calendar(int firstday , int firstmonth , int lastmonth , int kabise , int theme, int x = 0, int y = 0)
{
	for(int month=firstmonth ; month<=lastmonth ; month++)
	{
		firstday=month_calendar(firstday , month , x , y , kabise , theme);
		x+=37;
		if(x+37>consolesizex())
		{
			x=0;
			y+=11;
		}
	}
}

void set_theme(string a)
{
	if  (a=="Dark")
	{
		system("cls");
		system("color 0E");
		defualt_color(1);
		return;
	}
	if (a=="Light")
	{
		system("cls");
		system("color F0");
		defualt_color(2);
		return;
	}
}

int main()
{
	thread t1(print_menu_bar);
homepage:
	
	string theme="Dark";
	int theme_mode=1;

	set_theme(theme);
	defualt_color(theme_mode);
	set_menu_size();
	menu();
	
	int firstday_year=getfirstday();
	
    int kabise=leapcheck();
    
    int firstmonth=1 ; int lastmonth=12;
    int firstday=firstday_year;
    string a="Light";
calendar:
	
	set_calendar_size();
	
printcalendar:	
	
	is_in_calender_menu = true;
	calendar(firstday , firstmonth  , lastmonth , kabise , theme_mode);

	int key=key_menu(a);

	is_in_calender_menu = false;

	if (key==1) 
	{
	 system("cls");
	
	 goto homepage ;
	}
	
	else if (key==0) 
	{
	
Customize:
	 set_menu_size();
   	 system("cls");
   	 set_theme(theme);
   	 monthmenu("first");
  	 firstmonth=choosemonth();
   	 firstday=month_firstday(firstday_year , firstmonth );
    
   	 monthmenu("last");
   	 lastmonth=choosemonth();
   	 
   	 if (lastmonth<firstmonth)
   	 {
		cout<<"-------------------------------------------------\n";
		cout<<"                Solar Calender"<<endl;
		cout<<"-------------------------------------------------\n";
		cout<<"\n\n last month should not be smaller than first  ";
		cout<<"\n-------------------------------------------------\n";
		cout<<"\n\n Press a key to try another number... ";
		getch();
		goto Customize;
	 }
   	 
   	 goto calendar;
	}
	
	else if (key==3)
	{
		if (a=="Light") 
     	{
     		theme="Light";
     		set_theme(theme);
     		theme_mode=2;
			a="Dark";
     	}
     	
     	else if (a=="Dark")
     	{
			theme="Dark";
     		set_theme(theme);
     		theme_mode=1;
			a="Light";				
		}	
goto printcalendar;
	}
	t1.join();
return 0;
}	
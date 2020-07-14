/****************************************/
/*          选课系统   Ver1.00           */
/*          created by 张昊钰            */
/****************************************/
#include <iostream>
#include <fstream>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
using namespace std;


/*
 * 学生，教务，课程和相关操作的声明。
 */
struct user
{
	char	type;
	char	username[20];
	char	pw[20];
};
struct stu
{
	char	id[20];
	char	name[20];
	char	pw[20];
	int	clas;
	int	totalSelect;
};
struct course
{
	int	courseid;
	char	coursename[50];
	int	allp;
	int	selectp;
	int	leftp;
};
void gotoxy( int, int );


void HideCursor( int );


int checkuser( char* );


int checkDeanOrstu( char*, char*, char );


void regis( void );


void login( char );


void welcome( void );


/*
 * 一些涉及学生的操作声明。
 */
int countclass( void );


void checkname( char*, char* );


void updateSelp( int, int );


int checkfull( int );


int checkSelCourse( char*, int );


void reviseSelCourse( char*, int, int );


void select( char* );


void drop( char* );


void viewallsel( char* );


void student( char* );


/*
 * 一些涉及教务的操作声明。
 */
int checkcourse( char* );


int countcourse( void );


void deleteinfo( int );


void deletecourse( int );


void revisecourse( int, char*, int );


int showcourses( void );


void viewinfo( int );


void add( void );


void Delete( void );


void revise( void );


void viewcourse( void );


void dean( void );


/*
 * 主函数。
 */
int main()
{
	welcome();
	return(0);
}


/*
 * 接下来的两个函数将在程序中使用。
 */

/*
 * ///////////////////////////////
 * 函数gotoxy(int,int):光标定位//
 * ///////////////////////////////
 */
void gotoxy( int x, int y )
{
	COORD c;
	c.X	= x - 1;
	c.Y	= y - 1;
	SetConsoleCursorPosition( GetStdHandle( STD_OUTPUT_HANDLE ), c );
}


/*
 * ///////////////////////////////
 * 函数HideCursor(int):光标隐藏//
 * ///////////////////////////////
 */
void HideCursor( int mark )
{
	CONSOLE_CURSOR_INFO cursor_info = { 1, mark };
	SetConsoleCursorInfo( GetStdHandle( STD_OUTPUT_HANDLE ), &cursor_info );
}


/*
 * 接下来的功能主要是教务的。
 */

/*
 * ////////////////////////////////////////////////////////
 *   函数checkuser(char*):检查输入的用户名是否已存在    //
 * ////////////////////////////////////////////////////////
 */
int checkuser( char *s )
{
	ifstream	fin( "users.txt", ios_base::in );
	int		isfound = 0;
	string		tmp;
	char		t[50];
	while ( getline( fin, tmp ) )
	{
		int	p1	= tmp.find( ' ' );
		int	p2	= tmp.find( ' ', p1 + 1 );
		strcpy( t, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		if ( strcmp( t, s ) == 0 )
		{
			isfound = 1; break;
		}
	}
	if ( fin.eof() )
		fin.clear();
	if ( isfound )
		return(1);
	else return(0);
}


/*
 * ///////////////////////////////////////////////////////////
 *   函数checkDeanOrstu(char*,char *,char):检查对应的账号  //
 * ///////////////////////////////////////////////////////////
 */
int checkDeanOrstu( char *id, char *pw, char type )
{
	ifstream	fin( "users.txt", ios_base::in );
	int		isfound = 0;
	string		tmp;
	char		idt[25], pwt[25], typet;
	while ( getline( fin, tmp ) )
	{
		typet = tmp[0];
		if ( typet != type )
			continue;
		int	p1	= tmp.find( ' ' );
		int	p2	= tmp.find( ' ', p1 + 1 );
		strcpy( idt, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		if ( strcmp( idt, id ) )
			continue;
		strcpy( pwt, tmp.substr( p2 + 1 ).c_str() );
		if ( strcmp( pwt, pw ) )
			continue;
		else{ isfound = 1; break; }
	}
	if ( fin.eof() )
		fin.clear();
	if ( isfound )
		return(1);
	else return(0);
}


/*
 * ////////////////////////////////////////////////////////
 *   函数checkcourse(char*):检查输入的课程是否已存在    //
 * ////////////////////////////////////////////////////////
 */
int checkcourse( char *s )
{
	ifstream fin( "courses.txt", ios_base::in );
	if ( !fin.is_open() )
		return(0);
	string	tmp;
	int	isfound = 0;
	char	coursename[50];
	while ( getline( fin, tmp ) )
	{
		int	p1	= tmp.find( ' ' );
		int	p2	= tmp.find( ' ', p1 + 1 );
		strcpy( coursename, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		if ( strcmp( s, coursename ) == 0 )
		{
			isfound = 1; break;
		}else continue;
	}
	if ( isfound )
		return(1);
	else return(0);
}


/*
 * ///////////////////////////////////////////////////
 *   函数coursecount():统计当前已添加的课程总数    //
 * ///////////////////////////////////////////////////
 */
int countcourse()
{
	int		cnt = 0;
	string		tmp;
	ifstream	fin( "courses.txt", ios_base::in );
	while ( getline( fin, tmp ) )
		cnt++;
	fin.close();
	fin.clear();
	return(cnt);
}


/*
 * ////////////////////////////////////////////
 *   函数deleteinfo(int): 删除学生选课信息  //
 * ////////////////////////////////////////////
 */
void deleteinfo( int id )
{
/* 查看所有学生信息，对已选课程_id_的学生进行修改 */
	ifstream	fin( "students.txt", ios_base::in );
	string		tmp;
	stu		newstu[100];
	int		cnt = 0;
	int		sels[100][20];
	while ( getline( fin, tmp ) )
	{
		cnt++;
		sels[cnt][0] = 0;
		int p1 = tmp.find( ' ' );
		strcpy( newstu[cnt].id, tmp.substr( 0, p1 ).c_str() );
		int p2 = tmp.find( ' ', p1 + 1 );
		strcpy( newstu[cnt].pw, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		p1 = tmp.find( ' ', p2 + 1 );
		strcpy( newstu[cnt].name, tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
		p2			= tmp.find( ' ', p1 + 1 );
		newstu[cnt].clas	= atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		p1			= tmp.find( ' ', p2 + 1 );
		newstu[cnt].totalSelect = atoi( tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
		for ( int i = 1; i <= newstu[cnt].totalSelect; i++ )
		{
			p2 = tmp.find( ' ', p1 + 1 );
			int cid = atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
			if ( cid == id )
				sels[cnt][0] = 1;
			sels[cnt][i]	= cid;
			p1		= p2;
		}
	}
	fin.close();
	fin.clear();
	ofstream fout( "students.txt", ios_base::out | ios_base::trunc );
	for ( int i = 1; i <= cnt; i++ )
	{
		fout	<< newstu[i].id << ' ' << newstu[i].pw << ' ' << newstu[i].name << ' ' << newstu[i].clas << ' '
			<< newstu[i].totalSelect - sels[i][0] << ' ';
		for ( int j = 1; j <= newstu[i].totalSelect; j++ )
		{
			if ( sels[i][j] < id )
				fout << sels[i][j] << ' ';
			else if ( sels[i][j] > id )
				fout << sels[i][j] - 1 << ' ';
		}
		fout << '\n';
	}
	fout.close();
	fout.clear();
	return;
}


/*
 * //////////////////////////////////////////
 *   函数deletecourse(int): 删除当前课程  //
 * //////////////////////////////////////////
 */
void deletecourse( int id )
{
/* 除了要删除的课程之外全部重新写入 */
	deleteinfo( id );
	course		cs[20];
	ifstream	fin( "courses.txt", ios_base::in );
	int		cnt = 0;
	int		idt;
	string		tmp;
	while ( getline( fin, tmp ) )
	{
		int p1 = tmp.find( ' ' );
		idt = atoi( tmp.substr( 0, p1 ).c_str() );
		if ( idt == id )
			continue;
		else{
			cnt++;
			cs[cnt].courseid = cnt;
			int p2 = tmp.find( ' ', p1 + 1 );
			strcpy( cs[cnt].coursename, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
			p1		= tmp.find( ' ', p2 + 1 );
			cs[cnt].allp	= atoi( tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
			p2		= tmp.find( ' ', p1 + 1 );
			cs[cnt].selectp = atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
			cs[cnt].leftp	= atoi( tmp.substr( p2 + 1 ).c_str() );
		}
	}
	fin.close();
	fin.clear();
	ofstream fout( "courses.txt", ios_base::out | ios_base::trunc );
	for ( int i = 1; i <= cnt; i++ )
		fout	<< cs[i].courseid << ' ' << cs[i].coursename << ' '
			<< cs[i].allp << ' ' << cs[i].selectp << ' ' << cs[i].leftp << '\n';
	fout.close();
	fout.clear();
	return;
}


/*
 * ////////////////////////////////////////////////////
 *   函数revisecourse(int,char*,int): 修改当前课程  //
 * ////////////////////////////////////////////////////
 */
void revisecourse( int id, char *newname, int newp )
{
	course		cs[20];
	ifstream	fin( "courses.txt", ios_base::in );
	int		idt;
	int		cnt = 0;
	string		tmp;
	while ( getline( fin, tmp ) )
	{
		cnt++;
		int p1 = tmp.find( ' ' );
		idt			= atoi( tmp.substr( 0, p1 ).c_str() );
		cs[cnt].courseid	= idt;
		int p2 = tmp.find( ' ', p1 + 1 );
		strcpy( cs[cnt].coursename, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		p1		= tmp.find( ' ', p2 + 1 );
		cs[cnt].allp	= atoi( tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
		p2		= tmp.find( ' ', p1 + 1 );
		cs[cnt].selectp = atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		cs[cnt].leftp	= atoi( tmp.substr( p2 + 1 ).c_str() );
		if ( idt == id )
		{
			strcpy( cs[cnt].coursename, newname );
			cs[cnt].allp	= newp;
			cs[cnt].leftp	= newp - cs[cnt].selectp;
		}
	}
	fin.close();
	fin.clear();
	ofstream fout( "courses.txt", ios_base::out | ios_base::trunc );
	for ( int i = 1; i <= cnt; i++ )
		fout	<< cs[i].courseid << ' ' << cs[i].coursename << ' '
			<< cs[i].allp << ' ' << cs[i].selectp << ' ' << cs[i].leftp << '\n';
	fout.close();
	fout.clear();
	return;
}


/*
 * ///////////////////////////////////////////////
 *   函数showcourses():显示当前所有课程信息    //
 * ///////////////////////////////////////////////
 */
int showcourses()
{
	printf( "//=================================================//\n" );
	printf( "//   课程信息如下，请选择对应课程编号或者按0退出   //\n" );
	printf( "//=================================================//\n" );
	printf( "//     ---------------------------------------     //\n" );
	printf( "//    | 课程编号 |     课程名     | 已选/全部 |    //\n" );
	printf( "//     ---------------------------------------     //\n" );
	int		tot = countcourse();
	ifstream	fin( "courses.txt", ios_base::in );
	string		tmp;
	for ( int i = 1; i <= tot; i++ )
	{
		getline( fin, tmp );
		int	id, allp, selp;
		char	name[50];
		int	p1	= tmp.find( ' ' );
		int	p2	= tmp.find( ' ', p1 + 1 );
		id = atoi( tmp.substr( 0, p1 ).c_str() );
		strcpy( name, tmp.substr( p1, p2 - p1 ).c_str() );
		p1	= tmp.find( ' ', p2 + 1 );
		allp	= atoi( tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
		p2	= tmp.find( ' ', p1 + 1 );
		selp	= atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		printf( "//    | %-8d | %-14s | %4d/%-4d |    //\n", id, name, selp, allp );
		printf( "//     ---------------------------------------     //\n" );
	}
	fin.close();
	fin.clear();
	printf( "//=================================================//\n" );
	printf( "//                                                 //\n" );    /* line:8+2*tot; */
	printf( "//                                                 //\n" );    /* line:9+2*tot */
	printf( "//                                                 //\n" );    /* line:10+2*tot */
	printf( "//                                                 //\n" );    /* line:11+2*tot */
	printf( "//=================================================//\n" );
	return(tot);
}


/*
 * /////////////////////////////////////////
 *   函数regis():为老师或学生注册账号    //
 * /////////////////////////////////////////
 */
void regis()
{
	system( "cls" );
	char	ch, type;
	user	newid;
	stu	newstu;
	printf( "//=======================================================//\n" );
	printf( "//      接下来请依次选择您的身份，并输入用户名和密码     //\n" );
	printf( "//               用户名和密码不超过20个字符              //\n" );
	printf( "//=======================================================//\n" );
	printf( "//        选择您的身份：1(教师)      0(学生)             //\n" );                 /* line 5 */
	printf( "//                                                       //\n" );      /* line 6 */
	printf( "//                                                       //\n" );      /* line 7 */
	printf( "//                                                       //\n" );      /* line 8 */
	printf( "//                                                       //\n" );      /* line 9 */
	printf( "//                                                       //\n" );      /* line 10 */
	printf( "//=======================================================//\n" );
	printf( "//                                                       //\n" );      /* line 12 */
	printf( "//                                                       //\n" );      /* line 13 */
	printf( "//=======================================================//\n" );
	HideCursor( 0 );
	while ( 1 )
	{
		ch = getch();
		if ( ch == '1' || ch == '0' )
		{
			newid.type = ch; break;
		}
	}
	type = ch;
	HideCursor( 1 );
	gotoxy( 11, 6 );
	if ( ch == '1' )
		printf( "输入您的账号：" );
	else printf( "输入您的学号：" );
	while ( 1 )
	{
		HideCursor( 1 );
		scanf( "%s", &newid.username );
		if ( ch == '0' )
			strcpy( newstu.id, newid.username );
		if ( checkuser( newid.username ) )
		{
			gotoxy( 16, 7 );
			printf( "        用户名已存在！" );
			HideCursor( 0 );
			Sleep( 1000 );
			gotoxy( 16, 7 );
			printf( "                         " );
			gotoxy( 25, 6 );
			printf( "                         " );
			gotoxy( 25, 6 );
		}else break;
	}
	gotoxy( 11, 7 );
	printf( "                                       " );
	gotoxy( 11, 7 );
	printf( "输入您的密码：" );
	scanf( "%s", &newid.pw );
	if ( ch == '0' )
	{
		strcpy( newstu.pw, newid.pw );
		gotoxy( 11, 8 );
		printf( "输入您的姓名：" );
		scanf( "%s", newstu.name );
		gotoxy( 11, 9 );
		printf( "输入您的班级：" );
		scanf( "%d", &newstu.clas );
	}
	HideCursor( 0 );
	gotoxy( 13, 12 );
	printf( "请确认您的输入：1(确认)  0(取消)" );
	while ( 1 )
	{
		char ch = getch();
		if ( ch == '1' )
		{
			ofstream fout( "users.txt", ios_base::app | ios_base::out );
			fout << newid.type << ' ' << newid.username << ' ' << newid.pw << "\n";
			if ( type == '0' )
			{
				fout.clear();
				fout.close();
				fout.open( "students.txt", ios_base::app | ios_base::out );
				fout << newstu.id << ' ' << newstu.pw << ' ' << newstu.name << ' ' << newstu.clas << ' ' << 0 << ' ' << '\n';
			}
			fout.clear();
			fout.close();
			gotoxy( 25, 13 );
			printf( "注册成功！\n" );
			Sleep( 1500 );
			return;
		}else if ( ch == '0' )
		{
			gotoxy( 25, 13 );
			printf( "注册失败！" );
			Sleep( 1500 );
			return;
		}
	}
}


/*
 * //////////////////////////////
 *   函数add():老师添加课程   //
 * //////////////////////////////
 */
void add()
{
	char	ch;
	course	newcourse;
	while ( 1 )
	{
		system( "cls" );
		printf( "//============================================//\n" );
		printf( "// 请添加课程，您需要输入课程名和最大选课人数 //\n" );
		printf( "//============================================//\n" );
		printf( "//      请输入课程名：                        //\n" );
		printf( "//                                            //\n" );
		printf( "//                                            //\n" );
		printf( "//============================================//\n" );
		printf( "//                                            //\n" );
		printf( "//                                            //\n" );
		printf( "//============================================//\n" );
		gotoxy( 23, 4 );
		while ( 1 )
		{
			HideCursor( 1 );
			scanf( "%s", newcourse.coursename );
			if ( checkcourse( newcourse.coursename ) )
			{
				gotoxy( 15, 5 );
				printf( "课程已存在，是否退出？" );
				HideCursor( 0 );
				gotoxy( 14, 6 ); printf( "1(继续输入)  0(我要退出)" );
				while ( 1 )
				{
					ch = getch();
					if ( ch == '1' || ch == '0' )
						break;
				}
				if ( ch == '0' )
				{
					gotoxy( 19, 8 );
					printf( "添加失败！" );
					Sleep( 1500 );
					return;
				}
				gotoxy( 14, 6 ); printf( "                         " );
				gotoxy( 15, 5 ); printf( "                         " );
				gotoxy( 23, 4 ); printf( "                     " );
				gotoxy( 23, 4 );
			}else break;
		}
		gotoxy( 19, 5 ); printf( "                       " );
		gotoxy( 9, 5 );
		printf( "请输入课程人数：" );
		scanf( "%d", &newcourse.allp );
		gotoxy( 9, 6 );
		HideCursor( 0 );
		printf( "请选择：1(确认添加)   0(取消添加)" );
		while ( 1 )
		{
			ch = getch();
			if ( ch == '1' || ch == '0' )
				break;
		}
		if ( ch == '1' )
		{
			int tot = countcourse();
			++tot;
			ofstream fout( "courses.txt", ios_base::out | ios_base::app );
			fout << tot << ' ' << newcourse.coursename << ' ' << newcourse.allp << ' ' << 0 << ' ' << newcourse.allp << '\n';
			fout.close();
			fout.clear();
			HideCursor( 1 );
			gotoxy( 11, 8 ); printf( "添加课程成功，是否继续添加？" );
			HideCursor( 0 );
			gotoxy( 12, 9 ); printf( "1(继续添加)    0(返回)" );
			while ( 1 )
			{
				ch = getch();
				if ( ch == '1' || ch == '0' )
					break;
			}
			if ( ch == '1' )
				continue;
			else break;
		}else  {
			gotoxy( 19, 8 );
			printf( "添加失败！" );
			Sleep( 1500 );
			break;
		}
	}
	return;
}


/*
 * /////////////////////////////////
 *   函数Delete():老师删除课程   //
 * /////////////////////////////////
 */
void Delete()
{
	char	ch[2];
	char	c;
	while ( 1 )
	{
		int isquit = 0;
		system( "cls" );
		HideCursor( 0 );
		int tot = showcourses();
		while ( !isquit )
		{
			gotoxy( 6, 8 + 2 * tot );
			HideCursor( 1 );
			printf( "请输入您想删除的课程编号：" ); scanf( "%s", ch );
			int id = atoi( ch );
			if ( id == 0 )
				return;
			gotoxy( 6, 9 + 2 * tot );
			if ( id >= 1 && id <= tot )
			{
				printf( "您确定要删除课程 %d 吗：1(确认)  0(取消)", id );
				HideCursor( 0 );
				while ( 1 )
				{
					c = getch();
					if ( c == '1' )
					{
						deletecourse( id );
						gotoxy( 6, 10 + 2 * tot );
						printf( "删除成功,是否继续删除：1(继续)  0(退出)" );
						HideCursor( 0 );
						while ( 1 )
						{
							c = getch();
							if ( c == '0' || c == '1' )
								break;
						}
						if ( c == '1' )
						{
							isquit = 1; break;
						}else return;
					}else if ( c == '0' )
					{
						gotoxy( 19, 10 + 2 * tot );
						printf( "删除课程失败！" );
						Sleep( 1500 );
						return;
					}
				}
			}else  {
				printf( "错误的课程号，请重新输入。" );
				gotoxy( 16, 8 + 2 * tot ); printf( "                  " );
				continue;
			}
		}
	}
	return;
}


/*
 * /////////////////////////////////
 *   函数revise():老师修改课程   //
 * /////////////////////////////////
 */
void revise()
{
	char	ch[2], newname[50], c;
	int	newp;
	while ( 1 )
	{
		int isquit = 0;
		system( "cls" );
		HideCursor( 0 );
		int tot = showcourses();
		while ( !isquit )
		{
			gotoxy( 6, 8 + 2 * tot );
			HideCursor( 1 );
			printf( "请输入您想修改的课程编号：" );
			scanf( "%s", ch );
			int id = atoi( ch );
			if ( id == 0 )
				return;
			gotoxy( 6, 9 + 2 * tot );
			if ( id >= 1 && id <= tot )
			{
				printf( "                               " );
				gotoxy( 6, 9 + 2 * tot );
				printf( "请输入新的课程名称：" ); scanf( "%s", newname );
				gotoxy( 6, 10 + 2 * tot );
				printf( "请输入新的选课人数：" ); scanf( "%d", &newp );
				gotoxy( 6, 11 + 2 * tot );
				HideCursor( 0 );
				printf( "您确定要修改课程 %d 吗：1(确认)  0(取消)", id );
				while ( 1 )
				{
					c = getch();
					if ( c == '1' )
					{
						revisecourse( id, newname, newp );
						gotoxy( 6, 11 + 2 * tot );
						printf( "                                          " );
						gotoxy( 6, 11 + 2 * tot );
						printf( "修改课程成功，是否继续修改？1(继续) 0(退出)" );
						HideCursor( 0 );
						while ( 1 )
						{
							c = getch();
							if ( c == '0' || c == '1' )
								break;
						}
						if ( c == '1' )
						{
							isquit = 1; break;
						}else return;
					}else if ( c == '0' )
					{
						gotoxy( 6, 11 + 2 * tot );
						printf( "                                          " );
						gotoxy( 19, 11 + 2 * tot );
						printf( "修改课程失败！" );
						Sleep( 1500 );
						return;
					}
				}
			}else  {
				printf( "错误的课程号，请重新输入。" );
				gotoxy( 16, 8 + 2 * tot ); printf( "                         " );
				continue;
			}
		}
	}
}


/*
 * //////////////////////////////////////
 *   函数viewinfo(int):查看课程信息   //
 * //////////////////////////////////////
 */
void viewinfo( int id )
{
	system( "cls" );
	ifstream	fin( "courses.txt", ios_base::in );
	int		cnt = 0;
	char		coursename[50];
	string		tmp;
	while ( getline( fin, tmp ) )
	{
		int	p1	= tmp.find( ' ' );
		int	idt	= atoi( tmp.substr( 0, p1 ).c_str() );
		if ( idt == id )
		{
			int p2 = tmp.find( ' ', p1 + 1 );
			strcpy( coursename, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
			break;
		}
	}
	fin.close();
	fin.clear();
	printf( "//=================================================//\n" );
	printf( "//     %20s 的选课情况如下         //\n", coursename );
	printf( "//=================================================//\n" );
	printf( "//      -------------------------------------      //\n" );
	printf( "//      |序号|    学号    |   姓名   | 班级 |      //\n" );
	printf( "//      -------------------------------------      //\n" );
	fin.open( "students.txt", ios_base::in );
	stu newstu;
	while ( getline( fin, tmp ) )
	{
		int p1 = tmp.find( ' ' );
		strcpy( newstu.id, tmp.substr( 0, p1 ).c_str() );
		int p2 = tmp.find( ' ', p1 + 1 );
		p1 = tmp.find( ' ', p2 + 1 );
		strcpy( newstu.name, tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
		p2			= tmp.find( ' ', p1 + 1 );
		newstu.clas		= atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		p1			= tmp.find( ' ', p2 + 1 );
		newstu.totalSelect	= atoi( tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
		if ( newstu.totalSelect == 0 )
			continue;
		else{
			int isfound = 0, q;
			for ( int i = 1; i <= newstu.totalSelect; i++ )
			{
				q = tmp.find( ' ', p1 + 1 );
				int sel = atoi( tmp.substr( p1 + 1, q - p1 - 1 ).c_str() );
				if ( sel == id )
				{
					isfound = 1;
					break;
				}
				p1 = q;
			}
			if ( isfound )
			{
				cnt++;
				printf( "//      |%-4d|%-12s|%-10s|%-6d|      //\n", cnt, newstu.id, newstu.name, newstu.clas );
				printf( "//      -------------------------------------      //\n" );
			}
		}
	}
	printf( "//=================================================//\n" );
	printf( "//                  按任意键返回                   //\n" );
	printf( "//=================================================//\n" );
	HideCursor( 0 );
	char ch = getch();
	return;
}


/*
 * /////////////////////////////////////
 *   函数viewcourse():查看选课情况   //
 * /////////////////////////////////////
 */
void viewcourse()
{
	char ch[2];
	while ( 1 )
	{
		system( "cls" );
		int	isquit	= 0;
		int	tot	= showcourses();
		while ( !isquit )
		{
			HideCursor( 1 );
			gotoxy( 6, 8 + 2 * tot );
			printf( "请输入您想查看的课程的课程编号：" ); scanf( "%s", ch );
			int id = atoi( ch );
			if ( id == 0 )
				return;
			if ( id >= 1 && id <= tot )
				isquit = 1, viewinfo( id );
			else{
				gotoxy( 6, 9 + 2 * tot );
				printf( "错误的课程号，请重新输入！" );
				gotoxy( 6, 8 + 2 * tot );
				printf( "                                   " );
				continue;
			}
		}
	}
}


/*
 * /////////////////////////////
 *   函数dean():教师主界面   //
 * /////////////////////////////
 */
void dean()
{
	while ( 1 )
	{
		system( "cls" );
		char ch;
		HideCursor( 0 );
		printf( "//=======================================//\n" );
		printf( "//          老师您好，请选择！           //\n" );
		printf( "//=======================================//\n" );
		printf( "//              1.添加课程               //\n" );
		printf( "//              2.删除课程               //\n" );
		printf( "//              3.修改课程               //\n" );
		printf( "//              4.查看结果               //\n" );
		printf( "//              5.返回                   //\n" );
		printf( "//=======================================//\n" );
		ch = getch();
		if ( ch == '1' )
			add();
		else if ( ch == '2' )
			Delete();
		else if ( ch == '3' )
			revise();
		else if ( ch == '4' )
			viewcourse();
		else if ( ch == '5' )
			break;
		else continue;
	}
	return;
}


/*
 * 下面的功能主要针对学生。
 */

/*
 * /////////////////////////////////////////
 *   函数countclass():统计当前班级总数   //
 * /////////////////////////////////////////
 */
int countclass()
{
	int		cnt = 0;
	ifstream	fin( "students.txt", ios_base::in );
	string		tmp;
	while ( getline( fin, tmp ) )
	{
		int	p1	= tmp.find( ' ' );
		int	p2	= tmp.find( ' ', p1 + 1 );
		p1	= tmp.find( ' ', p2 + 1 );
		p2	= tmp.find( ' ', p1 + 1 );
		int curClass = atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		if ( curClass > cnt )
			cnt = curClass;
	}
	fin.close();
	fin.clear();
	return(cnt);
}


/*
 * ///////////////////////////////////////////////////
 *   函数checkname(char*,char*):找到对应id的姓名   //
 * ///////////////////////////////////////////////////
 */
void checkname( char *id, char *name )
{
	ifstream	fin( "students.txt", ios_base::in );
	string		tmp;
	int		p1, p2;
	while ( getline( fin, tmp ) )
	{
		p1 = tmp.find( ' ' );
		if ( strcmp( id, tmp.substr( 0, p1 ).c_str() ) == 0 )
		{
			p2	= tmp.find( ' ', p1 + 1 );
			p1	= tmp.find( ' ', p2 + 1 );
			strcpy( name, tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
			fin.close();
			return;
		}
	}
}


/*
 * ////////////////////////////////////////////////
 *   函数updateSelp(int,int):更新选课人数信息   //
 * ////////////////////////////////////////////////
 */
void updateSelp( int courseid, int type )
{
/* type==1 means a new selection while -1 a deletion of courseid */
	ifstream	fin( "courses.txt", ios_base::in );
	course		newc[20];
	int		cnt = 0;
	int		p1, p2;
	string		tmp;
	while ( getline( fin, tmp ) )
	{
		cnt++;
		p1			= tmp.find( ' ' );
		newc[cnt].courseid	= atoi( tmp.substr( 0, p1 ).c_str() );
		p2			= tmp.find( ' ', p1 + 1 );
		strcpy( newc[cnt].coursename, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		p1			= tmp.find( ' ', p2 + 1 );
		newc[cnt].allp		= atoi( tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
		p2			= tmp.find( ' ', p1 + 1 );
		newc[cnt].selectp	= atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		newc[cnt].leftp		= atoi( tmp.substr( p2 + 1 ).c_str() );
		if ( newc[cnt].courseid == courseid )
		{
			if ( type == 1 )
			{
				newc[cnt].leftp--;
				newc[cnt].selectp++;
			}else  {
				newc[cnt].leftp++;
				newc[cnt].selectp--;
			}
		}
	}
	fin.close();
	fin.clear();
	ofstream fout( "courses.txt", ios_base::out | ios_base::trunc );
	for ( int i = 1; i <= cnt; i++ )
		fout	<< newc[i].courseid << ' ' << newc[i].coursename << ' ' << newc[i].allp << ' ' << newc[i].selectp << ' '
			<< newc[i].leftp << '\n';
	fout.close();
	fout.clear();
	return;
}


/*
 * ///////////////////////////////////////////////
 *   函数checkfull(int):检查选课人数是否已满   //
 * ///////////////////////////////////////////////
 */
int checkfull( int courseid )
{
	ifstream	fin( "courses.txt", ios_base::in );
	string		tmp;
	int		p1, p2;
	while ( getline( fin, tmp ) )
	{
		p1 = tmp.find( ' ' );
		if ( atoi( tmp.substr( 0, p1 ).c_str() ) == courseid )
		{
			p2	= tmp.find( ' ', p1 + 1 );
			p1	= tmp.find( ' ', p2 + 1 );
			p2	= tmp.find( ' ', p1 + 1 );
			if ( atoi( tmp.substr( p2 + 1 ).c_str() ) <= 0 )
				return(1);
			else return(0);
		}
	}
}


/*
 * //////////////////////////////////////////////////
 *   函数checkSelCourse(char*,int):检查是否选课   //
 * //////////////////////////////////////////////////
 */
int checkSelCourse( char *id, int courseid )
{
	ifstream	fin( "students.txt", ios_base::in );
	string		tmp;
	char		idt[20];
	int		p1, p2;
	while ( getline( fin, tmp ) )
	{
		p1 = tmp.find( ' ' );
		strcpy( idt, tmp.substr( 0, p1 ).c_str() );
		if ( strcmp( idt, id ) == 0 )
		{
			p2	= tmp.find( ' ', p1 + 1 );
			p1	= tmp.find( ' ', p2 + 1 );
			p2	= tmp.find( ' ', p1 + 1 );
			p1	= tmp.find( ' ', p2 + 1 );
			int seln = atoi( tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
			for ( int i = 1; i <= seln; i++ )
			{
				p2 = tmp.find( ' ', p1 + 1 );
				int cid = atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
				if ( cid == courseid )
					return(1);
				p1 = p2;
			}
			return(0);
		}
	}
	return(0);
}


/*
 * ///////////////////////////////////////////////////////////
 *   函数reviseSelCourse(char*,int,int):修改学生选课信息   //
 * ///////////////////////////////////////////////////////////
 */
void reviseSelCourse( char *id, int courseid, int type )
{
	updateSelp( courseid, type );
/* type==1表示新建课程id， -1表示新建删除课程id */
	ifstream	fin( "students.txt", ios_base::in );
	string		tmp;
	stu		newstu[100];
	int		p1, p2, cnt = 0;
	int		sels[100][20];
	while ( getline( fin, tmp ) )
	{
		cnt++;
		p1 = tmp.find( ' ' );
		strcpy( newstu[cnt].id, tmp.substr( 0, p1 ).c_str() );
		p2 = tmp.find( ' ', p1 + 1 );
		strcpy( newstu[cnt].pw, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		p1 = tmp.find( ' ', p2 + 1 );
		strcpy( newstu[cnt].name, tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
		p2			= tmp.find( ' ', p1 + 1 );
		newstu[cnt].clas	= atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		p1			= tmp.find( ' ', p2 + 1 );
		newstu[cnt].totalSelect = atoi( tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
		for ( int i = 1; i <= newstu[cnt].totalSelect; i++ )
		{
			p2		= tmp.find( ' ', p1 + 1 );
			sels[cnt][i]	= atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
			p1		= p2;
		}
		if ( strcmp( id, newstu[cnt].id ) == 0 )
		{
			if ( type == 1 )
				sels[cnt][++newstu[cnt].totalSelect] = courseid;
			else{
				int i = 1, j = 1;
				while ( i <= newstu[cnt].totalSelect )
				{
					if ( sels[cnt][i] != courseid )
						sels[cnt][j] = sels[cnt][i], j++, i++;
					else i++;
				}
				newstu[cnt].totalSelect--;
			}
		}
	}
	fin.close();
	fin.clear();
	ofstream fout( "students.txt", ios_base::out | ios_base::trunc );
	for ( int i = 1; i <= cnt; i++ )
	{
		fout	<< newstu[i].id << ' ' << newstu[i].pw << ' ' << newstu[i].name << ' ' << newstu[i].clas << ' '
			<< newstu[i].totalSelect << ' ';
		for ( int j = 1; j <= newstu[i].totalSelect; j++ )
			fout << sels[i][j] << ' ';
		fout << '\n';
	}
	fin.close();
	fin.clear();
}


/*
 * //////////////////////////////////
 *   函数select(char*):学生选课   //
 * //////////////////////////////////
 */
void select( char *id )
{
	char	ch;
	char	s[10];
	char	name[20];
	checkname( id, name );
	int	courseid;
	course	newc;
	while ( 1 )
	{
		system( "cls" );
		printf( "//=================================================//\n" );
		printf( "// %8s同学你好，请选择对应的课程或者输入0退出 //\n", name );
		printf( "//=================================================//\n" );
		printf( "//      --------------------------------------     //\n" );
		printf( "//     | 课程编号 |     课程名     | 剩余人数 |    //\n" );
		printf( "//      --------------------------------------     //\n" );
		int		tot = countcourse();
		ifstream	fin( "courses.txt", ios_base::in );
		string		tmp;
		for ( int i = 1; i <= tot; i++ )
		{
			getline( fin, tmp );
			char	name[50];
			int	p1	= tmp.find( ' ' );
			int	p2	= tmp.find( ' ', p1 + 1 );
			newc.courseid = atoi( tmp.substr( 0, p1 ).c_str() );
			strcpy( newc.coursename, tmp.substr( p1, p2 - p1 ).c_str() );
			p1		= tmp.find( ' ', p2 + 1 );
			p2		= tmp.find( ' ', p1 + 1 );
			newc.leftp	= atoi( tmp.substr( p2 + 1 ).c_str() );
			printf( "//     | %-8d | %-14s | %-8d |    //\n", newc.courseid, newc.coursename, newc.leftp );
			printf( "//      --------------------------------------     //\n" );
		}
		fin.close();
		fin.clear();
		printf( "//=================================================//\n" );
		printf( "//                                                 //\n" );    /* line:8+2*tot; */
		printf( "//                                                 //\n" );    /* line:9+2*tot */
		printf( "//                                                 //\n" );    /* line:10+2*tot */
		printf( "//                                                 //\n" );    /* line:11+2*tot */
		printf( "//=================================================//\n" );
		while ( 1 )
		{
			gotoxy( 14, 8 + 2 * tot );
			printf( "请输入待选择的课程号：" );
			HideCursor( 1 );
			scanf( "%s", s );
			courseid = atoi( s );
			if ( courseid == 0 )
				return;
			else if ( courseid >= 1 && courseid <= tot )
			{
				if ( checkSelCourse( id, courseid ) )
				{
					gotoxy( 12, 9 + 2 * tot ); printf( "                            " );
					gotoxy( 20, 9 + 2 * tot );
					printf( "请勿重复选课！" );
					HideCursor( 0 );
					Sleep( 1000 );
					gotoxy( 15, 9 + 2 * tot );
					printf( "                    " );
					gotoxy( 20, 8 + 2 * tot );
					printf( "                       " );
					continue;
				}else if ( checkfull( courseid ) )
				{
					gotoxy( 12, 9 + 2 * tot ); printf( "                            " );
					gotoxy( 20, 9 + 2 * tot );
					printf( "课程名额不足！" );
					HideCursor( 0 );
					Sleep( 1000 );
					gotoxy( 15, 9 + 2 * tot );
					printf( "                    " );
					gotoxy( 20, 8 + 2 * tot );
					printf( "                       " );
					continue;
				}else break;
			}else  {
				gotoxy( 12, 9 + 2 * tot );
				printf( "错误的课程号，请重新输入。" );
				gotoxy( 20, 8 + 2 * tot );
				printf( "                       " );
				continue;
			}
		}
		gotoxy( 8, 9 + 2 * tot );
		printf( "您确定要选择课程 %d 吗：1(确定)  0(取消)", courseid );
		HideCursor( 0 );
		while ( 1 )
		{
			ch = getch();
			if ( ch == '0' )
			{
				gotoxy( 21, 10 + 2 * tot );
				printf( "选课失败！" );
				Sleep( 1500 );
				return;
			}else if ( ch == '1' )
				break;
		}
		HideCursor( 1 );
		reviseSelCourse( id, courseid, 1 );
		gotoxy( 14, 10 + 2 * tot );
		printf( "选课成功，是否继续选课？" );
		HideCursor( 0 );
		gotoxy( 16, 11 + 2 * tot );
		printf( "1(继续选课) 0(退出)" );
		while ( 1 )
		{
			ch = getch();
			if ( ch == '0' )
				return;
			else if ( ch == '1' )
				break;
		}
	}
	return;
}


/*
 * ////////////////////////////////
 *   函数drop(char*):学生退课   //
 * ////////////////////////////////
 */
void drop( char *id )
{
	char	name[20], cname[20];
	char	ch, s[10];
	checkname( id, name );
	int courseid;
	while ( 1 )
	{
		system( "cls" );
		printf( "//=================================================//\n" );
		printf( "// %8s同学你好，请选择对应的课程或者输入0退出 //\n", name );
		printf( "//=================================================//\n" );
		printf( "//      --------------------------------------     //\n" );
		printf( "//     | 课程编号 |     课程名     | 选课状态 |    //\n" );
		printf( "//      --------------------------------------     //\n" );
		int		tot = countcourse();
		ifstream	fin( "courses.txt", ios_base::in );
		string		tmp;
		for ( int i = 1; i <= tot; i++ )
		{
			getline( fin, tmp );
			int	p1	= tmp.find( ' ' );
			int	p2	= tmp.find( ' ', p1 + 1 );
			strcpy( cname, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
			int isSel = checkSelCourse( id, i );
			printf( "//     | %-8d | %-14s | %-8c |    //\n", i, cname, isSel ? 'Y' : 'N' );
			printf( "//      --------------------------------------     //\n" );
		}
		fin.close();
		fin.clear();
		printf( "//=================================================//\n" );
		printf( "//                                                 //\n" );    /* line:8+2*tot; */
		printf( "//                                                 //\n" );    /* line:9+2*tot */
		printf( "//                                                 //\n" );    /* line:10+2*tot */
		printf( "//                                                 //\n" );    /* line:11+2*tot */
		printf( "//=================================================//\n" );
		while ( 1 )
		{
			gotoxy( 14, 8 + 2 * tot );
			printf( "请输入待选择的课程号：" );
			HideCursor( 1 );
			scanf( "%s", s );
			courseid = atoi( s );
			if ( courseid == 0 )
				return;
			else if ( courseid >= 1 && courseid <= tot )
			{
				if ( !checkSelCourse( id, courseid ) )
				{
					gotoxy( 12, 9 + 2 * tot ); printf( "                            " );
					gotoxy( 19, 9 + 2 * tot );
					printf( "请选择已选的课！" );
					HideCursor( 0 );
					Sleep( 1000 );
					gotoxy( 15, 9 + 2 * tot );
					printf( "                    " );
					gotoxy( 20, 8 + 2 * tot );
					printf( "                       " );
					continue;
				}else break;
			}else  {
				gotoxy( 12, 9 + 2 * tot );
				printf( "错误的课程号，请重新输入。" );
				gotoxy( 20, 8 + 2 * tot );
				printf( "                       " );
				continue;
			}
		}
		gotoxy( 8, 9 + 2 * tot );
		printf( "您确定要退选课程 %d 吗：1(确定)  0(取消)", courseid );
		HideCursor( 0 );
		while ( 1 )
		{
			ch = getch();
			if ( ch == '0' )
			{
				gotoxy( 21, 10 + 2 * tot );
				printf( "退课失败！" );
				Sleep( 1500 );
				return;
			}else if ( ch == '1' )
				break;
		}
		HideCursor( 1 );
		reviseSelCourse( id, courseid, -1 );
		gotoxy( 14, 10 + 2 * tot );
		printf( "退课成功，是否继续退课？" );
		HideCursor( 0 );
		gotoxy( 16, 11 + 2 * tot );
		printf( "1(继续退课) 0(退出)" );
		while ( 1 )
		{
			ch = getch();
			if ( ch == '0' )
				return;
			else if ( ch == '1' )
				break;
		}
	}
}


/*
 * //////////////////////////////////////////////////
 *   函数viewallsel(char*):学生查看当前选课情况   //
 * //////////////////////////////////////////////////
 */
void viewallsel( char *id )
{
/* 1---->收集我们需要的信息 */
	system( "cls" );
	int	totalLenRow;                    /* totalLenRow是表的一行长度 */
	int	info[20][20]	= { 0 };        /* info[i][j]表示_i_班对_j_班的选择号 */
	int	courseLen[20]	= { 0 };
	int	totalSel[20]	= { 0 };
	int	cntclass	= countclass();
	int	cntcourse	= countcourse();
	char	name[20]	= { 0 };
	char	blank[200]	= { 0 }, blankt[200] = { 0 };
	char	border[200]	= { 0 };
	checkname( id, name );
	ifstream	finc( "courses.txt", ios_base::in );
	ifstream	fins( "students.txt", ios_base::in );
	string		tmp;
	int		p1, p2, cnt = 0;
	int		clas, aSel, borderL;
	while ( getline( finc, tmp ) )
	{
		cnt++;
		p1		= tmp.find( ' ' );
		p2		= tmp.find( ' ', p1 + 1 );
		courseLen[cnt]	= strlen( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		p1		= tmp.find( ' ', p2 + 1 );
		p2		= tmp.find( ' ', p1 + 1 );
		totalSel[cnt]	= atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
	}
	finc.close();
	finc.clear();
	while ( getline( fins, tmp ) )
	{
		p1	= tmp.find( ' ' );
		p2	= tmp.find( ' ', p1 + 1 );
		p1	= tmp.find( ' ', p2 + 1 );
		p2	= tmp.find( ' ', p1 + 1 );
		clas	= atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		p1	= tmp.find( ' ', p2 + 1 );
		aSel	= atoi( tmp.substr( p2 + 1, p1 - p2 - 1 ).c_str() );
		for ( int i = 1; i <= aSel; i++ )
		{
			p2 = tmp.find( ' ', p1 + 1 );
			int cSel = atoi( tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
			info[clas][cSel]++;
			p1 = p2;
		}
	}
	fins.close();
	fins.clear();
	totalLenRow = 8 + cntcourse; /* 两个课程之间有一个“|” */
	for ( int i = 1; i <= cntcourse; i++ )
		totalLenRow += 2 + courseLen[i];
	for ( int i = 0; i < totalLenRow; i++ )
		strcat( border, "=" );
	borderL = strlen( border );

/* 2---->显示_id_的信息 */
	finc.open( "courses.txt", ios_base::in );
	printf( "//==========%s==========//\n", border );
	for ( int i = 0; i < borderL / 2 - 5; i++ )
		blank[i] = ' ';
	blank[borderL / 2 - 5] = 0;
	for ( int i = 0; i < borderL - borderL / 2 - 5; i++ )
		blankt[i] = ' ';
	blankt[borderL - borderL / 2 - 5] = 0;
	printf( "//%s%8s同学你好，你的选课如下%s//\n", blank, name, blankt );
	printf( "//==========%s==========//\n", border );
	printf( "//          --------" );
	for ( int i = 1; i <= cntcourse; i++ )
	{
		for ( int j = 0; j < courseLen[i]; j++ )
			blank[j] = '-';
		blank[courseLen[i]] = 0;
		printf( "--%s-", blank );
	}
	printf( "          //\n" );
	printf( "//         |  姓名  |" );
	for ( int i = 1; i <= cntcourse; i++ )
	{
		getline( finc, tmp );
		p1	= tmp.find( ' ' );
		p2	= tmp.find( ' ', p1 + 1 );
		char coursename[50];
		strcpy( coursename, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		printf( " %s |", coursename );
	}
	finc.close();
	finc.clear();
	printf( "         //\n" );
	printf( "//          --------" );
	for ( int i = 1; i <= cntcourse; i++ )
	{
		for ( int j = 0; j < courseLen[i]; j++ )
			blank[j] = '-';
		blank[courseLen[i]] = 0;
		printf( "--%s-", blank );
	}
	printf( "          //\n" );
	printf( "//         |%-8s|", name );
	for ( int i = 1; i <= cntcourse; i++ )
	{
		int isSel = checkSelCourse( id, i );
		for ( int j = 0; j < courseLen[i] / 2; j++ )
			blank[j] = ' ';
		blank[courseLen[i] / 2] = 0;
		printf( " %s%c", blank, isSel ? 'Y' : 'N' );
		for ( int j = 0; j < courseLen[i] - courseLen[i] / 2; j++ )
			blank[j] = ' ';
		blank[courseLen[i] - courseLen[i] / 2] = 0;
		printf( "%s|", blank );
	}
	printf( "         //\n" );
	printf( "//          --------" );
	for ( int i = 1; i <= cntcourse; i++ )
	{
		for ( int j = 0; j < courseLen[i]; j++ )
			blank[j] = '-';
		blank[courseLen[i]] = 0;
		printf( "--%s-", blank );
	}
	printf( "          //\n" );

/* 3---->显示所有的选择信息 */
	printf( "//==========%s==========//\n", border );
	for ( int i = 0; i < borderL / 2 + 2; i++ )
		blank[i] = ' ';
	blank[borderL / 2 + 2] = 0;
	for ( int i = 0; i < 2 + borderL - borderL / 2; i++ )
		blankt[i] = ' ';
	blankt[2 + borderL - borderL / 2] = 0;
	printf( "//%s当前选课总览如下%s//\n", blank, blankt );
	printf( "//==========%s==========//\n", border );

	printf( "//          --------" );
	for ( int i = 1; i <= cntcourse; i++ )
	{
		for ( int j = 0; j < courseLen[i]; j++ )
			blank[j] = '-';
		blank[courseLen[i]] = 0;
		printf( "--%s-", blank );
	}
	printf( "          //\n" );
	finc.open( "courses.txt", ios_base::in );
	printf( "//         |  班级  |" );
	for ( int i = 1; i <= cntcourse; i++ )
	{
		getline( finc, tmp );
		p1	= tmp.find( ' ' );
		p2	= tmp.find( ' ', p1 + 1 );
		char coursename[50];
		strcpy( coursename, tmp.substr( p1 + 1, p2 - p1 - 1 ).c_str() );
		printf( " %s |", coursename );
	}
	printf( "         //\n" );
	finc.close();
	finc.clear();
	printf( "//          --------" );
	for ( int i = 1; i <= cntcourse; i++ )
	{
		for ( int j = 0; j < courseLen[i]; j++ )
			blank[j] = '-';
		blank[courseLen[i]] = 0;
		printf( "--%s-", blank );
	}
	printf( "          //\n" );

	for ( int i = 1; i <= cntclass; i++ )
	{
		printf( "//         |  %2d班  |", i );
		for ( int j = 1; j <= cntcourse; j++ )
		{
			for ( int k = 0; k < courseLen[j] / 2; k++ )
				blank[k] = ' ';
			blank[courseLen[j] / 2] = 0;
			printf( "%s%2d", blank, info[i][j] );
			for ( int k = 0; k < courseLen[j] - courseLen[j] / 2; k++ )
				blank[k] = ' ';
			blank[courseLen[j] - courseLen[j] / 2] = 0;
			printf( "%s|", blank );
		}
		printf( "         //\n" );
		printf( "//          --------" );
		for ( int j = 1; j <= cntcourse; j++ )
		{
			for ( int k = 0; k < courseLen[j]; k++ )
				blank[k] = '-';
			blank[courseLen[j]] = 0;
			printf( "--%s-", blank );
		}
		printf( "          //\n" );
	}

	printf( "//         | 总人数 |" );
	for ( int i = 1; i <= cntcourse; i++ )
	{
		for ( int j = 0; j < courseLen[i] / 2; j++ )
			blank[j] = ' ';
		blank[courseLen[i] / 2] = 0;
		printf( "%s%2d", blank, totalSel[i] );
		for ( int j = 0; j < courseLen[i] - courseLen[i] / 2; j++ )
			blank[j] = ' ';
		blank[courseLen[i] - courseLen[i] / 2] = 0;
		printf( "%s|", blank );
	}
	printf( "         //\n" );
	printf( "//          --------" );
	for ( int i = 1; i <= cntcourse; i++ )
	{
		for ( int j = 0; j < courseLen[i]; j++ )
			blank[j] = '-';
		blank[courseLen[i]] = 0;
		printf( "--%s-", blank );
	}
	printf( "          //\n" );

/* 4---->按任意键退出 */
	printf( "//==========%s==========//\n", border );
	for ( int i = 0; i < borderL / 2 + 4; i++ )
		blank[i] = ' ';
	blank[borderL / 2 + 4] = 0;
	for ( int i = 0; i < 4 + borderL - borderL / 2; i++ )
		blankt[i] = ' ';
	blankt[4 + borderL - borderL / 2] = 0;
	printf( "//%s按任意键退出%s//\n", blank, blankt );
	printf( "//==========%s==========//\n", border );
	HideCursor( 0 );
	char ch = getch();
}


/*
 * ////////////////////////////////
 *   函数student():学生主界面   //
 * ////////////////////////////////
 */
void student( char *id )
{
	while ( 1 )
	{
		system( "cls" );
		char ch;
		HideCursor( 0 );
		char name[50];
		checkname( id, name );
		printf( "//=======================================//\n" );
		printf( "//       %8s同学您好，请选择！      //\n", name );
		printf( "//=======================================//\n" );
		printf( "//               1.选课                  //\n" );
		printf( "//               2.退课                  //\n" );
		printf( "//               3.查看                  //\n" );
		printf( "//               4.返回                  //\n" );
		printf( "//=======================================//\n" );
		ch = getch();
		if ( ch == '1' )
			select( id );
		else if ( ch == '2' )
			drop( id );
		else if ( ch == '3' )
			viewallsel( id );
		else if ( ch == '4' )
			break;
		else continue;
	}
	return;
}


/*
 * ////////////////////////////////////////////
 *   函数login(char):为老师或学生登陆账号   //
 * ////////////////////////////////////////////
 */
void login( char type )
{
	system( "cls" );
	char id[20], pw[30];
	printf( "//=======================================//\n" );
	printf( "//    用户您好！请输入您的账号和密码     //\n" );
	printf( "//=======================================//\n" );
	printf( "//        输入您的账号：                 //\n" );
	printf( "//        输入您的密码：                 //\n" );
	printf( "//=======================================//\n" );
	printf( "//                                       //\n" );
	printf( "//                                       //\n" );
	printf( "//=======================================//\n" );
	while ( 1 )
	{
		HideCursor( 1 );
		gotoxy( 25, 4 ); scanf( "%s", id );
		gotoxy( 25, 5 ); scanf( "%s", pw );
		if ( checkDeanOrstu( id, pw, type ) )
			break;
		else{
			gotoxy( 4, 7 );
			printf( "账号不存在或账号密码不匹配，是否退出？" );
			gotoxy( 10, 8 );
			printf( "1(选择退出)  0(继续输入)" );
			HideCursor( 0 );
			char ch = getch();
			if ( ch == '1' )
				return;
			else{
				gotoxy( 10, 8 ); printf( "                         " );
				gotoxy( 3, 7 ); printf( "                                       " );
				gotoxy( 25, 5 ); printf( "               " );
				gotoxy( 25, 4 ); printf( "               " );
			}
		}
	}
	gotoxy( 10, 8 );
	printf( "登陆成功！即将进入页面！" );
	HideCursor( 0 );
	Sleep( 1500 );
	if ( type == '1' )
		dean();
	else student( id );
	return;
}


/*
 * /////////////////////////////
 *   函数welcome():主界面    //
 * /////////////////////////////
 */
void welcome( void )
{
	char ch;
	while ( 1 )
	{
		system( "cls" );
		HideCursor( 0 );
		printf( "//=======================================//\n" );
		printf( "//     欢迎使用西南某985高校选课系统     //\n" );
		printf( "//                请选择                 //\n" );
		printf( "//=======================================//\n" );
		printf( "//                1.注册                 //\n" );
		printf( "//                2.教务                 //\n" );
		printf( "//                3.学生                 //\n" );
		printf( "//                4.退出                 //\n" );
		printf( "//=======================================//\n" );
		printf( "//             系统使用说明              //\n" );
		printf( "// 1.本系统提供注册,请进入后按照提示操作 //\n" );
		printf( "// 2.请老师同学进入对应页面后再登陆      //\n" );
		printf( "// 3.目前仅支持键盘操作                  //\n" );
		printf( "//   created by 张昊钰   version:1.00    //\n" );
		printf( "//=======================================//\n" );
		ch = getch();
		if ( ch == '1' )
			regis();
		else if ( ch == '2' )
			login( '1' );
		else if ( ch == '3' )
			login( '0' );
		else if ( ch == '4' )
			break;
	}
	return;
}


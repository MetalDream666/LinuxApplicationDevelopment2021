#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define N_SPACES 	4 /* tab */
#define DX 			3

/*ABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZABCDEFGHIJKLMNOPQRSTUVWXYZ*/

static void
read_file(FILE *fd, WINDOW *win)
{
	char 			c = 0;
	int 			len = COLS - (2 * DX) + 1,
	/*int 			len = 10,*/
					read_bytes;
	char 		   *str = (char*)calloc(len, sizeof(char));

	while((c = wgetch(win)) != 27)
	{
		if ((c == 32) && (!feof(fd))) /* read and print the next string */
		{
			read_bytes = 0;
			while(1)
			{
				fgets(str + read_bytes, len - read_bytes, fd);
				if (ferror(fd))
				{
					printf("Error reading file\n");
					exit(1);
				}
				if (str[strlen(str)-1] != '\n')
				{
					read_bytes = len - 1;
					len *= 2;
					str = realloc(str, len);
				}
				else
				{
					break;
				}
			}
			wprintw(win, " %s", str);
			memset(str, 0, len);
		}
		/*box(win, 0, 0);*/
		wrefresh(win);
	}
	free(str);
}

int main(int argc, char **argv)
{
	FILE		   *fd;
	WINDOW		   *win;

	if (argc < 2)
	{
		printf("Filename not given\n");
		exit(1);
	}

	fd = fopen(argv[1], "r");
	if (!fd)
	{
		printf("%s: can't open file, or file does not exist\n", argv[1]);
		exit(1);
	}

	initscr();
	noecho();
	cbreak();
	printw("Filename: %s\n", argv[1]);
	refresh();

	win = newwin(LINES - 2*DX, COLS - 2*DX, DX, DX);
	keypad(win, TRUE);
	scrollok(win, TRUE);
	/*box(win, 0, 0); */
	wmove(win, 1, 0);
	
	read_file(fd, win);

	fclose(fd);
	endwin();
	return 0;
}

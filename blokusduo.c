#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <ctype.h>

#define GAME_BOARD_DIM 14
#define GAME_PIECE_DIM 5
#define GAME_PIECE_MAX 21
#define GAME_PIECE_ROT 8

#define GAME_LIST_REG 256


#define TeamTag "1PL"

#define DEBUG
typedef uint8_t cell_t;
typedef struct Coord {char Cord[2];} Coord;



cell_t basepieces[GAME_PIECE_MAX][GAME_PIECE_DIM][GAME_PIECE_DIM]=
	{
		{
			{2,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{2,0,0,0,0},
			{1,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{1,0,0,0,0},
			{2,0,0,0,0},
			{1,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{1,0,0,0,0},
			{2,1,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{1,0,0,0,0},
			{2,0,0,0,0},
			{1,0,0,0,0},
			{1,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,1,0,0,0},
			{0,2,0,0,0},
			{1,1,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{1,0,0,0,0},
			{2,1,0,0,0},
			{1,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{2,1,0,0,0},
			{1,1,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{1,2,0,0,0},
			{0,1,1,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{1,0,0,0,0},
			{1,0,0,0,0},
			{2,0,0,0,0},
			{1,0,0,0,0},
			{1,0,0,0,0}
		},
		{
			{0,1,0,0,0},
			{0,1,0,0,0},
			{0,2,0,0,0},
			{1,1,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,1,0,0,0},
			{0,1,0,0,0},
			{1,2,0,0,0},
			{1,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,1,0,0,0},
			{1,2,0,0,0},
			{1,1,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{1,1,0,0,0},
			{0,2,0,0,0},
			{1,1,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{1,0,0,0,0},
			{2,1,0,0,0},
			{1,0,0,0,0},
			{1,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{0,1,0,0,0},
			{0,2,0,0,0},
			{1,1,1,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{1,0,0,0,0},
			{1,0,0,0,0},
			{2,1,1,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{1,1,0,0,0},
			{0,2,1,0,0},
			{0,0,1,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},
		{
			{1,0,0,0,0},
			{1,2,1,0,0},
			{0,0,1,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},{
			{1,0,0,0,0},
			{1,2,1,0,0},
			{0,1,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		},{
			{0,1,0,0,0},
			{1,2,1,0,0},
			{0,1,0,0,0},
			{0,0,0,0,0},
			{0,0,0,0,0}
		}
	};

/**DataTatic**/

/** vectores int8*/
typedef struct Vec{
	int8_t l;
	int8_t c;
}Vec;

Vec VecNew(int l, int c){
	Vec vec;
	vec.l=(int8_t)l;
	vec.c=(int8_t)c;
	return vec;
}
Vec VecAdd(Vec a, Vec b){
	Vec vec;
	vec.l=a.l+b.l;
	vec.c=a.c+b.c;
	return vec;
}
Vec VecSub(Vec a, Vec b){
	Vec vec;
	vec.l=a.l-b.l;
	vec.c=a.c-b.c;
	return vec;
}
int Vecl (Vec vec){
	return vec.l;
}
int Vecc (Vec vec){
	return vec.c;
}
/****/
typedef struct PlayZoneReg{
	uint8_t l;
	uint8_t c;
	uint8_t Q;//quadrant free to player 1,2,3,4
	struct PlayZoneReg *a;
	struct PlayZoneReg *b;
}PlayZoneReg;
/**/
typedef struct Piece{
	cell_t form[GAME_PIECE_DIM][GAME_PIECE_DIM];
	int dl,dc;
	int cl,cc;
	int point;
}Piece;

typedef struct Player{
	Piece* hand[GAME_PIECE_MAX][GAME_PIECE_ROT];
	int point;
	PlayZoneReg Reg_hidden[GAME_LIST_REG];
	PlayZoneReg *RegPtr;
	cell_t mask;
	
}Player;
typedef struct Move{
	char txt[5];
	Piece *piece;
	int pl,pc;
	cell_t player_mask;
}Move;


typedef struct Game {
	cell_t board[GAME_BOARD_DIM][GAME_BOARD_DIM];
	Piece piece[GAME_PIECE_MAX][GAME_PIECE_ROT];
	Move move;
	Player me;
	Player opponent;
}Game;

void rot_clockwise(Piece *new, Piece *old){
int l,c;
for (l=0;l<GAME_PIECE_DIM;l++)
	for(c=0;c<GAME_PIECE_DIM;c++){
		if (l<(*old).dl &&c<(*old).dc)
			(*new).form[c][(*old).dl-1-l]=(*old).form[l][c];
		else
			(*new).form[c][l]=0;
		if (l==(*old).cl && c==(*old).cc){
			(*new).cl=c;
			(*new).cc=(*old).dl-1-l;
		}
	}
(*new).dl=(*old).dc;
(*new).dc=(*old).dl;
(*new).point=(*old).point;
}
void rot_mirror(Piece *new, Piece *old){
int l,c;
for (l=0;l<GAME_PIECE_DIM;l++)
	for(c=0;c<GAME_PIECE_DIM;c++){
		if (l<(*old).dl &&c<(*old).dc)
			(*new).form[l][(*old).dc-c-1]=(*old).form[l][c];
		else
			(*new).form[l][c]=0;
		if (l==(*old).cl && c==(*old).cc){
			(*new).cl=l;
			(*new).cc=(*old).dc-1-c;
		}
	}
(*new).dl=(*old).dl;
(*new).dc=(*old).dc;
(*new).point=(*old).point;
}

int pos2i(char c){
	switch (c){
		case '1':return 0;
		case '2':return 1;
		case '3':return 2;
		case '4':return 3;
		case '5':return 4;
		case '6':return 5;
		case '7':return 6;
		case '8':return 7;
		case '9':return 8;
		case 'a':return 9;
		case 'b':return 10;
		case 'c':return 11;
		case 'd':return 12;
		case 'e':return 13;
		case 'f':return 14;
	}
	return -1;
}
int i2pos(int i){
	char c;
	if (i<9){
		c='1';
	}else{
		c='a';
		i-=9;
	}
	c+=i;
	return c;
}
char rot2char(int i){
	return '0'+i;
}
int rot2int(char c){
	return (int)(c-'0');
}


char piece2char(int i){
	char c='a';
	c=c+i;
	return c;
}
int piece2int (char c){
	int i;
	i=(int)(c-'a');
	if (i<0)return -1;//no piece
	return i;
}


void PrintGamePieces(Game *game){
	int p,r,pl,pc;
	for (p=0;p<GAME_PIECE_MAX;p++){
		printf ("\nPIECE %d %c (+%d)\n",p,piece2char(p),(*game).piece[p][0].point);
		for (r=0;r<GAME_PIECE_ROT;r++){
			if ((*game).piece[p][r].point==0)continue;
			printf("[%d|%c] Rot %d (+%d){%dx%d <%d,%d>}\n",p,piece2char(p),r,(*game).piece[p][r].point,(*game).piece[p][r].dl,(*game).piece[p][r].dc,(*game).piece[p][r].cl,(*game).piece[p][r].cc);
			for (pl=0;pl<GAME_PIECE_DIM;pl++){
				if (pl==(*game).piece[p][r].dl) break;
				for(pc=0;pc<GAME_PIECE_DIM;pc++){
					if (pc==(*game).piece[p][r].dc ) break;
					if ((*game).piece[p][r].form[pl][pc]){
					if ((*game).piece[p][r].cl==pl && (*game).piece[p][r].cc==pc)
						printf("@");
					else
						printf("#");
					}
					else printf("░");
				}
				printf("\n");
			}
		}
	}
}

Game GameInit(){
	int l0,c0;
	Game newgame;
	for(l0=0;l0<GAME_BOARD_DIM;l0++)
		for (c0=0;c0<GAME_BOARD_DIM;c0++)
			newgame.board[l0][c0]=0;
		
	int p,pl,pc,ol,oc,c;
	for (p=0;p<GAME_PIECE_MAX;p++){
		ol=0;oc=0;c=0;
		for (pl=0;pl<GAME_PIECE_DIM;pl++)
			for(pc=0;pc<GAME_PIECE_DIM;pc++){
				if (basepieces[p][pl][pc]==0)
					newgame.piece[p][0].form[pl][pc]=0;
				else{
					newgame.piece[p][0].form[pl][pc]=255;
					c++;
					if (ol<pl) ol=pl;
					if (oc<pc) oc=pc;
				}
				if(basepieces[p][pl][pc]==2){
					newgame.piece[p][0].cl=pl;
					newgame.piece[p][0].cc=pc;
				}
			}
		newgame.piece[p][0].dc=oc+1;
		newgame.piece[p][0].dl=ol+1;
		newgame.piece[p][0].point=c;
	}
	int r;
	for (p=0;p<GAME_PIECE_MAX;p++)
		for (r=0;r<GAME_PIECE_ROT;r+=2){
			if (r!=0) rot_clockwise(&(newgame.piece[p][r]),&(newgame.piece[p][r-2]));
			rot_mirror(&(newgame.piece[p][r+1]),&(newgame.piece[p][r]));
		}
	 for (p=0;p<GAME_PIECE_MAX;p++)
		for (r=0;r<GAME_PIECE_ROT;r++){
			newgame.me.hand[p][r]=&(newgame.piece[p][r]);
			newgame.opponent.hand[p][r]=&(newgame.piece[p][r]);
		} 
		//remove repeated form  by turn their pointsa to zero
	int r0,v;
	for (p=0;p<GAME_PIECE_MAX;p++){
		for (r=0;r<GAME_PIECE_ROT;r++){
			for (r0=r+1;r0<GAME_PIECE_ROT;r0++){
				v=1;
				for (pl=0;pl<GAME_PIECE_DIM;pl++){
					for(pc=0;pc<GAME_PIECE_DIM;pc++){
						v= v&& (newgame.piece[p][r0].form[pl][pc]==newgame.piece[p][r].form[pl][pc]);
					}
				}
				v=v&&((newgame.piece[p][r0].cl==newgame.piece[p][r].cl)&&(newgame.piece[p][r0].cc==newgame.piece[p][r].cc));
				if (v)newgame.piece[p][r0].point=0;
			}
		}
	}
		
	newgame.me.point=0;
	newgame.me.mask=0x01;
	for (p=0;p<GAME_LIST_REG;p++){
		newgame.me.Reg_hidden[p].a=NULL;
		newgame.me.Reg_hidden[p].b=NULL;
		newgame.me.Reg_hidden[p].Q=0;
	}
	newgame.me.RegPtr=NULL;
	newgame.opponent.point=0;
	newgame.opponent.mask=0x02;
	for (p=0;p<GAME_LIST_REG;p++){
		newgame.opponent.Reg_hidden[p].a=NULL;
		newgame.opponent.Reg_hidden[p].b=NULL;
		newgame.opponent.Reg_hidden[p].Q=0;
	}
	newgame.opponent.RegPtr=NULL;
	return newgame;
}

/*moves*/
Move writePass(){
	Move move;
	move.piece=NULL;
	move.txt[0]='0';
	move.txt[1]='0';
	move.txt[2]='0';
	move.txt[3]='0';
	move.txt[4]='\0';
	return move;
}
Move writeMove(Game *game,int l,int c, int p, int r){
	Move move;
	move.txt[0]=i2pos(c);//i2pos(c+(*game).piece[p][r].cc);
	move.txt[1]=i2pos(l);//i2pos(l+(*game).piece[p][r].cl);
	move.txt[2]=piece2char(p);
	move.txt[3]=rot2char(r);
	move.txt[4]='\0';
	move.piece=&((*game).piece[p][r]);
	move.pl=l-(*game).piece[p][r].cl;
	move.pc=c-(*game).piece[p][r].cc;
	move.player_mask=(*game).me.mask;
	return move;
}
Move readMove(Game *game,char *str){
	Move move;
	if(str[0]=='0'&&str[1]=='0'&&str[2]=='0'&&str[3]=='0'){
		move.piece=NULL;
		return move;
	}
	move.txt[0]=str[0];
	move.txt[1]=str[1];
	move.txt[2]=str[2];
	move.txt[3]=str[3];
	move.piece=&((*game).piece[piece2int(str[2])][rot2int(str[3])]);
	move.pl=pos2i(str[1])-(*(move.piece)).cl;
	move.pc=pos2i(str[0])-(*(move.piece)).cc;
	move.player_mask=(*game).opponent.mask;
	return move;
}
void PrintMove(Move *move){
	//putchar('4');
	putchar((*move).txt[0]);
	putchar((*move).txt[1]);
	putchar((*move).txt[2]);
	putchar((*move).txt[3]);

	//printf("4%c%c%c%c",(*move).txt[0],(*move).txt[1],(*move).txt[2],(*move).txt[3]);
	fflush(stdout);
}

void RegisterMyMove(Game *game, Move *move){
	(*game).move=(*move);
}
int GetPieceIndex(Move *move){
	return piece2int((*move).txt[2]);
}
int GetPieceRotationIndex(Move *move){
	return piece2int((*move).txt[3]);
}
/** MapOperations**/
void PrintGameMap(Game  *game){
	int l,c;
	char a=0x40;
	printf("\n");
	for (l=-1;l<GAME_BOARD_DIM;l++){
		for(c=-1;c<GAME_BOARD_DIM;c++){
			if (c==-1 && l==-1)
					printf(" ");
			if(c==-1){
				if (l<9 && l!=-1)
					printf("%c",'1'+l);
				else if (l>=9)
					printf("%c",'a'+l-9);
				continue;
				}
			if(l==-1){
				if (c<9 && c!=-1)
					printf("%c",'1'+c);
				else if (c>=9)
					printf("%c",'a'+c-9);
				continue;
				}
			if ((*game).board[l][c]!=0){
				printf("%c",a+(*game).board[l][c]);
			}else printf(" "); 
		}
	printf("\n");
	
	}
}
void ForcePlace(Player player,Move move,Game *game){
	//drawn piece on board. describes the move on the board with player mask (id) without rules consideration
	int l,c;
	if(move.piece==NULL)return;//nothing to place
	for (l=0;l<(*move.piece).dl;l++)
		for(c=0;c<(*move.piece).dc;c++)
			if ((*move.piece).form[l][c])
				(*game).board[move.pl+l][move.pc+c]=move.player_mask;
	return;
	
}

void ApplyOponentMove(Game *game, char *mov){
	(*game).move=readMove(game,mov);//gets movements data;
	ForcePlace((*game).opponent,(*game).move,game);//performsthemove
}


/***RULES***/


/***TACTICS***/
// RegPtr->[b_a]<->[b_a]->NULL
void AddIntPoint (Player *p,int8_t l, int8_t c, int8_t Q){

	int index=0;
	index=(l<<4)+c;
	//printf("\t%p",(*p).Reg_hidden);
	//printf("[(%d %d %d) - %x]\n",l,c,Q,index);
	if((*p).RegPtr==NULL){
		(*p).RegPtr=&((*p).Reg_hidden[index]);
	}
	else if ((*p).Reg_hidden[index].Q!=0){
		
		if((*p).RegPtr!=&(*p).Reg_hidden[index]){
			//remove elemente from this postion, connect element before and after
			if((*p).Reg_hidden[index].b!=NULL)
				(*(*p).Reg_hidden[index].b).a=(*p).Reg_hidden[index].a;
			if((*p).Reg_hidden[index].a!=NULL)
				(*(*p).Reg_hidden[index].a).b=(*p).Reg_hidden[index].b;
			//put element at top
			(*p).Reg_hidden[index].b=NULL;
			(*p).Reg_hidden[index].a=(*p).RegPtr;
			(*(*p).RegPtr).b=&(*p).Reg_hidden[index];
			(*p).RegPtr=&(*p).Reg_hidden[index];
		}
	}
	else{
		(*p).Reg_hidden[index].a=(*p).RegPtr;
		(*(*p).RegPtr).b=&(*p).Reg_hidden[index];
		(*p).RegPtr=&(*p).Reg_hidden[index]	;
	}
	(*(*p).RegPtr).Q=Q;
	(*(*p).RegPtr).l=l;
	(*(*p).RegPtr).c=c;
	//printf(">>%p %i %i %i %p<<\n",(*(*p).RegPtr).b,(*(*p).RegPtr).Q,(*(*p).RegPtr).l,(*(*p).RegPtr).c,(*(*p).RegPtr).a);
}
int IsAnIntPoint(Player *player ,int l,int c){
	int index=0;
	index=(l<<4)+c;
	if ((*player).Reg_hidden[index].Q!=0)
		return 1;
	return 0;
	
}
void TakesIntPoint(Player *p,int8_t l, int8_t c){
	PlayZoneReg *ptr,*ptra,*ptrb;
	int index=0;
	index=(l<<4)+c;
	ptr=&(*p).Reg_hidden[index];
	if((*ptr).Q==0) return;
	ptrb=(*ptr).b;
	ptra=(*ptr).a;
	if ((*p).RegPtr==ptr){
		(*p).RegPtr=(*ptr).a;
	}
	if(ptrb!=NULL)
		(*ptrb).a=ptra;
	if (ptra!=NULL)
		(*ptra).b=ptrb;
	(*ptr).Q=0;
}
void RefreshIntPoint(Game *game ,int l,int c){
	//me
	if (IsAnIntPoint(&((*game).me),l,c))
		TakesIntPoint(&((*game).me),l,c);
	
	//Opponnent
	if (IsAnIntPoint(&((*game).opponent),l,c))
		TakesIntPoint(&((*game).opponent),l,c);
	
}
void printReg(Player *player){
	PlayZoneReg *ptr;
	ptr=(*player).RegPtr;
	while (ptr!=NULL){
		printf("<%c,%c,%i>",i2pos((*ptr).l),i2pos((*ptr).c),(*ptr).Q);
		ptr=(*ptr).a;
	}
	printf("\n");
}
void analysePlayerMove(Game *game,Player *player){
	int l,c;
	int l0,c0,l1,c1;
	Move *move=&((*game).move);
	if ((*move).piece==NULL)return;//its a pass, nothing to do
	l0=(*move).pl-1;
	c0=(*move).pc-1;
	if (l0<0) l0=0;
	if (c0<0) c0=0;
	l1=(*move).pl+( (*(*move).piece).dl )+1;
	c1=(*move).pc+( (*(*move).piece).dc )+1;
	if (l1>GAME_BOARD_DIM) l1=GAME_BOARD_DIM;
	if (c1>GAME_BOARD_DIM) c1=GAME_BOARD_DIM;
	for(l=l0;l<l1;l++)
		for(c=c0;c<c1;c++){
			if((*game).board[l][c]==(*move).player_mask){
				//remove invalids intpoints, old int points
				RefreshIntPoint(game , l,c);
				//AddNew IntPoints
				//testfirstquadrant <-1,1>
				if(l>0&&c<c1-1){
					if(	(*game).board[l-1][c+1]==0x00
						&& (*game).board[l][c+1]!=(*move).player_mask
						&& (*game).board[l-1][c]!=(*move).player_mask
					)
					AddIntPoint	(player,l-1,c+1,1);
				}
				//testsecondquadrant <-1,-1>
				if(l>0&&c>0){
					if(	(*game).board[l-1][c-1]==0x00
						&& (*game).board[l][c-1]!=(*move).player_mask
						&& (*game).board[l-1][c]!=(*move).player_mask
					)
					AddIntPoint	(player,l-1,c-1,2);
				}
				//testthirdtquadrant <1,-1>
				if(l<l1-1&&c>0){
					if(	(*game).board[l+1][c-1]==0x00
						&& (*game).board[l][c-1]!=(*move).player_mask
						&& (*game).board[l+1][c]!=(*move).player_mask
					)
					AddIntPoint	(player,l+1,c-1,3);
				}
				//testforthquadrant <1,1>
				if(l<l1-1&&c<c1-1){
					if(	(*game).board[l+1][c+1]==0x00
						&& (*game).board[l][c+1]!=(*move).player_mask
						&& (*game).board[l+1][c]!=(*move).player_mask
					)
					AddIntPoint	(player,l+1,c+1,4);
				}
			}
		}
	
}
//player funcs

void PlayerPieceRem(Player *player,int p){
	if (p<0)return;
	(*player).hand[p][0]=NULL;
}
int PlayerPieceExists(Player *player,int p){
	if ((*player).hand[p][0]==NULL)
		return 0;
	return 1;
}

void PrintPlayer(Player *p){
	char c='A'-1;
	int i;
	printf("\nPlayer %c\nPieces:",c+(*p).mask);
	for (i=0;i<GAME_PIECE_MAX;i++){
		if((*p).hand[i][0]==NULL)
				printf(" ");
		else
			printf("%c",piece2char(i));
	}
	printf("\nInteress: ");printReg(p);
}
//MePlaysThis

int TestMyMove(Game *game,Piece *piece,Vec drawnpos){
	int sl,sc;//startline startcolum on board testing
	int cl,cc;//current position on board
	int l,c;
	int mpoint=0;
	
	sl= Vecl(drawnpos)-(*piece).cl;
	sc= Vecc(drawnpos)-(*piece).cc;
	//test if piece is inside board
	if (sl<0) return 0;
	if (sc<0) return 0;
	if (sl+(*piece).dl>GAME_BOARD_DIM) return 0;
	if (sc+(*piece).dc>GAME_BOARD_DIM) return 0;
	//test piece in board
	for (l=0;l<(*piece).dl;l++)
		for (c=0;c<(*piece).dc;c++)
			if((*piece).form[l][c]!=0){//part of the piece
				cl=sl+l;
				cc=sc+c;
				if(IsAnIntPoint(&(*game).opponent,cl,cc)){mpoint+=10;}//if a move removes a playable zone from opponent its worthy more
				if ((*game).board[cl][cc]==0){//piece fits
					//check surround area up left down and rigth
					//up
					if(cl>0){
						if ((*game).board[cl-1][cc]==(*game).me.mask)return 0;
						if(IsAnIntPoint(&(*game).opponent,cl-1,cc)){mpoint+=5;}//if a move reduce a playable zone from opponent its worthy more
					}
					//left
					if(cc>0){
						if ((*game).board[cl][cc-1]==(*game).me.mask)return 0;
						if(IsAnIntPoint(&(*game).opponent,cl,cc-1)){mpoint+=5;}//if a move reduce a playable zone from opponent its worthy more
					}
					//bellow
					if(cl+1<GAME_BOARD_DIM){
						if ((*game).board[cl+1][cc]==(*game).me.mask)return 0;
						if(IsAnIntPoint(&(*game).opponent,cl+1,cc)){mpoint+=5;}//if a move reduce a playable zone from opponent its worthy more
					}
					//right
					if(cc+1<GAME_BOARD_DIM){
						if ((*game).board[cl][cc+1]==(*game).me.mask)return 0;
						if(IsAnIntPoint(&(*game).opponent,cl,cc+1)){mpoint+=5;}//if a move reduce a playable zone from opponent its worthy more
					}
				}else return 0;//piece not fit
				mpoint++;
			}
	return mpoint; //piece fits
}
int Play_ChoseMove(Game *game){
	//Algoritmo que pensa e escolhe a  melhor jogada
	PlayZoneReg *ptr;
	Vec  intpoint,drawnvec,cc,drawnpos;
	int l,c,diag,diagM;
	int p,r;
	Piece *piece;
	int cmp,rmp=0;//current move points,registed move points
	Move move;

	for (p=GAME_PIECE_MAX-1;p>=0;p--){
		if((*game).me.hand[p][0]==NULL) continue;//piece already played or invalid
			for(r=0;r<GAME_PIECE_ROT;r++){
				piece=(*game).me.hand[p][r];
				if ((*piece).point==0) continue;//rotation repeated
				ptr=(*game).me.RegPtr;
				while(ptr!=NULL){//iterate Interest Points
					//tries to fit piece into map, while occupying the interest point
					intpoint= VecNew((*ptr).l,(*ptr).c);
					cc= VecNew((*piece).cl,(*piece).cc);
					//chose a point of the piece to put it on the chosen point
					diagM=(*piece).dl<(*piece).dc?(*piece).dc:(*piece).dl;//selects the bigger limit to the diagMAx
					for(diag=0;diag<diagM;diag++){
						l=diag;
						for(c=diag;c<(*piece).dc;c++){
							if((*piece).form[l][c]!=0x00){
								drawnvec= VecNew(l,c);
								//calculate final vector. this vector point the positio so the piece fits like we pretend it
								drawnpos=VecAdd(VecSub(intpoint,drawnvec),cc);//posição para desenhar a peça
								//test move
								cmp=TestMyMove(game,piece,drawnpos);
								if(cmp>rmp){
									rmp=cmp;
									move=writeMove(game,Vecl(drawnpos),Vecc(drawnpos),p,r);
								}
							}
						}
						c=diag;
						for (l=diag+1;l<(*piece).dl;l++){
							if((*piece).form[l][c]!=0x00){
								drawnvec= VecNew(l,c);
								//calculate final vector. this vector point the positio so the piece fits like we pretend it
								drawnpos=VecAdd(VecSub(intpoint,drawnvec),cc);//posição para desenhar a peça
								//test move
								cmp=TestMyMove(game,piece,drawnpos);
								if(cmp>rmp){
									rmp=cmp;
									move=writeMove(game,Vecl(drawnpos),Vecc(drawnpos),p,r);
								}
							}
						}
					}



					//move to next interest point
					ptr=(*ptr).a;
				}

			}
	}
	if(rmp>0)RegisterMyMove(game,&move);//
	return rmp;
}
void MePlays(Game *game, int FirstPlay){
	Move move,pass;
	pass=writePass();//pass mov
	if (FirstPlay==1){
			if( (*((*game).me).RegPtr).l == 4 ){
				move=writeMove(game,	(*((*game).me).RegPtr).l,(*((*game).me).RegPtr).c,16,2);
			}else if ( (*((*game).me).RegPtr).l == 9 ){
				move=writeMove(game,	(*((*game).me).RegPtr).l,(*((*game).me).RegPtr).c,16,6);
			}else
				move=writeMove(game,	(*((*game).me).RegPtr).l,(*((*game).me).RegPtr).c,20,7);
			
			
			RegisterMyMove(game,&move);//resgister my move in this turn move
			
	}else{
			analysePlayerMove(game,&(*game).opponent);	
			PlayerPieceRem(&(*game).opponent,GetPieceIndex(&(*game).move));
			//my play
			RegisterMyMove(game,&pass);
			Play_ChoseMove(game);
	}
	PrintMove(&((*game).move));//send move to arbito
	ForcePlace((*game).me, (*game).move, game);
	PlayerPieceRem(&(*game).me,GetPieceIndex(&(*game).move));
	analysePlayerMove(game,&(*game).me);//analisa jogada
}
/**misc**/
void PrintGame(Game *game){
		PrintGameMap(game);
		PrintPlayer(&(*game).opponent);
		PrintPlayer(&(*game).me);
		printf(">>Your Turn: ");fflush(stdout);
}
/********/
#define IO_BUFFER_MAX_SIZE 12
char InBuffer[IO_BUFFER_MAX_SIZE];
char* GameRead(){
	int i,limit=0;
	read(1,InBuffer,1);//InBuffer[i]=getchar();
	switch(InBuffer[0]){
	case '0':break;
	case '1':limit=2;break;
	case '2':limit=1;break;
	case '3':limit=5;break;
	case '4':limit=4;break;
	case '9':break;
	default:InBuffer[0]=9; break;
	}
	read(1,InBuffer+1,limit);
	InBuffer[limit+1]='\0';
	for (i=1;i<=limit;i++){
	//c=tolower(InBuffer[i]);
	InBuffer[i]= (InBuffer[i]>='A'&&InBuffer[i]<='Z')? InBuffer[i]+32 :InBuffer[i];

	}
	return InBuffer;
}
void mySendOut(char *txt){

	int i=0;
	while(txt[i]!='\0'){
		i++;//putchar(txt[i++]);
		//if (i==1024) badexit(-1,"SendOut txt overflow, bigger than 1024\n");
	}
	write(1,txt,i);
}


#define ArbitoMSG_0 str[0]=='0'
#define ArbitoMSG_EndGame str[0]=='9'
#define ArbitoMSG_FirstMove str[0]=='2'
#define ArbitoMSG_SecondMove str[0]=='3'
#define ArbitoMSG_move str[0]=='4'

#define IPlayer_pass mySendOut("0000");

int main(int argc, char **argv){
	Game game,ngame;
	char *str;
	unsigned jogadas=0;
	ngame=GameInit();
	while(1){
		str=GameRead();
		if (ArbitoMSG_0 || ArbitoMSG_EndGame) {game=ngame;mySendOut(TeamTag);jogadas=0; continue;}
		//if (ArbitoMSG_EndGame)return 9;
		jogadas++;
		if (ArbitoMSG_FirstMove){
			AddIntPoint(&game.me,pos2i(str[1]),pos2i(str[1]),5);
			MePlays(&game,1);
		}else if (ArbitoMSG_SecondMove) {
			AddIntPoint(&game.me,pos2i(str[1]),pos2i(str[1]),5);
			ApplyOponentMove(&game,str+2);
			MePlays(&game,1);
		}else if(ArbitoMSG_move){
			ApplyOponentMove(&game,str+1);//IPlayer_pass
			MePlays(&game,jogadas);
		}
#ifdef DEBUG
			else continue;//avaoid enter key reading
			PrintGame(&game);
#endif
		game.move.piece=NULL;
		fflush(stdout);
	}
	return 0;
}
/*
■
■

*/

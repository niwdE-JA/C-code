#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

    int movelist[3][3];

	int you[3][3] = {{-1,-1,0}, {0,-1,0}, {0,0,0}};
	int rowtrack[3]= {0,0,0};
	int coltrack[3]= {0,0,0};
	int diatrack[3]= {-2,0,0};

	int thwart_list[3][3];

void play(int a[3][3] ,int b[3],int c[3],int d[3],int row, int col, int val){
		if(a[row][col] != 0){
			printf("ERROR!...Selected field is already filled");
			return;
		}
		a[row][col] = val;
		b[row]+=val;
		c[col]+=val;
		if(row==col){d[0]+=val;}
		if((row+col)==2){d[1]+=val;}
}





int check(int a[3], int b[3], int c[3]){
	for (int i=0;i<3;i++){
		for (int j=0;j<3;j++){
			for (int k=0;k<3;k++){
				if( (a[i]==-2 && b[j]==-2)||(a[i]==-2 &&c[k]==-2)|| (b[j]==-2 && c[k]==-2) ){
					return 1;
				}
			}
		}
	}
	return 0;
}

void copy2d(int source[3][3],int destination[3][3]){
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
		destination[i][j] = source[i][j];
		}
	}
}

void copy1d(int source[3],int destination[3]){
	for(int i=0;i<3;i++){
		destination[i] = source[i];
	}
}

void thwartAttack(){
	int rowsum[3] = {0,0,0};
	int colsum[3] = {0,0,0};
	int diasum[3] = {0,0,0};
	//first, calculate all relevant sums...
	for(int i = 0; i<3; i++){
		for(int j = 0; j<3; j++){
			rowsum[i]+= thwart_list[i][j];
			colsum[i]+= thwart_list[j][i];
			if(i!= 2){
				int k = (j +i*(j+2) )%3; //k collapses to j for i=0
				diasum[i] += thwart_list[j][k];
			}
		}
	}
	//then, check if move forces opponent to pure square...

	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(thwart_list[i][j] == 0){
				if(rowsum[i]==1 || colsum[j]==1 || (i==j && diasum[0]==1) || ( (i+j)==2 && diasum[1]==1)){
					movelist[i][j] = 1;
				}else{movelist[i][j] = 0;}
			}else if(thwart_list[i][j] ==10){
				if(rowsum[i]==11 || colsum[j]==11 || (i==j && diasum[0]==11) || ( (i+j)==2 && diasum[1]==11)){
					movelist[i][j] = 1;
				}else{movelist[i][j] = 0;}
			}else { movelist[i][j] = 0;}
		}
	}
}
 int random(int matrix[3][3]){
	 srand((unsigned)time(NULL));
	 int movecount = 0;
	 for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(matrix[i][j] ==1){movecount++;}
		}
	 }
	 int index = (rand() % movecount);
	 for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(matrix[i][j]==1){
				if(index == 0 ){
					//a little dumb trick here,lol...
					int ret = 10*i + j;//instead of using array, I encode each coordinate in a 2-digit number
					return ret;
				}
			}
			index--;
		}
	 }
	 
 }

int main(){

	for(int i=0;i<3; i++){
		if (rowtrack[i] == -2) {
			for(int j=0;j<3; j++){ you[i][j] = (you[i][j]==0)? rowtrack[i]++, 1 :you[i][j] ;//fill only empty entry, for row fixed at [i]
			} 
			break;
		}
		if (coltrack[i] == -2) {
			for(int j=0;j<3; j++){ you[j][i]= (you[j][i]==0)? coltrack[i]++, 1: you[j][i];//column fixed at [i]
			} 	
			break;
		}
		if (diatrack[i] == -2 && i!=2) {//this block would never run if i=2 because diatrack[2] will always be zero
			for(int j=0;j<3; j++){ 
				int k = (j +i*(j+2) )%3; //k collapses to j for i=0
				you[j][k] = (you[j][k]==0)? diatrack[i]++, 1: you[j][k]; 
			}
			break;
		}
	}
	//continue;
	
	
	for(int i=0;i<3;i++){printf("[ %d, %d, %d ]\n",you[i][0],you[i][1],you[i][2]);}
	printf("[%d, %d]",diatrack[0],diatrack[1]);  
	
	//to check for 2 ways, first create copies of all game data...
	int ghostgame[3][3];
	int ghostrow[3];
	int ghostcol[3];
	int ghostdia[3];
	
	
	copy2d(you, thwart_list);
	
	copy2d(you, ghostgame);/*templates*/
	copy1d(rowtrack, ghostrow);
	copy1d(coltrack, ghostcol);
	copy1d(diatrack, ghostdia);
	//...then modify the data by fixing opponent's mark "-1" for every free entry(i.e entries with zero.)
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(ghostgame[i][j]==0){
				play( ghostgame, ghostrow, ghostcol, ghostdia, i, j, -1);
				if ( check(ghostrow, ghostcol, ghostdia) ){ 
					thwart_list[i][j] = 10;
				}
				
				//{ ...This block changes the templates back 
				copy2d(you, ghostgame);
				copy1d(rowtrack, ghostrow);
				copy1d(coltrack, ghostcol);
				copy1d(diatrack, ghostdia);
				//}
			}
		}
	}
	
	thwartAttack();
	//now, to pick which move to make...
	int coordinates = random(movelist);
	//now, decode thecoordinates to x and y values
	int y = (coordinates% 10);
	int x = (coordinates-y)/10;
	play(you, rowtrack, coltrack, diatrack, x, y, 1);
	
	//finally, move-based attack, and what to do with move list before game body
	return 0;
}
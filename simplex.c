/*,,,,,,,,,,identifing variables,,,,,,,,,,,,,,,,*/

/*  NC   : Number of Constraints */
/*  NV   : Number of variables in main function */
/* table : simplex table of size NC+1 x NV+1 */
/*  rap  : stores smallest ratio >0 */
/* XMAX  : stores greatest coeffitient of main function */
/*  v    : auxilary variable */ 
/* p1,p2 : line , column index of Pivot */
/* XERR  : Boolean, if true No solution */
/* r1    : = 1 for maximize, = -1 for minimize */
/* r2    : auxilary variable for inputs */
/* NOPTIMAL : Boolean, if false continue iterations */

#include<stdio.h>
#include<math.h>


#define row 10    /* maximum number of constraints */
#define col 10    /* maximum number of variables in the main function */

void Data();          
void Pivot();
void Formula();
void Optimize();
void Simplex();
void Results();

int NC, NV, NOPTIMAL,p1,p2,XERR;   /*initializing variables */
double table[row][col];    /* simplex table */

void main(){
	printf("/*,,,,,,,,,,,,,,,,,,,,,,,,,,,,Linear Programming,,,,,,,,,,,,,,,,,,,,,,,,*/");
	Data();
	Simplex();
	Results();

}
void Data()
{

	double r1,r2;
	char R;
	int i,j;
	
	printf("\n MAXIMIZE (Y/N)? ");
	scanf("%c", &R);
	printf("\n Number of variables of main function? "); /* n */
	scanf("%d" , &NV);
	printf("\n Number of constraints? "); /* m */
	scanf("%d" ,&NC);

	if(R=='Y' || R=='y'){
		r1 = 1.0;
	}
	else{
		r1 = -1.0;
	}

	printf("\n input coefficients of main function: \n");
	for(j=1;j<=NV ; j++){
		printf(" #%d ? ", j);
		scanf("%lf", &r2);
		table[1][j+1] = r2*r1;    //fill the table with coeffitients in main function
	}
	printf(" right hand side ? ");
	scanf("%lf",&r2);
	table[1][1] = r2*r1;		//add the right hand ide value of main function in to the 						//table

	for(i=1;i<=NC;i++){
		printf("\n constraints %d:\n",i);
		for(j=1;j<=NV;j++){
			printf(" #%d ? ",j);
			scanf("%lf", &r2);
			table[i+1][j+1] = -r2;   //fill the table from coefficients of constraints 							//function
		}
		printf(" right hand side ? ");
		scanf("%lf",&table[i+1][1]);     //add the right hand ide value of main function in 							//to the table
	}

	printf("\n\n Results:\n\n");
	
	for(j=1; j<=NV ; j++){
		table[0][j+1] =j;
	}
	for(i=NV+1; i<= NV+NC; i++){
		table[i-NV+1][0] = i;	
	}
	
// print the table
	for(i=0; i<=NC+1; i++){
		for(j=0; j<=NV+1; j++){
			printf("%lf ", table[i][j]);
		}
		printf("\n");
	}
}

void Simplex()
{
	e10: Pivot();  //label
	Formula();
	Optimize();
	if(NOPTIMAL == 1){
		goto e10;
	}
}

void Pivot()
{
	double rap,v,XMAX;
	int i,j;

	XMAX = 0.0;
	for(j=2; j<=NV+1; j++){
		if(table[1][j] > 0.0 && table[1][j] > XMAX){
			XMAX = table[1][j];
			p2 = j;
		}
	}
	printf("p2 is %d\n",p2);
	printf("XMAX is %lf\n",XMAX);

	rap = 999999.0;
	for(i=2; i<=NC+1; i++){
		if(table[i][p2]>=0.0){
			goto e10;
		}
		v = fabs(table[i][1] / table[i][p2]);
		printf(" v is %lf\n",v);
		if(v<rap){
			rap = v;
			p1 = i;
		}
	
	e10: ; //labels
	}
	printf(" p1 is %d\n", p1);
	v = table[0][p2];
	table[0][p2] = table[p1][0];
	table[p1][0] = v;

	for(i=0; i<=NC+1; i++){
		for(j=0; j<=NV+1; j++){
			printf("%lf ", table[i][j]);
		}
		printf("\n");
	}
}

void Formula()
{
	//labels: e60,e70,e100,e110;
	int i,j;
	
	for(i=1; i<=NC+1; i++){
		if(i==p1){
			goto e70;
		}
		for(j=1; j<=NV+1; j++){
			if(j==p2){
				goto e60;
			}
			table[i][j] -= table[p1][j]*table[i][p2] / table[p1][p2];
		e60: ; //label
		}
	e70: ;  //label
	}

	table[p1][p2] = 1.0 / table[p1][p2];
	for(j=1; j<=NV+1; j++){
		if(j==p2){
			goto e100;
		}
		table[p1][j] *= fabs(table[p1][p2]);
	e100: ;    //label
	}

	for(i=1; i<=NC+1;i++){
		if(i==p2){
			goto e110;
		}
		table[i][p2] *= table[p1][p2];
	e110: ;    //label
	} 
			
}

void Optimize()
{
	int i,j;
	for(i=2; i<=NC+1; i++){
		if(table[i][1] < 0.0){
			XERR = 1;
		}
	}
	NOPTIMAL = 0;
	if(XERR ==1){
		return;
	}
	for(j=2; j<=NV+1; j++){
		if(table[1][j] > 0.0){
			NOPTIMAL = 1;
		}
	}
	
			
}	

void Results()
{
	//labels: e30,e70,e100;
	int i,j;

	if(XERR == 0){
		goto e30;
	}
	printf(" NO Solution.\n");
	goto e100;

e30:    for(i=1; i<=NV; i++){      //label
		for(j=2; j<=NC; j++){
			if(table[j][0] != 1.0*i){
				goto e70;
			}
			printf(" Variable #%d: %f\n", i, table[j][1]);
		e70: ;  //label
		}
	}
	printf("\n main function: %f\n", table[1][1]);
	
e100: printf("\n");
	
}

	
		

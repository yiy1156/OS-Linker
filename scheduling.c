#include <stdio.h>
#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h> 
#include <stdbool.h>


typedef struct process {
	int PID;		
	int BT;
	int IOT;							
	int AT;								
	int PRI;	
	int CPU_TIME;	
	int state; // 0: not start, // 1: first running, // 2: blocking // 3: 2nd running, // 4: done					
	int WT,TAT,RPT, RPT2;						
	int firstBT;	
	int order;				
	bool done;							
}pc;


int isNumber(char x[]);
void FCFS(pc p[], int n);
void SJF(pc p[], int n);
void RR(pc p[], int n, int quan);
struct process createProcess(int numOfPro, pc p[], int data[]);

int main(int argc, char *argv[]) {
	
	if (argc != 3)
	{
		fprintf(stderr, "USAGE:./scheduler <input-file> <scheduling algorithm> \n");
		exit(-1);
	}
	
	FILE *fp;
	int check, quantum, o=0, i=0, count=0, numOfPro=0;
	int data[500];
	char *token;	
	char buff[255],cont[255],choice[500],filepath[500], ch;
	char delimit[] = ",\n";		

	strcpy(filepath, argv[1]);						
	strcpy(choice, argv[2]);							
	
	fp = fopen( filepath , "r");
	while ((ch = fgetc(fp)))
	{
		if (feof (fp))
		{
			break;
		}
		buff[i] = ch;
		i++;
		count++;
	}							
	fclose(fp);
	
	for(i = 3 ; i<count ; i++)
	{ 
		cont[i-3] = buff[i];
	}

	numOfPro = buff[0] - '0';                                         	
	struct process p[numOfPro];
	i = 0;

	token = strtok(cont, delimit);	
	while(token != NULL) 
	{
		data[o] = atoi(token);
		sscanf(token, "%d %d %d %d", &(p[i].PID), &(p[i].BT), &(p[i].IOT), &(p[i].AT));
		token = strtok(NULL, delimit);
		i++;
	}	
	
    
    printf("===========================\n");
    printf("PID\tcpu\tiot\tarrival\n");
	for (i = 0 ; i<numOfPro ; i++)
	{
		printf("%d\t%d\t%d\t%d\n", p[i].PID, p[i].BT, p[i].IOT, p[i].AT);
	}
	printf("===========================\n");
	printf("\t  OUTPUT");
	
	char str1[] = "FCFS";
	char str2[] = "SJF";
	
	check = isNumber(choice);
	
	if (check == 0)
	{
		quantum = atoi(choice);
		printf("\nRR with q = %d: ",quantum);	
		RR(p,numOfPro,quantum);
	}
	else
	{
		if(strcmp(str1, choice) == 0 )
		{
			printf("\nFCFS:\n");
			FCFS(p,numOfPro);
		}	
		else if(strcmp(str2, choice) == 0 )
		{
			printf("\n\nSJF: ");
			SJF(p,numOfPro);
		}
		else
		{
			printf("\nWrong Input!!! *You allow to input \"FCFS\",\"SJF\",and number for RR only\n");
		}
	}
}

int isNumber(char x[])
{
	int i,check = 0;
	for(i = 0; i<strlen(x) ; i++)
	{
		if(48 > x[i] || 57 < x[i])
		{
			check = 1;
			break;
		}
	}
	return check;
}

struct process createProcess(int numOfPro, pc p[], int data[])
{
	int n = 0;
	int i = 0;
    for(i=0; i<numOfPro; i++)
    {
        if(i==0)
        {
            p[i].PID = data[i];
            p[i].BT  = data[i+1];
            p[i].AT = data[i+2];
            p[i].PRI = data[i+3];
        }
        else
        {
            n = i*4;
            p[i].PID = data[n];
            p[i].BT  = data[n+1];
            p[i].AT = data[n+2];
            p[i].PRI = data[n+3];
        }
    }
	return *p;
}

void sort_FCFS(pc p[], int n)
{
	pc temp;
	int i, j;
	for(i = 0; i < n - 1; i++)
	{
		for(j = 1; j < n; j++)
		{
			if( p[i].order > p[j].order )
			{
				temp = p[i];
				p[i] = p[j];
				p[j] = temp;
			}			
		}
	}
}

int getMaxOrder(pc p[], int n)
{
	int i;
	int max_order = 0;
	for(i = 0; i < n; i++)
	{
		if( p[i].order > max_order )
			max_order = p[i].order;				
	}

	return max_order;
}

void sort_AT_FCFS(pc p[], int n)
{
	pc temp;
	int i,j;
	for(i = 1 ; i<n ;i++)
	{
		for(j = 0; j<n-1 ; j++)
		{
			if(p[j].AT > p[j+1].AT)
			{
				temp = p[j];
				p[j] = p[j+1];
				p[j+1] = temp;
			}
			else if(p[j].AT == p[j+1].AT)
			{
				if(p[j].PRI > p[j+1].PRI)
				{
					temp = p[j];
					p[j] = p[j+1];
					p[j+1] = temp;
				}
				else if (p[j].PRI == p[j+1].PRI)
				{
					if(p[j].BT > p[j+1].BT)
					{
						temp = p[j];
						p[j] = p[j+1];
						p[j+1] = temp;
					}
					else if(p[j].BT == p[j+1].BT)
					{
						if(p[j].PID > p[j+1].PID)
						{
							temp = p[j];
							p[j] = p[j+1];
							p[j+1] = temp;
						}
					}
				}
			}
		}
	}
}

void sort_AT_SJF(pc p[], int n)
{
	pc temp;
	int i,j;
	for(i = 1 ; i<n ;i++)
	{
		for(j = 0; j<n-1 ; j++)
		{
			if(p[j].AT > p[j+1].AT)
			{
				temp = p[j];
				p[j] = p[j+1];
				p[j+1] = temp;
			}
			else if(p[j].AT == p[j+1].AT)
			{
				if(p[j].BT > p[j+1].BT)
				{
					temp = p[j];
					p[j] = p[j+1];
					p[j+1] = temp;
				}
				else if (p[j].BT == p[j+1].BT)
				{
					if(p[j].PRI > p[j+1].PRI)
					{
						temp = p[j];
						p[j] = p[j+1];
						p[j+1] = temp;
					}
					else if(p[j].PRI == p[j+1].PRI)
					{
						if(p[j].PID > p[j+1].PID)
						{
							temp = p[j];
							p[j] = p[j+1];
							p[j+1] = temp;
						}
					}
				}
			}
		}
	}
}

void sort_AT_RR(pc p[], int n)
{
	pc temp;
	int i,j;
	for(i = 1 ; i<n ;i++)
	{
		for(j = 0; j<n-1 ; j++)
		{
			if(p[j].AT > p[j+1].AT)
			{
				temp = p[j];
				p[j] = p[j+1];
				p[j+1] = temp;
			}
			else if(p[j].AT == p[j+1].AT)
			{
				if(p[j].PRI > p[j+1].PRI)
				{
					temp = p[j];
					p[j] = p[j+1];
					p[j+1] = temp;
				}
				else if (p[j].PRI == p[j+1].PRI)
				{
					if(p[j].BT > p[j+1].BT)
					{
						temp = p[j];
						p[j] = p[j+1];
						p[j+1] = temp;
					}
					else if(p[j].BT == p[j+1].BT)
					{
						if(p[j].PID > p[j+1].PID)
						{
							temp = p[j];
							p[j] = p[j+1];
							p[j+1] = temp;
						}
					}
				}
			}
		}
	}
}

void FCFS(pc p[], int n)
{
	int s[n];
	int i;
	int finishingTime;
	double cpuUtil=0;
	int c = 0; // cycle
	int selected_id = -1;	
	int count = 0;
	// 0: not start, 
	// 1: first running, 
	// 2: blocking 
	// 3: 2nd running, 
	// 4: done					
	// init ready time
	for(i = 0; i < n; i++)
	{
		p[i].RPT = p[i].AT;
		p[i].state = 0;		
		p[i].order = i;
		p[i].CPU_TIME = p[i].BT / 2;
	}

	while(true)
	{
		// check all is done
		bool all_done = true;
		for(i = 0; i < n; i++)
		{
			if( p[i].state != 4 )
			{				
				all_done = false;
				break;
			}
		}

		if( all_done )
			break;

		sort_FCFS(p, n);
		// selected id
		if( selected_id < 0 )
		{
			for(i = 0; i < n; i++)
			{
				if(p[i].RPT <= c)
				{
					p[i].RPT = c;
					selected_id = p[i].PID;
					if( p[i].state == 0 )
						p[i].state = 1; // running
					if( p[i].state == 2 )
						p[i].state = 3; // running
					break;
				}
			}
		}
		
		if( selected_id < 0 )					
		{
			c++;
			continue;
		}

		// change selected pid state		
		for(i = 0; i < n; i++)
		{
			if(p[i].PID == selected_id)
			{
				if( c - p[i].RPT >= p[i].CPU_TIME)
				{
					if( p[i].state == 1 ) // 1st running
					{
						p[i].RPT = p[i].RPT + p[i].CPU_TIME + p[i].IOT;	
						p[i].state = 2;		
						selected_id = -1;											
					}
					if( p[i].state == 3 )	// 2nd running
					{
						p[i].state = 4;	
						selected_id = -1;																								
					}
				}
			}
		}

		for(i = 0; i < n; i++)
			s[i] = 0;

		// output state		
		for(i = 0; i < n; i++)
		{	
			if( p[i].state == 0 )	// not started
			{
				if( c < p[i].RPT )	
				{
					s[p[i].PID] = 0;
					continue;
				}
				else
				{				
					if( selected_id < 0 )
					{
						selected_id = p[i].PID;
						p[i].state = 2;
						// printf(" %d: running", p[i].PID);
						s[p[i].PID] = 2;
					}
					else		
					{
						// printf(" %d: ready", p[i].PID);
						s[p[i].PID] = 1;
					}
				}
			}
			else if( p[i].state == 1 || p[i].state == 3 )	// running
			{
				if( c < p[i].RPT )	
				{
					s[p[i].PID] = 0;
					continue;
				}
				else
				{						
					// printf(" %d: running", p[i].PID);
					s[p[i].PID] = 2;
				}
			}
			else if( p[i].state == 2 ) // blocked
			{
				if( c + 1 == p[i].RPT)
					p[i].order = getMaxOrder(p, n) + 1;	
				
				if( c < p[i].RPT )
				{
					// printf(" %d: blocked", p[i].PID);
					s[p[i].PID] = 3;
				}
				else
				{
					if( selected_id < 0 )
					{
						// Add on end of Queue						
						// printf(" %d: running", p[i].PID);	
						s[p[i].PID] = 2;
						selected_id = p[i].PID;
						p[i].state = 3;					
					}
					else
					{
						// printf(" %d: ready", p[i].PID);
						s[p[i].PID] = 1;
					}
				}
			} 
			
		}

		printf("%d", c);
		count = 0;
		for(i = 0; i < n; i++)
			if( s[i] > 0 )
				count++;
		if( count < 1 )
			break;

		for(i = 0; i < n; i++)
		{
			switch(s[i])
			{
				case 1:
					printf(" %d: ready", i);
					break;
				case 2:
					printf(" %d: running", i);
					break;
				case 3:
					printf(" %d: blocked", i);
					break;

			}
		}

		printf("\n");

		c++;
	}
	printf("\n");
	finishingTime=c-1;
	printf("finishing time: %d", finishingTime);
	cpuUtil = (double) finishingTime/(double) c;
	printf("\n");
	printf("cpu utilization: %f", cpuUtil);
	printf("\n");
	printf("Turnaround process 0: %d", count);
	
}

void SJF(pc p[], int n)
{
	int s[n];
	int i;
	int finishingTime;
	double cpuUtil=0;
	int c = 0; // cycle
	int selected_id = -1;	
	int count = 0;
	// 0: not start, 
	// 1: first running, 
	// 2: blocking 
	// 3: 2nd running, 
	// 4: done					
	// init ready time
	for(i = 0; i < n; i++)
	{
		p[i].RPT = p[i].AT;
		p[i].state = 0;		
		p[i].order = i;
		p[i].CPU_TIME = p[i].BT / 2;
	}

	while(true)
	{
		// check all is done
		bool all_done = true;
		for(i = 0; i < n; i++)
		{
			if( p[i].state != 4 )
			{				
				all_done = false;
				break;
			}
		}

		if( all_done )
			break;

		sort_AT_SJF(p, n);
		// selected id
		if( selected_id < 0 )
		{
			for(i = 0; i < n; i++)
			{
				if(p[i].RPT <= c)
				{
					p[i].RPT = c;
					selected_id = p[i].PID;
					if( p[i].state == 0 )
						p[i].state = 1; // running
					if( p[i].state == 2 )
						p[i].state = 3; // running
					break;
				}
			}
		}
		
		if( selected_id < 0 )					
		{
			c++;
			continue;
		}

		// change selected pid state		
		for(i = 0; i < n; i++)
		{
			if(p[i].PID == selected_id)
			{
				if( c - p[i].RPT >= p[i].CPU_TIME)
				{
					if( p[i].state == 1 ) // 1st running
					{
						p[i].RPT = p[i].RPT + p[i].CPU_TIME + p[i].IOT;	
						p[i].state = 2;		
						selected_id = -1;											
					}
					if( p[i].state == 3 )	// 2nd running
					{
						p[i].state = 4;	
						selected_id = -1;																								
					}
				}
			}
		}

		for(i = 0; i < n; i++)
			s[i] = 0;

		// output state		
		for(i = 0; i < n; i++)
		{	
			if( p[i].state == 0 )	// not started
			{
				if( c < p[i].RPT )	
				{
					s[p[i].PID] = 0;
					continue;
				}
				else
				{				
					if( selected_id < 0 )
					{
						selected_id = p[i].PID;
						p[i].state = 2;
						// printf(" %d: running", p[i].PID);
						s[p[i].PID] = 2;
					}
					else		
					{
						// printf(" %d: ready", p[i].PID);
						s[p[i].PID] = 1;
					}
				}
			}
			else if( p[i].state == 1 || p[i].state == 3 )	// running
			{
				if( c < p[i].RPT )	
				{
					s[p[i].PID] = 0;
					continue;
				}
				else
				{						
					// printf(" %d: running", p[i].PID);
					s[p[i].PID] = 2;
				}
			}
			else if( p[i].state == 2 ) // blocked
			{
				if( c + 1 == p[i].RPT)
					p[i].order = getMaxOrder(p, n) + 1;	
				
				if( c < p[i].RPT )
				{
					// printf(" %d: blocked", p[i].PID);
					s[p[i].PID] = 3;
				}
				else
				{
					if( selected_id < 0 )
					{
						// Add on end of Queue						
						// printf(" %d: running", p[i].PID);	
						s[p[i].PID] = 2;
						selected_id = p[i].PID;
						p[i].state = 3;					
					}
					else
					{
						// printf(" %d: ready", p[i].PID);
						s[p[i].PID] = 1;
					}
				}
			} 
			
		}

		printf("%d", c);
		count = 0;
		for(i = 0; i < n; i++)
			if( s[i] > 0 )
				count++;
		if( count < 1 )
			break;

		for(i = 0; i < n; i++)
		{
			switch(s[i])
			{
				case 1:
					printf(" %d: ready", i);
					break;
				case 2:
					printf(" %d: running", i);
					break;
				case 3:
					printf(" %d: blocked", i);
					break;

			}
		}

		printf("\n");

		c++;
	}
	printf("\n");
	finishingTime=c-1;
	printf("finishing time: %d", finishingTime);
	cpuUtil = (double) finishingTime/(double) c;
	printf("\n");
	printf("cpu utilization: %f", cpuUtil);
	printf("\n");
	printf("Turnaround process 0: %d", count);
	
}

void RR(pc p[], int n, int quan)
{
	int s[n];
	int i;
	int finishingTime;
	double cpuUtil=0;
	int c = 0; // cycle
	int selected_id = -1;	
	int count = 0;
	// 0: not start, 
	// 1: first running, 
	// 2: blocking 
	// 3: 2nd running, 
	// 4: done					
	// init ready time
	for(i = 0; i < n; i++)
	{
		p[i].RPT = p[i].AT;
		p[i].state = 0;		
		p[i].order = i;
		p[i].CPU_TIME = p[i].BT / 2;
	}

	while(true)
	{
		// check all is done
		bool all_done = true;
		for(i = 0; i < n; i++)
		{
			if( p[i].state != 4 )
			{				
				all_done = false;
				break;
			}
		}

		if( all_done )
			break;

		sort_AT_RR(p, n);
		// selected id
		if( selected_id < 0 )
		{
			for(i = 0; i < n; i++)
			{
				if(p[i].RPT <= c)
				{
					p[i].RPT = c;
					selected_id = p[i].PID;
					if( p[i].state == 0 )
						p[i].state = 1; // running
					if( p[i].state == 2 )
						p[i].state = 3; // running
					break;
				}
			}
		}
		
		if( selected_id < 0 )					
		{
			c++;
			continue;
		}

		// change selected pid state		
		for(i = 0; i < n; i++)
		{
			if(p[i].PID == selected_id)
			{
				if( c - p[i].RPT >= p[i].CPU_TIME)
				{
					if( p[i].state == 1 ) // 1st running
					{
						p[i].RPT = p[i].RPT + p[i].CPU_TIME + p[i].IOT;	
						p[i].state = 2;		
						selected_id = -1;											
					}
					if( p[i].state == 3 )	// 2nd running
					{
						p[i].state = 4;	
						selected_id = -1;																								
					}
				}
			}
		}

		for(i = 0; i < n; i++)
			s[i] = 0;

		// output state		
		for(i = 0; i < n; i++)
		{	
			if( p[i].state == 0 )	// not started
			{
				if( c < p[i].RPT )	
				{
					s[p[i].PID] = 0;
					continue;
				}
				else
				{				
					if( selected_id < 0 )
					{
						selected_id = p[i].PID;
						p[i].state = 2;
						// printf(" %d: running", p[i].PID);
						s[p[i].PID] = 2;
					}
					else		
					{
						// printf(" %d: ready", p[i].PID);
						s[p[i].PID] = 1;
					}
				}
			}
			else if( p[i].state == 1 || p[i].state == 3 )	// running
			{
				if( c < p[i].RPT )	
				{
					s[p[i].PID] = 0;
					continue;
				}
				else
				{						
					// printf(" %d: running", p[i].PID);
					s[p[i].PID] = 2;
				}
			}
			else if( p[i].state == 2 ) // blocked
			{
				if( c + 1 == p[i].RPT)
					p[i].order = getMaxOrder(p, n) + 1;	
				
				if( c < p[i].RPT )
				{
					// printf(" %d: blocked", p[i].PID);
					s[p[i].PID] = 3;
				}
				else
				{
					if( selected_id < 0 )
					{
						// Add on end of Queue						
						// printf(" %d: running", p[i].PID);	
						s[p[i].PID] = 2;
						selected_id = p[i].PID;
						p[i].state = 3;					
					}
					else
					{
						// printf(" %d: ready", p[i].PID);
						s[p[i].PID] = 1;
					}
				}
			} 
			
		}

		printf("%d", c);
		count = 0;
		for(i = 0; i < n; i++)
			if( s[i] > 0 )
				count++;
		if( count < 1 )
			break;

		for(i = 0; i < n; i++)
		{
			switch(s[i])
			{
				case 1:
					printf(" %d: ready", i);
					break;
				case 2:
					printf(" %d: running", i);
					break;
				case 3:
					printf(" %d: blocked", i);
					break;

			}
		}

		printf("\n");

		c++;
	}
	printf("\n");
	finishingTime=c-1;
	printf("finishing time: %d", finishingTime);
	cpuUtil = (double) finishingTime/(double) c;
	printf("\n");
	printf("cpu utilization: %f", cpuUtil);
	printf("\n");
	printf("Turnaround process 0: %d", count);
	
}
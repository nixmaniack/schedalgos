/*
 ============================================================================
 Name        : schedalgo.c
 Author      : Muneeb Shaikh
 Version     : 0.2.7
 Copyright   : GPLv3
 Description : Process Scheduling Algorithms.
 	 	 	 	 1.	First Come First Serve
				 2.	Shortest Job First (Preemptive)
				 3.	Priority Based (Non-Preemptive)
				 4.	Round Robin (Non-Preemptive)
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**************Global Variable Declarations **********************************/

static int currentTime=0;		/*This variable holds current time and increases as processes get executed*/
int numOfProcesses;				/*variable to hold number of processes*/
int currentRunningProcess;		/*Keeps track of current running process*/

typedef struct {
	char name[3];
	int arrivalTime;
	int burstTime;
	int startTime;
	int finishTime;
	int waitingTime;
	int IsActiveFor;
}proc;

typedef struct {
	char name[3];
}names;

/**************Function Declarations **********************************/

void psort(proc *, int);			/* Sorts Processes according to Burst Time */
int selectProcess(proc *);			/* Selects the process having shortest burst time */
void execProcess(proc *, int, names *);		/* Executes process got from selectProcess() */
int getTotalBurstTime(proc *, int);        /* Calculates total burst Time */
void fcfs(proc *, names *);			/*Declaration for FCFS function*/
void sjfp(proc *, names *);			/*Declaration for SJFP function*/
void priority(proc *, names *);		/*Declaration for Priority Based function*/
void prioSort(proc *, int);			/*Declaration for sorting according to highest priority function*/
void roundrobin(proc *, names *);  	/*Declaration for Round Robin function*/

/**************Main starts here **********************************/

int main(void) {
	proc process[10];
	names pnames[10];
	int choice;

	do{
	        printf("\n\n\t\t\t*MENU*\n");
	        printf("          \n \
	                1. First Come First Serve \n \
	                2. Shortest Job First (Preemptive)\n \
	                3. Priority Based (Non-Preemptive)\n \
	                4. Round Robin (Non-Preemptive)\n \
	                5. EXIT \n \
	                ");
	        printf("Enter your choice: ");
	        scanf("%d", &choice);
	        switch(choice) {
	            case 1:
	                fcfs(process, pnames);
	                break;
	            case 2:
	                sjfp(process, pnames);
	                break;
	            case 3:
	                priority(process, pnames);
	                break;
	            case 4:
	                roundrobin(process, pnames);
	                break;
	            case 5:
	                exit(0);
	            default:
	                printf("\nInvalid Choice\n");
	                break;
	        }
	    }while(choice != '5');



	return EXIT_SUCCESS;
}
/*** Main ends here ***/

/*********************** Function Definitions **********************************/

/**
 * Function Name: 	psort()
 * Function:		Sorts Process Array according arrivalTime in ascending order
 * Input:			Process Array, Total number of process
 * Output:			Sorted process array
 * Affects:			Destroys original process array
 */
void psort(proc pproc[], int numOfProcesses) {
	int i, j, test;
	proc temp;

	for(i = numOfProcesses - 1; i > 0; i--) {
		test = 0;
		for(j=0; j < i; j++) {
			if((pproc[j].arrivalTime) > (pproc[j+1].arrivalTime) ) {
				temp = pproc[j];
				pproc[j] = pproc[j+1];
				pproc[j+1] = temp;
				test=1;
			}
		}
		if(test==0) break;
	}
}


/**
 * Function Name: 	selectProcess
 * Function:		Selects the process having the smallest NON-ZERO burstTime from the currently arrived processes
 * Input:			CurrentTime(from global variable), Sorted process array
 * Output:			Index of Process having smallest burstTime from arrived processes
 * Affects:			Nothing
 */

int selectProcess(proc pproc[]) {

	int i=0, j, indexOfleastProcessTime, minBurstTime;
	indexOfleastProcessTime = -1;

	while ( pproc[i].arrivalTime <= currentTime && i < numOfProcesses) {
		i++; 									/* Find array of arrived processes*/
	}

	i--;		/*since array index starts from zero and we already incremented 'i' in while loop we have to decrement so that we don't cross currentTime*/

	for(j=0; j <= i && pproc[j].burstTime == 0; j++); /* Find first non-zero value in array*/

	/* Finding index of minimum burstTime */
	minBurstTime = pproc[j].burstTime;
	indexOfleastProcessTime = j;

	while(j<=i) {
		if(pproc[j].burstTime != 0 && pproc[j].burstTime < minBurstTime) {
			minBurstTime = pproc[j].burstTime;
			indexOfleastProcessTime = j;
		}
		j++;
	}

	/*Keep record of startTime*/
	if(indexOfleastProcessTime == 0 && pproc[indexOfleastProcessTime].startTime == 0) {
		pproc[indexOfleastProcessTime].startTime = 0;
	} else if(pproc[indexOfleastProcessTime].startTime == 0) {
		pproc[indexOfleastProcessTime].startTime = currentTime;
	}

	currentRunningProcess = indexOfleastProcessTime; /*This is not needed now but you may use it while animating Gantt Chart*/

	return indexOfleastProcessTime;
}

/**
 * Function Name: 	execProcess
 * Function: 		Executes process returned by the selectProcess function
 * Input: 			Sorted process Array, totalBurstTIme, another process array for storing sequence of executed processes
 * Output:			Status of executing process
 * Affects:			currentTime, burstTime of process under execution
 */
void execProcess(proc pproc[], int totalBurstTime, names pnames[]) {
	int indexOfleastProcessTime, i=0;

	while (currentTime != totalBurstTime) { /*Terminating Condition*/
		printf("\n\nFetching next Shortest Process...\n");

		indexOfleastProcessTime = selectProcess(pproc); /* Call to process selection function */

		printf("\nExecuting Process %s \n", pproc[indexOfleastProcessTime].name);

		pproc[indexOfleastProcessTime].burstTime -= 1;
		pproc[indexOfleastProcessTime].IsActiveFor += 1;

		/*Stores finish time if current process gets totally executed i.e. burstTime becomes zero and also calculates waiting time*/
		if(pproc[indexOfleastProcessTime].burstTime == 0){

			pproc[indexOfleastProcessTime].finishTime = currentTime+1;
/*
 * 		This waiting time is total waiting time. It means that idle time spent by the process since it has arrived
 * 		pproc[indexOfleastProcessTime].waitingTime = pproc[indexOfleastProcessTime].finishTime-pproc[indexOfleastProcessTime].IsActiveFor-pproc[indexOfleastProcessTime].startTime;
 *
 */

			pproc[indexOfleastProcessTime].waitingTime = pproc[indexOfleastProcessTime].startTime - pproc[indexOfleastProcessTime].arrivalTime ;
            /*Handle if no process arrives at time 0*/
            if(pproc[indexOfleastProcessTime].waitingTime < 0) {
                pproc[indexOfleastProcessTime].waitingTime =0;
            }
		}

		system("sleep 1"); 				/*Get the feeling of process being executed :) (Same as delay() function in Windows)*/
		strcpy(pnames[i].name, pproc[indexOfleastProcessTime].name); /*Store the process name to display in chart*/
		i++;
		currentTime += 1;
	}

}

/**
 * Function Name:	getTotalBurstTime
 * Function: 		Calculates total burst time by summing burst time of individual process from process array
 * Input:			process array
 * Output:			totalBurstTime
 * Affects:			Nothing
 */

int getTotalBurstTime(proc pproc[], int numOfProcesses) {
	int i, sum = 0;
	for(i=0; i< numOfProcesses; i++) {
		sum += pproc[i].burstTime;
	}
	return sum;

}

/**
 * Function Name: 	sjfp
 * Function: 		To perform Shortest Job First Pre-emptive algorithm
 * Input: 			Process Array, another process array for storing sequence of executed processes
 * Output:			None
 * Affects:			currentTime, process array structure, sequencing structure i.e. pnames
 */

void sjfp(proc process[], names pnames[]) {

		int i, totalServiceTime;
		currentTime = 0;
		puts("Shortest Job First Pre-emptive Algorithm\n");
		puts("Enter Number of Processes: ");
		scanf("%d", &numOfProcesses);

		for(i=0; i<numOfProcesses; i++) {
			printf("Enter Process Name(Max 2 chars): ");
			scanf("%s", process[i].name);
			printf("Enter Arrival Time for Process %s: ", process[i].name);
			scanf("%d", &process[i].arrivalTime);
			printf("Enter Burst Time for Process %s: ", process[i].name);
			scanf("%d", &process[i].burstTime);

			/* Initialise other properties accordingly*/
			process[i].startTime = 0;
			process[i].waitingTime = 0;
			process[i].IsActiveFor = 0;

		}

		printf("\n\nPName\tArrTime\tBTime\n");

		for(i=0; i<numOfProcesses; i++) {
			printf("\n%s\t%d\t%d", process[i].name, process[i].arrivalTime, process[i].burstTime);
		}

		psort(process, numOfProcesses);
		puts("\n");

		totalServiceTime = getTotalBurstTime(process, numOfProcesses);
		printf("\nTotal Burst Time: %d \n", totalServiceTime );

	    execProcess(process, getTotalBurstTime(process, numOfProcesses ), pnames);

	    printf("\n\nPName\tArrTime\tBTime\tSTime\tFTime\tWTime\tTATime\n");


		/*Display final table*/
		for(i=0; i<numOfProcesses; i++) {
			printf("\n%s\t%d\t%d\t%d\t%d\t%d\t%d", process[i].name, process[i].arrivalTime, process[i].IsActiveFor,
					process[i].startTime, process[i].finishTime, process[i].waitingTime, process[i].finishTime - process[i].arrivalTime);
		}


		printf("\n\nAll Processes executed...\n\n");

		printf("Gantt Chart:\n");

	    /**
	     * Displaying time axis
	     * NOTE: This is not generic for loop. YOU HAVE TO ADJUST value of i, and font size of terminal accordingly to get proper view in line
	     */
	    for(i=0; i<=20; i++) {
	        printf("|%2d ",i);
	    }
	    printf("\n");

	    /*Display processes sequentially*/
		for(i=0; i<currentTime; i++) {
			printf("|%s  ", pnames[i].name );
		}
		printf("|\n");
}


/**
 * Function Name: 	fcfs
 * Function: 		To perform First Come First Serve algorithm
 * Input: 			Process Array, another process array for storing sequence of executed processes
 * Output:			None
 * Affects:			currentTime, process array structure, sequencing structure i.e. pnames
 */

void fcfs(proc process[], names pnames[]) {
	int i, totalServiceTime;
	currentTime = 0;
	puts("\n\t\t\tFirst Come First Serve Algorithm\n");
	puts("Enter Number of Processes: ");
	scanf("%d", &numOfProcesses);

	for(i=0; i<numOfProcesses; i++) {
		printf("Enter Process Name(Max 2 chars): ");
		scanf("%s", process[i].name);
		printf("Enter Arrival Time for Process %s: ", process[i].name);
		scanf("%d", &process[i].arrivalTime);
		printf("Enter Burst Time for Process %s: ", process[i].name);
		scanf("%d", &process[i].burstTime);

		/* Initialise other properties accordingly*/
		process[i].startTime = 0;
		process[i].waitingTime = 0;
		process[i].IsActiveFor = 0;

	}

	printf("\n\nPName\tArrTime\tBTime\n");

	for(i=0; i<numOfProcesses; i++) {
		printf("\n%s\t%d\t%d", process[i].name, process[i].arrivalTime, process[i].burstTime);
	}

	psort(process, numOfProcesses); /*To sort process array according to arrival time*/
	puts("\n");

	totalServiceTime = getTotalBurstTime(process, numOfProcesses);
	printf("\nTotal Burst Time: %d \n", totalServiceTime );

	for (i=0; i<numOfProcesses; i++) {
		for(; process[i].burstTime > 0; process[i].burstTime -= 1) {
			printf("\nFetching next Process...\n");
			printf("\nExecuting Process %s ...\n", process[i].name);
			strcpy(pnames[currentTime].name, process[i].name);
			system("sleep 1");
			process[i].IsActiveFor += 1;
			currentTime +=1;
		}
		process[i].finishTime = currentTime;
		process[i+1].startTime = currentTime;
		process[i].waitingTime = process[i].startTime - process[i].arrivalTime;

	}


	printf("\n\nPName\tArrTime\tBTime\tSTime\tFTime\tWTime\tTATime\n");


	/*Display final table*/
	for(i=0; i<numOfProcesses; i++) {
		printf("\n%s\t%d\t%d\t%d\t%d\t%d\t%d", process[i].name, process[i].arrivalTime, process[i].IsActiveFor,
				process[i].startTime, process[i].finishTime, process[i].waitingTime, process[i].finishTime - process[i].arrivalTime);
	}


	printf("\n\nAll Processes executed...\n\n");

	printf("Gantt Chart:\n");

    /**
     * Displaying time axis
     * NOTE: This is not generic for loop. YOU HAVE TO ADJUST value of i, and font size of terminal accordingly to get proper view in line
     */
    for(i=0; i<=20; i++) {
        printf("|%2d ",i);
    }
    printf("\n");

    /*Display processes sequentially*/
	for(i=0; i<currentTime; i++) {
		printf("|%s  ", pnames[i].name );
	}
	printf("|\n");

}


/**
 * Function Name: 	priority
 * Function: 		To perform Priority Based Scheduling algorithm
 * Input: 			Process Array, another process array for storing sequence of executed processes
 * Output:			None
 * Affects:			currentTime, process array structure, sequencing structure i.e. pnames
 * IMP NOTES:		1. IN THIS ALGORITHM, I HAVE TAKEN _PRIORITY BIT_ as arrivalTime bit in structure JUST TO AVOID EXTRA FIELD IN PROCESS STRUCTURE
 * 					2. Here the priority is more if priority bit is high unlike *nix systems where highest priority is for bit 0 and decreases as priority bit value increases.
 */

void priority(proc process[], names pnames[]) {
	int i, totalServiceTime;
	currentTime = 0;
	puts("Priority Based (Non-Preemptive) Algorithm\n");
	puts("Enter Number of Processes: ");
	scanf("%d", &numOfProcesses);

	for(i=0; i<numOfProcesses; i++) {
		printf("Enter Process Name(Max 2 chars): ");
		scanf("%s", process[i].name);
		printf("Enter Priority for Process %s: ", process[i].name);
		scanf("%d", &process[i].arrivalTime);
		printf("Enter Burst Time for Process %s: ", process[i].name);
		scanf("%d", &process[i].burstTime);

		/* Initialise other properties accordingly*/
		process[i].startTime = 0;
		process[i].waitingTime = 0;
		process[i].IsActiveFor = 0;

	}

	printf("\n\nPName\tPriority\tBTime\n");

	for(i=0; i<numOfProcesses; i++) {
		printf("\n%s\t%d\t\t%d", process[i].name, process[i].arrivalTime, process[i].burstTime);
	}

	prioSort(process, numOfProcesses);

	puts("\n");

	totalServiceTime = getTotalBurstTime(process, numOfProcesses);
	printf("\nTotal Burst Time: %d \n", totalServiceTime );

	for (i=0; i<numOfProcesses; i++) {
		for(; process[i].burstTime > 0; process[i].burstTime -= 1) {
			printf("\nFetching next Process...\n");
			printf("\nExecuting process %s ...\n", process[i].name);
			system("sleep 1");
			strcpy(pnames[currentTime].name, process[i].name);
			process[i].IsActiveFor += 1;
			currentTime +=1;
		}
		process[i].finishTime = currentTime;
		process[i+1].startTime = currentTime;
		/*If we consider arrival time then use this
		process[i].waitingTime = process[i].startTime - process[i].arrivalTime;
		*/
		process[i+1].waitingTime = process[i+1].startTime;

	}



	printf("\n\nPName\tPriority\tBTime\tSTime\tFTime\tWTime\tTATime\n");


	/*Display final table*/
	for(i=0; i<numOfProcesses; i++) {
		printf("\n%s\t%d\t\t%d\t%d\t%d\t%d\t%d", process[i].name, process[i].arrivalTime, process[i].IsActiveFor,
				process[i].startTime, process[i].finishTime, process[i].waitingTime, process[i].finishTime - 0);
	}


	printf("\n\nAll Processes executed...\n\n");

	printf("Gantt Chart:\n");

	/**
	 * Displaying time axis
	 * NOTE: This is not generic for loop. YOU HAVE TO ADJUST value of i, and font size of terminal accordingly to get proper view in line
	 */
	for(i=0; i<=20; i++) {
		printf("|%2d ",i);
	}
	printf("\n");

	/*Display processes sequentially*/
	for(i=0; i<currentTime; i++) {
		printf("|%s  ", pnames[i].name );
	}
	printf("|\n");



}


/**
 * Function Name: 	priSort
 * Function: 		To perform sorting of processes based on priorities algorithm. The processes are sorted in descending order of their priorities.
 * Input: 			Process Array, Total number of process
 * Output:			Sorted Process Array according to highest priority first
 * Affects:			Process Array
 * IMP NOTES:		Considering the arrivalTime as priority bit.
 */

void prioSort(proc pproc[], int numOfProcesses) {
	int i, j, test;
	proc temp;

	for(i = numOfProcesses - 1; i > 0; i--) {
		test = 0;
		for(j=0; j < i; j++) {
			if((pproc[j].arrivalTime) < (pproc[j+1].arrivalTime) ) {
				temp = pproc[j];
				pproc[j] = pproc[j+1];
				pproc[j+1] = temp;
				test=1;
			}
		}
		if(test==0) break;
	}
}

/**
 * Function Name: 	roundrobin
 * Function: 		To perform Round Robin Scheduling algorithm
 * Input: 			Process Array, another process array for storing sequence of executed processes
 * Output:			None
 * Affects:			currentTime, process array structure, sequencing structure i.e. pnames
 */

void roundrobin(proc process[], names pnames[]) {
	int i, j, totalServiceTime, timeQuanta;
	currentTime = 0;
	puts("Round Robin (Non-Preemptive) Algorithm\n");
	puts("Enter Number of Processes: ");
	scanf("%d", &numOfProcesses);

	for(i=0; i<numOfProcesses; i++) {
		printf("Enter Process Name(Max 2 chars): ");
		scanf("%s", process[i].name);
		printf("Enter Arrival Time for Process %s: ", process[i].name);
		scanf("%d", &process[i].arrivalTime);
		printf("Enter Burst Time for Process %s: ", process[i].name);
		scanf("%d", &process[i].burstTime);

		/* Initialise other properties accordingly*/
		process[i].startTime = -1;
		process[i].waitingTime = 0;
		process[i].IsActiveFor = 0;

	}

	printf("Enter Time Quanta: ");
	scanf("%d", &timeQuanta);

	printf("\n\nPName\tArrTime\tBTime\n");

	for(i=0; i<numOfProcesses; i++) {
		printf("\n%s\t%d\t%d", process[i].name, process[i].arrivalTime, process[i].burstTime);
	}

	psort(process, numOfProcesses);
	puts("\n");

	totalServiceTime = getTotalBurstTime(process, numOfProcesses);
	printf("\nTotal Burst Time: %d \n", totalServiceTime );

	for (i = 0; currentTime <= totalServiceTime-1; i++) {

		/*To keep looping in a round fashion for given number of processes e.g.  1 2 3 1 2 3 1 2 3... */
		if (i == numOfProcesses) {
			i=0;
		}

		/*Execute current selected process for given Time Quantum*/
		for (j = 0; j < timeQuanta; j++) {

			/*Execute only if process has arrived*/
			if(process[i].arrivalTime <= currentTime) {
				if(process[i].burstTime == 0) {
					continue;						/*If burstTime is zero then we have executed the process already so skip pass*/
				}

				process[i].burstTime -=1;			/*Decrease burst time and increase active time*/
				process[i].IsActiveFor += 1;


				if(process[i].startTime == -1) {		/*If process is being executed for first time then current time is start time*/
					process[i].startTime = currentTime;
				}

				printf("\nFetching next Process...\n");
				printf("\nExecuting process %s ...\n", process[i].name);
				system("sleep 1");
				strcpy(pnames[currentTime].name, process[i].name);		/*Store it for further display*/
				currentTime +=1;										/*Increase current Time*/
				if(process[i].burstTime == 0) {							/*If burst time becomes zero mark the finish time of process*/
					process[i].finishTime = currentTime;
				}

			} else {
				currentTime +=1;										/*This part is not tested but only primary implementation is done for process those are not yet arrived and there's no process to be executed. Filling the IDLE TIME GAP*/
				printf("\nNo Process is being executed..\n");
			}
		}

	}





	printf("\n\nPName\tArrTime\tBTime\tSTime\tFTime\tWTime\tTATime\n");


	/*Display final table*/
	for(i=0; i<numOfProcesses; i++) {
		printf("\n%s\t%d\t%d\t%d\t%d\t%d\t%d", process[i].name, process[i].arrivalTime, process[i].IsActiveFor,
				process[i].startTime, process[i].finishTime, process[i].startTime - process[i].arrivalTime, process[i].finishTime - process[i].arrivalTime);
	}


	printf("\n\nAll Processes executed...\n\n");

	printf("Gantt Chart:\n");

	/**
	 * Displaying time axis
	 * NOTE: This is not generic for loop. YOU HAVE TO ADJUST value of i, and font size of terminal accordingly to get proper view in line
	 */
	for(i=0; i<=20; i++) {
		printf("|%2d ",i);
	}
	printf("\n");

	/*Display processes sequentially*/
	for(i=0; i<currentTime; i++) {
		printf("|%s  ", pnames[i].name );
	}
	printf("|\n");


}































/* These line is purposefully written at end */

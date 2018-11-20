/* 
 * stoplight.c
 *
 * 31-1-2003 : GWA : Stub functions created for CS161 Asst1.
 *
 * NB: You can use any synchronization primitives available to solve
 * the stoplight problem in this file.
 */


/*
 * 
 * Includes
 *
 */

#include <types.h>
#include <lib.h>
#include <test.h>
#include <thread.h>
#include <synch.h>

/*
 *
 * Constants
 *
 */

struct lock *NW;
struct lock *NE;
struct lock *SW;
struct lock *SE;
struct lock *intersection;
struct lock *mainlock;

// not sure if needed but a lock so that one car prints at a time
struct lock *printmsg;
 
int numcars; //number of cars in the intersection

/*
 * Number of cars created.
 */

#define NCARS 20


/*
 *
 * Function Definitions
 *
 */


/*
 * gostraight()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement passing straight through the
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
gostraight(unsigned long cardirection,
           unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */
        
        (void) cardirection;
        (void) carnumber;
}


/*
 * turnleft()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a left turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnleft(unsigned long cardirection,
         unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        (void) cardirection;
        (void) carnumber;
}


/*
 * turnright()
 *
 * Arguments:
 *      unsigned long cardirection: the direction from which the car
 *              approaches the intersection.
 *      unsigned long carnumber: the car id number for printing purposes.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      This function should implement making a right turn through the 
 *      intersection from any direction.
 *      Write and comment this function.
 */

static
void
turnright(unsigned long cardirection,
          unsigned long carnumber)
{
        /*
         * Avoid unused variable warnings.
         */

        (void) cardirection;
        (void) carnumber;
}

/* enterNW()
 */



/*
 * approachintersection()
 *
 * Arguments: 
 *      void * unusedpointer: currently unused.
 *      unsigned long carnumber: holds car id number.
 *
 * Returns:
 *      nothing.
 *
 * Notes:
 *      Change this function as necessary to implement your solution. These
 *      threads are created by createcars().  Each one must choose a direction
 *      randomly, approach the intersection, choose a turn randomly, and then
 *      complete that turn.  The code to choose a direction randomly is
 *      provided, the rest is left to you to implement.  Making a turn
 *      or going straight should be done by calling one of the functions
 *      above.
 */
 
static
void
approachintersection(void * unusedpointer,
                     unsigned long carnumber)
{	

        int cardirection;
	int turndirection; //added random turn direction
        /*
         * Avoid unused variable and function warnings.
         */

        (void) unusedpointer;
        (void) carnumber;
	(void) gostraight;
	(void) turnleft;
	(void) turnright;

        /*
         * cardirection and turn direction is set randomly.
         */

        cardirection = random() % 4; //0 North, 1 East, 2 South, 3 West
	
	turndirection = random() % 3;//0 Straight, 1 Right, 2 Left
	// use the car direction and the turn direction, to determine route and actions to take 
	switch (cardirection){
		case 0: // north
			switch (turndirection){
				case 0: // straight - north to south
					kprintf("Approaching, Carnumber: %lu, From North to South\n",carnumber);
					
						
					// dont let more than three cars in the intersection at once
					// to prevent deadlock
        				if (numcars >=2) {
                				lock_acquire(mainlock);
        				}
					//if numcars < 3
					//aquire the lock needed for the first quadrant of the intersection
						lock_acquire(NW);

					// once the intersection is entered, aquire the lock that allows for
					// updating number of cars in the intersection and increment it
					// then print to show the intersection has entered successfully
						lock_acquire(intersection);
						numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From North to South\n",carnumber);

					// aquire the next quadrant necessary for the car to enter
						lock_acquire(SW);

					// once the car has entered the new quadrant, release hold on the old one
                                        	lock_release(NW);

					// once the car needs to leave the intersection, print the status, release the
					// last quadrant, and update number of cars in the intersection 
						kprintf("Leaving, Carnumber: %lu, From North to South\n",carnumber);
						lock_release(SW);
						lock_acquire(intersection);
						numcars--;
						lock_release(intersection);

					break;

				/* 
 				 * The rest of approach_intersection() works the same as above
 				 * with slight differences depending on the direction the car is coming from
 				 * and the direction it is going. However it is generally the same as above
 				 * so most of the comments will be left out to avoid redundancy
 				 * 
 				 * At the end of the switch statement is a lock release if the number of cars in
 				 * the intersection is less than 3
 				 */

				case 1: // right - north to west
                                        kprintf("Approaching, Carnumber: %lu, From North to West\n",carnumber);
        				if (numcars >=2) {
                			lock_acquire(mainlock);
        				}
					//if numcars < 3
                                                lock_acquire(NW);
                                                lock_acquire(intersection);
                                                numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From North to West\n",carnumber);
						
						kprintf("Leaving, Carnumber: %lu, From North to West\n",carnumber);
						lock_release(NW);
						lock_acquire(intersection);
						numcars--;
                                                lock_release(intersection);
					break;
				case 2: // left - north to east
                                        kprintf("Approaching, Carnumber: %lu, From North to East\n",carnumber);
        				if (numcars >=2) {
                			lock_acquire(mainlock);
        				} 
					       lock_acquire(NW);
                                                lock_acquire(intersection);
                                                numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From North to East\n",carnumber);
                                                lock_acquire(SW);
                                                lock_release(NW);
						lock_acquire(SE);
                                                lock_release(SW);
						
						kprintf("Leaving, Carnumber: %lu, From North to East\n",carnumber);
						lock_release(SE);
						lock_acquire(intersection);
						numcars--;
                                                lock_release(intersection);
					break;
			}			 
			break;
		case 1: // east
			switch (turndirection){
                                case 0: // straight - east to west
                                        kprintf("Approaching, Carnumber: %lu, From East to West\n",carnumber);
        				if (numcars >=2) {
                			lock_acquire(mainlock);
        				} 
					       lock_acquire(NE);
                                                lock_acquire(intersection);
                                                numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From East to West\n",carnumber);
                                                lock_acquire(NW);
                                                lock_release(NE);
                                                
                                                
						kprintf("Leaving, Carnumber: %lu, From East to West\n",carnumber);
						lock_release(NW);
						lock_acquire(intersection);
						numcars--;
                                                lock_release(intersection);
                                        break;
                                case 1: // right - east to north
                                        kprintf("Approaching, Carnumber: %lu, From East to North\n",carnumber);
        				if (numcars >=2) {
                			lock_acquire(mainlock);
        				}
					       lock_acquire(NE);
                                                lock_acquire(intersection);
                                                numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From East to North\n",carnumber);
                                                
						kprintf("Leaving, Carnumber: %lu, From East to North\n",carnumber);
						lock_release(NE);
						lock_acquire(intersection);
						numcars--;
                                                lock_release(intersection);
					break;
                                case 2: // left - east to south
                                        kprintf("Approaching, Carnumber: %lu, From East to South\n",carnumber);
        				if (numcars >=2) {
                			lock_acquire(mainlock);
        				}
				                lock_acquire(NE);
                                                lock_acquire(intersection);
                                                numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From East to South\n",carnumber);
                                                lock_acquire(NW);
                                                lock_release(NE);
                                                lock_acquire(SW);
                                                
						lock_release(NW);
                                                
						kprintf("Leaving, Carnumber: %lu, From East to South\n",carnumber);
						lock_release(SW);
						lock_acquire(intersection);
						numcars--;
                                                lock_release(intersection);
                                        break;
                        }
			break;
		case 2: // south
			switch (turndirection){
                                case 0: // straight - south to north
                                        kprintf("Approaching, Carnumber: %lu, From South to North\n",carnumber);
        				if (numcars >=2) {
                			lock_acquire(mainlock);
        				} 
					      lock_acquire(SE);
                                                lock_acquire(intersection);
                                                numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From South to North\n",carnumber);
                                                lock_acquire(NE);
                                                lock_release(SE);
                                                
						kprintf("Leaving, Carnumber: %lu, From South to North\n",carnumber);
						lock_release(NE);
						lock_acquire(intersection);
                                                numcars--;
                                                lock_release(intersection);
                                case 1: // right - south to east
                                        kprintf("Approaching, Carnumber: %lu, From South to East\n", carnumber);
        				if (numcars >=2) {
                			lock_acquire(mainlock);
					}
					       lock_acquire(SE);
                                                lock_acquire(intersection);
                                                numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From South to East\n",carnumber);
                                                
						
						kprintf("Leaving, Carnumber: %lu, From South to East\n",carnumber);
                                                lock_release(SE);
						lock_acquire(intersection);
						numcars--;
                                                lock_release(intersection);
                                        //}
                                        break;
                                case 2: // left - south to west
                                        kprintf("Approaching, Carnumber: %lu, From South to West\n", carnumber);
        				if (numcars >=2) {
                				lock_acquire(mainlock);
        				} 
					       lock_acquire(SE);
                                                lock_acquire(intersection);
                                                numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From South to West\n",carnumber);
                                                lock_acquire(NE);
                                                lock_release(SE);
                                                lock_acquire(NW);
                                                lock_release(NE);
                                                
						kprintf("Leaving, Carnumber: %lu, From South to West\n",carnumber);
                                                lock_release(NW);
						lock_acquire(intersection);
						numcars--;
                                                lock_release(intersection);
                                        //}
                                        break;
                        } 
			break;
		case 3: // west
			switch (turndirection){
                                case 0: // straight - west to east
                                        kprintf("Approaching, Carnumber: %lu, From West to East\n", carnumber);
        				if (numcars >=2) {
                				lock_acquire(mainlock);
        				} 
					       lock_acquire(SW);
                                                lock_acquire(intersection);
                                                numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From West to East\n",carnumber);
                                                lock_acquire(SE);
                                                lock_release(SW);
                                                
						
						kprintf("Leaving, Carnumber: %lu, From West to East\n",carnumber);
                                                lock_release(SE);
						lock_acquire(intersection);	
						numcars--;
                                                lock_release(intersection);
                                        break;
                                case 1: // right - west to south
                                        kprintf("Approaching, Carnumber: %lu, From West to South\n", carnumber);
        				if (numcars >=2) {
                				lock_acquire(mainlock);
        				} 
					       lock_acquire(SW);
                                                lock_acquire(intersection);
                                                numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From West to South\n",carnumber);
                                                
						kprintf("Leaving, Carnumber: %lu, From West to South\n",carnumber);
                                                lock_release(SW);
						lock_acquire(intersection);
						numcars--;
                                                lock_release(intersection);
                                        break;
                                case 2: // left - west to north
                                        kprintf("Approaching, Carnumber: %lu, From West to North\n", carnumber);
        				if (numcars >=2) {
                				lock_acquire(mainlock);
					}
					       lock_acquire(SW);
                                                lock_acquire(intersection);
                                                numcars++;
						lock_release(intersection);
						kprintf("Entering, Carnumber: %lu, From West to North\n",carnumber);
                                                lock_acquire(SE);
                                                lock_release(SW);
                                                lock_acquire(NE);
                                                lock_release(SE);
                                                
						kprintf("Leaving, Carnumber: %lu, From West to North\n",carnumber);
                                                lock_release(NE);
						lock_acquire(intersection);	
						numcars--;
                                                lock_release(intersection);
                                        break;
                        }
			break;
	}
	// release the mainlock on the intersection if numcars isnt at max capacity
	if (numcars<3) 
	lock_release(mainlock);
}


/*
 * createcars()
 *
 * Arguments:
 *      int nargs: unused.
 *      char ** args: unused.
 *
 * Returns:
 *      0 on success.
 *
 * Notes:
 *      Driver code to start up the approachintersection() threads.  You are
 *      free to modiy this code as necessary for your solution.
 */

int
createcars(int nargs,
           char ** args)
{
        int index, error;

        /*
         * Avoid unused variable warnings.
         */

        (void) nargs;
        (void) args;

	// create locks
	numcars = 0;
	NW = lock_create("NW");
	NE = lock_create("NE");
	SW = lock_create("SW");
	SE = lock_create("SE");
	printmsg = lock_create("printmsg");
	intersection = lock_create("intersection");
	mainlock = lock_create("mainlock");
        /*
         * Start NCARS approachintersection() threads.
         */

        for (index = 0; index < NCARS; index++) {

                error = thread_fork("approachintersection thread",
                                    NULL,
                                    index,
                                    approachintersection,
                                    NULL
                                    );

                /*
                 * panic() on error.
                 */

                if (error) {
                        
                        panic("approachintersection: thread_fork failed: %s\n",
                              strerror(error)
                              );
                }
        }
        return 0;
}

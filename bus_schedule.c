#include "simlib.h"

#define EVENT_ARRIVAL_1 1       /* Event type for person arrival at location 1. */
#define EVENT_ARRIVAL_2 2       /* Event type for person arrival at location 2. */
#define EVENT_ARRIVAL_3 3       /* Event type for person arrival at location 3. */
#define EVENT_BUS_ARRIVAL_1 4   /* Event type for bus arrival at location 1. */
#define EVENT_BUS_ARRIVAL_2 5   /* Event type for bus arrival at location 2. */
#define EVENT_BUS_ARRIVAL_3 6   /* Event type for bus arrival at location 3. */
#define EVENT_BUS_DEPARTURE_1 7 /* Event type for bus departure at location 1. */
#define EVENT_BUS_DEPARTURE_2 8 /* Event type for bus departure at location 2. */
#define EVENT_BUS_DEPARTURE_3 9 /* Event type for bus departure at location 3. */
#define EVENT_UNLOADING_1 10    /* Event type for unloading at location 1. */
#define EVENT_UNLOADING_2 11    /* Event type for unloading at location 2. */
#define EVENT_UNLOADING_3 12    /* Event type for unloading at location 3. */
#define EVENT_LOADING_1 13      /* Event type for loading at location 1. */
#define EVENT_LOADING_2 14      /* Event type for loading at location 2. */
#define EVENT_LOADING_3 15      /* Event type for loading at location 3. */
#define EVENT_END 16

#define LIST_QUEUE_1 1            /* List number for queue at location 1. */
#define LIST_QUEUE_2 2            /* List number for queue at location 2. */
#define LIST_QUEUE_3 3            /* List number for queue at location 3. */
#define LIST_QUEUE_BUS_ARRIVE_1 4 /* List number for server. */
#define LIST_QUEUE_BUS_ARRIVE_2 5
#define LIST_QUEUE_BUS_ARRIVE_3 6

#define SAMPST_AVERAGE 1
#define SAMPST_NUMBER 2
#define SAMPST_MAXIMUM 3
#define SAMPST_MINIMUM 4

#define TIMEST_AVERAGE 1
#define TIMEST_MAXIMUM 2
#define TIMEST_MINIMUM 3

#define SAMPST_DELAYS_1 1 /* sampst variable for delays in queue at location 1. */
#define SAMPST_DELAYS_2 2 /* sampst variable for delays in queue at location 2. */
#define SAMPST_DELAYS_3 3 /* sampst variable for delays in queue at location 3. */
#define SAMPST_SERVER_1 4 /* sampst variable for server delays at location 1. */
#define SAMPST_SERVER_2 5 /* sampst variable for server delays at location 2. */
#define SAMPST_SERVER_3 6 /* sampst variable for server delays at location 3. */
#define SAMPST_LOOP_1 7   /* sampst variable for loop delays at location 1. */
#define SAMPST_LOOP_2 8   /* sampst variable for loop delays at location 2. */
#define SAMPST_LOOP_3 9   /* sampst variable for loop delays at location 3. */
#define SAMPST_PERSON 10  /* sampst variable for person delay. */

#define STREAM_INTERARRIVAL_1 1 /* Random-number stream for interarrivals at location 1. */
#define STREAM_INTERARRIVAL_2 2 /* Random-number stream for interarrivals at location 2. */
#define STREAM_INTERARRIVAL_3 3 /* Random-number stream for interarrivals at location 3. */
#define STREAM_UNLOADING 4      /* Random-number stream for unloading. */
#define STREAM_LOADING 5        /* Random-number stream for loading. */
#define STREAM_DESTINATION 6    /* Random-number stream for destination */

#define MAX_QUEUE 10000

// Problem statement
//
// Consider a car-rental system shown in Fig. 2.72, with all distances given in miles.
// People arrive at location i (where i 5 1, 2, 3) with independent exponential interarrival times at respective rates of 14, 10, and 24 per hour. Each location has a FIFO queue
// with unlimited capacity. There is one bus with a capacity of 20 people and a speed of
// 30 miles per hour. The bus is initially at location 3 (car rental), and leaves immediately
// in a counterclockwise direction. All people arriving at a terminal want to go to the
// car rental. All people arriving at the car rental want to go to terminals 1 and 2 with
// respective probabilities 0.583 and 0.417. When a bus arrives at a location, the following rules apply:
// • People are fi rst unloaded in a FIFO manner. The time to unload one person is distributed uniformly between 16 and 24 seconds.
// • People are then loaded on the bus up to its capacity, with a loading time per person
// that is distributed uniformly between 15 and 25 seconds.
// • The bus always spends at least 5 minutes at each location. If no loading or unloading
// is in process after 5 minutes, the bus will leave immediately.
//  Run a simulation for 80 hours and gather statistics on:
// (a) Average and maximum number in each queue
// (b)
// Average and maximum delay in each queue(c) Average and maximum number on the bus
// (d) Average, maximum, and minimum time the bus is stopped at each location
// (e) Average, maximum, and minimum time for the bus to make a loop (departure
// from the car rental to the next such departure)
// ( f ) Average, maximum, and minimum time a person is in the system by arrival
// location
//  Use the following random-number stream assignments:
//  i, interarrival times at location i (where i 5 1, 2, 3)
//  4, unloading times
//  5, loading times
//  6, determining destination of an arrival at the car rental
/* Declare non-simlib global variables. */
FILE *outfile;

/* Declare Constant Variables */
const float AVG_ARRIVAL_TIME_STATION_1 = 1.0 / (14.0 / 60); // 1/avg per minute
const float AVG_ARRIVAL_TIME_STATION_2 = 1.0 / (10.0 / 60); // 1/avg per minute
const float AVG_ARRIVAL_TIME_STATION_3 = 1.0 / (24.0 / 60); // 1/avg per minute
const int MAX_CAPACITY = 20;
const double MAX_TIME = 80.0 * 60;   // in minutes
const double SPEED_PER_MINUTE = 0.5; // 30 miles per hour
const double DISTANCE_31 = 4.5;      // miles
const double DISTANCE_12 = 1;        // miles
const double DISTANCE_23 = 4.5;      // miles

const double MIN_OFFBOARD = 16 / 60.0; // 16 seconds
const double MAX_OFFBOARD = 24 / 60.0; // 24 seconds

const double MIN_ONBOARD = 15 / 60.0; // 15 seconds
const double MAX_ONBOARD = 25 / 60.0; // 25 seconds
const double BUS_STAY = 5;          // 5 minutes

/** Global variable*/
int waiting_duration = 0;

// Function declaration
void init_model();
void report();
void sched_arrival(int);
void bus_arrival(int);
void unload(int);
void load(int);
void depart(int);
int get_bus_size();
int get_destination(int);

int main()
{
    outfile = fopen("bus-sim.out", "w");
    init_simlib();
    maxatr = 10;

    /* Initialize the model. */
    init_model();

    do
    {
        /* Determine the next event. */
        timing();

        /* Invoke the appropriate event function. */
        switch (next_event_type)
        {
        case EVENT_ARRIVAL_1:
            sched_arrival(1);
            break;
        case EVENT_ARRIVAL_2:
            sched_arrival(2);
            break;
        case EVENT_ARRIVAL_3:
            sched_arrival(3);
            break;
        case EVENT_BUS_ARRIVAL_1:
            bus_arrival(1);
            break;
        case EVENT_BUS_ARRIVAL_2:
            bus_arrival(2);
            break;
        case EVENT_BUS_ARRIVAL_3:
            bus_arrival(3);
            break;
        case EVENT_UNLOADING_1:
            unload(1);
            break;
        case EVENT_UNLOADING_2:
            unload(2);
            break;
        case EVENT_UNLOADING_3:
            unload(3);
            break;
        case EVENT_LOADING_1:
            load(1);
            break;
        case EVENT_LOADING_2:
            load(2);
            break;
        case EVENT_LOADING_3:
            load(3);
            break;
        case EVENT_BUS_DEPARTURE_1:
            depart(1);
            break;
        case EVENT_BUS_DEPARTURE_2:
            depart(2);
            break;
        case EVENT_BUS_DEPARTURE_3:
            depart(3);
            break;
        }
        /* If the event just executed was not the end-simulation event (type
           EVENT_END_SIMULATION), continue simulating.  Otherwise, end the
           simulation. */
    } while (next_event_type != EVENT_END);

    /* Invoke the report generator and end the simulation. */
    report();
    fclose(outfile);
    return 0;
}

int get_bus_size() {
    return list_size[LIST_QUEUE_BUS_ARRIVE_1] + list_size[LIST_QUEUE_BUS_ARRIVE_2] + list_size[LIST_QUEUE_BUS_ARRIVE_3];
}
void depart(int station) {
    // 3 -> 1 -> 2 -> 3
    printf("[%.1f]     Bus departing from station %d, bus occupancy: %d \n", sim_time, station, get_bus_size());
    switch(station) {
        case 3:
            event_schedule(sim_time + DISTANCE_31 / SPEED_PER_MINUTE, EVENT_BUS_ARRIVAL_1);
            break;
        case 2:
            event_schedule(sim_time + DISTANCE_23 / SPEED_PER_MINUTE, EVENT_BUS_ARRIVAL_3);
            break;
        case 1:
            event_schedule(sim_time + DISTANCE_12 / SPEED_PER_MINUTE, EVENT_BUS_ARRIVAL_2);
            break;
    }
}

void load(int station) {
    int current_station_queue = LIST_QUEUE_1 + (station - 1);
    int event_load = EVENT_LOADING_1 + (station - 1);

    // if current station queue not empty, add to bus
    if (list_size[current_station_queue] != 0 && get_bus_size() < MAX_CAPACITY) {
        list_remove(FIRST, current_station_queue);

        // Get the timing and destination of current person
        double person_arrival_time = transfer[1];
        int person_destination = transfer[2];

        int target_station_queue = LIST_QUEUE_BUS_ARRIVE_1 + (person_destination - 1);
        list_file(LAST, target_station_queue);

        // Reset waiting variable
        waiting_duration = 0;

        // Schedule next loading
        event_schedule(sim_time + uniform(MIN_ONBOARD, MAX_ONBOARD, STREAM_LOADING), event_load);
    } else {
        printf("[%.1f] Bus still waiting at minute {%d}, current capacity: %d\n", sim_time, waiting_duration, get_bus_size());
        
        // Leave if already wait for too long or already full
        if (waiting_duration >= BUS_STAY || get_bus_size() == MAX_CAPACITY) {
            printf("[%.1f] Bus finished loading, current capacity: %d\n", sim_time, get_bus_size());
            event_schedule(sim_time, EVENT_BUS_DEPARTURE_1 + (station - 1));
            // Reset waiting to 0
            waiting_duration = 0;
        }
        else
        {
            // Requeue loading for the next minute
            waiting_duration += 1;
            event_schedule(sim_time + 1, event_load);
        }
    }   
}

void unload(int station)
{

    int customer_destination_queue = LIST_QUEUE_BUS_ARRIVE_1 + (station - 1);
    int event_unload = EVENT_UNLOADING_1 + (station - 1);
    // Pop the first from queue
    // List must be > 0
    if (list_size[customer_destination_queue] == 0)
    {
        printf("ERROR while unloading! Queue already empty");
        exit(1);
    }
    else
    {
        list_remove(FIRST, customer_destination_queue);
        // Queue next person to remove
        bus_arrival(station);
    }
}

void bus_arrival(int station)
{
    // Can be called from main, or from unload function.

    // If queue bus not empty, schedule offloading
    int customer_destination_queue = LIST_QUEUE_BUS_ARRIVE_1 + (station - 1);
    int event_unload = EVENT_UNLOADING_1 + (station - 1);
    int event_loading = EVENT_LOADING_1 + (station - 1);

    if (list_size[customer_destination_queue] != 0)
    {
        event_schedule(sim_time + uniform(MIN_OFFBOARD, MAX_OFFBOARD, STREAM_UNLOADING), event_unload);
    } else {
        printf("[%.1f] Bus finished offloading, current capacity: %d\n", sim_time, get_bus_size());
        // Onboard now
        event_schedule(sim_time + uniform(MIN_ONBOARD, MAX_ONBOARD, STREAM_LOADING), event_loading);
    }
}
void sched_arrival(int station)
{

    // 1. Put current person in queue
    // 2. Schedule next arrival

    // Person just arrived at queue, station, queue with attributes:
    // 1. Time of arrival
    // 2. Destination
    transfer[1] = sim_time;
    transfer[2] = get_destination(station);

    switch (station)
    {
    case 1:
        list_file(LAST, LIST_QUEUE_1);
        // printf("New person arrived at station 1\n");
        sampst(1, 1);
        event_schedule(sim_time + expon(AVG_ARRIVAL_TIME_STATION_1, STREAM_INTERARRIVAL_1), EVENT_ARRIVAL_1);
        break;
    case 2:
        list_file(LAST, LIST_QUEUE_2);
        // printf("New person arrived at station 2\n");
        sampst(1, 2);
        event_schedule(sim_time + expon(AVG_ARRIVAL_TIME_STATION_2, STREAM_INTERARRIVAL_2), EVENT_ARRIVAL_2);
        break;
    case 3:
        list_file(LAST, LIST_QUEUE_3);
        sampst(1, 3);
        // printf("New person arrived at station 3\n");
        event_schedule(sim_time + expon(AVG_ARRIVAL_TIME_STATION_3, STREAM_INTERARRIVAL_3), EVENT_ARRIVAL_3);
        break;
    }
}

int get_destination(int current_station)
{
    if (current_station == 3)
    {
        return (uniform(0, 1, STREAM_DESTINATION) < 0.583) ? 1 : 2;
    }
    return 3;
}

void init_model()
{

    // Schedule end of simulation
    event_schedule(MAX_TIME, EVENT_END);
    // Schedule station 1 arrival
    event_schedule(expon(AVG_ARRIVAL_TIME_STATION_1, STREAM_INTERARRIVAL_1), EVENT_ARRIVAL_1);
    // Schedule station 2 arrival
    event_schedule(expon(AVG_ARRIVAL_TIME_STATION_2, STREAM_INTERARRIVAL_2), EVENT_ARRIVAL_2);
    // Schedule station 3 arrival
    event_schedule(expon(AVG_ARRIVAL_TIME_STATION_3, STREAM_INTERARRIVAL_3), EVENT_ARRIVAL_3);

    // Schedule bus departure at station 3
    event_schedule(0, EVENT_BUS_DEPARTURE_3);
}

void report()
{
    // TODO: LEGIT REPORT
    fprintf(outfile, "\n\n\n\nReport for 80 hour simulation\n\n");
    sampst(0, -1);
    printf("\nNumber of person on station 1: %14.2lf", transfer[SAMPST_NUMBER]);
    sampst(0, -2);
    printf("\nNumber of person on station 2: %14.2lf", transfer[SAMPST_NUMBER]);
    sampst(0, -3);
    printf("\nNumber of person on station 3: %14.2lf", transfer[SAMPST_NUMBER]);
    // Number 1
    fprintf(outfile, "\n\n\n\nStatistics 1: Average and maximum number in each queue\n\n");
    // Number 2
    fprintf(outfile, "\n\n\n\nStatistics 2: Average and maximum delay in each queue\n\n");
    // Number 3
    fprintf(outfile, "\n\n\n\nStatistics 3: Average and maximum number on the bus\n\n");
    // Number 4
    fprintf(outfile, "\n\n\n\nStatistics 4: Average, maximum, and minimum time the bus is stopped at each location\n\n");
    // Number 5
    fprintf(outfile, "\n\n\n\nStatistics 5: Average, maximum, and minimum time for the bus to make a loop\n\n");
    // Number 6
    fprintf(outfile, "\n\n\n\nStatistics 6: Average, maximum, and minimum time a person is in the system by arrival location\n\n");
    fprintf(outfile, "\n\n\n\nEnd of report\n\n");
}
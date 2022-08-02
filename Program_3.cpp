/***************************************************************************************************************************
File: Program_3.cpp
Author: Kento Maeda
Date:   November 5-19, 2021

A Monte Carlo simualation of 3 virtual memory replacement algorithms.

Procedures:
LRU         A procedure that simulates virtual memory replacement with the Least Recently Used policy.
            Returns page faults as an integer.

FIFO        A procedure that simulates virtual memory replacement with the First In First Out policy.
            Returns page faults as an integer.

Clock       A procedure that simulates virtual memory replacement with the Clock policy.
            Returns page faults as an integer.

main        A driver procedure that randomly generates 1000 page traces to be experimented on by the 3 algorithms.
            Also outputs the results of the 3 virtual memory replacement algorithms.

*****************************************************************************************************************************/

// Program follows suggested structure for Pgm3

#include <boost/random.hpp>
#include <boost/math/distributions/normal.hpp>

#include <iostream>
#include <bits/stdc++.h>

using namespace std;

/************************************************************************************************************
int LRU(int wss, int data[])
Author: Kento Maeda
Date: November 5-19, 2021

LRU             A procedure that simulates virtual memory replacement with the Last Recently Used policy.
                Returns page faults as an integer.

Parameters:
int wss         The working set size.
int data[]      The pages.
*************************************************************************************************************/


int LRU(int wss, int data[])
{
    // Declare faults to be returned

    int faults = 0;

    // Working set array of integers

    int workingSet[wss];

    // Keep track of the amount of time it has been
    // since the page was last referenced.

    int time[wss];

    // Declare a set

    unordered_set <int> s;

    // Index for the first working spaces.

    int initiationIndex = 0;

    // Initialize arrays.

    for (int i = 0; i < wss; i++)
    {
        workingSet[i] = 0;
        time[i] = 0;
    }

    for (int i = 0; i < 1000; i++)
    {
        // Denote passage of time

        for (int j = 0; j < wss; j++)
        {
            time[j]++;
        }

        // If the set size is less than the working set size
        // and the current page address in the stream is not in the working set...
        // (This statement will not be called once the working set reaches capacity)

        if ((int) s.size() < wss && s.find(data[i]) == s.end())
        {
            s.insert(data[i]); // Insert the current page address into the working space
            workingSet[initiationIndex] = data[i];
            time[initiationIndex] = 0;
            initiationIndex++;
        }

        // If the set size has reached capacity
        // and the current page address in the stream produces a fault...

        else if (s.find(data[i]) == s.end())
        {
            int max = 0;

            // Find the least recently used address

            for (int j = 0; j < wss; j++)
            {
                if (time[j] > max)
                {
                    max = time[j];
                }
            }

            // Eliminiate the least recently used address

            for (int j = 0; j < wss; j++)
            {
                if (time[j] == max)
                {
                    s.erase(workingSet[j]);
                    s.insert(data[i]);
                    workingSet[j] = data[i];
                    time[j] = 0;
                    faults++;
                }
            }
        }

        // If the address is identical to one already in the working space,
        // reset the timer.

        else
        {
            for (int j = 0; j < wss; j++)
            {
                if (workingSet[j] == data[i])
                {
                    time[j] = 0;
                }
            }
        }
    }

    return faults;
}

/************************************************************************************************************
int FIFO(int wss, int data[])
Author: Kento Maeda
Date: November 5-19, 2021
FIFO            A procedure that simulates virtual memory replacement with the First In First Out policy.
                Returns page faults as an integer.

Parameters:
int wss         The working set size.
int data[]      The pages.
*************************************************************************************************************/

int FIFO(int wss, int data[])
{
    int faults = 0; // Declare faults to be returned
    unordered_set <int> s; // Declare the working set
    queue <int> q; // Declare the FIFO queue

    // Repeat for all 1000 elements in the data array

    for (int i = 0; i < 1000; i++)
    {
        // If the set size is less than the working set size
        // and the current page address in the stream is not in the working set...
        // (This statement will not be called once the working set reaches capacity)

        if ((int) s.size() < wss && s.find(data[i]) == s.end())
        {
            s.insert(data[i]);  // Insert the current page address into the working space
            q.push(data[i]);    // Insert the current page address into the FIFO queue
        }

        // If the set size has reached capacity
        // and the current page address in the stream produces a fault...

        else if (s.find(data[i]) == s.end())
        {
            s.erase(q.front()); // Erase the first number in the FIFO queue
            q.pop(); // Pop the first number in the FIFO queue
            s.insert(data[i]); // Insert the current data address into the working set
            q.push(data[i]); // Insert the current data address into the FIFO queue
            faults++; // Increment a page fault
        }
    }

    return faults; // Return page faults
}

/************************************************************************************************************
int LRU(int wss, int data[])
Author: Kento Maeda
Date: November 5-19, 2021

Clock           A procedure that simulates virtual memory replacement with the Clock policy.
                Returns page faults as an integer.

Parameters:
int wss         The working set size.
int data[]      The pages.
*************************************************************************************************************/
int Clock(int wss, int data[])
{
    // Return page faults.

    int faults = 0;

    // Pointer to be used.

    int framePointer = 0;

    // Working set of integers

    unordered_set <int> s;

    // Array of use bits

    bool useBit[wss];

    // Array copy of the working set

    int workingSet[wss];

    // Initialize the arrays

    for (int i = 0; i < wss; i++)
    {
        useBit[i] = 0;
        workingSet[i] = 0;
    }

    for (int i = 0; i < 1000; i++)
    {
        // Wrap the frame pointer if it overflows.

        if (framePointer >= wss)
        {
            framePointer = 0;
        }

        // Initialize the working set

        if ((int) s.size() < wss && s.find(data[i]) == s.end())
        {
            s.insert(data[i]);
            useBit[framePointer] = 1;
            workingSet[framePointer] = data[i];
            framePointer++;
        }

        // If the page number is found within the working set, set the use bit to 1.
        // This does not increment the frame pointer.

        else if (!(s.find(data[i]) == s.end()))
        {
            for (int j = 0; j < wss; j++)
            {
                if (workingSet[j] == data[i])
                {
                    useBit[j] = 1;
                }
            }
        }

        // If the page is not found within the working set, keep incrementing the frame pointer
        // until a frame with use bit 0 is found.

        else if (s.find(data[i]) == s.end())
        {
            bool missionComplete = 0;

            while (!missionComplete)
            {
                // Wrap the frame pointer if it overflows.

                if (framePointer >= wss)
                {
                    framePointer = 0;
                }

                // If a use bit is found at 0 at the current frame pointer, immediately replace it.

                if (useBit[framePointer] == 0)
                {
                    s.erase(workingSet[framePointer]);
                    s.insert(data[i]);
                    useBit[framePointer] = 1;
                    workingSet[framePointer] = data[i];
                    missionComplete = 1;
                    faults++;
                }

                // If the use bit at the current frame pointer is 1, set it to 1
                // and advance the frame pointer.

                else if (useBit[framePointer] == 1)
                {
                    useBit[framePointer] = 0;
                }

                framePointer++;

            }
        }
    }

    return faults;
}


int main(int argc, char *argv[])
{
    // Declare integer arrays to be used in the program

    int data[1000];
    int LRUResults[20];
    int FIFOResults[20];
    int ClockResults[21];

    // Seed the RNG with a normal distribution

    boost::mt19937 rng;
    boost::normal_distribution<> nd(10, 2);
    boost::variate_generator<boost::mt19937&, boost::normal_distribution<> > var_nor(rng, nd);

    // Experiments loop

    for (int i = 0; i < 1000; i++)
    {
        // Trace Loop

        for (int j = 0; j < 1000; j++)
        {
            // Generate a random number from a normal distribution
            // with a mean of ten and a standard deviation of two.
            // There are ten regions which have their own base address.

            int norm = var_nor();

            data[j] = (10 * ((int) (j / 100)) + norm);
        }

        for (int wss = 4; wss <= 20; wss++)
        {
            // Determine and accumulate the number of page
            // faults for each algorithm base on the current
            // working set size and the current trace.

            LRUResults[wss] += LRU(wss, data);
            FIFOResults[wss] += FIFO(wss, data);
            ClockResults[wss] += Clock(wss, data);
        }
    }

    for (int wss = 4; wss <= 20; wss++)
    {
        // Output statistics

        cout << "LRU Result wss " << wss << ": " << LRUResults[wss] / 1000 << endl; // Output LRUResults[wss]

        cout << "FIFO Result wss " << wss << ": " << FIFOResults[wss] / 1000 << endl; // Output FIFOResults[wss]

        cout << "Clock Result wss " << wss << ": " << ClockResults[wss] / 1000 << endl; // Output ClockResults[wss];

    }

    return 0;
}

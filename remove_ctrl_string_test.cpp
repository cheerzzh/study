/*
 *  gettime - get time via clock_gettime
 *      N.B.: OS X does not support clock_gettime
 *
 *          Paul Krzyzanowski
 *          */

#include <stdio.h>  /* for printf */
#include <stdint.h> /* for uint64 definition */
#include <stdlib.h> /* for exit() definition */
#include <time.h>   /* for clock_gettime */
#include <unistd.h>
#include <string>

#define BILLION 1000000000L

int localpid(void) {
        static int a[9] = { 0 };
            return a[0];
}

void remove_ctrl_cstrings(char* destp,char const* srcp, size_t size)
{
    for(size_t i = 0;i<size;++i)
    {
        if(srcp[i]>=0x20)
        {
            *destp++=srcp[i];
        }
    }
    *destp = 0;
}

std::string remove_ctrl(std::string s)
{
    std::string result;
    for(int i=0; i<s.length();i++)
    {
        if(s[i]>=0x20)
        {
            result = result + s[i];
        }
    }
    return result;
}


std::string remove_ctrl_mutating(std::string s)
{
    std::string result;
    for(int i=0; i<s.length();i++)
    {
        if(s[i]>=0x20)
        {
            result += s[i];
        }
    }
    return result;
}

std::string remove_ctrl_reserve(std::string s)
{
    std::string result;
    result.reserve(s.length());
    for(int i=0; i<s.length();i++)
    {
        if(s[i]>=0x20)
        {
            result += s[i];
        }
    }
    return result;
}

std::string remove_ctrl_ref_args(std::string const& s)
{
    std::string result;
    result.reserve(s.length());
    for(int i=0; i<s.length();i++)
    {
        if(s[i]>=0x20)
        {
            result += s[i];
        }
    }
    return result;
}

std::string remove_ctrl_ref_args_it(std::string const& s)
{
    std::string result;
    result.reserve(s.length());
    //for(int i=0; i<s.length();i++)
    for(std::string::const_iterator it=s.begin();it<s.end();it++)
    {
        if(*it>=0x20)
        {
            result += *it;
        }
    }
    return result;
}

main(int argc, char **argv)
{
    uint64_t diff;
    struct timespec start, end;
    int i;
    
    int sumDiff = 0;
    int nTime = 100000;
    std::string s("1 2 3 4  5 d  df df d f df e rwe g d vf g g er g fg f gerg rg erg df g d fg r g");
    /* measure monotonic time */
    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    //sleep(1);   /* do stuff */
    for(int i=0; i<nTime;i++)
    {
        std::string result = remove_ctrl(s);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);   /* mark the end time */

    sumDiff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    diff = sumDiff / nTime;
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);

    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    //sleep(1);   /* do stuff */
    for(int i=0; i<nTime;i++)
    {
        std::string result = remove_ctrl_mutating(s);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);   /* mark the end time */

    sumDiff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    diff = sumDiff / nTime;
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);

    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    //sleep(1);   /* do stuff */
    for(int i=0; i<nTime;i++)
    {
        std::string result = remove_ctrl_reserve(s);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);   /* mark the end time */

    sumDiff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    diff = sumDiff / nTime;
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);

    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    //sleep(1);   /* do stuff */
    for(int i=0; i<nTime;i++)
    {
        std::string result = remove_ctrl_ref_args(s);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);   /* mark the end time */

    sumDiff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    diff = sumDiff / nTime;
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);

    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    //sleep(1);   /* do stuff */
    for(int i=0; i<nTime;i++)
    {
        std::string result = remove_ctrl_ref_args_it(s);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);   /* mark the end time */

    sumDiff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    diff = sumDiff / nTime;
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);
    
    // initialize char *
    char* result = (char*)malloc(sizeof(char)*(s.length()+1));
    clock_gettime(CLOCK_MONOTONIC, &start); /* mark start time */
    //sleep(1);   /* do stuff */
    for(int i=0; i<nTime;i++)
    {
        remove_ctrl_cstrings(result,s.c_str(),s.length());
    }
    clock_gettime(CLOCK_MONOTONIC, &end);   /* mark the end time */

    sumDiff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    diff = sumDiff / nTime;
    printf("elapsed time = %llu nanoseconds\n", (long long unsigned int) diff);
    printf("result : %s\n",result);
    /* now re-do this and measure CPU time */
    /* the time spent sleeping will not count (but there is a bit of overhead */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);    /* mark start time */
    sleep(1);   /* do stuff */
    clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);      /* mark the end time */

    diff = BILLION * (end.tv_sec - start.tv_sec) + end.tv_nsec - start.tv_nsec;
    printf("elapsed process CPU time = %llu nanoseconds\n", (long long unsigned int) diff);

    exit(0);
}


// Implements a spell-checker

// The <sys/time.h> header defines the timeval structure that includes at least the following members:
// time_t         tv_sec      seconds
// suseconds_t    tv_usec     microseconds
// https://blog.csdn.net/xiaorenwuzyh/article/details/22645987

#include <ctype.h>
#include <stdio.h>
#include <sys/resource.h>
#include <sys/time.h>

#include "dictionary.h"

// Undefine any definitions
#undef calculate
#undef getrusage

// Default dictionary
#define DICTIONARY "dictionaries/large"

// Prototype; Returns number of seconds between b and a. 資源用量rusage結構體的形式返回到usage指標指向的空間:
double calculate(const struct rusage* b, const struct rusage* a);

int main(int argc, char *argv[])
{
    // Check for correct number of args
    if (argc != 2 && argc != 3)
    {
        printf("Usage: speller [dictionary] text\n");
        return 1;
    }

    // Structures for timing data
    struct rusage before, after;

    // Benchmarks
    double time_load = 0.0, time_check = 0.0, time_size = 0.0, time_unload = 0.0;

    // Determine dictionary to use
    char* dictionary = (argc == 3) ? argv[1] : DICTIONARY; // if dictionary = (argc == 3) return argv[1] (user define); else return DICTIONARY(default: "dictionaries/large")

    // Load dictionary
    // int getrusage(int who, struct rusage* usage);
    // who:
    // RUSAGE_SELF：獲取當前進程的資源使用信息。以當前進程的相關信息來填充rusage(數據)結構;  RUSAGE_CHILDREN：獲取子進程的資源使用信息。rusage結構中的數據都將是當前進程的子進程的信息
    // usage: 指向存放資源使用信息的結構指針。
    getrusage(RUSAGE_SELF, &before); //from <sys/resource.h>, <sys/time.h>
    bool loaded = load(dictionary);
    getrusage(RUSAGE_SELF, &after);

    // Exit if dictionary not loaded
    if (!loaded)
    {
        printf("Could not load %s.\n", dictionary);
        return 1;
    }

    // Calculate time to load dictionary
    time_load = calculate(&before, &after); //20201209

    // Try to open text
    char* text = (argc == 3) ? argv[2] : argv[1];
    FILE* file = fopen(text, "r");
    if (file == NULL)
    {
        printf("Could not open %s.\n", text);
        unload();
        return 1;
    }

    // Prepare to report misspellings
    printf("\nMISSPELLED WORDS\n\n");

    // Prepare to spell-check
    int index = 0, misspellings = 0, words = 0;
    char word[LENGTH + 1]; //LENGTH is defined as 45 in dictionary.h   表示字的長度最多就是45

    // Spell-check each word in text
    for (int c = fgetc(file); c != EOF; c = fgetc(file))  // int fgetc(FILE *stream) 得到下一個字符（unsigned char類型）
    // 傳回值如果是 EOF，可能是有錯誤發生或是檔案終止(end-of-file)，所以應該利用 feof 或是 ferror 來判定錯誤發生或是檔案終止。
    {
        // Allow only alphabetical characters and apostrophes
        if (isalpha(c) || (c == '\'' && index > 0)) // index > 0 確保 ' 不會出現在字的開頭
        {
            // Append character to word
            word[index] = c;
            index++;

            // Ignore alphabetical strings too long to be words
            if (index > LENGTH)
            {
                // Consume remainder of alphabetical string
                while ((c = fgetc(file)) != EOF && isalpha(c));  //while(...)讓fgetc指針繼續直到EOF或非字母才結束

                // Prepare for new word
                index = 0;
            }
        }

        // Ignore words with numbers (like MS Word can)
        else if (isdigit(c))
        {
            // Consume remainder of alphanumeric string
            while ((c = fgetc(file)) != EOF && isalnum(c)); //while(...)讓fgetc指針繼續直到EOF或非字母/數字才結束

            // Prepare for new word
            index = 0;
        }

        // We must have found a whole word
        else if (index > 0) //不是字母, ', 或數字且index > 0 的情況 --> 找到完整的字了
        {
            // Terminate current word
            word[index] = '\0';  // 加上string 的結束符. 用單引號包起來，再加轉義，實際上就是0，只不過它表示的是字符。

            // Update counter
            words++;

            // Check word's spelling
            getrusage(RUSAGE_SELF, &before);
            bool misspelled = !check(word); //check(word) return true if it's not misspelled
            getrusage(RUSAGE_SELF, &after);

            // Update benchmark
            time_check += calculate(&before, &after);

            // Print word if misspelled
            if (misspelled)
            {
                printf("%s\n", word);
                misspellings++;
            }

            // Prepare for next word
            index = 0;
        }
    }

    // Check whether there was an error
    if (ferror(file))  //ferror()用来检测文件流是否发生了错误 after c gets EOF
    {
        fclose(file);
        printf("Error reading %s.\n", text);
        unload();
        return 1;
    }

    // Close text
    fclose(file);

    // Determine dictionary's size
    getrusage(RUSAGE_SELF, &before);
    unsigned int n = size();  // Returns number of words in dictionary if loaded else 0 if not yet loaded
    getrusage(RUSAGE_SELF, &after);

    // Calculate time to determine dictionary's size
    time_size = calculate(&before, &after);

    // Unload dictionary
    getrusage(RUSAGE_SELF, &before);
    bool unloaded = unload();
    getrusage(RUSAGE_SELF, &after);

    // Abort if dictionary not unloaded
    if (!unloaded)
    {
        printf("Could not unload %s.\n", dictionary);
        return 1;
    }

    // Calculate time to unload dictionary
    time_unload = calculate(&before, &after);

    // Report benchmarks
    printf("\nWORDS MISSPELLED:     %d\n", misspellings);
    printf("WORDS IN DICTIONARY:  %d\n", n);
    printf("WORDS IN TEXT:        %d\n", words);
    printf("TIME IN load:         %.2f\n", time_load);
    printf("TIME IN check:        %.2f\n", time_check);
    printf("TIME IN size:         %.2f\n", time_size);
    printf("TIME IN unload:       %.2f\n", time_unload);
    printf("TIME IN TOTAL:        %.2f\n\n",
           time_load + time_check + time_size + time_unload);

    // Success
    return 0;
}

// Returns number of seconds between b and a
// 其中tv_sec是由凌晨開始算起的秒數，tv_usec則是微秒(10E-6 second)。秒跟微秒部份相加才是完整時間
double calculate(const struct rusage* b, const struct rusage* a)
{
    if (b == NULL || a == NULL)
    {
        return 0.0;
    }
    else
    {
        return ((((a->ru_utime.tv_sec * 1000000 + a->ru_utime.tv_usec) -
                  (b->ru_utime.tv_sec * 1000000 + b->ru_utime.tv_usec)) +
                 ((a->ru_stime.tv_sec * 1000000 + a->ru_stime.tv_usec) -
                  (b->ru_stime.tv_sec * 1000000 + b->ru_stime.tv_usec)))
                / 1000000.0);
    }
}


/*

struct timeval
{
__time_t tv_sec;        //econds.
__suseconds_t tv_usec;  //Microseconds.
};


// timeval 轉 double
double timeval_to_double(struct timeval * tv)
{
    double d;
    d = tv->tv_sec + tv->tv_usec / 1000000;
    return d;
}


struct rusage {
        struct timeval ru_utime; // user time used --> 回傳timeval struct
        struct timeval ru_stime; // system time used --> 系統時間是CPU花費在系統調用上的上執行內核指令的時間。
        long ru_maxrss; // maximum resident set size
        long ru_ixrss; // integral shared memory size
        long ru_idrss; // integral unshared data size
        long ru_isrss; // integral unshared stack size
        long ru_minflt; // page reclaims
        long ru_majflt; // page faults
        long ru_nswap;// swaps
        long ru_inblock; // block input operations
        long ru_oublock; // block output operations
        long ru_msgsnd; // messages sent
        long ru_msgrcv; //messages received
        long ru_nsignals; // signals received
        long ru_nvcsw; // voluntary context switches
        long ru_nivcsw; // involuntary context switches
};



得到CPU total 使用时间的用法：

struct rusage rup;
getrusage(RUSAGE_SELF, &rup);
long sec = rup.ru_utime.tv_sec + rup.ru_stime.tv_sec;
long usec = rup.ru_utime.tv_usec + rup.ru_stime.tv_usec;
sec += usec/1000000;
usec %= 1000000;

*/


// sleep for 3 seconds (non-busy)
/* sleep是不占用系统时间的
sleep(3);
$data = getrusage();
echo “User time: “.
($data['ru_utime.tv_sec'] +
$data['ru_utime.tv_usec'] / 1000000);
echo “System time: “.
($data['ru_stime.tv_sec'] +
$data['ru_stime.tv_usec'] / 1000000);
*/
/* 输出
User time: 0.011552
System time: 0
*/
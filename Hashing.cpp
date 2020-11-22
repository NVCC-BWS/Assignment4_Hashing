// Assignment 4 - Hashing. CODE.
// ITP - 132, by Bryce Summers
// Version 1.0, last modified on 11.22.2020

// STUDENT: Implement everything above the main function in this file.
//          Look at WeatherRecord and Dictionary Record classes in
//          the Hashing.h file.

/*
 * Instructions:
 *
 *  1. Implement All of these functions, starting with hash1
 *     below this block comment. hash1 and equal1 are already solved for you.
 *
 *  2. Try to meet or beat the official performance time benchmarks.
 *     Continue to tweak your hash implementations until you are
 *     satisfied with your code's performance.
 *
 *     If your code performs very slowly, you will receive a
 *     TIME OUT error and testing will give up.
 *
 *  3. Your Equal and hash functions must be consistent. The log
 *     should not have any correctness errors.
 *
 *     (Aim for less than 200 milliseconds for every problem.)
 *
 *     Read benchmark_1_log.txt, benchmark_2_log.txt, etc. to see
 *     how fast the REFERENCE SOLUTION worked on my computer.
 *
 *     I've also included a benchmark for the starter code on my computer.
 *     See: benchmark_starter_code_log_1.txt
 *
 *  Reminder: If two objects are equal,
 *            there hash values should also be equal.
 *
 *  Hint: Try to evenly distribute all of the objects in each data set
 *        across various sized tables.
 *  Hint: Try something and see if it works, then reflect on why / why not.
 *  Hint: take a look at the data sets being tested!
 *  Hint: Look at the Hash Set Implementations in the .h file.
 */

 /*
   You must look at dataset_1.txt - dataset_6_example.txt to understand
   the datasets you will be hashing. Your goal is evenly map every record
   in a data set to integers.

   Data Set 1: Weather, One variable changes, and is evenly distributed.
   Data Set 2: Weather, One variable changes, not as evenly distributed.
   Data Set 3: Weather, 3 variables change, a bit nefarious.
   Data Set 4: Weather, All fields change, even distribution.
   Data Set 5: Dictionary entries.
    - Key equality: Are entry terms equal?
    - full equality: Are both entry terms and definitions equal?
   Data Set 6: *Random* Weather Logs. (Uses equal4 for equality check).
 */

#include "Hashing.h"

using namespace std;

 // IMPLEMENT me first!
int hash1(WeatherRecord obj)
{
    return obj.day;
}


bool equal1(WeatherRecord obj1, WeatherRecord obj2)
{
    return obj1.day == obj2.day;
}


int hash2(WeatherRecord obj)
{
    return -1; // IMPLEMENT ME!
}


bool equal2(WeatherRecord obj1, WeatherRecord obj2)
{
    return false; // IMPLEMENT ME!
}


int hash3(WeatherRecord obj)
{
    return -1; // IMPLEMENT ME!
}


bool equal3(WeatherRecord obj1, WeatherRecord obj2)
{
    // Hint: use the && operator and check whether
    // corresponding fields are equal.

    return false; // IMPLEMENT ME!
}


int hash4(WeatherRecord obj)
{
    return -1; // IMPLEMENT ME!
}

// Please use this when testing floats for equality.
inline bool closeEnough(float a, float b, float err)
{
    float diff = (a >= b) ? (a - b) : (b - a);
    return diff < err;
}

bool equal4(WeatherRecord obj1, WeatherRecord obj2)
{
    // FIXME: Use all fields from obj1 and obj2 to determine
    //        equality.

    float e = obj1.error_bound + obj2.error_bound;
    return closeEnough(obj1.temperature, obj2.temperature, e);
}

// The definition should not affect the hash value.
// If two objects have the same term, they should produce the same
// hash value.
int keyHash5(DictionaryRecord obj)
{
    // Hint: Use most / all of the characters in the term strings
    // You can convert chars to ints: int val = (int)c;
    // The multiplication operator can be very helpful in evenly
    // distributing values.

    return -1; // IMPLEMENT ME.
}


// Returns true if the terms and the definitions are the same.
bool equal5(DictionaryRecord obj1, DictionaryRecord obj2)
{
    return false; // IMPLEMENT ME.
}


// Only check whether the terms are equal.
// Definitions don't need to be equal, since they are not the key.
bool keysEqual5(DictionaryRecord obj1, DictionaryRecord obj2)
{
    return obj1.term == obj2.term;
}


int hash6(WeatherRecord obj)
{
    return -1; // IMPLEMENT ME.
}

//#define DEBUG // uncomment this to produce hash value log files for all tests.


// YOUR WORK ABOVE THIS LINE.
// -------------- (LINE) --------------------------
// PLEASE DON't MODIFY code below this line.

int main ()
{
    benchmark("log.txt");
    cout << "END OF PROGRAM" << endl;
}

void benchmark(string logFilename)
{

    TestData testLog;

    cout << "Reminder: A chain's size is how many objects were assigned an identical\n"
            "          hash value modulus the number of 'buckets' in the table.\n\n";

    //------------------
    // Declare Data Sets
    //------------------

    Dataset<WeatherRecord> dataset1;
    Dataset<WeatherRecord> dataset2;
    Dataset<WeatherRecord> dataset3_small;
    Dataset<WeatherRecord> dataset3_large;
    Dataset<WeatherRecord> dataset4;
    Dataset<DictionaryRecord> dataset5;
    Dataset<WeatherRecord> dataset6;


    //----------------
    // Load Data Sets.
    //----------------

    loadDataSet("dataset_1.txt", dataset1);
    loadDataSet("dataset_2.txt", dataset2);
    loadDataSet("dataset_3_small.txt", dataset3_small);
    loadDataSet("dataset_3_large.txt", dataset3_large);
    loadDataSet("dataset_4.txt", dataset4);
    loadDataSet("dataset_5.txt", dataset5);
    loadDataSet("correct_final_dictionary_dataset_5.txt", dataset5.earlierDuplicatesRemoved);
    //loadDataSet("dataset_6.txt", dataset6);
    generateDataset6(dataset6, "dataset_6_randomly_generated");


    //----------------------------------------------------
    // Measure Performance of Hash and equality functions.
    //----------------------------------------------------

    OUT out;

    // Data set 1.
    out.log("Data set 1: Set Problem\n");
    runTests(test_all_here, dataset1, hash1, equal1, 100, testLog);
    runTests(test_all_here, dataset1, hash1, equal1, 1000, testLog);
    runTests(test_all_here, dataset1, hash1, equal1, 10000, testLog);
    out.log(testLog.str() + string("\n"));
    testLog.clear();

    // Data set 2.
    out.log("Data set 2: Set Problem\n");
    runTests(test_all_here, dataset2, hash2, equal2, 100, testLog);
    runTests(test_all_here, dataset2, hash2, equal2, 1000, testLog);
    out.log(testLog.str() + string("\n"));
    testLog.clear();

    // Data set 3. (comes in 2 sizes.)
    out.log("Data set 3: Set Problem\n");
    size_t table_size_3_small = 5000;
    size_t table_size_3_large = 44000;
    runTests(test_all_here, dataset3_small, hash3, equal3, table_size_3_small, testLog);
    out.log(testLog.str());
    testLog.clear();
    runTests(test_all_here, dataset3_large, hash3, equal3, table_size_3_large, testLog);
    out.log(testLog.str());
    testLog.clear();

    // Data set 4.
    out.log("Data set 4: Set Problem\n");
    runTests(test_all_here, dataset4, hash4, equal4, 10000, testLog);
    out.log(testLog.str() + string("\n"));
    testLog.clear();

    // Data set 5.
    size_t table_size = 100;
    out.log("Data set 5: Dictionary Problem\n");
    // For Dictionaries, entries are equal in purpose if their terms are equal.
    dataset5.keysEqualFunction = keysEqual5;
    runTests(test_dictionary, dataset5, keyHash5, equal5, table_size, testLog, 3);
    out.log(testLog.str() + string("\n"));
    testLog.clear();

    // Data set 6.
    out.log("Data set 6: Set Problem\n");
    out.log(" - ***Regenerating RANDOM Data Set***\n");
    generateDataset6(dataset6, "dataset_6.txt");
    runTests(test_all_here, dataset6, hash6, equal4, 10000, testLog, 3);
    out.log(testLog.str() + string("\n"));
    testLog.clear();

    out.writeToFile(logFilename);
}

template <class E>
void runTests(bool (*test)(Dataset<E> & dataset,
                           int (*hashFunction)(E),
                           bool (*equalFunction)(E, E),
                           size_t table_size,
                           TestData & testLog),
                Dataset<E> & dataset,
                int  (*hashFunction )(E),
                bool (*equalFunction)(E, E),
                size_t table_size,
                TestData & testLog, // OUTPUT
                const int reps
               )
{

    #ifdef DEBUG
    logHashValues(dataset, hashFunction, table_size);
    #endif // DEBUG

    stringstream log;

    log << "(table size: " << table_size
        << ", " << dataset.name << " (size: " << dataset.data.size() << ")" << endl;
    testLog.newTest(log.str());
    log.clear();
    const Time START_TIME = startTimer();
    for(int i = 0; i < reps; i++)
    {
        bool noError = test(dataset, hashFunction, equalFunction, table_size, testLog);

        // Test produced an error other a time out.
        if(!noError)
        {
            break;
        }

        // This batch of tests has timed out.
        if(timeSince(START_TIME) > TestData::GIVE_UP_SERIES_OF_TESTS_TIME)
        {
            stringstream msg;
            msg << " - TIMED OUT! Repeated testing took more than " << (TestData::GIVE_UP_SERIES_OF_TESTS_TIME / 1000) << " seconds\n";
            testLog.log(msg.str());
            break;
        }
    }


    return; // Output is in the testData Object.
}

// Only works for a set with no duplicates.
template <class E>
bool test_all_here(Dataset<E> & dataset,
                   int (*hashFunction)(E),
                   bool (*equalFunction)(E, E),
                   size_t table_size,
                   TestData & testLog)
{
    Time TIME_START = startTimer();

    // hashValues will be interpreted mod table_size.
    HashSet<E> set(table_size);

    // HashSets should work faster than vectorsets.

    set.setHashFunction(hashFunction);
    set.setEqualFunction(equalFunction);

    int lineNumber = 0;
    for(E obj : dataset.data)
    {
        bool result = set.add(obj);
        if(!result)
        {
            stringstream err;
            err << "add failed (line #" << lineNumber << ")" << endl;
            err << obj.str() << endl;
            err << "This means that your equality function "
                   "should return false more often. (false positive).\n\n";
            testLog.log(err.str());
            return false;
        }
        else if(timeSince(TIME_START) > TestData::GIVE_UP_TIME)
        {
            testLog.log(" - TIMED OUT! (adding objs to set)\n");
            return true;
        }

        lineNumber++;
    }


    if(set.countElements() < dataset.data.size())
    {
        testLog.log(
                "\n"
                "ERROR: Not all Objects added to the table.\n"
                "       This means that your equality function should\n"
                "       return false more often. (false positive).\n\n");
        return false;
    }


    for(E obj : dataset.data)
    {
        if(!set.contains(obj))
        {
            testLog.log(
                    "\n"
                    "ERROR: Cannot find object.\n"
                    "       If your hash function is non-random, this means that your equality function should\n"
                    "       return true more often (false negative).\n\n"
            );
            return false;
        }
        else if(timeSince(TIME_START) > TestData::GIVE_UP_TIME)
        {
            testLog.log(" - TIMED OUT! (Checking that all objs are now in the set.)\n");
            return true;
        }
    }

    stopTimer(testLog);

    logStatistics(set, testLog);

    return true;
}


template <class E>
bool test_dictionary(Dataset<E> & dataset,
                     int (*hashFunction)(E),
                     bool (*equalsFunction)(E, E),
                     size_t table_size,
                     TestData & testLog)
{
    Time TIME_START = startTimer();

    // hashValues will be interpreted mod table_size.
    HashSet<E> set(table_size);

    // HashSets should work faster than vectorsets.

    set.setHashFunction(hashFunction);
    set.setEqualFunction(dataset.keysEqualFunction);

    for(E obj : dataset.data)
    {
        // If term already present, update its entry by removing the old
        // entry and adding this new entry.
        // We use the new entry as the key for deletion, since it is equal
        // to the old entry assuming the definition is ignored in the
        // equality function.

        set.remove(obj);
        set.add(obj);

        if(timeSince(TIME_START) > TestData::GIVE_UP_TIME)
        {
            testLog.log(" - TIMED OUT! (adding objs to set)\n");
            return true;
        }
    }


    vector<E> & correct = dataset.earlierDuplicatesRemoved;

    if(correct.size() == 0)
    {
        cout << "ERROR: dataset has no 'correct' list of entries to\n"
                "       test the dictionary test results with!\n"
                "       assign something to dataset.earlierDuplicatesRemoved";
        return false;
    }

    if(set.countElements() < correct.size())
    {
        testLog.log(
                "\n"
                "ERROR: Not all Objects added to the table.\n"
                "       This means that your key equality function "
                "should return false more often. (false positive).\n\n");
        return false;
    }
    else if(set.countElements() > correct.size())
    {
        testLog.log(
                    "\n"
                    "ERROR: Some Objects not removed.\n"
                    "       If your hash function is non-random, this means that your key equality \n"
                    "       function should return true more often (false negative).\n\n"
         );
         return false;
    }


    // Verify correctness using a full (keys + values) function.
    set.setEqualFunction(equalsFunction);

    for(E obj : correct)
    {
        if(!set.contains(obj))
        {
            testLog.log(
                    "\n"
                    "ERROR: Cannot find object:\n");
            testLog.log(obj.str());
            testLog.log(
                    "       If your hash function is non-random, this means that your "
                           "full equality \n"
                    "       function should return true more often (false negative).\n\n"
            );
            return false;
        }
        else if(timeSince(TIME_START) > TestData::GIVE_UP_TIME)
        {
            testLog.log(" - TIMED OUT! (Checking that all objs are now in the set.)\n");
            return true;
        }
    }

    stopTimer(testLog);

    logStatistics(set, testLog);

    return true;
}

template <class E>
void logStatistics(HashSet<E> set, TestData & testLog)
{
    // Output Message.
    stringstream out;
    //out << " - " << stopTime << "(last time)" << endl;

    size_t chain_length = set.maxChainLength();
    size_t table_size = set.countBuckets();
    size_t set_size = set.countElements();
    size_t optimum = set_size / table_size;
    if(set_size % table_size != 0)
    {
        optimum++; // Round up.
    }

    float factor = chain_length * 1.0 / optimum;
    out << std::fixed;
    out << std::setprecision(2);
    out << " - Objs in set at end: " << set.countElements() << endl;
    out << " - Max Chain Size: " << chain_length << " (" << factor << " x the optimum)";
    out << endl;
    out << " - Average Non-empty chain size: " << set.averageChainLength();
    out << endl;

    testLog.lastMessage = out.str();
}

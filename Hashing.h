// ITP - 132, Assignment 4 - Hashing. HEADER.
// Written by Bryce Summers on November 12th, 2020.
// Version 1.0, last modified on November.22.2020

// STUDENT: Look at the WeatherRecord and Dictionary Record classes.
//          They at the very start of this .h file.
//          Your code will be in the Hashing.cpp file.

#ifndef HASHING_H // Don't redeclare these interfaces if they are already defined.
#define HASHING_H // Tell the world that we've now defined MAIN_H

#include <iostream> // Input / Output Streams
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <fstream>
#include <iomanip> // Pretty print decimal to 2 figures.

using namespace std;


class WeatherRecord
{
public:
    int    day    = 0;
    int    hour   = 0;
    int    minute = 0;

    float  temperature  = 0.0;
    float  wind_speed   = 0.0;
    char wind_direction = '0';

    // floats may have an error up to this amount.
    // If e is the sum of the error bounds of two objects,
    // Consider their float fields equal if they are within e of each other.
    float error_bound = .0001;

    bool verified = false;

    string str()
    {
        stringstream out;

        out << "Weather: ";
        out << "[day: "     << day;
        out << "  hour: "   << hour;
        out << "  minute: " << minute;

        out << "  temp: "       << temperature;
        out << "  wind_speed: " << wind_speed;
        out << "  wind_dir:   " << wind_direction;

        out << "  verified:   " << verified;
        out << " ]";

        return out.str();
    }
};

class DictionaryRecord
{
public:
    string term;
    string definition;
    string str()
    {
        stringstream out;
        out << "Term:       " << term << endl;
        out << "Definition: " << definition << endl;
        return out.str();
    }
};


// Record Objects that you should become familiar with (above).
// -----------------------------------------------------------------------------
// Various structures and behind the scenes methods (below).


template <class E>
class Set
{
    // Interface functions. Every HashSet must implement these.
    virtual bool add(E obj)      = 0;
    virtual bool contains(E obj) = 0;
    virtual bool remove(E obj)   = 0;

public:
    bool (*equal)(E obj1, E obj2);

public:
    void setEqualFunction(bool (*func)(E, E))
    {
        equal = func;
    }
};


template<class E>
class VectorSet : public Set<E>
{
private:
    std::vector<E> data;

public:
    bool add(E obj);
    bool contains(E obj);
    bool remove(E obj);

    int size()
    {
        return data.size();
    };

    typename
    std::vector<E>::iterator begin()
    {
        return data.begin();
    }

    typename
    std::vector<E>::iterator end()
    {
        return data.end();
    }

private:
    typename std::vector<E>::iterator find(E obj);
};

template<class E>
class HashSet : public Set<E>
{
private:
    std::vector<VectorSet<E>> chains;

public:

    // Construct enough chains for the size of this hash table.
    HashSet(int size)
    {
       for(int i = 0; i < size; i++)
       {
           chains.push_back(VectorSet<E>());
       }
    }

static inline size_t bucket(int hashValue, size_t bucketNumber);

protected:
    int  (*hash)(E obj);

public:

    void setHashFunction(int (*func)(E))
    {
        hash = func;
    }

    void setEqualFunction(bool (*func)(E, E))
    {
        Set<E>::equal = func;

        for(auto iter = chains.begin(); iter != chains.end(); ++iter)
        {
            iter -> setEqualFunction(func);
        }
    }

    bool add(E obj);
    bool contains(E obj);
    bool remove(E obj);


    size_t maxChainLength()
    {
        size_t max_length = 0;
        for(auto iter = chains.begin(); iter != chains.end(); ++iter)
        {
            size_t len = iter -> size();
            if(len > max_length)
            {
                max_length = len;
            }
        }

        return max_length;
    };

    float averageChainLength()
    {
        size_t total_length = 0;
        size_t num_non_empty_chains = 0;
        for(auto iter = chains.begin(); iter != chains.end(); ++iter)
        {
            size_t chain_length = iter -> size();
            total_length += chain_length;

            if(chain_length > 0)
            {
                num_non_empty_chains++;
            }
        }

        return total_length * 1.0 / num_non_empty_chains;
    };

    size_t countBuckets()
    {
        return chains.size();
    }

    size_t countElements() // How many E objects are in the table right now?
    {
        size_t count = 0;

        for(auto iter = chains.begin(); iter != chains.end(); ++iter)
        {
            count += iter -> size();
        }

        return count;
    }

    void toVector(std::vector<E> & out)
    {
        for(VectorSet<E> chain : chains)
        for(E elem : chain)
        {
            out.push_back(elem);
        }
    }

private:
    typename std::vector<E>::iterator find(E obj);
};

template<class E>
bool VectorSet<E>::add(E folk)
{
    if(!this -> contains(folk))
    {
        data.push_back(folk);
        return true;
    }

    return false;
}


template<class E>
typename std::vector<E>::iterator VectorSet<E>::find(E obj)
{
    typename vector<E>::iterator iter = data.begin();

    while(iter != data.end())
    {
        if(this -> equal(*iter, obj))
        {
            break; // return iter.
        }

        ++iter;
    } // returns data.end if it gets here.

    return iter;
}

template<class E>
bool VectorSet<E>::contains(E obj)
{
    return find(obj) != data.end();
}

template<class E>
bool VectorSet<E>::remove(E obj)
{
    typename vector<E>::iterator iter = find(obj);
    if(iter != data.end())
    {
        data.erase(iter);
        return true;
    }

    return false;
}

// bound to range 0 - bucketNumber - 1
// Assumes bucketNumber is greater than 0.
template <class E>
inline size_t HashSet<E>::bucket(int hashValue, size_t bucketNumber)
{
    hashValue = hashValue % bucketNumber;
    return hashValue >= 0 ? (hashValue) : (hashValue + bucketNumber);
}

template<class E>
bool HashSet<E>::add(E obj)
{
    size_t hashValue = bucket(this -> hash(obj), countBuckets());

    // find the chain, then add it.
    VectorSet<E> & chain = chains[hashValue];
    bool result = chain.add(obj); // Try to add it.

    return result; // Fails if obj was a duplicate.
}

template<class E>
bool HashSet<E>::contains(E obj)
{
    size_t hashValue = bucket(this -> hash(obj), countBuckets());

    // find the chain, then add it.
    VectorSet<E> & chain = chains[hashValue];

    // Return true if the chain contains the object.
    return chain.contains(obj);
}

template<class E>
bool HashSet<E>::remove(E obj)
{
    size_t hashValue = bucket(this -> hash(obj), countBuckets());

    // find the chain, then add it.
    VectorSet<E> & chain = chains[hashValue];

    // Remove the object from the chain.
    // Return true if that result actually
    // removed an object.
    return chain.remove(obj);
}


// Stores a list of Objects as they appear within a .txt file.
template<class E>
class Dataset
{
public:
    vector<E> data;
    string name; // filename.

    // Used to align parallel logs (like hash values) at the same line
    // numbers as in the original.
    int lines_per_entry_in_original_file = 1;

    // Stores what the dataset should look like after processing.
    vector<E> earlierDuplicatesRemoved;
    vector<E> laterDuplicatesRemoved;

    // A special equality function that only checks if two objects have
    // the same identity ('key' information). This is critical
    // for Associative array applications like a dictionary where
    // keys are dynamically associated with different values.
    // It can also be useful for database querires, where you can
    // search for all object that match a few of the fields.
    bool (*keysEqualFunction)(E, E);

    // Randomizes the order of the data.
    void shuffle()
    {
        size_t N = data.size();
        for(size_t b = N - 1; b > 0; b--)
        {
            size_t a = rand() % (b + 1);

            E temp = data[a];
            data[a] = data[b];
            data[b] = temp;
        }
    }

    void removeEarlierDuplicates(int (*hash)(E),
                                 bool (*equal)(E, E))
    {
        HashSet<E> set(data.size());

        set.setHashFunction(hash);
        set.setEqualFunction(equal);

        for(E elem : data)
        {
            set.remove(elem);
            set.add(elem);
        }

        set.toVector(earlierDuplicatesRemoved);
    }


    void removeLaterDuplicates(int (*hash)(E),
                                 bool (*equal)(E, E))
    {
        HashSet<E> set(data.size());

        set.setHashFunction(hash);
        set.setEqualFunction(equal);

        for(E elem : data)
        {
            set.add(elem);
        }

        set.toVector(this.laterDuplicatesRemoved);
    }
};


// Generates a random set of data.
void generateDataset6(Dataset<WeatherRecord> & dataset, string filename)
{
    dataset.name = filename;

    vector<char> wind_dirs = {'N', 'E', 'S', 'W'};
    vector<bool> yesNo = {true, false};

    int size = 70000;

    for(int counter = 0; counter < size; counter++)
    {
        WeatherRecord obj;

        obj.day = rand();
        obj.hour = rand();
        obj.minute = rand();
        obj.temperature = (rand() % 10000) / 100.0;
        obj.wind_speed = (rand() % 12000) / 100.0;
        obj.wind_direction = wind_dirs[rand() % 4];
        obj.verified = yesNo[rand() % 2];

        dataset.data.push_back(obj);
    }

    return;
}


void logDataSet(Dataset<WeatherRecord> & dataset)
{
    ofstream out(dataset.name);

    vector<WeatherRecord> & data = dataset.data;
    for(auto iter = data.begin(); iter != data.end(); ++iter)
    {
        WeatherRecord & w = *iter;

        out << w.str() << endl;
    }

    out.close();
}

void loadDataSet(string filename, Dataset<WeatherRecord> & dataset)
{
    ifstream in(filename);

    if(in.is_open() == false)
    {
        cout << "ERROR: " << filename << " does not exist." << endl;
        return;
    }

    dataset.name = filename;

    string dump;
    in >> dump; // "Weather: "

    int lineCount = 0;

    while(in.fail() == false)
    {
        WeatherRecord w;

        in >> dump; // "[day: "
        in >> w.day;

        in >> dump; // "  hour: "
        in >>  w.hour;

        in >> dump; // "  minute: "
        in >>  w.minute;

        in >> dump; // "  temp: "
        in >>  w.temperature;

        in >> dump; // "  wind_speed: "
        in >>  w.wind_speed;

        in >> dump; // "  wind_dir:   "
        in >>  w.wind_direction;

        in >> dump; // "  verified:   "
        in >>  w.verified;

        in >> dump; // " ]\n";

        if(in.fail() == false)
        {
            dataset.data.push_back(w);
            lineCount++;
        }
        else
        {
            cout << "DataSet load failed on line #" << lineCount
                 << "in " << filename << endl;
            return;
        }

        // Try to begin the next row. If it fails the loop must end.
        in >> dump; // "Weather: "
    }

    in.close();
}



//---------------------
// dictionary records.
//---------------------


// Call this to load one vector of data from a file.
void loadDataSet(string filename, vector<DictionaryRecord> & data)
{
    ifstream in(filename);

    if(in.is_open() == false)
    {
        cout << "ERROR: " << filename << " does not exist." << endl;
        return;
    }

    int lineCount = 0;

    while(in.fail() == false)
    {
        DictionaryRecord def;

        getline(in, def.term);
        getline(in, def.definition);

        if(in.fail() == false)
        {
            data.push_back(def);
            lineCount++;
        }
        else if(in.eof() == false)
        {
            cout << "DataSet load failed on line #" << lineCount
                 << "in " << filename << endl;
            return;
        }

        string blank;
        getline(in, blank);

        if(blank != "")
        {
            cout << "Non blank line found! (line #" << lineCount
                 << "in " << filename << endl;
        }
    }

    in.close();
}


void loadDataSet(string filename, Dataset<DictionaryRecord> & dataset)
{
    dataset.name = filename;
    dataset.lines_per_entry_in_original_file = 3;
    loadDataSet(filename, dataset.data);
}


// Hash Value Logging.
template <class E>
void logHashValues(Dataset<E> & dataset, int (*hashfunction)(E), size_t tableSize)
{
    stringstream filename;
    filename << "hash_values_mod_" << tableSize << "_of_";
    filename << dataset.name;
    ofstream out(filename.str());

    if(out.is_open() == false)
    {
        return;
    }

    const int padBlanks = dataset.lines_per_entry_in_original_file - 1;

    for(E elem: dataset.data)
    {
        size_t value = HashSet<E>::bucket(hashfunction(elem), tableSize);
        out << value << endl;

        for(int i = 0; i < padBlanks; i++)
        {
            out << endl;
        }
    }

    out.close();
}

// Performance Benchmarking.
void benchmark(string logFileName);


class TestData
{
public:
    int numTimes;
    size_t totalTime;
    size_t recordTime;
    string lastMessage;

    TestData()
    {
        clear();
    }

    static size_t GIVE_UP_SERIES_OF_TESTS_TIME;
    static size_t GIVE_UP_TIME;

private:
    stringstream myLog;
    bool atLeastOneTestHasBeenRun;

    void clearTest()
    {
        numTimes = 0;
        totalTime = 0;
        recordTime = 1000000; // MAX_UNSIGNED_INT
        lastMessage = "";
    }

public:
    void newTest(string preamble) // title of test.
    {
        myLog << lastTestResults(); // Log the results of the previous test.
        clearTest(); // Prepare for a new test.
        myLog << preamble << endl; // Log title of next test.
    }

    void addTime(size_t time) // Allows for multiple trials of the same test.
    {
        numTimes++;
        totalTime += time;
        if(time < recordTime)
        {
            recordTime = time;
        }

        atLeastOneTestHasBeenRun = true;
    }

    // Returns an empty string if no test has been run yet
    // (since the last clear().)
    string lastTestResults()
    {
        stringstream out;

        if(atLeastOneTestHasBeenRun)
        {
            out << " - " << totalTime / numTimes << " average milliseconds.\n";
            out << " - " << recordTime << " quickest milliseconds.\n";
            out << lastMessage << endl;
        }

        return out.str();
    }

    string str()
    {
        stringstream out;
        out << myLog.str() << lastTestResults();
        return out.str();
    }

    // Resets this TestData Object.
    void clear()
    {
        myLog.str("");
        clearTest();
        atLeastOneTestHasBeenRun = false;
    }

    void log(string msg)
    {
        myLog << msg;
    }
};

template<class E>
void runTests(bool (*test)(Dataset<E> & dataset,
                           int (*hashFunction)(E),
                           bool (*equalFunction)(E, E),
                           size_t table_size,
                           TestData & testLog),
              Dataset<E> & dataset,
              int  (*hashFunction )(E),
              bool (*equalFunction)(E, E),
              size_t table_size,
              TestData & testLog,
              const int reps = 10
             );

// Returns a string describing the time taken
template<class E>
bool test_all_here(Dataset<E> & dataset,
                   int (*hashFunction)(E),
                   bool (*equalFunction)(E, E),
                   size_t table_size,
                   TestData & testLog);

template <class E>
bool test_dictionary(Dataset<E> & dataset,
                     int (*hashFunction)(E),
                     bool (*equalFunction)(E, E),
                     size_t table_size,
                     TestData & testLog);

template <class E>
void logStatistics(HashSet<E> set);

// https://stackoverflow.com/questions/3220477/how-to-use-clock-in-c
typedef std::chrono::high_resolution_clock Clock;
typedef std::chrono::_V2::system_clock::time_point Time;

Time time_start = Clock::now();
const size_t ONE_SECOND = 1000;
size_t TestData::GIVE_UP_TIME = ONE_SECOND;
size_t TestData::GIVE_UP_SERIES_OF_TESTS_TIME = ONE_SECOND*3;

Time startTimer()
{
    time_start = Clock::now();
    return time_start;
}

// Computes time since a non-global time start.
inline size_t timeSince(Time time_start)
{
    auto time_lap = Clock::now();
    return std::chrono::duration_cast<
           std::chrono::milliseconds>
           (time_lap - time_start).count();
}

// returns time in milliseconds.
size_t stopTimer()
{
    auto time_lap = Clock::now();

    size_t time = std::chrono::duration_cast<
           std::chrono::milliseconds>
           (time_lap - time_start).count();

    return time;
}

// returns time in milliseconds.
size_t stopTimer(TestData & testData)
{
    size_t time = stopTimer();

    testData.addTime(time);

    return time;
}

/*
string itTookThisLong(TestData & testData)
{
    size_t time = stopTimer(testData);

    stringstream out;
    out << time
        << " milliseconds";
    return out.str();
}*/


// Allows me to write to a log file and the output at the same time.
// The log file is helpful for those programmers who with to take
// notes on performance before and after they make changes.
class OUT
{
private:
  stringstream logStream;

public:
    void log(string str)
    {
        cout << str;
        logStream << str;
    }

    void writeToFile(string filename)
    {
        ofstream out(filename);
        if(out.is_open() == false)
        {
            cout << "Failed to write log to " << filename;
            return;
        }

        out << logStream.str();
        cout << "Log has been written to " << filename << endl;
        return;
    }
};

#endif // HASHING_H

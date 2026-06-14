#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// -------------------------------------------------------------------
//  2 Struct Data Types & Friend 1 & 2 - Shinru
// -------------------------------------------------------------------
// Struct 1: Song -> Stores comprehensive track information
struct Song {
    int id;
    string title;
    string artist;
    string genre; 
    int popularity;
    int duration; 

    // Constructors
    Song() : id(0), popularity(0), duration(0) {}
    
    Song(int i, string t, string a, string g, int p, int d) 
        : id(i), title(t), artist(a), genre(g), popularity(p), duration(d) {}

    // Friend 1 Declaration: Enables standard sequential streaming outputs
    friend ostream& operator<<(ostream& os, const Song& song);
    
    // Friend 2 Declaration: Enables standardized structured database input streams
    friend istream& operator>>(istream& is, Song& song);
};

// Friend 1 Function Implementation
ostream& operator<<(ostream& os, const Song& song) {
    os << song.id << " " << song.title << " " << song.artist << " " << song.genre << " " << song.popularity << " " << song.duration;
    return os;
}

// Friend 2 Function Implementation
istream& operator>>(istream& is, Song& song) {
    is >> song.id >> song.title >> song.artist >> song.genre >> song.popularity >> song.duration;
    return is;
}

// Struct 2: Doubly linked list node architecture
struct Node {
    Song data;
    Node* prev;
    Node* next;

    Node(Song s) : data(s), prev(nullptr), next(nullptr) {}
};

// -------------------------------------------------------------------
//   Doubly Linked List & Dynamic Memory Access (new / delete) - Shinru
// -------------------------------------------------------------------
class DoublyLinkedList {
private:
    Node* head;
    Node* tail;
    int size;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    ~DoublyLinkedList(); 

    void append(Song s);          // Dynamically allocates memory using 'new'
    void removeNode(int songId);  // Safely deallocates memory using 'delete'
    void clear();                 // Batch cleanup of all active heap memory segments
    
    Node* getHead() const { return head; }
    int getSize() const { return size; }

    //   Overloaded Feature 1: Polymorphic Song Additions
    void addSong(Song newSong);  // Overload A: Administrator directly appends track to pool
    void addSong(int songId, const DoublyLinkedList& globalPool); // Overload B: Customer clones song via global lookup
};

// Destructor: Performs automatic cascading memory cleanup ---
DoublyLinkedList::~DoublyLinkedList() {
    clear(); 
}

// Linked List Operation: Append a new node safely to the tail ---
void DoublyLinkedList::append(Song s) {
    Node* newNode = new Node(s); // Dynamic memory allocation: new
    if (head == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

// Linked List Operation: Isolate and delete a node via targeted track lookup
void DoublyLinkedList::removeNode(int songId) {
    Node* current = head;
    while (current != nullptr) {
        if (current->data.id == songId) {
            if (current == head) {
                head = head->next;
                if (head != nullptr) head->prev = nullptr;
                else tail = nullptr;
            } else if (current == tail) {
                tail = tail->prev;
                if (tail != nullptr) tail->next = nullptr;
                else head = nullptr;
            } else {
                current->prev->next = current->next;
                current->next->prev = current->prev;
            }
            delete current; // Dynamic memory deallocation: delete
            size--;
            return;
        }
        current = current->next;
    }
}

// Linked List Operation: Wipe whole structure to prevent structural memory leakage
void DoublyLinkedList::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* nextNode = current->next;
        delete current; // Memory reclamation
        current = nextNode;
    }
    head = nullptr;
    tail = nullptr;
    size = 0;
}

// Overload A Function Implementation: Appends direct full-object details into storage tails
void DoublyLinkedList::addSong(Song newSong) {
    this->append(newSong);
}

// Overload B Function Implementation: Searches master dataset via track index keys to replicate items
void DoublyLinkedList::addSong(int songId, const DoublyLinkedList& globalPool) {
    Node* current = globalPool.getHead();
    bool found = false;

    while (current != nullptr) {
        if (current->data.id == songId) {
            this->append(current->data);
            found = true;
            break; 
        }
        current = current->next; 
    }

    // Standard fallback print instead of throwing stdexcept elements
    if (!found) {
        cout << "[Error] The requested song ID does not exist in the system pool. Operation aborted.\n";
    }
}

// -------------------------------------------------------------------
//   2 Base Classes & Friend Functions 3 - Shinru
// -------------------------------------------------------------------
// Base Class 1: User Account Core Schema
class User {
protected:
    string username;
    string password;
    string role; 

public:
    User() : username(""), password(""), role("") {}
    User(string u, string p, string r) : username(u), password(p), role(r) {}
    virtual ~User() {}

    virtual void displayMenu() = 0; // Pure virtual enforcement for system layouts

    string getUsername() const { return username; }
    string getRole() const { return role; }
    bool checkLogin(string u, string p) { return (username == u && password == p); }

    // Friend 3: Encrypts and flushes secure credential buffers to disk allocations
    friend void saveUserToFile(ofstream& file, const User& user);
};

// Friend 3 Function Implementation
void saveUserToFile(ofstream& file, const User& user) {
    string encryptedPass = user.password;
    for (size_t i = 0; i < encryptedPass.length(); i++) {
        encryptedPass[i] ^= 0x5A; // Reversible XOR Cypher bitwise streaming
    }
    file << user.username << " " << encryptedPass << " " << user.role << endl;
}

// Base Class 2: Abstract Media Exporter Constraint
class MediaItem {
public:
    virtual ~MediaItem() {}
    virtual void exportToTXT(ofstream& file) = 0; 
};


// -------------------------------------------------------------------
//   Sorting (Quick Sort) & Searching (Binary Search) - Shinru
// -------------------------------------------------------------------
class AlgorithmManager {
private:
    static void swapSongs(Song& x, Song& y) {
        Song temp = x;
        x = y;
        y = temp;
    }

    // Core Partitioner Routine running over manual structural array indices
    static int partition(Song arr[], int low, int high, int criteria) {
        Song pivot = arr[high]; 
        int i = (low - 1);

        for (int j = low; j <= high - 1; j++) {
            bool condition = false;
            if (criteria == 1) condition = (arr[j].id < pivot.id);               // Criteria 1: Sort by ID (Ascending)
            else if (criteria == 2) condition = (arr[j].duration < pivot.duration); // Criteria 2: Sort by Length (Ascending)
            else if (criteria == 3) condition = (arr[j].popularity > pivot.popularity); // Criteria 3: Sort by Popularity/Playcount Leaderboard (Descending)
            else if (criteria == 4) condition = (arr[j].title < pivot.title);     // Criteria 4: Sort by Title Alphabetical order (Ascending)

            if (condition) {
                i++;
                swapSongs(arr[i], arr[j]);
            }
        }
        swapSongs(arr[i + 1], arr[high]);
        return (i + 1);
    }

    // Classic Recursive Quick Sort Algorithm Implementation
    static void quickSort(Song arr[], int low, int high, int criteria) {
        if (low < high) {
            int pivotIndex = partition(arr, low, high, criteria);
            quickSort(arr, low, pivotIndex - 1, criteria);
            quickSort(arr, pivotIndex + 1, high, criteria);
        }
    }

public:
    // Linked List Quick Sort Wrapper
    static void sortLinkedList(DoublyLinkedList& list, int criteria) {
        int size = list.getSize();
        if (size <= 1) return; 

        // 1. Manually instantiate buffer elements into an unmanaged sequential heap block
        Song* songArray = new Song[size];

        // 2. Dump linked structural elements sequentially down into flat arrays
        Node* current = list.getHead();
        int index = 0;
        while (current != nullptr) {
            songArray[index++] = current->data;
            current = current->next;
        }

        // 3. Invoke the main Quick Sort computation over raw pointers
        quickSort(songArray, 0, size - 1, criteria);

        // 4. Wipe linked collections clean, and reconstruct node flows by following sorted indexes
        list.clear();
        for (int i = 0; i < size; i++) {
            list.append(songArray[i]);
        }

        // 5. Clean array memory blocks to remove footprints
        delete[] songArray;
    }

    //  Overload A: Pure Array BINARY SEARCH via Sorted Track ID Indices
    //  Requirement: The dataset MUST be sorted by ID before invoking this!
    static int searchSong(const DoublyLinkedList& list, int targetId) {
        int size = list.getSize();
        if (size == 0) return -1;

        Song* songArray = new Song[size];
        Node* current = list.getHead();
        int index = 0;
        while (current != nullptr) {
            songArray[index++] = current->data;
            current = current->next;
        }

        int low = 0; 
        int high = size - 1;
        int resultIndex = -1;

        // Classical Binary Search Execution Loop
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (songArray[mid].id == targetId) {
                resultIndex = mid; 
                break;
            }
            if (songArray[mid].id < targetId) low = mid + 1;
            else high = mid - 1;
        }

        delete[] songArray; 
        return resultIndex; // Returns corresponding raw array index (-1 if absent)
    }

    //  Overload B: Pure BINARY SEARCH via Track Title String Keys
    //  Feature: To strictly run Binary Search on arbitrary string entries,
    //  the array buffer automatically triggers Quick Sort by Title before running.
    static int searchSong(const DoublyLinkedList& list, string targetTitle) {
        int size = list.getSize();
        if (size == 0) return -1;

        Song* songArray = new Song[size];
        Node* current = list.getHead();
        int index = 0;
        while (current != nullptr) {
            songArray[index++] = current->data;
            current = current->next;
        }

        // Enforce manual Quick Sort on title sequences to satisfy Binary Search conditions
        quickSort(songArray, 0, size - 1, 4); // 4 = Sorting by Title string

        int low = 0; 
        int high = size - 1;
        int resultIndex = -1;

        // Perform standard String Binary Search
        while (low <= high) {
            int mid = low + (high - low) / 2;
            if (songArray[mid].title == targetTitle) {
                resultIndex = mid;
                break;
            }
            if (songArray[mid].title < targetTitle) {
                low = mid + 1;
            } else {
                high = mid - 1;
            }
        }

        delete[] songArray; 
        return resultIndex; 
    }
};


// -------------------------------------------------------------------
//   3 Derived Modules & Friend Function 4 - yx/ys/ex/js - 所有东西都可以更改，这边只是大概, 要看着guideline，看有什么少了，什么多了，什么不对，就更改， 记得有些要调用我写的东西，不要忘记try catch.
// -------------------------------------------------------------------
class Customer; 

// Derived Class 1: Administrative Control Framework - Eexuan / Jason
class Admin : public User {
public:
    Admin() : User("", "", "Admin") {}
    Admin(string u, string p) : User(u, p, "Admin") {}
    ~Admin() override {}

    // TEAMMATE TODO (Eexuan/Jason): Hook these functions to complete administrative assignments
    void displayMenu() override;
    void addSongToPool(DoublyLinkedList& pool);
    void editSongInPool(DoublyLinkedList& pool);
    void displayMusicPool(const DoublyLinkedList& pool);
    void searchSongInPool(const DoublyLinkedList& pool);
    void sortMusicPool(DoublyLinkedList& pool);
    void deleteSongFromPool(DoublyLinkedList& pool);
    void generateSystemReport(const DoublyLinkedList& pool); 
};

// Derived Class 2: Customer Private Access Framework - Yuxuan / Yungsin
class Customer : public User, public MediaItem {
protected:
    DoublyLinkedList myPlaylist; // Private list powered by custom Doubly Linked List
    string playlistName;

public:
    Customer() : User("", "", "Customer"), playlistName("My Playlist") {
        this->role = "Normal_Customer";
    }
    
    Customer(string u, string p) : User(u, p, "Customer"), playlistName("My Playlist") {
        this->role = "Normal_Customer";
    }
    
    ~Customer() override {}

    // TEAMMATE TODO (Yuxuan/Yungsin): Hook these functions to complete customer operations
    void displayMenu() override;
    void createPlaylist();
    void addSongToPlaylist(DoublyLinkedList& globalPool);
    void editPlaylistName();
    void displayPlaylist();
    void searchInPlaylist();
    void sortPlaylist();
    void removeSongFromPlaylist();
    void generatePlaylistSummary(ofstream& file);

    void exportToTXT(ofstream& file) override {
        generatePlaylistSummary(file);
    }

    // Dynamic Popularity Increments - Intersects with Shinru's Sorting Metric Scopes
    virtual void playMusic(Song& currentSong) {
        currentSong.popularity++; // Increment play count score dynamically upon play sessions
        cout << "\n[Notice] Non-Premium account. Playing 30-second commercial advertisement...\n";
        cout << "--> Streaming Track: " << currentSong.title << " [Current Total Plays: " << currentSong.popularity << "]\n";
    }

    // Friend 4: Restores records and dumps playlist snapshots securely across operational shutdowns
    friend void savePlaylistToFile(ofstream& file, const Customer& customer);
    friend void loadPlaylistFromFile(ifstream& file, Customer& customer, DoublyLinkedList& globalPool);
};

// Friend 4 Function 1 Implementation
void savePlaylistToFile(ofstream& file, const Customer& customer) {
    file << customer.username << " " << customer.playlistName << " " << customer.myPlaylist.getSize() << " ";
    Node* curr = customer.myPlaylist.getHead();
    while (curr != nullptr) {
        file << curr->data.id << " "; 
        curr = curr->next;
    }
    file << endl;
}

// Friend 4 Function 2 Implementation
void loadPlaylistFromFile(ifstream& file, Customer& customer, DoublyLinkedList& globalPool) {
    string uname, pName;
    int songCount, sId;
    if (file >> uname >> pName >> songCount) {
        customer.playlistName = pName;
        customer.myPlaylist.clear(); 
        
        for (int i = 0; i < songCount; i++) {
            file >> sId;
            customer.myPlaylist.addSong(sId, globalPool);
        }
    }
}

// Derived Class 3: Elevated Client Stream Node (VIP Polymorphism) - Yuxuan / Yungsin
class PremiumCustomer : public Customer {
public:
    PremiumCustomer(string u, string p) : Customer(u, p) { 
        this->role = "Premium_Customer"; 
    }
    ~PremiumCustomer() override {}

    void playMusic(Song& currentSong) override {
        currentSong.popularity++; // Dynamic incremental popularity loop booster
        cout << "\n[VIP Premium Mode] Commencing ad-free premium operational audio streams...\n";
        cout << "--> Streaming Track: " << currentSong.title << " [Current Total Plays: " << currentSong.popularity << "]\n";
    }
};

// Admin Modules - Eexuan/Jason 
void Admin::displayMenu() {
    int choice = -1;
    while (true) {
        cout << "\n===============================" << endl;
        cout << "    STAFF / ADMIN MODULE MENU  " << endl;
        cout << "===============================" << endl;
        cout << "3. Add New Song" << endl;
        cout << "4. Edit/Update Song" << endl;
        cout << "5. Display Music Pool" << endl;
        cout << "6. Search Song" << endl;
        cout << "7. Sort Music Pool" << endl;
        cout << "8. Delete Song" << endl;
        cout << "9. Summary Report" << endl;
        cout << "0. Logout & Exit" << endl;
        cout << "Enter selection option (0-9): ";
        cin >> choice;

        if (choice == 0) {
            cout << "[System] Logging out out from staff credentials session.\n";
            break;
        }

        // TEAMMATE TODO: Implement the switch control behaviors mapping to functions below
        switch (choice) {
            case 3: cout << "Executing: Add New Song...\n"; break;
            case 4: cout << "Executing: Edit/Update Song...\n"; break;
            case 5: cout << "Executing: Display Music Pool...\n"; break;
            case 6: cout << "Executing: Search Song...\n"; break;
            case 7: cout << "Executing: Sort Music Pool...\n"; break;
            case 8: cout << "Executing: Delete Song...\n"; break;
            case 9: cout << "Executing: Summary Report...\n"; break;
            default: cout << "[Warning] Choice out of bounds. Retry!\n"; break;
        }
    }
}

// TEAMMATE SKELETONS
void Admin::addSongToPool(DoublyLinkedList& pool)
{
    Song s;

    cout << "\n========== ADD NEW SONG ==========\n";

    try
	{
	    cout << "Enter Song ID: ";
	    cin >> s.id;
	
	    if(cin.fail())
	    {
	        throw "Invalid Song ID!";
	    }
	}
	catch(const char* msg)
	{
	    cout << msg << endl;
	
	    cin.clear();
	    cin.ignore(1000, '\n');
	
	    return;
	}

    cout << "Enter Song Title (use _ instead of space): ";
    cin >> s.title;

    cout << "Enter Artist (use _ instead of space): ";
    cin >> s.artist;

    cout << "Enter Genre (use _ instead of space): ";
    cin >> s.genre;

    cout << "Enter Popularity: ";
    cin >> s.popularity;

    cout << "Enter Duration in seconds: ";
    cin >> s.duration;

    pool.append(s);

    ofstream file("music_pool.txt", ios::app);
    file << s << endl;
    file.close();

    cout << "\n[Success] Song added successfully!\n";
}
void Admin::editSongInPool(DoublyLinkedList& pool)
{
    int id;
    Node* current = pool.getHead();

    cout << "\n========== EDIT SONG ==========\n";
    try
	{
	    cout << "Enter Song ID to edit: ";
	    cin >> id;
	
	    if(cin.fail())
	    {
	        throw "Invalid Song ID!";
	    }
	}
	catch(const char* msg)
	{
	    cout << msg << endl;
	
	    cin.clear();
	    cin.ignore(1000, '\n');
	
	    return;
	}

    while (current != nullptr)
    {
        if (current->data.id == id)
        {
            cout << "Enter New Title: ";
            cin >> current->data.title;

            cout << "Enter New Artist: ";
            cin >> current->data.artist;

            cout << "Enter New Genre: ";
            cin >> current->data.genre;

            cout << "Enter New Popularity: ";
            cin >> current->data.popularity;

            cout << "Enter New Duration: ";
            cin >> current->data.duration;

            cout << "\n[Success] Song updated successfully!\n";
            return;
        }

        current = current->next;
    }

    cout << "\n[Error] Song ID not found.\n";
}
void Admin::displayMusicPool(const DoublyLinkedList& pool)
{
    Node* current = pool.getHead();

    cout << "\n========== MUSIC POOL ==========\n";

    if (current == nullptr)
    {
        cout << "No songs available in the music pool.\n";
        return;
    }

    while (current != nullptr)
    {
        cout << "Song ID: " << current->data.id << endl;
        cout << "Title: " << current->data.title << endl;
        cout << "Artist: " << current->data.artist << endl;
        cout << "Genre: " << current->data.genre << endl;
        cout << "Popularity: " << current->data.popularity << endl;
        cout << "Duration: " << current->data.duration << " seconds" << endl;
        cout << "--------------------------------\n";

        current = current->next;
    }
}
void Admin::searchSongInPool(const DoublyLinkedList& pool)
{
    int id;

    cout << "\n========== SEARCH SONG ==========\n";
    cout << "Enter Song ID: ";
    cin >> id;

    int result = AlgorithmManager::searchSong(pool, id);

    if(result != -1)
        cout << "\nSong Found!\n";
    else
        cout << "\nSong Not Found!\n";
}
void Admin::sortMusicPool(DoublyLinkedList& pool)
{
    int choice;

    cout << "\n========== SORT MUSIC POOL ==========\n";
    cout << "1. Sort by Song ID\n";
    cout << "2. Sort by Duration\n";
    cout << "3. Sort by Popularity\n";
    cout << "4. Sort by Title\n";
    cout << "Enter your choice: ";
    cin >> choice;

    if (choice >= 1 && choice <= 4)
    {
        AlgorithmManager::sortLinkedList(pool, choice);
        cout << "\n[Success] Music pool sorted successfully!\n";
        displayMusicPool(pool);
    }
    else
    {
        cout << "\n[Error] Invalid sorting choice.\n";
    }
}
void Admin::deleteSongFromPool(DoublyLinkedList& pool)
{
    int id;

    cout << "\n========== DELETE SONG ==========\n";
    try
	{
	    cout << "Enter Song ID: ";
	    cin >> id;
	
	    if(cin.fail())
	    {
	        throw "Invalid Song ID!";
	    }
	}
	catch(const char* msg)
	{
	    cout << msg << endl;
	
	    cin.clear();
	    cin.ignore(1000, '\n');
	
	    return;
	}

    pool.removeNode(id);

    cout << "\nDelete operation completed.\n";
}
void Admin::generateSystemReport(const DoublyLinkedList& pool)
{
    Node* current = pool.getHead();

    int totalSongs = 0;
    int totalDuration = 0;

    while(current != nullptr)
    {
        totalSongs++;
        totalDuration += current->data.duration;
        current = current->next;
    }

    ofstream file("admin_report.txt");

    file << "===== ADMIN REPORT =====\n";
    file << "Total Songs: " << totalSongs << endl;

    if(totalSongs > 0)
    {
        file << "Average Duration: "
             << totalDuration / totalSongs
             << " seconds\n";
    }

    file.close();

    cout << "\nReport generated successfully.\n";
}

//  Customer Modules - Yuxuan/Yungsin
void Customer::displayMenu() {
    int choice = -1;
    while (true) {
        cout << "\n===============================" << endl;
        cout << "      CUSTOMER MODULE MENU     " << endl;
        cout << "===============================" << endl;
        cout << "3. Create New Playlist" << endl;
        cout << "4. Add Song to Playlist" << endl;
        cout << "5. Edit Playlist Name" << endl;
        cout << "6. Display / Browse My Playlist" << endl;
        cout << "7. Search in Playlist" << endl;
        cout << "8. Sort My Playlist" << endl;
        cout << "9. Remove Song from Playlist" << endl;
        cout << "10. Backup & Export Summary Report" << endl;
        cout << "0. Logout & Exit" << endl;
        cout << "Enter selection option (0-10): ";
        cin >> choice;

        if (choice == 0) {
            cout << "[System] Logging out out from customer session allocations.\n";
            break;
        }

        // TEAMMATE TODO: Implement the switch control behaviors mapping to functions below
        switch (choice) {
            case 3: cout << "Executing: Create New Playlist...\n"; break;
            case 4: cout << "Executing: Add Song to Playlist...\n"; break;
            case 5: cout << "Executing: Edit Playlist Name...\n"; break;
            case 6: cout << "Executing: Display Playlist...\n"; break;
            case 7: cout << "Executing: Search in Playlist...\n"; break;
            case 8: cout << "Executing: Sort My Playlist...\n"; break;
            case 9: cout << "Executing: Remove Song...\n"; break;
            case 10: cout << "Executing: Backup & Export Summary Report...\n"; break;
            default: cout << "[Warning] Choice out of bounds. Retry!\n"; break;
        }
    }
}

// TEAMMATE SKELETONS
void Customer::createPlaylist() {}
void Customer::addSongToPlaylist(DoublyLinkedList& globalPool) {}
void Customer::editPlaylistName() {}
void Customer::displayPlaylist() {}
void Customer::searchInPlaylist() {}
void Customer::sortPlaylist() {}
void Customer::removeSongFromPlaylist() {}
void Customer::generatePlaylistSummary(ofstream& file) {}


// -------------------------------------------------------------------
//   Central Orchestration Driver Engine - yx/ys/ex/js - 所有东西都可以更改，这边只是大概, 要看着guideline，看有什么少了，什么多了，什么不对，就更改， 记得有些要调用我写的东西
// -------------------------------------------------------------------
class SystemEngine {
private:
    DoublyLinkedList globalMusicPool; 
    User* currentUser; 

public:
    SystemEngine() : currentUser(nullptr) {}
    ~SystemEngine() { if(currentUser) delete currentUser; }

    void initSystem() {
        ifstream file("music_pool.txt");
        if (!file.is_open()) {
            cout << "[System Warning] 'music_pool.txt' was not found. An empty library has been initialized.\n";
            ofstream newFile("music_pool.txt");
            newFile.close();
            return;
        }
        
        Song s;
		
		while (file >> s)
		{
		    globalMusicPool.append(s);
		}
        // TEAMMATE TODO: Read all songs using the >> operator in a loop and load into globalMusicPool
        file.close();
    }

    // TEAMMATE TODO: 
    void run() {
        int choice = -1;
        while (true) {
            cout << "\n========================================" << endl;
            cout << "    MUSIC PLAYLIST MANAGEMENT SYSTEM    " << endl;
            cout << "========================================" << endl;
            cout << "1. Register" << endl;
            cout << "2. Login" << endl;
            cout << "0. Exit System" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            if (cin.fail()) {
                cout << "[Notice] Invalid option entered! Please input numeric choices only.\n";
                cin.clear();           
                string dummy; cin >> dummy;
                continue;
            }

            // TEAMMATE TODO: Option 1: Account Registration Business Flows
            if (choice == 1) {
                //
            }
            
            // TEAMMATE TODO: Option 2: Account Authentication Business Flows
            if (choice == 2) {
                //
            }

            if (choice == 0) { 
                //
                break; 
            }
        }
    }
};

int main() {
    SystemEngine engine;
    engine.initSystem();
    engine.run();
    return 0;
}

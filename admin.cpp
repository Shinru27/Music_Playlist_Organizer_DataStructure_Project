#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>

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
//   3 Derived Modules & Friend Function 4 - yx/ys/ex/js - 
// -------------------------------------------------------------------

// Derived Class 1: Administrative Control Framework - Eexuan / Jason
class Admin : public User {
public:
    Admin() : User("", "", "Admin") {}
    Admin(string u, string p) : User(u, p, "Admin") {}
    ~Admin() override {}

    // TEAMMATE TODO (Eexuan/Jason): Hook these functions to complete administrative assignments
    void displayMenu() override;
    void adminMenu(DoublyLinkedList& pool);
    void addSongToPool(DoublyLinkedList& pool);
    void editSongInPool(DoublyLinkedList& pool);
    void displayMusicPool(const DoublyLinkedList& pool);
    void searchSongInPool(const DoublyLinkedList& pool);
    void sortMusicPool(DoublyLinkedList& pool);
    void deleteSongFromPool(DoublyLinkedList& pool);
    void generateSystemReport(const DoublyLinkedList& pool); 
};


/* =====================================================================
   ADMIN MODULE
   ===================================================================== */
void saveMusicPoolToFile(const DoublyLinkedList& pool)
{
    ofstream file("music_pool.txt");

    Node* current = pool.getHead();

    while(current != nullptr)
    {
        file << current->data << endl;
        current = current->next;
    }

    file.close();
}

// Admin Modules - Eexuan/Jason 
void Admin::displayMenu()
{
    cout << "\nAdmin menu requires music pool access.\n";
}

void Admin::adminMenu(DoublyLinkedList& pool)
{
    int choice = -1;

    while (true)
    {
        cout << "\n===============================" << endl;
        cout << "    STAFF / ADMIN MODULE MENU  " << endl;
        cout << "===============================" << endl;
        cout << "1. Add New Song" << endl;
        cout << "2. Edit/Update Song" << endl;
        cout << "3. Display Music Pool" << endl;
        cout << "4. Search Song" << endl;
        cout << "5. Sort Music Pool" << endl;
        cout << "6. Delete Song" << endl;
        cout << "7. Summary Report" << endl;
        cout << "0. Logout & Exit" << endl;
        cout << "Enter selection option (0-9): ";
        cin >> choice;

        if (choice == 0)
        {
            cout << "[System] Logging out from admin session.\n";
            break;
        }

        switch (choice)
        {
            case 1: addSongToPool(pool); break;
            case 2: editSongInPool(pool); break;
            case 3: displayMusicPool(pool); break;
            case 4: searchSongInPool(pool); break;
            case 5: sortMusicPool(pool); break;
            case 6: deleteSongFromPool(pool); break;
            case 7: generateSystemReport(pool); break;
            default: cout << "[Warning] Invalid choice.\n"; break;
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

            saveMusicPoolToFile(pool);
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
    
    cout << left
         << setw(6)  << "ID"
         << setw(25) << "Title"
         << setw(20) << "Artist"
         << setw(15) << "Genre"
         << setw(12) << "Popularity"
         << setw(10) << "Duration" << endl;
    cout << string(88, '-') << endl;

    while (current != nullptr)
    {
        cout << left
             << setw(6)  << current->data.id
             << setw(25) << current->data.title
             << setw(20) << current->data.artist
             << setw(15) << current->data.genre
             << setw(12) << current->data.popularity
             << setw(10) << current->data.duration
             << endl;

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
	saveMusicPoolToFile(pool);
	
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



// -------------------------------------------------------------------
//   Central Orchestration Driver Engine - yx/ys/ex/js - 
// -------------------------------------------------------------------
class SystemEngine {
private:
    DoublyLinkedList globalMusicPool; 
    User* currentUser; 
	
	string adminUsername;
	string adminPassword;
		
public:
    SystemEngine() : currentUser(nullptr), adminUsername("admin"), adminPassword("1234") {}
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
            if (choice == 2)
			{
			    string username;
			    string password;
			
			    cout << "\n========== ADMIN LOGIN ==========\n";
			    cout << "Username: ";
			    cin >> username;
			
			    cout << "Password: ";
			    cin >> password;
			
			    if (username == adminUsername && password == adminPassword)
			    {
			        cout << "\n[Success] Admin login successful!\n";
			
			        Admin admin(username, password);
			        
					admin.adminMenu(globalMusicPool);
			    }
			    else
			    {
			        cout << "\n[Error] Invalid username or password!\n";
			    }
			}
            if (choice == 0) { 
                //
                break; 
            }
        }
    }
};


/* =====================================================================
   CUSTOMER MODULE
   ===================================================================== */
   
// =====================================================================
//  Member A's own Quick Sort & Binary Search (used only inside Customer)
// =====================================================================

// ---- Quick Sort (structure follows lecture template: Swap / Partition / QuickSort) ----
void swapSong(int x, int y, Song a[])
{
    Song temp;
    temp = a[x];
    a[x] = a[y];
    a[y] = temp;
}

int partitionSong(int left, int right, Song a[], int criteria)
{
    Song pivot;
    int i, pivotpos;
    swapSong(left, (left + right) / 2, a);  // move middle element to front as pivot
    pivot = a[left];
    pivotpos = left;
    for (i = left + 1; i <= right; i++)
    {
        bool smaller;
        if      (criteria == 1) smaller = (a[i].title    < pivot.title);     // sort by Title
        else if (criteria == 2) smaller = (a[i].duration < pivot.duration);  // sort by Duration
        else                    smaller = (a[i].id       < pivot.id);        // sort by ID (criteria == 3)

        if (smaller)
            swapSong(++pivotpos, i, a);
    }
    swapSong(left, pivotpos, a);
    return pivotpos;
}

void quickSortSong(int left, int right, Song a[], int criteria)
{
    int pivotpos;
    if (left < right)
    {
        pivotpos = partitionSong(left, right, a, criteria);
        quickSortSong(left, pivotpos - 1, a, criteria);
        quickSortSong(pivotpos + 1, right, a, criteria);
    }
}

// ---- Binary Search (structure follows lecture template: first/last/mid/found) ----
int binarySearchSongById(Song table[], int size, int target)
{
    int first, last, mid, found;
    first = 0;
    last  = size - 1;
    found = 0;
    while ((found == 0) && (first <= last))
    {
        mid = (first + last) / 2;
        if (target == table[mid].id)
            found = 1;
        else
        {
            if (target < table[mid].id)
                last  = mid - 1;
            else
                first = mid + 1;
        }
    }
    if (found) return mid;
    else       return -1;
}

int binarySearchSongByTitle(Song table[], int size, string target)
{
    int first, last, mid, found;
    first = 0;
    last  = size - 1;
    found = 0;
    while ((found == 0) && (first <= last))
    {
        mid = (first + last) / 2;
        if (target == table[mid].title)
            found = 1;
        else
        {
            if (target < table[mid].title)
                last  = mid - 1;
            else
                first = mid + 1;
        }
    }
    if (found) return mid;
    else       return -1;
}

/* =====================================================================
   CUSTOMER MODULE
   ===================================================================== */
 
// Derived Class 2: Customer
class Customer : public User, public MediaItem {
 
protected:
    DoublyLinkedList myPlaylist;   // customer's own playlist
    string playlistName;           // name of the playlist
    DoublyLinkedList* poolPtr;     // points to the shared music pool so customers can search/add songs from it
 
public:
 
    // default constructor
    Customer() : User("", "", "Customer"), playlistName("My Playlist"), poolPtr(nullptr) {
        role = "Normal_Customer";
    }
 
    // constructor with username + password, used when registering or logging in
    Customer(string u, string p) : User(u, p, "Customer"), playlistName("My Playlist"), poolPtr(nullptr) {
        role = "Normal_Customer";
    }
 
    ~Customer() override {}
 
    //function declaration
    void displayMenu() override;
    void createPlaylist();
    bool addSongToPlaylist(DoublyLinkedList& globalPool);
    void editPlaylistName();
    void displayPlaylist();
    void searchInPlaylist();
    void sortPlaylist();
    void removeSongFromPlaylist();
    void generatePlaylistSummary(ofstream& file);
    void viewSavedSummary();
 
    // called right after login so customer can reach the shared music pool
    void setGlobalPool(DoublyLinkedList* pool) {
        poolPtr = pool;
    }
 
    // a normal account can only keep 15 songs in the playlist
    // (Overridden Function 1) PremiumCustomer will override this with a bigger number
    virtual int getPlaylistLimit() {
        return 15;
    }
 
    // save the playlist summary into the txt.file
    // (Overridden Function 2)
    void exportToTXT(ofstream& file) override {
        generatePlaylistSummary(file);
    }
 
    // plays a song and shows an ad first
    // (Overridden Function 3) PremiumCustomer will override this to skip the ad
    virtual void playMusic(Song& s) {
        s.popularity++;  //every time a song is played, its popularity count goes up by 1
        cout << "\n[Ad] Playing a short ad before your song..." << endl;
        cout << "Now playing: " << s.title << "  (Total plays: " << s.popularity << ")" << endl;
    }
 
    // Friend Function 4 - saving/loading customer's playlist
    friend void savePlaylistToFile(ofstream& file, const Customer& customer);
    friend void loadPlaylistFromFile(ifstream& file, Customer& customer, DoublyLinkedList& globalPool);
};
 
 
// -------- PremiumCustomer (wong / Member B will add this part) --------
//Premium Customer Derived Class
class PremiumCustomer : public Customer {
	public:
	//default constructor
	PremiumCustomer(): Customer () {
		role = "Premium_Customer";
	}
	
	//constructore with username and password
	PremiumCustomer (string u, string p) : 
	Customer (u, p){
		role = "Premium_Customer";
	}
	
	~PremiumCustomer() override {}
	
	
	//Override 1: 
	int getPlaylistLimit() 
	override {
		return 50;
	}
	
	//Override 2: VIP users skip the ad
	//Premium Customer no ads
	void playMusic(Song &s) 
	override {
		s.popularity++;
		cout<<"\n [VIP Exclusive] Playing songs smoothly without ads..."<<endl;
		cout<<"Now playing: "<<s.title<<" (TOTAL PLAYS: "<<s.popularity<<")"<<endl;
		
	}
	
}; 

//try catch 
class CustomerException {
	private:
    string message;
public:
    CustomerException(string msg) : message(msg) {}
    string what() const 
	{ return message; }
};



// Friend 4: save / load playlist to and from playlists.txt 
// only save the song IDs, the full song details already stored in music_pool.txt
void savePlaylistToFile(ofstream& file, const Customer& customer) {
    // Step 1: write the username, playlist name, song count
    
    file << customer.username << "|" 
         << customer.playlistName << "|" 
         << customer.myPlaylist.getSize() << "|";
 
    // Step 2: walk through the linked list and write each song's ID, one by one
    Node* curr = customer.myPlaylist.getHead();
    while (curr != nullptr) {
        file << curr->data.id << " ";
        curr = curr->next;
    }
    file << endl;
}
 
// Reads back one playlist entry from playlists.txt and rebuilds it for this customer. 
void loadPlaylistFromFile(ifstream& file, Customer& customer, DoublyLinkedList& globalPool) {
    string line;
    if (!getline(file, line)) return;
 
    // Step 1: try to read username, playlist name, and song count from the file 
    int pos1 = -1, pos2 = -1, pos3 = -1;
    for (int i = 0; i < (int)line.size(); i++) {
        if (line[i] == '|') {
            if      (pos1 == -1) pos1 = i;
            else if (pos2 == -1) pos2 = i;
            else if (pos3 == -1) { pos3 = i; break; }
        }
    }
    if (pos1 == -1 || pos2 == -1 || pos3 == -1) return;

    string uname  = line.substr(0, pos1);
    string pName  = line.substr(pos1 + 1, pos2 - pos1 - 1);
    string countStr = line.substr(pos2 + 1, pos3 - pos2 - 1);
    string songPart = line.substr(pos3 + 1);

    // 手动把 countStr 转成 int
    int songCount = 0;
    for (int i = 0; i < (int)countStr.size(); i++) {
        if (countStr[i] >= '0' && countStr[i] <= '9')
            songCount = songCount * 10 + (countStr[i] - '0');
    }

    customer.playlistName = pName;
    customer.myPlaylist.clear();
 
        // Step 2: read each song ID one at a time, then look it up in the shared pool and add a copy of it into this customer's playlist
        int songId = 0;
	    bool reading = false;
	    for (int i = 0; i <= (int)songPart.size(); i++) {
	        char c = (i < (int)songPart.size()) ? songPart[i] : ' ';
	        if (c >= '0' && c <= '9') {
	            songId = songId * 10 + (c - '0');
	            reading = true;
	        } else if (reading) {
	            customer.myPlaylist.addSong(songId, globalPool);
	            songId = 0;
	            reading = false;
	        }
	    }
    }
 
 
//  Customer member functions (Member A) 
// customer create a new playlist
void Customer::createPlaylist() {
    // Step 1: don't allow creating a new playlist if there are already songs in it
    if (myPlaylist.getSize() > 0) {
        cout << "[Error] You already have songs in your playlist." << endl;
        cout << "        Remove all songs first before creating a new playlist." << endl;
        return;
    }
    
    // Step 2: ask the user for a new playlist name
    cout << "Enter a name for your new playlist: ";
    cin.ignore();
    string name;
    getline(cin, name);
 
    // Step 3: make sure the name they typed isn't empty
    if (name.empty()) {
        cout << "[Error] Playlist name cannot be blank." << endl;
        return;
    }
 
    // Step 4: save the new name and tell the user it worked
    playlistName = name;
    cout << "[Done] Playlist \"" << playlistName << "\" has been created!" << endl;
}

// customer add one song ID from the shared pool into their own playlist. 
bool Customer::addSongToPlaylist(DoublyLinkedList& globalPool) {
    
    try {
    	// Step 1: check if the playlist has already reached its limit
	    if (myPlaylist.getSize() >= getPlaylistLimit()) {
	        throw CustomerException(
	         "[Error] Your playlist is full. Max songs allowed: " + 
			 to_string (getPlaylistLimit()));
	    }
	 
	    int id;
	    int poolSize = globalPool.getSize();

	    while (true) {

	    	// Step 2: ask the user which song ID they want to add
	    	cout << "Enter the Song ID you want to add: ";
		    cin >> id;
		 
		    // Step 3: make sure they actually typed a number
		    if (cin.fail()) {
		        cin.clear();
		        cin.ignore(1000, '\n');
		        cout << "[Notice] Please enter a valid number. Try again.\n";
        		continue;
		    }
	    
	    	// Step 4: check the customer's own playlist first and don't add the same song twice
	    	Node* curr = myPlaylist.getHead();
	    	bool alreadyInPlaylist = false;
	    	while (curr != nullptr) {
	        	if (curr->data.id == id) {
	            	alreadyInPlaylist = true;
	            	break;
	        	}
	        	curr = curr->next;
	    	}
	    
	    	if (alreadyInPlaylist) {
            	cout << "[Notice] Song ID " << id << " is already in your playlist. Try again.\n";
            	continue;   
        	}
	    
	    	Song* arr = new Song[poolSize];
	 
	    	// Step 5: search the shared music pool for a song that matches this ID
	    	Node* poolCurr = globalPool.getHead();
	    	int idx = 0;
	    	while (poolCurr != nullptr) {
	        	arr[idx++] = poolCurr->data;
	        	poolCurr = poolCurr->next;
	    	}
	    
	    	quickSortSong (0, poolSize - 1, arr, 3);
	    	int result = binarySearchSongById (arr, poolSize, id);
	    
	    	// Step 6: if we went through the whole pool and never found it, tell the user
	    	if (result == -1) {
	    		delete [] arr;
	       		cout << "[Notice] Song ID " << id << " does not exist in the music pool. Try again.\n";
        		continue;
	    	}
	 	
	 		myPlaylist.append(arr[result]);
	 		delete[] arr;
	    	cout << "[Done] Song added to your playlist." << endl;
	    	
	    	
	    	//show current playlist count vs limit
	    	cout << "[Info] You now have " << myPlaylist.getSize() 
	    	     << " / " << getPlaylistLimit() << " songs in your playlist." << endl;
	    	     
	    	break;   
	    }
	    
	    return false;

	}
	catch (const CustomerException &e) {
		cout<<"[Notice] " <<e.what() <<endl;
		return true;
	}
}

// let customer rename their playlist
void Customer::editPlaylistName() {
    cout << "Current playlist name: " << playlistName << endl;
    cout << "Enter new name: ";
    cin.ignore();
    string newName;
    getline(cin, newName);
 
    if (newName.empty()) {
        cout << "[Error] Playlist name cannot be blank." << endl;
        return;
    }
 
    playlistName = newName;  // overwrite the old name with the new one
    cout << "[Done] Playlist renamed to \"" << playlistName << "\"." << endl;
}
 
// Shows every song currently in the customer's playlist. 
void Customer::displayPlaylist() {
    // if there's nothing in the list, just stop here
    if (myPlaylist.getSize() == 0) {
        cout << "[Info] Your playlist is empty." << endl;
        return;
    }
 
    cout << "\n--- " << playlistName << " (" << myPlaylist.getSize() << " songs) ---" << endl;
    
    cout << left
         << setw(4)  << "No."
         << setw(6)  << "ID"
         << setw(25) << "Title"
         << setw(20) << "Artist"
         << setw(15) << "Genre"
         << setw(8)  << "Plays"
         << setw(8)  << "Sec" << endl;
    cout << string(86, '-') << endl;
    
    
    // print one song per line from the head to tail of linked list
    Node* curr = myPlaylist.getHead();
    int num = 1;
    while (curr != nullptr) {
        cout << left
             << setw(4)  << num
             << setw(6)  << curr->data.id
             << setw(25) << curr->data.title
             << setw(20) << curr->data.artist
             << setw(15) << curr->data.genre
             << setw(8)  << curr->data.popularity
             << setw(8)  << curr->data.duration
             << endl;
        curr = curr->next;  // move on to the next node in the list
        num++;
    }
}

// customer search for a song inside their own playlist, either by ID or by title.
void Customer::searchInPlaylist() {
    // nothing to search if the playlist is empty
    if (myPlaylist.getSize() == 0) {
        cout << "[Info] Your playlist is empty. Nothing to search." << endl;
        return;
    }

    cout << "Search by:" << endl;
    cout << "1. Song ID" << endl;
    cout << "2. Song Title" << endl;
    cout << "Choice: ";
    int option;
    cin >> option;

    // Step 1: dump linked list into array (needed for binary search's random access)
    int size = myPlaylist.getSize();
    Song* arr = new Song[size];         // Member A's own dynamic memory allocation
    Node* curr = myPlaylist.getHead();  // using Shinru's public getHead()
    int idx = 0;
    while (curr != nullptr) {
        arr[idx++] = curr->data;
        curr = curr->next;
    }

    if (option == 1) {
        cout << "Enter Song ID: ";
        int targetId;
        cin >> targetId;

        // sort by ID first before binary search 鈥?using Member A's own quickSortSong (criteria 3)
        quickSortSong(0, size - 1, arr, 3);

        int result = binarySearchSongById(arr, size, targetId);
        if (result == -1) {
            cout << "[Error] Song ID " << targetId << " not found in your playlist." << endl;
        } else {
            cout << "[Found] "
                 << "ID: "    << arr[result].id         << " | "
                 << arr[result].title                   << " | "
                 << arr[result].artist                  << " | "
                 << arr[result].genre                   << " | "
                 << "Plays: " << arr[result].popularity << " | "
                 << arr[result].duration << "s" << endl;
        }
    }
    else if (option == 2) {
        cout << "Enter Song Title (use _ for spaces, e.g. Shape_of_You): ";
        string title;
        cin >> title;

        // sort by title first before binary search 鈥?using Member A's own quickSortSong (criteria 1)
        quickSortSong(0, size - 1, arr, 1);

        int result = binarySearchSongByTitle(arr, size, title);
        if (result == -1) {
            cout << "[Error] \"" << title << "\" not found in your playlist." << endl;
        } else {
            cout << "[Found] "
                 << "ID: "    << arr[result].id         << " | "
                 << arr[result].title                   << " | "
                 << arr[result].artist                  << " | "
                 << arr[result].genre                   << " | "
                 << "Plays: " << arr[result].popularity << " | "
                 << arr[result].duration << "s" << endl;
        }
    }
    else {
        cout << "[Error] Invalid option." << endl;
    }

    delete[] arr;   // Member A's own dynamic memory deallocation
}

// customer sort their playlist using alphabet or song length
void Customer::sortPlaylist() {
    if (myPlaylist.getSize() <= 1) {
        cout << "[Error] Need at least 2 songs to sort." << endl;
        return;
    }

    cout << "Sort by:" << endl;
    cout << "1. Title (A to Z)" << endl;
    cout << "2. Duration (shortest first)" << endl;
    cout << "Choice: ";
    int option;
    cin >> option;

    if (option != 1 && option != 2) {
        cout << "[Error] Invalid option." << endl;
        return;
    }

    // Step 1: dump linked list (Shinru's DoublyLinkedList) into a plain array
    int size = myPlaylist.getSize();
    Song* arr = new Song[size];         // Member A's own dynamic memory allocation
    Node* curr = myPlaylist.getHead();  // using Shinru's public getHead()
    int idx = 0;
    while (curr != nullptr) {
        arr[idx++] = curr->data;
        curr = curr->next;
    }

    // Step 2: run Member A's own Quick Sort
    // option 1 = sort by Title (criteria 1), option 2 = sort by Duration (criteria 2)
    quickSortSong(0, size - 1, arr, option);

    // Step 3: clear the list and rebuild it in sorted order
    myPlaylist.clear();                 // using Shinru's public clear()
    for (int i = 0; i < size; i++) {
        myPlaylist.append(arr[i]);      // using Shinru's public append()
    }
    delete[] arr;                       // Member A's own dynamic memory deallocation

    cout << "[Done] Sorted by "
         << (option == 1 ? "title (A to Z)." : "duration (shortest first).") << endl;
    displayPlaylist();
}

// customer remove one song from their playlist by typing its ID. 
void Customer::removeSongFromPlaylist() {
    if (myPlaylist.getSize() == 0) {
        cout << "[Error] Your playlist is already empty." << endl;
        return;
    }
 
    cout << "Enter Song ID to remove: ";
    int id;
    cin >> id;
 
    int sizeBefore = myPlaylist.getSize();
    myPlaylist.removeNode(id);
 
    if (myPlaylist.getSize() == sizeBefore) {
        cout << "[Error] Song ID " << id << " is not in your playlist." << endl;
        return;
    }
 
    cout << "[Done] Song removed from playlist." << endl;
}

// Builds a short summary of the playlist (total songs, total time, total plays),
// and displays it and saves it into a txt file at the same time. 
void Customer::generatePlaylistSummary(ofstream& file) {
    // make sure the file was actually opened successfully before writing to it
    if (!file.is_open()) {
        cout << "[Error] Cannot open file to save summary." << endl;
        return;
    }
 
    // Step 1: add up the duration and play count of every song in the playlist
    int totalDuration = 0;
    int totalPlays = 0;
 
    Node* curr = myPlaylist.getHead();
    while (curr != nullptr) {
        totalDuration += curr->data.duration;
        totalPlays += curr->data.popularity;
        curr = curr->next;
    }
 
    // Step 2: display the summary on screen for the user to see right now
    cout << "\n============================" << endl;
    cout << "     PLAYLIST SUMMARY       " << endl;
    cout << "============================" << endl;
    cout << "Owner       : " << username << endl;
    cout << "Playlist    : " << playlistName << endl;
    cout << "Total Songs : " << myPlaylist.getSize() << endl;
    cout << "Total Time  : " << totalDuration << " seconds" << endl;
    cout << "Total Plays : " << totalPlays << endl;
    cout << "============================" << endl;
 
    // Step 3: write the exact same summary into the txt file so it can be read again later
    file << "============================" << endl;
    file << "     PLAYLIST SUMMARY       " << endl;
    file << "============================" << endl;
    file << "Owner       : " << username << endl;
    file << "Playlist    : " << playlistName << endl;
    file << "Total Songs : " << myPlaylist.getSize() << endl;
    file << "Total Time  : " << totalDuration << " seconds" << endl;
    file << "Total Plays : " << totalPlays << endl;
    file << "============================" << endl;
 
    cout << "[Done] Summary saved to customer_summary.txt" << endl;
}
 
// Opens customer_summary.txt and prints whatever was saved in it earlier. 
void Customer::viewSavedSummary() {
    ifstream inFile("customer_summary.txt");
    if (!inFile.is_open()) {
        cout << "[Error] No saved summary found." << endl;
        cout << "        Please generate one first using menu option 8." << endl;
        return;
    }
 
    cout << "\n--- Reading from customer_summary.txt ---" << endl;
    string line;
    while (getline(inFile, line)) {
        cout << line << endl;
    }
    inFile.close();
}
 
// The main menu loop for a logged-in customer.
// It keeps showing the menu and asking for a choice until the customer logs out (enters 0) 
void Customer::displayMenu() {
    int choice;
 
    while (true) {
        cout << "\n===============================" << endl;
        cout << "      CUSTOMER MODULE MENU     " << endl;
        cout << "===============================" << endl;
        cout << "1. Create New Playlist" << endl;
        cout << "2. Add Song to Playlist" << endl;
        cout << "3. Edit Playlist Name" << endl;
        cout << "4. Display My Playlist" << endl;
        cout << "5. Search in Playlist" << endl;
        cout << "6. Sort My Playlist" << endl;
        cout << "7. Remove Song from Playlist" << endl;
        cout << "8. Save Summary Report to File" << endl;
        cout << "9. View Saved Summary from File" << endl;
        cout << "10. Play a Song"<<endl;
        cout << "0. Logout" << endl;
        cout << "Enter choice: ";
 
        // try catch
        try {
        	cin>>choice;
        	if (cin.fail()) {
        		throw CustomerException ("Please enter the choice between 0 to 10");
			}
			if (choice < 0|| choice > 10) {
				throw CustomerException ("Please enter the choice between 0 to 10");
			}
			
		}
		catch (const CustomerException &e) {
			cout<<"[Notice] "<<e.what()<<endl;
			cin.clear();
			cin.ignore(1000, '\n');
			continue;
		}
		
		if (choice == 0)
		{
			cout <<"[System] Logging out ..." <<endl;
			break;
		}
        
 
        // call the matching function based on what the customer picked
        switch (choice) {
            case 1:
                createPlaylist();
                break;
            case 2:
                // only try to add a song if the shared music pool was actually linked in
                if (poolPtr != nullptr) {
                	bool keepAdding = true;
                	
                	while (keepAdding) {
                	//display all the songs
                	cout << "\n========== AVAILABLE SONGS ==========\n\n";
                	cout << left
                	     << setw(4)  << "No."
                	     << setw(6)  << "ID"
                	     << setw(25) << "Title"
                	     << setw(18) << "Artist"
                	     << setw(15) << "Genre"
                	     << setw(10) << "Duration" << endl;
                	cout << string(78, '-') << endl;
                	
			        Node* curr = poolPtr->getHead();
			        int num = 1;
			        while (curr != nullptr) {
			            cout << left
			                 << setw(4)  << num
			                 << setw(6)  << curr->data.id
			                 << setw(25) << curr->data.title
			                 << setw(18) << curr->data.artist
			                 << setw(15) << curr->data.genre
			                 << setw(10) << (to_string(curr->data.duration) + "s")
			                 << endl;
			            curr = curr->next;
			            num++;
			        }
			        cout << "\n\n=====================================\n\n";
                    
                    bool isFull = addSongToPlaylist (*poolPtr);
                    
                    //if playlist full
                    if (isFull)
                    {
                    	keepAdding = false;
                    	break;
					}
					
                    
                    //ask user whether they want to add another song
                    char again;
                    while (true) {
                    	cout << "\nDo you want to add another song? (y/n): ";
		                cin >> again;
		
		                if (cin.fail()) {
		                    cin.clear();
		                    cin.ignore(1000, '\n');
		                    cout << "[Notice] Please enter y or n.\n";
		                    continue;
		                }
		                if (again != 'y' && again != 'Y' && again != 'n' && again != 'N') {
		                    cout << "[Notice] Please enter y or n.\n";
		                    continue;
		                    
		                }
		                break;
		            }
		
			            if (again == 'n' || again == 'N') {
			                keepAdding = false;
			            }
					}
            	}else {
            		cout << "[Error] Music pool is not loaded." << endl;
				}
                break;
            case 3:
                editPlaylistName();
                break;
            case 4:
                displayPlaylist();
                break;
            case 5:
                searchInPlaylist();
                break;
            case 6:
                sortPlaylist();
                break;
            case 7:
                removeSongFromPlaylist();
                break;
            case 8: {
                // open the summary file in "append" mode so old summaries aren't deleted
                ofstream f("customer_summary.txt", ios::app);
                generatePlaylistSummary(f);
                f.close();
                break;
            }
            case 9:
                viewSavedSummary();
                break;
            case 10:{
            	if (myPlaylist.getSize() == 0) {
            		cout<<"[Info] Your playlist is empty." <<endl;
            		break;
				}
				cout<<"Enter Song ID to play: ";
				
				int pid;
				cin>>pid;
				Node* curr = myPlaylist.getHead();
				bool pfound = false;
				while (curr != nullptr) {
					if (curr->data.id == pid){
						playMusic(curr->data);
						pfound = true;
						break;
					}
					curr = curr->next;
				}
				if (!pfound)
				cout<<"[Erorr] Song ID "<<pid<<" not in your playlist."<<endl;
				break;
			}
            default:
                cout << "[Error] Invalid option. Please try again." << endl;
                break;
        }
    }
}



 
// ---------- helper functions for register / login (Member A) ----------
// Shinru's saveUserToFile() locks the password with XOR 0x5A before saving.
// XOR-ing with the same key a second time undoes it, that's why this just XORs again.
string decryptPassword(string enc) {
    string dec = enc;
    for (int i = 0; i < (int)dec.length(); i++) {
        dec[i] ^= 0x5A;
    }
    return dec;
}
 
// creates a new Normal_Customer account and appends it to users.txt
User* registerCustomer() {
    cout << "\n--- Register New Account ---" << endl;
    cout << "Enter username: ";
    string uname;
    cin >> uname;
 
    // check if this username already exists
    ifstream checkFile("users.txt");
    string u, p, r;
    bool taken = false;
 
    while (checkFile >> u >> p >> r) {
        if (u == uname) {
            taken = true;
            break;
        }
    }
    checkFile.close();
 
    if (taken) {
        cout << "[Error] That username is already taken. Please try another one." << endl;
        return nullptr;
    }
 
    // ask for password
    cout << "Enter password: ";
    string pass;
    cin >> pass;
    
    //
    
    //ask user which account type they want
    int accType = 0;
    while (true) {
    	cout<<"\n\nSelect account type: \n";
    	cout << "1. Normal_Customer (15 songs limit, ads while playing)\n";
        cout << "2. Premium_Customer (50 songs limit, no ads)\n";
        cout << "Enter choice: ";
        cin>>accType;
        
        if (cin.fail()) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[Notice] Please enter 1 or 2.\n";
            continue;
        }
        if (accType != 1 && accType != 2) {
            cout << "[Notice] Please enter 1 or 2.\n";
            continue;
        }
        break;
        
        cout<<endl;
	}
	
 
    // make a temporary Customer Customer/PremiumCustomer just so we can reuse
    Customer* newUser  = nullptr;
    if (accType == 1) {
    	newUser = new Customer (uname,pass);
	}else {
		newUser = new PremiumCustomer(uname, pass);
	}
 
    // open users.txt in "append" mode so we add to the end instead of overwriting it
    ofstream outFile("users.txt", ios::app);
    if (!outFile.is_open()) {
        cout << "[Error] Cannot open users.txt to save account." << endl;
        delete newUser;
        return nullptr;
    }
 
    saveUserToFile(outFile, *newUser);
    outFile.close();
 
    delete newUser; 
 
    cout << "[Done] Account created! You can now log in." << endl;
    return nullptr;
}
 
// logs in a Customer or Premium_Customer and loads their saved playlist
User* loginCustomer(DoublyLinkedList& globalPool) {
    cout << "\n--- Customer Login ---" << endl;
    cout << "Username: ";
    string uname;
    cin >> uname;
    cout << "Password: ";
    string pass;
    cin >> pass;
 
    // Step 1: open users.txt to look for this username
    ifstream userFile("users.txt");
    if (!userFile.is_open()) {
        cout << "[Error] Cannot open users.txt." << endl;
        return nullptr;
    }
 
    string fileUser, fileEncPass, fileRole;
    User* loggedIn = nullptr;
    bool found = false;
 
    // Step 2: read the file line by line until we find a matching username
    while (userFile >> fileUser >> fileEncPass >> fileRole) {
		if (fileUser == uname) {
            found = true;
 
            // Step 3: decrypt the saved password and compare it with what was typed
            if (decryptPassword(fileEncPass) != pass) {
                cout << "[Error] Wrong password." << endl;
                userFile.close();
                return nullptr;
            }
            
            // Step 4: create the right kind of account object based on their saved role
            if (fileRole == "Normal_Customer") {
                loggedIn = new Customer(uname, pass);
            }
            else if (fileRole == "Premium_Customer") {
                //  once PremiumCustomer class is ready, change the line below to
                //     loggedIn = new PremiumCustomer(uname, pass);
                loggedIn = new PremiumCustomer(uname, pass); // temporary, until PremiumCustomer is added
            }
            else {
                cout << "[Error] This is an Admin account. Please use the Admin login." << endl;
                userFile.close();
                return nullptr;
            }
            break;
        }
        
        
    }
    userFile.close();
 
    if (!found) {
        cout << "[Error] Username not found." << endl;
        return nullptr;
    }
 
    // Step 5: link the shared music pool to this customer, then load their saved playlist
    Customer* custPtr = dynamic_cast<Customer*>(loggedIn);
    if (custPtr != nullptr) {
        custPtr->setGlobalPool(&globalPool);
 
        // open playlists.txt and look for the line that belongs to this username
        ifstream playlistFile("playlists.txt");
        if (playlistFile.is_open()) {
            bool playlistFound = false;
            string lineUser;
 
            while (true) {
                streampos pos = playlistFile.tellg();
			    string line;
			    if (!getline(playlistFile, line)) break;
			    if (line.empty()) continue;
			
			    
			    int pipePos = -1;
			    for (int i = 0; i < (int)line.size(); i++) {
			        if (line[i] == '|') { pipePos = i; break; }
			    }
			    if (pipePos == -1) continue;
			
			    string lineUser = line.substr(0, pipePos);
			
			    if (lineUser == uname) {
			        // read again the playlist file
			        playlistFile.seekg(pos);
			        loadPlaylistFromFile(playlistFile, *custPtr, globalPool);
			        playlistFound = true;
			        break;
			    }
            }
            playlistFile.close();
 
            if (!playlistFound) {
                cout << "[Info] No saved playlist found. Starting fresh." << endl;
            }
        }
    }
 
    cout << "[Done] Welcome, " << uname << "!" << endl;
    return loggedIn;
}
 
// saves this customer's playlist back into playlists.txt when they log out
// rewrites the whole file through a temp file so other users' data stays untouched
void updatePlaylistFile(Customer& customer) {
    ifstream inFile("playlists.txt");
    ofstream tempFile("temp_playlists.txt");
 
    if (!tempFile.is_open()) {
        cout << "[Error] Cannot create temp file to save playlist." << endl;
        return;
    }
 
    bool saved = false;
 
    // Step 1: copy every other user's playlist as-is, but replace this customer's old playlist line with their newest data
    if (inFile.is_open()) {
        string line;
	    while (getline(inFile, line)) {
	        if (line.empty()) continue;
	
	        // find the first | users
	        int pos = -1;
	        for (int i = 0; i < (int)line.size(); i++) {
	            if (line[i] == '|') { pos = i; break; }
	        }
	        if (pos == -1) continue;
	
	        string lineUser = line.substr(0, pos);
	
	        if (lineUser == customer.getUsername()) {
	            // write the new customer
	            savePlaylistToFile(tempFile, customer);
	            saved = true;
	        }
	        else {
	            // copy others data
	            tempFile << line << endl;
	        }
	    }
	    inFile.close();
    }
    // Step 2: if this is the first time saving, just add a brand new one at the end
    if (!saved) {
        savePlaylistToFile(tempFile, customer);
    }
 
    tempFile.close();
    // Step 3: replace the old playlists.txt with the new updated temp file
    remove("playlists.txt");
    rename("temp_playlists.txt", "playlists.txt");
 
    cout << "[System] Playlist saved to file." << endl;
}

DoublyLinkedList globalMusicPool; // shared pool, loaded once when the program starts
 
// reads music_pool.txt and loads every song into globalMusicPool
void loadMusicPool() {
    ifstream file("music_pool.txt");
    if (!file.is_open()) {
        cout << "[System Warning] music_pool.txt not found. Starting with an empty pool." << endl;
        return;
    }
 
    Song s;
    while (file >> s) {              // uses Shinru's operator>> overload
        globalMusicPool.append(s);   // load it straight into the shared pool
    }
 
    file.close();
    cout << "[System] Loaded " << globalMusicPool.getSize() << " songs from music_pool.txt" << endl;
}

int main() {
    
    // Step 1: load all the songs from file before anything else happens
    loadMusicPool();
 
    int choice = -1;
 
    // Step 2: keep showing the main menu until the user chooses to exit
    while (true) {
        cout << "\n========================================" << endl;
        cout << "  MUSIC PLAYLIST ORGANIZER " << endl;
        cout << "========================================" << endl;
        cout << "1. Admin Login" << endl;
        cout << "2. Customer Register" << endl;
        cout << "3. Customer Login" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;
        
		
 
        if (cin.fail()) {
            cout << "[Notice] Please enter a number." << endl;
            cin.clear();
            string dummy;
            cin >> dummy;
            continue;
            system("cls");
        }else{
        	
        	//admin register and login
        if (choice == 1) {
        	
        	string username;
			string password;
			string adminUsername = "admin";
			string adminPassword = "1234"; 
			
			cout << "\n========== ADMIN LOGIN ==========\n";
			cout << "Username: ";
			cin >> username;
			
			cout << "Password: ";
			cin >> password;
			
			if (username == adminUsername && password == adminPassword)
			{
			    cout << "\n[Success] Admin login successful!\n";
			
			    Admin admin(username, password);
			        
				admin.adminMenu(globalMusicPool);
			}
			else
			{
			    cout << "\n[Error] Invalid username or password!\n";
			}
			system("cls");
		}
 
        else if (choice == 2) {
            registerCustomer();
        }
        else if (choice == 3) {
            // try to login, it will show the customer menu if successful
            User* loggedIn = loginCustomer(globalMusicPool);
 
            if (loggedIn != nullptr) {
                loggedIn->displayMenu(); // runs the Customer menu loop until they logout
 
                // after logging out, save the playlist back to playlists.txt
                Customer* custPtr = dynamic_cast<Customer*>(loggedIn);
                if (custPtr != nullptr) {
                    updatePlaylistFile(*custPtr);
                }
 
                delete loggedIn;  // free the memory for this account now that we're done with it
            }
        }
        else if (choice == 0) {
            cout << "[System] Goodbye!" << endl;
            break;
        }
        else {
            cout << "[Error] Invalid choice." << endl;
            
        }
        	
		}
        
        
    }
 
    return 0;
}

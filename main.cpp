/******************************************
*                                         *
*    A C++ Terminal-Based Text Editor     *
*                                         *
*              By Soumya Savarn             *
*                                         *
******************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
using namespace std;

// ANSI escape codes for text formatting
#define RESET_TEXT "\033[0m"          // Reset special formatting (such as color).
#define BLACK_TEXT "\033[30m"         // Black text.
#define RED_TEXT "\033[31m"           // Red text.
#define GREEN_TEXT "\033[32m"         // Green text.
#define YELLOW_TEXT "\033[33m"        // Yellow text.
#define BLUE_TEXT "\033[34m"          // Blue text.
#define MAGENTA_TEXT "\033[35m"       // Magenta text.
#define CYAN_TEXT "\033[36m"          // Cyan text.
#define WHITE_TEXT "\033[37m"         // White text.
#define WHITE_BACKGROUND "\033[47m"   // White background.
#define BLACK_BACKGROUND "\033[40m"   // Black background.

// Function to display the main menu and get user choice
int menu() {
    int choice;

//    system("CLS");  // Clear the screen
    
    cout << YELLOW_TEXT;
    cout << "\t\t\t\t\t     Developer: Soumya Savarn\n" << "\n" << endl;
    cout << CYAN_TEXT;
    cout << "1. Create a new file                   " << endl;
    cout << "2. Read and open from an existing file " << endl;
    cout << "3. Edit a line from a file             " << endl;
    cout << "4. Delete file                         " << endl;
    cout << "5. Compile code (C++)                  " << endl;
    cout << "0. Exit                                " << endl;
    cout << "Enter your choice . . .                " << endl;
    cout << "$ ";
    cin >> choice;

    return choice;
}

// Function to display the edit menu and get user choice
int editMenu() {
    int choice;
    cout << GREEN_TEXT;

    cout << "1. Edit Line"  << endl;
    cout << "2. Remove Line"  << endl;
    cout << "0. Back to menu"  << endl;
    cout << "Enter your choices . . .    ";
    cin >> choice;
    
    return choice;
}

// Function to prompt user for a file name
string askFileName(string query) {
    string fileName;
    cout << "Enter the name of the file and its extension to " << query << " : ";
    cin.ignore();
    getline(cin, fileName);
//    system("CLS");

    return fileName;
}

// Structure for linked list node
struct FileNode {
    string line;
    FileNode* next;
};

FileNode *head = NULL, *tail = NULL, *cur, *newNode, *nextNode, *del;

// TextEditor class for handling file operations
class TextEditor {
private:
    string file_Name;
    ofstream ofile;
    ifstream ifile;

public:
    TextEditor(string file_Name = "file.txt") { this->file_Name = file_Name; } // Constructor
    ~TextEditor(){}; // Destructor

    // Default functions (Create and read)
    void writeFile();           // Method for writing a file.
    bool readFile();            // Method for reading a file.

    // Edit functions with linked lists
    void createLineLinkedList(string);  // Make the first node of the linked list.
    void addFirstLine(string);          // Add a node to the first linked list, used for adding a line.
    void addMiddleLine(string, int);    // Add a node at the specified position in the linked list.
    void addLastLine(string);           // Add a node at the last position of the linked list.
    void removeLinkedList(FileNode*&); // Remove all nodes from the linked list.
    void printLinkedList();             // Print linked list to the file.
    int countLinkedList();              // Count nodes in the linked list.

    // Modify (Edit and Delete) and run it.
    void readFileEdit();                // Read a file to add into the linked list.
    void modifyFile();                  // A controller method for modifying the file.
    void editLine(int);                 // Edit a line at the specified position.
    void addLine();                     // Menu controller for adding a line.
    void removeLine();                  // Remove a line.
    void deleteFile();                  // Method for deleting a file.
    void compileCode();                 // Method for compiling a C++ file.
};

// Method to create a linked list node with the given string and set it as the head
void TextEditor::createLineLinkedList(string line) {
    head = new FileNode();
    head->line = line;
    head->next = NULL;
    tail = head; // Set tail to head, as this is the first node.
}

// Method to add a node as the first node of the linked list
void TextEditor::addFirstLine(string line) {
    newNode = new FileNode();
    newNode->line = line;
    newNode->next = head; // Set next of the new node to head because it's the first node.
    head = newNode;       // Set head as the new node.
}

// Method to add a new line at the specified position in the linked list
void TextEditor::addMiddleLine(string line, int position) {
    newNode = new FileNode();
    newNode->line = line;
    cur = head;          // Set the current node as head.
    int count = 1;       // Variable to count the loop until the specified position.
    while (count < position - 1) {
        cur = cur->next;  // Move to the next position.
        count++;
    }

    newNode->next = cur->next; // Set the new node's next to the current node's next.
    cur->next = newNode;       // Set the next of the current node to the new node.
}

// Method to add a line at the last position in the linked list
void TextEditor::addLastLine(string line) {
    newNode = new FileNode();
    newNode->line = line;
    newNode->next = nullptr;

    if (tail == nullptr) {
        head = newNode;
        tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
}

// Method to count all nodes in the linked list
int TextEditor::countLinkedList() {
    cur = head;
    int count = 1;
    while (cur->next != nullptr) {
        cur = cur->next;
        count++;
    }

    return count;
}

// Method to open a file and insert its content into the linked list for editing
void TextEditor::readFileEdit() {
    string line;

    ifile.open(file_Name);
    if (ifile.is_open()) {
        int iterator = 1;

        while (!ifile.eof()) {
            getline(ifile, line);
            if (iterator == 1)
                createLineLinkedList(line);
            else
                addLastLine(line);
            
            iterator++;
        }

        ifile.close();
    } else {
        cout << "Failed to open the file." << endl;
    }
}

// Menu controller function for modifying a file
void TextEditor::modifyFile() {
    if (readFile()) {
        bool breakPoint = true;
        while (breakPoint) {
            readFileEdit();
            switch (editMenu()) {
                

                case 1:
                    cout << "Enter the line number you want to edit: ";
                    int line;
                    cin >> line;
                    editLine(line);
                    break;

                case 2:
                    removeLine();
                    break;
                case 0:

                    breakPoint = false;
                    break;

                default:
                    break;
            }

            if (breakPoint) {
//                system("CLS");
                readFile();
            }
        }
    }
}


// Method to edit a line at the specified position
void TextEditor::editLine(int ch) {
    cur = head;
    int count = 1;
    // Find the line to edit.
    while (cur != nullptr && count < ch) {
        cur = cur ->next;
        count++;
    }

    // If not found, exit without making any changes.
    if (cur == nullptr) {
        cout << "Line not found" << endl;
//        system("PAUSE");
        return;
    } else {
        string newLine;
        cout << "Insert the new content: ";
        cin.ignore();
        getline(cin, newLine);
        cur->line = newLine;
        printLinkedList();
//        system("PAUSE");
    }
}

// Method to remove a line from the linked list
void TextEditor::removeLine() {
    readFileEdit();
    int ch, count = 1;
    cout << "Enter the line number you want to remove: ";
    cin >> ch;

    cur = head;
    del = nullptr;
    bool found = false;
    if (ch == 1) {
        head = head->next;
        delete cur;
        found = true;
    } else {
        while (cur != nullptr && count < ch - 1) {
            cur = cur->next;
            count++;
        }

        if (cur != nullptr && cur->next != nullptr) {
            del = cur->next;
            cur->next = del->next;
            delete del;
            found = true;
        }
    }

    if (found) {
        // Rewrite the file.
        printLinkedList();
        cout << "Line successfully removed." << endl;
//        system("PAUSE");
    } else {
        cout << "Line not found." << endl;
//        system("PAUSE");
    }
}

// Method to print the linked list to the file
void TextEditor::printLinkedList() {
    ofile.open(file_Name);
    int i = 1; // To prevent an unwanted <ENTER> at the last line.
    cur = head;

    while (cur != nullptr) {
        if (i == 1) {
            ofile << cur->line;
            cur = cur->next;
            i++;
        } else ofile << endl << cur->line;
    }

    ofile.close();
    removeLinkedList(head);
    cout << endl;
}

// Method to remove all nodes from the linked list
void TextEditor::removeLinkedList(FileNode*& head) {
    cur = head;
    nextNode = nullptr;

    while (cur != nullptr) {
        nextNode = cur->next;
        delete cur;
        cur = nextNode;
    }

    head = nullptr;
}

// Method to read all lines from a file
bool TextEditor::readFile() {
    int iterator = 1;
    string line;

    ifile.open(file_Name);
    if (ifile.is_open()) {
        while (!ifile.eof()) {
            getline(ifile, line);
            cout << BLUE_TEXT << setw(3) << iterator << "| " << WHITE_TEXT << line << endl;
            iterator++;
        }
        
        ifile.close();
//        system("PAUSE");
        return true;
    } else {
        cout << "Failed to open the file." << endl;
//        system("PAUSE");
        return false;
    }
}

// Method to write to a file
void TextEditor::writeFile() {
    string line;
    ofile.open(file_Name);
    
    cout << "Write your file, end it with \"END.\"" << endl;
    int i = 1; // To prevent an unwanted <ENTER> in the first line of the file.
    while (line != "END.") {
        getline(cin, line);
        
        if (line == "END.") break;

        if (i == 1) {
            ofile << line;
            i++;
        } else ofile << endl << line ;
    }
}

// Method to delete a file
void TextEditor::deleteFile() {
    const char* c_filename = file_Name.c_str();

    // Remove the file using remove() function from <cstdio>.
    if (remove(c_filename) == 0)
        cout << "File '" << file_Name << "' deleted successfully.\n";
    else
        cout << "Failed to delete file '" << file_Name << "'.\n";
    
}

// Method to compile a C++ file
void TextEditor::compileCode() {
    // Build the compilation command.
    string command = "g++ -o " + file_Name.substr(0, file_Name.find_last_of('.')) + " " + file_Name;

    // Execute the compilation command using system().
    int result = system(command.c_str());

    if (result == 0)
        cout << "Code compiled successfully.\n";
    else
        cout << "Compilation failed.\n";

    system("PAUSE");
}


int main() {
    bool breakingPoint = true;
    
    while(breakingPoint) {
        switch(menu()) {
            case 1: {
                string file_Name = askFileName("create");
                TextEditor myText(file_Name);
                myText.writeFile();
                break;
            }

            case 2: {
                string file_Name = askFileName("open");
                TextEditor myText(file_Name);
                myText.readFile();
                break;
            }

            case 3: {
                string file_Name = askFileName("open");
                TextEditor myText(file_Name);
                myText.modifyFile();
                break;
            }

            case 4: {
                string file_Name = askFileName("delete");
                TextEditor myText(file_Name);
                myText.deleteFile();
//                system("PAUSE");
                break;
            }
            
            case 5: {
                string file_Name = askFileName("to compile (C++)");
                TextEditor myText(file_Name);
                myText.compileCode();
                break;
            }


            case 0:
                breakingPoint = false; // Exit condition.
            break;

            default:                
                break;
        }
    }
    
    return 0;
}

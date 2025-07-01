// Include Statements
#include <map>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <string>
#include <string.h>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <dirent.h>
#include <list>
#include <errno.h>
#include <signal.h>
#include <strings.h>

#define GN 27
#define BUFFER_SIZE 256
#define FNAME_SIZE 612
#define NSCORES_IN_SCOREBOARD 10

using namespace std;


// Global Variables

bool superVerbose = false;
bool verbose = false;
bool keepTempFiles = false;

string wordListFileName;
int portNumber = 58000 + GN;
int lineN = 1;
bool randomWordGetting = false; //true if random, false if sequential
string gamesDir = "server/GAMES/";
string scoresDir = "server/SCORES/";
string imagesDir = "server/IMAGES/";


// Structures
struct puppet
{
    int fd;
    struct addrinfo *res;
};

struct SCORELIST
{
    int n_scores;
    int score[10];
    char PLID[10][7];
    char word[10][30];
    int n_succ[10];
    int n_tot[10];
};



// Function Prototypes
// Function Prototypes of all functions defined in this file
puppet openUDPSocket(string port);
puppet openTCPSocket(string port);
string getWordAndHintRandom();
string getWordAndHintSequential();
bool hasActiveGame(string plid);
bool hasPastGame(string plid);
fstream getActiveGameFile(string plid);
string getWord(fstream& file);
string getHint(fstream& file);
int getNPlays(fstream& file);
string getLastTPlay(fstream& file);
string getLastGPlay(fstream& file);
int getNMissPlays(fstream& file);
int getMaxErrors(fstream& file);
int getMaxErrors(string word);
bool hasBeenTriedLetter(string letter, fstream& file);
bool hasBeenTriedWord(string word, fstream& file);
bool isLetterInWord(string letter, fstream& file);
bool isWord(string word, fstream& file);
bool isWordGuessed(fstream& file);
bool isGameLost(fstream& file);
int getNOccurences(string word, string letter);
int getOccurence(string word, string letter, int i);
void newActiveGame(string plid, string word, string hint);
void addTPlay(fstream& file, string letter);
void addGPlay(fstream& file, string word);
void endGame(fstream& file, string plid, string code);

bool dirExists(string dir);

string handleStart(string message);
string handlePlay(string message);
string handleGuess(string message);
string handleQuit(string message);
string handleReveal(string message);
void udpHandler(puppet udp, char buffer[BUFFER_SIZE], struct sockaddr_in cliaddr, socklen_t len);
long getFileSize(char *fname);
void tcpSender(puppet tcp, string message);
int scoreboardFileGetter(char *fname);
void tcpHandler(puppet tcp, char buffer[BUFFER_SIZE]);
int FindLastGame(char *PLID, char *fname);
int FindTopScores(SCORELIST *list);
string makeScoreboardFile();




/*  
    ============================= - Main Function - =============================
*/

int max(int x, int y){
    if(x > y){
        return x;
    }
    else{
        return y;
    }
}

//./GS word_file [-p port] [-v]
int main(int argc, char *argv[]){
    //set the global variables according to the arguments
    if(argc < 2){
        cout << "Usage: ./GS word_file [-p port] [-v]" << endl;
        exit(1);
    }
    wordListFileName = argv[1];
    for(int i = 2; i < argc; i++){
        if(strcmp(argv[i], "-p") == 0){
            portNumber = atoi(argv[i+1]);
            i++;
        }
        else if(strcmp(argv[i], "-v") == 0){
            verbose = true;
        }
        else{
            cout << "Usage: ./GS word_file [-p port] [-v]" << endl;
            exit(1);
        }
    }

    //If "GAMES" directory does not exist, create it, use makedir and not the fylesystem header
    if(mkdir(gamesDir.c_str(), 0777) == -1){
        if(errno != EEXIST){
            perror("mkdir");
            exit(1);
        }
    }
    //If "SCORES" directory does not exist, create it
    if(mkdir(scoresDir.c_str(), 0777) == -1){
        if(errno != EEXIST){
            perror("mkdir");
            exit(1);
        }
    }
    //If "IMAGES" directory does not exist, create it
    if(mkdir(imagesDir.c_str(), 0777) == -1){
        if(errno != EEXIST){
            perror("mkdir");
            exit(1);
        }
    }

    //Socket creation and handling
    int connfd, nready, maxfdp1;
    char buffer[BUFFER_SIZE];
    pid_t childpid;
    fd_set rset;
    ssize_t n;
    socklen_t len;
    const int on = 1;
    struct sockaddr_in cliaddr, servaddr;
    void sig_chld(int);

    // create listening TCP socket
    puppet tcp = openTCPSocket(to_string(portNumber));
    listen(tcp.fd, 10);
 
    // create UDP socket
    puppet udp = openUDPSocket(to_string(portNumber));
 
    // clear the descriptor set
    FD_ZERO(&rset);
 
    // get maxfd
    maxfdp1 = max(tcp.fd, udp.fd) + 1;
    for (;;) {
        // set tcp.fd and udp.fd in readset
        FD_SET(tcp.fd, &rset);
        FD_SET(udp.fd, &rset);
 
        // select the ready descriptor
        nready = select(maxfdp1, &rset, NULL, NULL, NULL);
 
        // if tcp socket is readable then handle
        // it by accepting the connection
        if (FD_ISSET(tcp.fd, &rset)) {
            len = sizeof(cliaddr);
            connfd = accept(tcp.fd, (struct sockaddr*)&cliaddr, &len);
            if ((childpid = fork()) == 0) {
                close(tcp.fd);
                bzero(buffer, sizeof(buffer));
                if(verbose && superVerbose)cout << "Message From TCP client: " << endl;
                read(connfd, buffer, sizeof(buffer));
                puppet newtcp;
                newtcp.fd = connfd;
                tcpHandler(newtcp, buffer);
                close(connfd);
                exit(0);
            }
            close(connfd);
        }
        // if udp socket is readable receive the message.
        if (FD_ISSET(udp.fd, &rset)) {
            len = sizeof(cliaddr);
            bzero(buffer, sizeof(buffer));
            if(verbose && superVerbose) cout << "Message from UDP client: " << endl;
            n = recvfrom(udp.fd, buffer, sizeof(buffer), 0,
                        (struct sockaddr*)&cliaddr, &len);
            udpHandler(udp, buffer, cliaddr, len);
        }
    }
}




/*  
    ============================= - Function Definitions - =============================
*/

puppet openUDPSocket(string port){
    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints,*res;
    struct sockaddr_in addr;
    puppet p;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        cout << "Error opening socket" << endl;
        exit(1);
    }

    int yes = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    memset(&hints,0,sizeof hints);

    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_DGRAM; //UDP socket
    hints.ai_flags=AI_PASSIVE;

    char* portChar = new char[port.length() + 1];
    strcpy(portChar, port.c_str());

    errcode = getaddrinfo(NULL,portChar,&hints,&res);
    if(errcode!=0){
        cout << "Error getting address info" << endl;
        exit(1);
    }

    n=bind(fd,res->ai_addr, res->ai_addrlen);
    if(n==-1){
        cout << "Error binding socket" << endl;
        exit(1);
    }

    p.fd = fd;
    p.res = res;
    return p;
}

puppet openTCPSocket(string port){
    int fd,errcode;
    ssize_t n;
    socklen_t addrlen;
    struct addrinfo hints,*res;
    struct sockaddr_in addr;
    puppet p;

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1) {
        cout << "Error opening socket" << endl;
        exit(1);
    }

    int yes = 1;

    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    memset(&hints,0,sizeof hints);

    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_STREAM; //TCP socket
    hints.ai_flags=AI_PASSIVE;

    char* portChar = new char[port.length() + 1];
    strcpy(portChar, port.c_str());

    errcode = getaddrinfo(NULL,portChar,&hints,&res);
    if(errcode!=0){
        cout << "Error getting address info" << endl;
        exit(1);
    }

    n=bind(fd,res->ai_addr,res->ai_addrlen);
    if(n==-1){
        cout << "Error binding socket" << endl;
        exit(1);
    }

    p.fd = fd;
    p.res = res;
    return p;
}

// ---------------- File Organization. ---------------- //

void resetFile(fstream& file){
    file.clear();
    file.seekg (0, ios::beg);
}

/*
    Organization information:
    In the directory where the player aplication runs ther is also a text file with the words and hints
    The file is called wordlist.txt. There are also image files of hints, the names of which are gathered from the wordlist.txt file
    There are two subdirectories: GAMES and SCORES.
    Inside of GAMES there are files with information of active games, their names are GAME_plid.txt where plid is the player id
    There are also subdirectories inside of GAMES, each with the name of the player id, inside of which there are files with the information of past games
    A game file has a first line with the word, a space, and then the filename for hint image. The following lines are composed of "code play"
    where code is either T for play of a letter or G for guess of a word. If T, play is a letter, if G, play is a word.
    Inside the subdiretories of GAMES with names of the player id, the files have the designation: YYYYMMDDHHMMSS_code.txt
    where code can be W, F or Q for win, fail or quit, respectively.
*/

    // === Getting info == //

//returns a random word and hint from the word list file
//word list fine has a word and a hint separated by a space on each line
string getWordAndHintRandom(){
    //open the file
    fstream file(wordListFileName);
    if(!file.is_open()){
        cout << "Error opening file1" << endl;
        exit(1);
    }
    //get the number of lines
    int nLines = 0;
    string line;
    while(getline(file, line)){
        nLines++;
    }
    //get a random line
    int randomLine = rand() % nLines;
    file.clear();
    file.seekg(0, ios::beg);
    for(int i = 0; i < randomLine; i++){
        getline(file, line);
    }
    //get the word and hint
    string word = line.substr(0, line.find(" "));
    string hint = line.substr(line.find(" ") + 1);
    //close the file
    file.close();
    //return the word and hint
    return word + " " + hint;
}

string getWordAndHintSequential(){
    //open the file
    fstream file(wordListFileName);
    if(!file.is_open()){
        cout << "Error opening file2" << endl;
        exit(1);
    }
    if(verbose && superVerbose) cout << "File opened for word and hint" << endl;
    //get the number of lines
    string line;
    for(int i = 0; i < lineN; i++){
        getline(file, line);
    }
    //if there isnt a next line, set lineN to 1
    if(file.eof()){
        lineN = 1;
    }
    else{
        lineN++;
    }
    //get the word and hint
    string word = line.substr(0, line.find(" "));
    string hint = line.substr(line.find(" ") + 1);
    //close the file
    file.close();
    //return the word and hint
    return word + " " + hint;
}

bool hasActiveGame(string plid){
    //check if file exists in gamesDir
    string fileName = gamesDir + "GAME_" + plid + ".txt";
    fstream file;
    file.open(fileName);
    if(!file.is_open()){
        return false;
    }
    file.close();
    return true;
}

bool hasPastGame(string plid){
    //open the file
    fstream file(gamesDir + plid + "/GAME_" + plid + ".txt");
    if(!file.is_open()){
        return false;
    }
    file.close();
    return true;
}

fstream getActiveGameFile(string plid){
    //open the file
    string fileName = gamesDir + "GAME_" + plid + ".txt";
    fstream file;
    file.open(fileName);
    if(!file.is_open()){
        cout << "Error opening file3" << endl;
        exit(1);
    }
    return file;
}

//get the word from an active game file, given the file
string getWord(fstream& file){
    string line;
    resetFile(file);
    getline(file, line);
    if(verbose && superVerbose) cout << "Line: " << line << endl;
    return line.substr(0, line.find(" "));
}

//get the hint from an active game file, given the file
string getHint(fstream& file){
    string line;
    resetFile(file);
    getline(file, line);
    return line.substr(line.find(" ") + 1);
}

//get the number of plays from an active game file, given the file
int getNPlays(fstream& file){
    int nPlays = 0;
    string line;
    resetFile(file);
    getline(file, line);
    while(getline(file, line)){
        nPlays++;
    }
    if(verbose && superVerbose) cout << "nPlays: " << nPlays << endl;
    return nPlays;
}

//return a string "index play" where index is the index of the play and play is the last play of code T
//if there are no plays of code T, return "0 0"
string getLastTPlay(fstream& file){
    string line;
    int index = 0;
    int lastTIndex = 0;
    string lastTPlay = "0";
    resetFile(file);
    while(getline(file, line)){
        index++;
        if(line[0] == 'T'){
            lastTIndex = index;
            lastTPlay = line;
        }
    }
    return to_string(lastTIndex) + " " + lastTPlay;
}

string getLastGPlay(fstream& file){
    string line;
    int index = 0;
    int lastGIndex = 0;
    string lastGPlay = "0";
    resetFile(file);
    while(getline(file, line)){
        index++;
        if(line[0] == 'G'){
            lastGIndex = index;
            lastGPlay = line;
        }
    }
    return to_string(lastGIndex) + " " + lastGPlay;
}

//get the number of missed plays from an active game file, given the file
int getNMissPlays(fstream& file){
    int nMissPlays = 0;
    string line;
    string word = getWord(file);
    resetFile(file);
    getline(file, line);
    while(getline(file, line)){
        if(line[0] == 'T'){
            if(word.find(line[2]) == string::npos){
                nMissPlays++;
            }
        }
        else if(line[0] == 'G'){
            if(word != line.substr(2)){
                nMissPlays++;
            }
        }
    }
    return nMissPlays;
}

int getNHitPlays(fstream& file){
    int nHitPlays = 0;
    string line;
    string word = getWord(file);
    resetFile(file);
    getline(file, line);
    while(getline(file, line)){
        if(line[0] == 'T'){
            if(word.find(line[2]) != string::npos){
                nHitPlays++;
            }
        }
        else if(line[0] == 'G'){
            if(word == line.substr(2)){
                nHitPlays++;
            }
        }
    }
    return nHitPlays;
}

int getMaxErrors(fstream& file){
    int nLetters = getWord(file).length();
    if(nLetters <= 6){
        return 7;
    }
    else if(nLetters <= 10){
        return 8;
    }
    else{
        return 9;
    }
}

//same but for a given string
int getMaxErrors(string word){
    int nLetters = word.length();
    if(nLetters <= 6){
        return 7;
    }
    else if(nLetters <= 10){
        return 8;
    }
    else{
        return 9;
    }
}

//check if letter has been tried in a T play, given an active file
bool hasBeenTriedLetter(string letter, fstream& file){
    string line;
    resetFile(file);
    getline(file, line);
    while(getline(file, line)){
        if(line[0] == 'T'){
            if(line[2] == letter[0]){
                return true;
            }
        }
    }
    return false;
}

//check if word has been tried in a G play, given an active file
bool hasBeenTriedWord(string word, fstream& file){
    string line;
    resetFile(file);
    getline(file, line);
    while(getline(file, line)){
        if(line[0] == 'G'){
            if(line.substr(2) == word){
                return true;
            }
        }
    }
    return false;
}

//check if letter is part of the word, given an active file
bool isLetterInWord(string letter, fstream& file){
    string word = getWord(file);
    if(verbose && superVerbose) cout << "word: " << word << "letter: " << letter << endl;
    return word.find(letter) != string::npos;
}

//check if word is the word, given an active file
bool isWord(string word, fstream& file){
    return word == getWord(file);
}

//check if all the letters of the word have been guessed in T plays
bool isWordGuessed(fstream& file){
    string word = getWord(file);
    string line;
    int total = 0;
    resetFile(file);
    getline(file, line);
    while(getline(file, line)){
        if(line[0] == 'T'){
            //add to total the number of occurences of the letter in the word;
            total += getNOccurences(word, line.substr(2));
            if(verbose && superVerbose) cout << "total: " << total << " letter: " << line.substr(2) << endl;
        }
    }
    if(verbose && superVerbose) cout << "total: " << total << " word.length(): " << word.length() << endl;
    return total == word.length();
}

//check if the game is lost, given an active file
bool isGameLost(fstream& file){
    return getNMissPlays(file) >= getMaxErrors(file);
}

int getNOccurences(string word, string letter){
    int nOccurences = 0;
    for(int i = 0; i < word.length(); i++){
        if(word[i] == letter[0]){
            nOccurences++;
        }
    }
    return nOccurences;
}

int getOccurence(string word, string letter, int i){
    int nOccurences = 0;
    if (verbose && superVerbose)cout << "word: " << word << endl;
    for(int j = 0; j < word.length(); j++){
        if(word[j] == letter[0]){
            if (verbose && superVerbose)cout << "found occurence at " << j << endl;
            nOccurences++;
            if(nOccurences == i){
                if (verbose && superVerbose)cout << "returning " << j << endl;
                return j+1;
            }
        }
    }
    return -1;
}



    // === Saving info == //

//in the GAMES directive, create a game file for the given plid, given word and given hint
//remmeber that the name should be GAME_plid.txt and the composition of these files is stated in comments above
void newActiveGame(string plid, string word, string hint){
    //create the file name
    string fileName = gamesDir + "GAME_" + plid + ".txt";
    //open file for writting
    fstream file;
    file.open(fileName, ios::out);
    //write the word and hint, use write
    file << word << " " << hint << endl;
    
    //close the file
    file.close();
}

//given an active and open game file, ad a T play to it, the letter is also given
void addTPlay(fstream& file, string letter){
    //reach the end of the file
    file.seekg(0, ios::end);
    //write the play
    file << "T " << letter << endl;
    return;
}

//given an active game file, ad a G play to it, the word is also given
void addGPlay(fstream& file, string word){
    //reach the end of the file
    file.seekg(0, ios::end);
    //write the play
    file << "G " << word << endl;
    return;
}

/*
bool dirExists(string dir){
    struct stat info;
    if(stat(dir.c_str(), &info) != 0){
        return false;
    }
    else if(info.st_mode & S_IFDIR){
        return true;
    }
    else{
        return false;
    }
}
*/

//given an active game file and a plid, copy the contents of this file to a new one in the subdirectory of GAMES/ that corresponds with the plid
//remember the format of these files, YYYYMMDDHHMMSS_code.txt, code is also given to this function. Also delete the active game file
void endGame(fstream& file, string plid, string code){
    //get the current time
    if (verbose && superVerbose)cout << "endGame" << endl;
    time_t now = time(0);
    tm *ltm = localtime(&now);
    //create the new file name
    string fileNameEnd = to_string(1900 + ltm->tm_year) + to_string(1 + ltm->tm_mon) + to_string(ltm->tm_mday) + "_" + to_string(ltm->tm_hour) + to_string(ltm->tm_min) + to_string(ltm->tm_sec) + "_" + code + ".txt";
    //open the new file
    string fileName = gamesDir + plid + "/" + fileNameEnd;
    if(verbose && superVerbose) cout << "fileName1: " << fileName << endl;
    string dir = gamesDir + plid;
    //check if dir exists, if not, create it, use mkdir logic
    if(!dirExists(dir)){
        if(verbose && superVerbose) cout << "dir does not exist" << endl;
        if(mkdir(dir.c_str(), 0777) == -1){
            cout << "Error creating directory" << endl;
            exit(1);
        }
    }
    
    fstream newFile;
    newFile.open(fileName, ios::out);
    if(!newFile.is_open()){
        cout << "Error opening file5" << endl;
        exit(1);
    }
    //copy the contents of the old file to the new one
    string line;
    resetFile(file);
    while(getline(file, line)){
        if(verbose && superVerbose) cout << "lineeeeeee: " << line << endl;
        newFile << line << endl;
    }
    //close the files
    newFile.close();
    file.close();
    //delete the old file
    remove((gamesDir + "GAME_" + plid + ".txt").c_str());
    return;
}

void makeScoreFile(fstream& gameFile, string plid){
    //get the current time
    time_t now = time(0);
    tm *ltm = localtime(&now);
    //create the new file name
    string fileNameEnd = to_string(1900 + ltm->tm_year) + to_string(1 + ltm->tm_mon) + to_string(ltm->tm_mday) + "_" + to_string(ltm->tm_hour) + to_string(ltm->tm_min) + to_string(ltm->tm_sec) + ".txt";
    //Find Score, cant do yet
    int score = 0;
    string word = getWord(gameFile);
    int hitPlays = getNHitPlays(gameFile);
    int nPlays = getNPlays(gameFile);
    score = (hitPlays * 100) / nPlays;
    string scoreString = to_string(score);
    //make the scoreString always be 3 digits
    while(scoreString.length() < 3){
            scoreString = "0" + scoreString;
        }
    //open the new file
    string fileName = scoresDir + scoreString + "_" + plid + "_" + fileNameEnd;
    if(verbose && superVerbose) cout << "fileName: " << fileName << endl;
    fstream newFile;
    newFile.open(fileName, ios::out);
    if(!newFile.is_open()){
        cout << "Error opening file5" << endl;
        exit(1);
    }
    string line = to_string(score) + " " + plid + " " + word + " " + to_string(hitPlays) + " " + to_string(nPlays);
    newFile << line << endl;
    //close the files
    newFile.close();
}






string handleStart(string message){
    string toSend = "RSG";
    //there must be a "\n" at the seventh position and every character before it must be a number, otherwise return toSend + "ERR"
    if(message[6] != '\n'){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Message does not end with \\n" << endl;
        return toSend;
    }
    for(int i = 0; i < 6; i++){
        if(!isdigit(message[i])){
            toSend += " ERR";
            if(verbose && superVerbose) cout << "Plid isnt digits" << endl;
            return toSend;
        }
    }
    //get the plid
    string plid = message.substr(0, 6);
    //check if plid is currently in use with file logic
    if(hasActiveGame(plid)){
        toSend += " NOK";
        if(verbose && superVerbose) cout << "Plid already in use" << endl;
        return toSend;
    }
    else{
        toSend += " OK";
        if(verbose && superVerbose) cout << "Plid is not in use1" << endl;
        string wordAndHint;
        if(randomWordGetting){
            wordAndHint = getWordAndHintRandom();
        }
        else{
            wordAndHint = getWordAndHintSequential();
        }
        if(verbose && superVerbose) cout << "Word and hint is " << wordAndHint << endl;
        //word will be before the " "
        int space = wordAndHint.find(" ");
        string word = wordAndHint.substr(0, space);
        if(verbose && superVerbose) cout << "Word is " << word << endl;
        //hint will be after the " "
        string hint = wordAndHint.substr(space + 1);
        if(verbose && superVerbose) cout << "Hint is " << hint << endl;
        //create a new game with the plid, word and hint
        newActiveGame(plid, word, hint);
        //add the length of the word and the max number of errors to the message
        toSend += " " + to_string(word.length()) + " " + to_string(getMaxErrors(word));
    }
    return toSend;

}

string handlePlay(string message){
    string toSend = "RLG";
    //plid will be before the first space
    int space1 = message.find(" ");
    if(space1 == -1){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Message does not have a space" << endl;
        return toSend;
    }
    string plid = message.substr(0, space1);
    //letter will be before the second space
    int space2 = message.find(" ", space1 + 1);
    if(space2 == -1){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Message does not have a second space" << endl;
        return toSend;
    }
    string letter = message.substr(space1 + 1, space2 - space1 - 1);
    if(verbose && superVerbose) cout << "Letter is " << letter << endl;
    //letter must be a single character
    if(letter.length() != 1){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Letter is not a single character" << endl;
        return toSend;
    }
    //trials will be before the \n
    int newline = message.find("\n");
    if(newline == -1){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Message does not end with \\n" << endl;
        return toSend;
    }
    string trials = message.substr(space2 + 1, newline - space2 - 1);
    //trials must be a number
    for(int i = 0; i < trials.length(); i++){
        if(!isdigit(trials[i])){
            toSend += " ERR";
            if(verbose && superVerbose) cout << "Trials is not a number" << endl;
            return toSend;
        }
    }
    int trialsInt = stoi(trials);
    //check if plid is in use
    if(!hasActiveGame(plid)){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Plid is not in use2" << endl;
        return toSend;
    }

    //get the game file of this plid
    fstream file = getActiveGameFile(plid);
    //check if it is the last plg of the game
    string lastTPlay = getLastTPlay(file);
    //get letter from lastTPlay
    int lastTPlaySpace = lastTPlay.find(" ");
    string lastTPlayLetter = lastTPlay.substr(lastTPlaySpace + 1);
    //get trials from file
    int nPlays = getNPlays(file);
    bool isLastPLG = (nPlays == trialsInt && lastTPlayLetter == letter);
    if (verbose && superVerbose) cout << "Is last PLG: " << isLastPLG << endl;

    //check if trials is correct
    if(trialsInt != nPlays + 1 && !isLastPLG){
        toSend += " INV " + trials;
        if(verbose && superVerbose) cout << "Trials is not correct" << endl;
    }

    //check if letter has been tried and isnt last PLG
    else if(!isLastPLG && hasBeenTriedLetter(letter, file)){
        toSend += " DUP " + trials;
        if(verbose && superVerbose) cout << "Letter has already been tried" << endl;
    }
    //check if letter is part of the word
    else if(isLetterInWord(letter, file)){
        //add letter to word in file as a T play
        if(!isLastPLG){
            if (verbose && superVerbose) cout << "Added T play" << endl;
            addTPlay(file, letter);
        }
        if(isWordGuessed(file)){
            toSend += " WIN " + trials;
            if(verbose && superVerbose) cout << "Game has been won" << endl;
            //end game with code W, endGame(fstream file, string plid, string code)
            makeScoreFile(file, plid);
            endGame(file, plid, "W");
            return toSend;
        }
        else{
            toSend += " OK " + trials;
            string word = getWord(file);
            //find and add the number of occurences of the letter in the word and their positions
            int nOccurences = getNOccurences(word, letter);
            toSend += " " + to_string(nOccurences);
            for(int i = 1; i <= nOccurences; i++){
                toSend += " " + to_string(getOccurence(word, letter, i));
            }
            if(verbose && superVerbose) cout << "Letter is in the word" << endl;
        }
    }
    else{
        if(!isLastPLG){
            addTPlay(file, letter);
        }
        //if the game in file is over
        if(isGameLost(file)){
            toSend += " OVR " + trials;
            if(verbose && superVerbose) cout << "Game has been lost" << endl;
            endGame(file, plid, "F");
            return toSend;
        }
        else{
            toSend += " NOK " + trials;
            if(verbose && superVerbose) cout << "Letter is not in the word" << endl;
        }
    }
    file.close();
    return toSend;
}

string handleGuess(string message){
    string toSend = "RWG";
    //plid will be before the first space
    int space1 = message.find(" ");
    if(space1 == -1){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Message does not have a space" << endl;
        return toSend;
    }
    string plid = message.substr(0, space1);
    //word will be before the second space
    int space2 = message.find(" ", space1 + 1);
    if(space2 == -1){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Message does not have a second space" << endl;
        return toSend;
    }
    string word = message.substr(space1 + 1, space2 - space1 - 1);
    //trials will be before the \n
    int newline = message.find("\n");
    if(newline == -1){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Message does not end with \\n" << endl;
        return toSend;
    }
    string trials = message.substr(space2 + 1, newline - space2 - 1);
    //trials must be a number
    for(int i = 0; i < trials.length(); i++){
        if(!isdigit(trials[i])){
            toSend += " ERR";
            if(verbose && superVerbose) cout << "Trials is not a number" << endl;
            return toSend;
        }
    }
    int trialsInt = stoi(trials);
    //check if plid is in use
    if(!hasActiveGame(plid)){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Plid is not in use3" << endl;
        return toSend;
    }
    //get the game file of this plid
    fstream file = getActiveGameFile(plid);
    //check if it is the last pwg of the game
    string lastGPlay = getLastGPlay(file);
    //get word from lastGPlay
    int lastGPlaySpace = lastGPlay.find(" ");
    string lastGPlayWord = lastGPlay.substr(lastGPlaySpace + 1);
    //get trials from file
    int nPlays = getNPlays(file);
    bool isLastPWG = (nPlays == trialsInt && lastGPlayWord == word);
    //check if trials is correct
    if(trialsInt != nPlays + 1 && !isLastPWG){
        toSend += " INV " + trials;
        if(verbose && superVerbose) cout << "Trials is not correct" << endl;
    }
    //check if word has been tried" + trials
    else if(hasBeenTriedWord(word, file) && !isLastPWG){
        toSend += " DUP " + trials;
        if(verbose && superVerbose) cout << "Word has already been tried" << endl;
    }
    else if(isWord(word, file)){
        if(!isLastPWG){
            addGPlay(file, word);
        }
        toSend += " WIN " + trials;
        if(verbose && superVerbose) cout << "Game has been won" << endl;
        makeScoreFile(file, plid);
        endGame(file, plid, "W");
        return toSend;
    }
    else{
        if(!isLastPWG){
            addGPlay(file, word);
        }
        if(isGameLost(file)){
            toSend += " OVR " + trials;
            if(verbose && superVerbose) cout << "Game has been lost" << endl;
            endGame(file, plid, "F");
            return toSend;
        }
        else{
            toSend += " NOK " + trials;
            if(verbose && superVerbose) cout << "Word is not the word" << endl;
        }
    }
    file.close();
    return toSend;
}

string handleQuit(string message){
    string toSend = "RQT";
    //plid will be before the first \n
    int newline = message.find("\n");
    if(newline == -1){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Message does not end with \\n" << endl;
        return toSend;
    }
    string plid = message.substr(0, newline);
    //check if plid is in use
    if(!hasActiveGame(plid)){
        toSend += " NOK";
        if(verbose && superVerbose) cout << "Plid is not in use4" << endl;
        return toSend;
    }
    else{
        toSend += " OK";
        if(verbose && superVerbose) cout << "Game has been quit" << endl;
        fstream file = getActiveGameFile(plid);
        endGame(file, plid, "Q");
        return toSend;
    }
}

string handleReveal(string message){
    string toSend = "RRV";
    //plid will be before the first \n
    int newline = message.find("\n");
    if(newline == -1){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Message does not end with \\n" << endl;
        return toSend;
    }
    string plid = message.substr(0, newline);
    //check if plid is in use
    if(!hasActiveGame(plid)){
        toSend += " ERR";
        if(verbose && superVerbose) cout << "Plid is not in use5" << endl;
        return toSend;
    }
    else{
        fstream file = getActiveGameFile(plid);
        toSend += " " + getWord(file);
        file.close();
        if(verbose && superVerbose) cout << "Word has been revealed" << endl;
        return toSend;
    }
}

void udpHandler(puppet udp, char buffer[BUFFER_SIZE], struct sockaddr_in cliaddr, socklen_t len){
    string message = buffer;
    string toSend = "";
    if(verbose) cout << "Received message: " << message << endl;
    //command will be before the first space
    int space1 = message.find(" ");
    string command = message.substr(0, space1);
    //rest will be after the first space
    string rest = message.substr(space1 + 1);
    if(verbose && superVerbose) cout << "Command: " << command << " | Rest: " << rest << endl;
    if(command == "SNG"){
        toSend = handleStart(rest);
    }
    else if(command == "PLG"){
        toSend = handlePlay(rest);
    }
    else if(command == "PWG"){
        toSend = handleGuess(rest);
    }
    else if(command == "QUT"){
        toSend = handleQuit(rest);
    }
    else if(command == "REV"){
        toSend = handleReveal(rest);
    }
    else{
        toSend = "ERR";
    }
    toSend += '\n';
    if(verbose) cout << "Sending message: " << toSend << endl;
    sendto(udp.fd, toSend.c_str(), toSend.length(), 0, (struct sockaddr *)&cliaddr, len);
}

long getFileSize(char *fname){
    struct stat stat_buf;
    int rc = stat(fname, &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

void tcpSender(puppet tcp, char *fpath, char *fname){
    FILE *fp;
    char buffer[BUFFER_SIZE], writterBuffer[BUFFER_SIZE];
    int nread, nwritten;
    fp = fopen(fpath, "r");
    if(fp == NULL){
        cout << "Error opening file6" << endl;
        exit(1);
    }
    //put into writterbuffer "fname fsize "
    long fsize = getFileSize(fpath);
    if(fsize == -1){
        cout << "Error getting file size" << endl;
        exit(1);
    }
    sprintf(writterBuffer, "%s %ld ", fname, fsize);
    //write to socket
    nwritten = write(tcp.fd, writterBuffer, strlen(writterBuffer));
    if(nwritten < 0){
        cout << "Error writing to socket" << endl;
        exit(1);
    }
    if (verbose && superVerbose) cout << "Sent: " << writterBuffer << "With size: " << strlen(writterBuffer) << endl;
    //read from file and write to socket
    while((nread = fread(buffer, 1, BUFFER_SIZE, fp)) > 0){
        nwritten = write(tcp.fd, buffer, nread);
        if(nwritten < 0){
            cout << "Error writing to socket" << endl;
            exit(1);
        }
        if (verbose && superVerbose) cout << "Sent: " << buffer << "With size: " << nread << endl;
    }
    fclose(fp);
    //write "\n" to socket
    nwritten = write(tcp.fd, "\n", 1);
    if(nwritten < 0){
        cout << "Error writing to socket" << endl;
        exit(1);
    }
    if (verbose && superVerbose) cout << "Sent: " << "\n" << "With size: " << 1 << endl;
    return;
}

//
string makeStateFileActive(string fpath_to_game_file, string plid){
    string fname = "server/STATE_" + plid + ".txt";
    string toWrite = "\tActive game found for player: " + plid + "\n";
    fstream file;
    file.open(fpath_to_game_file, ios::in);
    string word = getWord(file);
    string letters = "";
    int nPlays = getNPlays(file);
    if(nPlays == 0){
        toWrite += "\t--- No Transactions Found ---\n";
    }
    else{
        toWrite += "\t--- Transactions Found: " + to_string(nPlays) + " ---\n";
        string line;
        resetFile(file);
        getline(file, line);
        while(getline(file, line)){
            if(line[0] == 'T'){
                toWrite += "\tLetter trial: " + line.substr(2) + " - ";
                if(getNOccurences(word, line.substr(2)) != 0){
                    toWrite += "TRUE\n";
                    letters += line.substr(2);
                }
                else{
                    toWrite += "FALSE\n";
                }
            }
            if(line[0] == 'G'){
                toWrite += "\tWord guess: " + line.substr(2) + " - ";
                if(line.substr(2) == word){
                    toWrite += "TRUE\n";
                    letters += line.substr(2);
                }
                else{
                    toWrite += "FALSE\n";
                }
            }
        }
        toWrite += "\tSolved so far: ";
        for(int i = 0; i < word.length(); i++){
            if(letters.find(word[i]) != string::npos){
                if (verbose && superVerbose) cout << "Found letter: " << word[i] << " at: " << i << endl;
                toWrite += word[i];
            }
            else{
                toWrite += "_";
            }
        }
        toWrite += "\n";
    }
    fstream finalFile;
    finalFile.open(fname, ios::out);
    finalFile << toWrite;
    finalFile.close();
    file.close();
    if (verbose && superVerbose) cout << "Created file: " << fname << endl;
    if (verbose && superVerbose) cout << "File contents: " << toWrite << endl;
    return fname;
}

string makeStateFileFinished(string fpath_to_game_file, string plid){
    string fname = "server/STATE_" + plid + ".txt";
    string toWrite = "\tLast finalized game for player: " + plid + "\n";
    fstream file;
    file.open(fpath_to_game_file, ios::in);
    string word = getWord(file);
    toWrite + "\t Word: " + word + "; hint file: " + getHint(file) + "\n";
    string letters = "";
    int nPlays = getNPlays(file);
    if(nPlays == 0){
        toWrite += "\t--- No Transactions Found ---\n";
    }
    else{
        toWrite += "\t--- Transactions Found: " + to_string(nPlays) + " ---\n";
        string line;
        resetFile(file);
        getline(file, line);
        while(getline(file, line)){
            if(line[0] == 'T'){
                toWrite += "\tLetter trial: " + line.substr(2) + " - ";
                if(getNOccurences(word, line.substr(2)) != 0){
                    toWrite += "TRUE\n";
                    letters += line.substr(2);
                }
                else{
                    toWrite += "FALSE\n";
                }
            }
            if(line[0] == 'G'){
                toWrite += "\tWord guess: " + line.substr(2) + " - ";
                if(line.substr(2) == word){
                    toWrite += "TRUE\n";
                    letters += line.substr(2);
                }
                else{
                    toWrite += "FALSE\n";
                }
            }
        }
        toWrite += "\tSolved so far: ";
        for(int i = 0; i < word.length(); i++){
            if(letters.find(word[i]) != string::npos){
                if (verbose && superVerbose) cout << "Found letter: " << word[i] << " at: " << i << endl;
                toWrite += word[i];
            }
            else{
                toWrite += "_";
            }
        }
        toWrite += "\n";
    }
    //code will be after the second "_" and before the first "." in the file path
    int underscore1 = fpath_to_game_file.find("_");
    int underscore2 = fpath_to_game_file.find("_", underscore1 + 1);
    int dot = fpath_to_game_file.find(".");
    //code is between the second underscore and the first dot
    string code = fpath_to_game_file.substr(underscore2 + 1, dot - underscore2 - 1);
    toWrite += "\tTermination: ";
    if(verbose && superVerbose) cout << "Code: " << code << endl;
    if(code == "W"){
        toWrite += "WIN\n";
    }
    else if(code == "F"){
        toWrite += "FAIL\n";
    }
    else if(code == "Q"){
        toWrite += "QUIT\n";
    }
    else{
        toWrite += "UNKNOWN\n";
    }

    fstream finalFile;
    finalFile.open(fname, ios::out);
    finalFile << toWrite;
    finalFile.close();
    file.close();
    if (verbose && superVerbose) cout << "Created file: " << fname << endl;
    if (verbose && superVerbose) cout << "File contents: " << toWrite << endl;
    return fname;
}

void printScores(SCORELIST scores){
    for(int i = 0; i < 10; i++){
        cout << scores.score[i] << "\t" << scores.PLID[i] << "\t" << scores.word[i] << "\t" << scores.n_succ[i] << "\t" << scores.n_tot[i] << endl;
    }
}

string makeScoreboardFile2(){
    //fname should be TOPSCORES_YYYYYMMDD_HHMMSS.txt
    string fname = "TOPSCORES_";
    time_t now = time(0);
    tm *ltm = localtime(&now);
    fname += to_string(1900 + ltm->tm_year);
    fname += to_string(1 + ltm->tm_mon);
    fname += to_string(ltm->tm_mday);
    fname += "_";
    fname += to_string(ltm->tm_hour);
    fname += to_string(ltm->tm_min);
    fname += to_string(ltm->tm_sec);
    fname += ".txt";

    SCORELIST scores;
    int check = FindTopScores(&scores);
    if(check == -1){
        if (verbose && superVerbose)cout << "Error: could not find any scores" << endl;
        return "";
    }
    printScores(scores);
    //return "IDC";
    string toWrite = "\n-------------------------------- TOP 10 SCORES --------------------------------\n\n";
    toWrite += "    SCORE PLAYER     WORD                             GOOD TRIALS  TOTAL TRIALS\n\n";
    if (verbose && superVerbose)cout << "check:" << scores.n_scores << endl;
    string score, word, exampleFromTejo, sucess, total;
    for(int i = 0; i < scores.n_scores; i++){
        toWrite += to_string(i) + " - ";
        //make a string out of score that always has 3 digits, even if score is less than 100
        score = to_string(scores.score[i]);
        while(score.length() < 3){
            score = "0" + score;
        }
        if (verbose && superVerbose)cout << "score: " << score << endl;
        toWrite += score + "  ";
        toWrite += scores.PLID[i];
        if (verbose && superVerbose)cout << "PLID: " << scores.PLID[i] << endl;
        toWrite += "  ";
        word = scores.word[i];
        toWrite += word;
        if (verbose && superVerbose)cout << "word: " << word << endl;
        exampleFromTejo = "thunderbird                             ";
        //add to toWrite as many spaces as exampleFromTejo.size() - scores.word[i].length
        for(int j = 0; j < exampleFromTejo.length() - word.length(); j++){
            toWrite += " ";
        }
        sucess = to_string(scores.n_succ[i]);
        toWrite += sucess;
        //add to toWrite as many spaces as 2 - sucess.length
        for(int j = 0; j < 2 - sucess.length(); j++){
            toWrite += " ";
        }
        toWrite += "            ";
        total = to_string(scores.n_tot[i]);
        toWrite += total;
        //add to toWrite as many spaces as 2 - total.length
        for(int j = 0; j < 2 - total.length(); j++){
            toWrite += " ";
        }
        toWrite += "\n";

    }
    if(verbose && superVerbose) cout << "toWrite: " << toWrite << endl;
    //write to file
    fstream finalFile;
    //if a file already exists, it must be written over
    finalFile.open(fname, ios::out | ios::trunc);
    finalFile << toWrite;
    finalFile.close();
    if (verbose && superVerbose) cout << "Created file: " << fname << endl;
    if (verbose && superVerbose) cout << "File contents: " << toWrite << endl;
    return fname;
}

time_t getFileTime(string fpath){
    struct stat attr;
    stat(fpath.c_str(), &attr);
    return attr.st_mtime;
}

//files will be in the format: "YYYYMMDD_HHMMSS_code.txt"
// maybe be worthless
string getMostRecentFile(string dir){
    DIR *dp;
    struct dirent *ep;
    dp = opendir(dir.c_str());
    if(dp != NULL){
        string mostRecentFile = "";
        time_t mostRecentTime = 0;
        while(ep = readdir(dp)){
            string fname = ep->d_name;
            if(fname.length() > 4 && fname.substr(fname.length() - 4) == ".txt"){
                string date = fname.substr(0, 8);
                string time = fname.substr(9, 6);
                string code = fname.substr(16, 3);
                if(code == "STA"){
                    string fpath = dir + "/" + fname;
                    time_t fileTime = getFileTime(fpath);
                    if(fileTime > mostRecentTime){
                        mostRecentTime = fileTime;
                        mostRecentFile = fname;
                    }
                }
            }
        }
        closedir(dp);
        return mostRecentFile;
    }
    else{
        cout << "Error opening directory" << endl;
        exit(1);
    }
}

bool dirExists(string dir){
    DIR *dp;
    struct dirent *ep;
    dp = opendir(dir.c_str());
    if(dp != NULL){
        closedir(dp);
        return true;
    }
    else{
        return false;
    }
}

void tcpHandler(puppet tcp, char buffer[BUFFER_SIZE]){
    string message = buffer;
    if(verbose) cout << "Received message: " << message << endl;
    //command will be before the first space
    int space1 = message.find(" ");
    //if there is no space, command is before the first \n
    if(space1 == -1){
        space1 = message.find("\n");
    }
    string command = message.substr(0, space1);
    //command can be GSB GHL or STA
    if(verbose && superVerbose) cout << "Command: " << command << endl;
    if(command == "GSB"){
        //check if there is a \n right after GSB, if not, write "ERR\n" to socket
        /*
        if(message[space1 + 1] != '\n'){
            string toSend = "ERR\n";
            if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
            write(tcp.fd, toSend.c_str(), toSend.length());
        }
        */
        //else{
            //write "RSB " to socket
            string toSend = "RSB ";
            //get the scoreboard file name
            string fpath = makeScoreboardFile();
            string fname;
            //it will be path from the first / forward
            int slash = fpath.find("/");
            if(slash != -1){
                fname = fpath.substr(slash + 1);
            }
            else{
                fname = fpath;
            }
            if(fpath != ""){
                //if got, write "OK " and send the file
                toSend += "OK ";
                if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
                write(tcp.fd, toSend.c_str(), toSend.length());
                tcpSender(tcp, (char *)fpath.c_str(), (char *)fname.c_str());
                //remove the file
                if(!keepTempFiles)
                    remove(fname.c_str());
            }
            else{
                //if not, write "EMPTY\n" to socket
                toSend += "EMPTY\n";
                if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
                write(tcp.fd, toSend.c_str(), toSend.length());
            }
        //}
        return;
    }
    else if(command == "GHL"){
        //plid will be before the \n, not the space
        int space2 = message.find("\n", space1 + 1);
        string plid = message.substr(space1 + 1, space2 - space1 - 1);
        //plid must be 6 chracaters long and they must all be digits
        if(plid.length() != 6){
            string toSend = "ERR\n";
            if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
            write(tcp.fd, toSend.c_str(), toSend.length());
            return;
        }
        for(int i = 0; i < 6; i++){
            if(!isdigit(plid[i])){
                string toSend = "ERR\n";
                if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
                write(tcp.fd, toSend.c_str(), toSend.length());
                return;
            }
        }
        //check if plid is in active use
        if(!hasActiveGame(plid)){
            string toSend = "ERR\n";
            if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
            write(tcp.fd, toSend.c_str(), toSend.length());
            return;
        }
        else{
            //write "RHL " to socket
            string toSend = "RHL ";
            //check if there is a game for this plid if not, write "NOK\n" to socket
            if(!hasActiveGame(plid)){
                toSend += "NOK\n";
                if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
                write(tcp.fd, toSend.c_str(), toSend.length());
            }
            else{
                //if there is a game, write "OK " and send the file
                toSend += "OK ";
                if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
                write(tcp.fd, toSend.c_str(), toSend.length());
                //send the hint file
                fstream file = getActiveGameFile(plid);
                string hint = getHint(file);
                string hintPath = imagesDir + hint;
                tcpSender(tcp, (char *)hintPath.c_str(), (char *)hint.c_str());
                file.close();
            }
        }
        return;
    }
    else if(command == "STA"){
        //plid will be before the \n, not the space
        int space2 = message.find("\n", space1 + 1);
        string plid = message.substr(space1 + 1, space2 - space1 - 1);
        //plid must be 6 chracaters long and they must all be digits
        if(plid.length() != 6){
            string toSend = "ERR\n";
            if(verbose && superVerbose) cout << "Sending message1: " << toSend << endl;
            write(tcp.fd, toSend.c_str(), toSend.length());
            return;
        }
        for(int i = 0; i < 6; i++){
            if(!isdigit(plid[i])){
                string toSend = "ERR\n";
                if(verbose && superVerbose) cout << "Sending message2: " << toSend << endl;
                write(tcp.fd, toSend.c_str(), toSend.length());
                return;
            }
        }
        /*
        //check if plid is in active use
        if(!hasActiveGame(plid)){
            string toSend = "ERR\n";
            if(verbose && superVerbose) cout << "Sending message3: " << toSend << endl;
            write(tcp.fd, toSend.c_str(), toSend.length());
            return;
        }
        */
        //else{
        //write "RST " to socket
        string toSend = "RST ";
        //check if there is a game for this plid if not, write "NOK\n" to socket
        if(verbose && superVerbose) cout << "plid: " << plid << endl;
        if(!hasActiveGame(plid)){
            if(verbose && superVerbose) cout << "does not has active game." << endl;
            //check if derictory gamesDir + plid + "/" exists
            string dir = gamesDir + plid + "/";
            if(verbose && superVerbose) cout << "dir: " << dir << endl;
            if(!dirExists(dir)){
                if(verbose && superVerbose) cout << "does not have dir." << endl;
                //if not, write "NOK\n" to socket
                toSend += "NOK\n";
                if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
                write(tcp.fd, toSend.c_str(), toSend.length());
            }
            else{
                if(verbose && superVerbose) cout << "has dir." << endl;
                char fileNameChar[100];
                int got = FindLastGame((char *)plid.c_str(), fileNameChar);
                if(got == -1){
                    //if not, write "NOK\n" to socket
                    toSend += "NOK\n";
                    if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
                    write(tcp.fd, toSend.c_str(), toSend.length());
                }
                else{
                    string filename = fileNameChar;
                    if(verbose && superVerbose) cout << "filename: " << filename << endl;
                    //if yes, write "OK " and send the file
                    toSend += "FIN ";
                    if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
                    write(tcp.fd, toSend.c_str(), toSend.length());
                    //files are named YYYYMMDD_HHMMSS_code.txt, get the most recent one as well as the code
                    if(verbose && superVerbose) cout << "filename: " << filename << endl;
                    string name = "STATE_" + plid + ".txt";
                    string fileToSend = makeStateFileFinished(filename, plid);
                    if(verbose && superVerbose) cout << "fileToSend: " << fileToSend << endl;
                    tcpSender(tcp, (char *)fileToSend.c_str(), (char *)name.c_str());
                    if(!keepTempFiles)
                        remove(fileToSend.c_str());
                }
            }
        }
        else{
            if(verbose && superVerbose) cout << "have active game." << endl;
            //find last game file
            string filename = gamesDir + "GAME_" + plid + ".txt";
            string filenameProper = "GAME_" + plid + ".txt";
            //try to open the file
            fstream file;
            file.open(filename.c_str(), ios::in);
            //if file does not exist, write "NOK\n" to socket
            if(file.is_open()){
                file.close();
                if(verbose && superVerbose) cout << "got last game." << endl;
                //if got, write "ACT " and send the file
                toSend += "ACT ";
                if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
                write(tcp.fd, toSend.c_str(), toSend.length());
                string name = "STATE_" + plid + ".txt";
                string fileToSend = makeStateFileActive(filename, plid);
                tcpSender(tcp, (char *)fileToSend.c_str(), (char *)name.c_str());
                //delete fileToSend
                if(!keepTempFiles)
                    remove(fileToSend.c_str());
            }
            else{
                if(verbose && superVerbose) cout << "did not get last game." << endl;
                //if not, write "NOK\n" to socket
                toSend += "NOK\n";
                if(verbose && superVerbose) cout << "Sending message: " << toSend << endl;
                write(tcp.fd, toSend.c_str(), toSend.length());
            }
        }
        //}
        return;
    }
    //write "ERR\n" to socket
    else {
        string toSend = "ERR\n";
        if(verbose && superVerbose) cout << "Sending message4: " << toSend << endl;
        write(tcp.fd, toSend.c_str(), toSend.length());
        return;
    }
}

int FindLastGame(char *PLID, char *fname){
    struct dirent **filelist;
    int n_entries, found;
    char dirname[20];

    sprintf(dirname, "server/GAMES/%s/", PLID);
    n_entries = scandir(dirname, &filelist, 0, alphasort);
    found = 0;
    if (n_entries <= 0)
        return (0);
    else{
        while (n_entries--){
            if (filelist[n_entries]->d_name[0]!='.'){
                sprintf(fname, "server/GAMES/%s/%s", PLID, filelist[n_entries]->d_name);
                found = 1;
            }
            free(filelist[n_entries]);
            if(found)
                break;
        }
        free(filelist);
    }
    return (found);
}

int FindTopScores(SCORELIST *list){
    struct dirent **filelist;
    int n_entries, i_file;
    char fname [271];
    FILE *fp;

    n_entries = scandir("server/SCORES/", &filelist, 0, alphasort);

    i_file = 0;
    if (n_entries < 0){
        return (0);
    }
    else{
        while (n_entries--){
            if (filelist[n_entries]->d_name[0] != '.'){
                sprintf(fname, "server/SCORES/%s", filelist[n_entries]->d_name);
                fp = fopen(fname, "r");
                if (fp != NULL){
                    fscanf(fp, "%d %s %s %d %d" ,
                        &list->score[i_file], list->PLID[i_file], list->word[i_file], &list->n_succ[i_file],
                        &list->n_tot[i_file]);
                    if (verbose && superVerbose)cout << "Here are the words: " << list->word[i_file] << endl; //debug
                    fclose(fp);
                    ++i_file;
                }
            }
            free(filelist[n_entries]);
            if (i_file == NSCORES_IN_SCOREBOARD)
                break;
        }
        free(filelist);
    }
    list->n_scores = i_file;
    return (i_file);
}

string makeScoreboardFile(){
    //fname should be TOPSCORES_YYYYYMMDD_HHMMSS.txt
    string fname = "server/TOPSCORES_";
    time_t now = time(0);
    tm *ltm = localtime(&now);
    fname += to_string(1900 + ltm->tm_year);
    fname += to_string(1 + ltm->tm_mon);
    fname += to_string(ltm->tm_mday);
    fname += "_";
    fname += to_string(ltm->tm_hour);
    fname += to_string(ltm->tm_min);
    fname += to_string(ltm->tm_sec);
    fname += ".txt";

    /*
    SCORELIST scores;
    int check = FindTopScores(&scores);
    if(check == -1){
        cout << "Error: could not find any scores" << endl;
        return "";
    }
    printScores(scores);
    */
    //return "IDC";
    string toWrite = "\n-------------------------------- TOP 10 SCORES --------------------------------\n\n";
    toWrite += "    SCORE PLAYER     WORD                             GOOD TRIALS  TOTAL TRIALS\n\n";





    struct dirent **filelist;
    int n_entries = 10, i_file;
    char fileName [271];
    FILE *fp;

    n_entries = scandir("server/SCORES/", &filelist, 0, alphasort);

    i_file = 0;
    if (n_entries < 0){
        return (0);
    }
    else{
        while (n_entries--){
            if (filelist[n_entries]->d_name[0] != '.'){
                sprintf(fileName, "server/SCORES/%s", filelist[n_entries]->d_name);
                fp = fopen(fileName, "r");
                if (fp != NULL){
                    int score;
                    char PLID[7];
                    char word[30];
                    int n_succ;
                    int n_tot;
                    fscanf(fp, "%d %s %s %d %d" , &score, PLID, word, &n_succ, &n_tot);
                    string scoreStr = to_string(score);
                    string plidStr = PLID;
                    string wordStr = word;
                    string n_succStr = to_string(n_succ);
                    string n_totStr = to_string(n_tot);
                    fclose(fp);

                    toWrite += to_string(i_file) + " - ";
                    while(scoreStr.length() < 3){
                        scoreStr = "0" + scoreStr;
                    }
                    if (verbose && superVerbose)cout << "score: " << scoreStr << endl;
                    toWrite += scoreStr + "  ";

                    toWrite += plidStr + "  ";

                    toWrite += wordStr;

                    string exampleFromTejo = "thunderbird                             ";
                    //add to toWrite as many spaces as exampleFromTejo.size() - scores.word[i].length
                    for(int i = 0; i < exampleFromTejo.size() - wordStr.size(); i++){
                        toWrite += " ";
                    }

                    toWrite += n_succStr;
                    //add to toWrite as many spaces as 2 - n_succStr.length()
                    for(int i = 0; i < 2 - n_succStr.length(); i++){
                        toWrite += " ";
                    }

                    toWrite += n_totStr;
                    //add to toWrite as many spaces as 2 - n_totStr.length()
                    for(int i = 0; i < 2 - n_totStr.length(); i++){
                        toWrite += " ";
                    }

                    toWrite += "\n";

                    ++i_file;
                }
            }
            free(filelist[n_entries]);
            if (i_file == NSCORES_IN_SCOREBOARD)
                break;
        }
        free(filelist);
    }
    
    if(verbose && superVerbose) cout << "toWrite: " << toWrite << endl;
    //write to file
    fstream finalFile;
    //if a file already exists, it must be written over
    finalFile.open(fname, ios::out | ios::trunc);
    finalFile << toWrite;
    finalFile.close();
    if (verbose && superVerbose) cout << "Created file: " << fname << endl;
    if (verbose && superVerbose) cout << "File contents: " << toWrite << endl;
    return fname;
}
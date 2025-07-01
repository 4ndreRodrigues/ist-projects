/*
This file will create the player side of a hangman game which will comminucate with a server
through sockets. The player will be able to play the game and send the server the letters they
guess. The server will then send back the results of the guess and the player will be able to
see the results of their guess. Other features will also be implemented. The communication will
follow a specific protocol.
*/

// Include statements
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
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/wait.h>
#include <errno.h>

#define GN 27                           //Group Number
#define BUFFER_SIZE 256                 // Buffer size
#define TIMEOUT_SECONDS 5               // Timeout seconds
#define MAX_TIMEOUT_SEND_ATTEMPTS 3     // Timeout attempts

using namespace std;

// Global variables

string plyd;                // Player id
bool superVerbose = true;   // Super verbose mode



// Classes
class Game{
    public:

    int numLetters;
    int maxErrors;
    int numMisses;
    string word;
    int trials = 1;
    char lastLetter;
    string lastWord;

    // Constructors
    Game(){
        numLetters = 0;
        maxErrors = 0;
        numMisses = 0;
        word = "";
        lastLetter = ' ';
        lastWord = "";
    }

    Game(int numLetters, int maxErrors){
        this->numLetters = numLetters;
        this->maxErrors = maxErrors;
        numMisses = 0;
        for(int i = 0; i < numLetters; i++){
            word.push_back('_');
        }
    }

    // Methods
    void printWord(){
        for(int i = 0; i < numLetters; i++){
            cout << word[i];
        }
        cout << endl;
    }   //used for testing purposes

    string composeWord(){
        string word = "";
        for(int i = 0; i < numLetters; i++){
            word.push_back(' ');
            word.push_back(this->word[i]);
        }
        return word;
    }

    string newGameMessage(){
        string message = "New game started (max " + to_string(maxErrors) + " errors):" + composeWord();
        return message;
    }

    string positivePlayMessage(char letter, vector<int> positions){
        if(letter >= 'a' && letter <= 'z'){
            letter -= 32;
        }
        //for every int in positions array, change the corresponding index in word to letter
        for(int i = 0; i < positions.size(); i++){
            word[positions[i] - 1] = letter;
        }

        string message = "Yes, \"";
        message.push_back(letter);
        message += "\" is part of the word:" + composeWord();
        return message;
    }

    string negativePlayMessage(char letter){
        if(letter >= 'a' && letter <= 'z'){
            letter -= 32;
        }
        numMisses++;
        string message = "No, \"";
        message.push_back(letter); 
        message += "\" is not part of the word (" + to_string(maxErrors - numMisses) + " misses left):" + composeWord();
        return message;
    }

    bool hasWon(){
        for(int i = 0; i < numLetters; i++){
            if(word[i] == '_'){
                return false;
            }
        }
        return true;
    }

    bool hasLost(){
        if(numMisses == maxErrors){
            return true;
        }
        return false;
    }

    string winPlayMessage(){
        string message = "You won!" + composeWord();
        return message;
    }

    string loseMessage(){
        string message = "You lost!" + composeWord();
        return message;
    }

    string positiveGuessMessage(string word){
        for(int i = 0; i < word.length(); i++){
            if(word[i] >= 'a' && word[i] <= 'z'){
                word[i] -= 32;
            }
        }
        this->word = word;
        string message = "You won!" + composeWord();
        return message;
    }

    string negativeGuessMessage(string word){
        for(int i = 0; i < word.length(); i++){
            if(word[i] >= 'a' && word[i] <= 'z'){
                word[i] -= 32;
            }
        }
        numMisses++;
        string message = "No, \"" + word + "\" is not the word (" + to_string(maxErrors - numMisses) + " misses left):" + composeWord();
        return message;
    }

    void completeWithLetter(){
        char letter = this->lastLetter;
        if(letter >= 'a' && letter <= 'z'){
            letter -= 32;
        }
        for(int i = 0; i < numLetters; i++){
            if(word[i] == '_'){
                word[i] = letter;
            }
        }
    }
};

Game game = Game(); // Game object - Global variable

// Structs
struct protocol
{
    string message;
    bool UDP;
    bool exit = false;
    bool send = true;
};

struct puppet
{
    int fd;
    struct addrinfo *res;
};



// Function prototypes
protocol protocolStart(int numLetters, int maxErrors);
protocol protocolPlay(char letter, vector<int> positions);
protocol protocolGuessWord(string word);
protocol protocolQuit();
protocol protocolReveal();
protocol protocolScoreboard();
protocol protocolHint();
protocol protocolState();
protocol protocolEncoder(string message);
void decodeStart(string message);
void decodePlay(string message);
void decodeGuess(string message);
void decodeQuit(string message);
void decodeReveal(string message);
void protocolDecoder(string message);

puppet openUDPSocket(string address, string port);
puppet openTCPSocket(string address, string port);
void closeSocket(puppet puppet);
void tcpProcessor(puppet tcp);
string processFileGetter(string message, int space2, puppet tcp, char buffer[BUFFER_SIZE]);
void tcpHandler(puppet tcp, protocol prot);
void udpHandler(puppet udp, protocol prot);





/*  
    ============================= - Main Function - =============================
*/

/*Called with:
./player [-n GSIP] [-p GSport]*/
int main(int argc, char *argv[]){
    //if GSIP is not not specified, use localhost
    string gsIp = "localhost";
    //if it is specified then use the specified IP
    if(argc > 1){
        gsIp = argv[2];
    }
    //if GSport is not specified, use 58000 + group number (GN)
    int gsPort = 58000 + GN;
    //int gsPort = 58011;
    //if it is specified then use the specified port
    if(argc > 3){
        gsPort = atoi(argv[4]);
    }

    cout << "\t------------------ Manual for player: ------------------" << endl;
    cout << "\tstart/st PLID - start a new game. PLID - player ID" << endl;
    cout << "\tplay/pl LETTER - play a letter. LETTER - letter to play" << endl;
    cout << "\tguess/gs WORD - guess the word. WORD - word to guess" << endl;
    cout << "\tscoreboard/sb - show scoreboard" << endl;
    cout << "\thint/h - show hint" << endl;
    cout << "\tstate/st - show state" << endl;
    cout << "\tquit - quit the game" << endl;
    cout << "\texit - exit the program" << endl;
    cout << "\t--------------------------------------------------------" << endl;

    //create a UDP socket to connect to the game server
    //pass port as string because openUDPSocket expects a string
    puppet udp = openUDPSocket(gsIp, to_string(gsPort));

    protocol prot;
    //while prot doesnt have the exit flag set, read lines from standard input and change prot by processing the line with protocolEncoder
    while(!(prot.exit)){
        string line;
        getline(cin, line);
        prot = protocolEncoder(line);

        if(superVerbose)cout << "Protocol message: " << prot.message << endl;
        //if prot has the UDP flag set, send the message to the game server
        if(prot.send){
            if(prot.UDP){
                udpHandler(udp, prot);
            }
            //if the prot has the TCP flag set, fork a child process to handle the TCP connection with tcpHandler
            else{
                //pass port as string because openTCPSocket expects a string
                puppet tcp = openTCPSocket(gsIp, to_string(gsPort));
                if(superVerbose)cout << "TCP socket created" << endl;
                tcpHandler(tcp, prot);
            }
        }
    }
    //close cocket and exit
    closeSocket(udp);
    return 0;
}






/*  
    ============================= - Function Definitions - =============================
*/

//  ---------------- = Protocol Functions = ----------------

// Functions to create the strings to be sent to the server

// SNG plyd
protocol protocolStart(){
    protocol start;
    start.message = "SNG " + plyd;
    start.UDP = true;
    return start;
}

// PLG plyd letter trials
protocol protocolPlay(char letter){
    protocol play;
    play.message = "PLG " + plyd + " " + letter + " " + to_string(game.trials);
    play.UDP = true;
    game.lastLetter = letter;
    return play;
}

// PGW plyd word trials
protocol protocolGuessWord(string word){
    protocol guessWord;
    guessWord.message = "PWG " + plyd + " " + word + " " + to_string(game.trials);
    guessWord.UDP = true;
    game.lastWord = word;
    return guessWord;
}

// QUT plyd
protocol protocolQuit()
{
    protocol quit;
    quit.message = "QUT " + plyd;
    quit.UDP = true;
    return quit;
}

// REV plyd
protocol protocolReveal()
{
    protocol reveal;
    reveal.message = "REV " + plyd;
    reveal.UDP = true;
    return reveal;
}

// GSB
protocol protocolScoreboard()
{
    protocol scoreboard;
    scoreboard.message = "GSB";
    scoreboard.UDP = false;
    return scoreboard;
}

//GHL plyd
protocol protocolHint()
{
    protocol hint;
    hint.message = "GHL " + plyd;
    hint.UDP = false;
    return hint;
}

// STA plyd
protocol protocolState()
{
    protocol state;
    state.message = "STA " + plyd;
    state.UDP = false;
    return state;
}

/*
    The protocolEncoder will take in a string typed by the user and encode it into the protocol
    format. The protocolEncoder will then return the encoded string as a protocol structure.
    It will also process anything that needs to be done on the players side upon the recevied command.

    These are the possibilities of input:
    Variables are between []
    1. "start [plyd]" or "sg [plyd]" - Sets the player id to the given id and uses protocolStart().
    2. "play [letter]" or "pl [letter]" - Uses protocolPlay() with the given letter.
    3. "guess [word]" or "gw [word]" - Uses protocolGuess() with the given word.
    4. "scoreboard" or "sb" - Uses protocolScoreboard().
    5. "hint" or "h" - Uses protocolHint().
    6. "state" or "st" - Uses protocolState().
    7. "quit" - Uses protocolQuit().
    8. "exit" - Uses protocolQuit() and sets the exit flag to true.
    9. "rev" - Uses protocolReveal().
*/
protocol protocolEncoder(string input){
    protocol encoded;
    string command;
    string letter;
    string word;
    string id;
    int space = input.find(" ");
    if(space != -1){
        command = input.substr(0, space);
        if(command == ""){
            command = input;
        }
        if(command == "start" || command == "sg"){
            id = input.substr(space + 1);
            //if must be 6 characters and they msut be digits
            if(id.length()!= 6){
                cout << "Invalid player id. MUST be a 6 digit number." << endl;
                encoded.send = false;
            }
            else{
                for(int i = 0; i < 6; i++){
                    if(!isdigit(id[i])){
                        cout << "Invalid player id. MUST be a 6 digit number." << endl;
                        encoded.send = false;
                        return encoded;
                    }
                }
                
                //there cant be anything after id
                if(input.length() > space + 7){
                    cout << "Invalid player id. MUST be a 6 digit number." << endl;
                    encoded.send = false;
                }
                else{
                    plyd = id;
                    encoded = protocolStart();
                }
            }
        }
        else if(command == "play" || command == "pl"){
            letter = input.substr(space + 1);
            //letter must be a single character and it must be a letter
            if(letter.length() != 1){
                cout << "Invalid letter. MUST be a single letter." << endl;
                encoded.send = false;
            }
            else if(!isalpha(letter[0])){
                cout << "Invalid letter. MUST be a single letter." << endl;
                encoded.send = false;
            }
            //there cant be anything after letter
            else if(input.length() > space + 3){
                cout << "Invalid letter. MUST be a single letter." << endl;
                encoded.send = false;
            }
            else{
                encoded = protocolPlay(letter[0]);
            }
        }
        else if(command == "guess" || command == "gw"){
            word = input.substr(space + 1);
            //all the characters of word must be letters
            for(int i = 0; i < word.length(); i++){
                if(!isalpha(word[i])){
                    cout << "Invalid word. MUST be a single word." << endl;
                    encoded.send = false;
                    return encoded;
                }
            }
            //there cant be anything after word
            if(input.length() > space + word.length() + 2){
                cout << "Invalid word. MUST be a single word." << endl;
                encoded.send = false;
            }
            else{
                encoded = protocolGuessWord(word);
            }
        }
        else{
            cout << "Invalid command" << endl;
            encoded.send = false;
        }
    }
    else{
        if(input == "quit"){
            encoded = protocolQuit();
        }
        else if(input == "exit"){
            encoded = protocolQuit();
            encoded.exit = true;
        }
        else if(input == "rev"){
            encoded = protocolReveal();
        }
        else if(input == "scoreboard" || input == "sb"){
            encoded = protocolScoreboard();
        }
        else if(input == "hint" || input == "h"){
            encoded = protocolHint();
        }
        else if(input == "state" || input == "st"){
            encoded = protocolState();
        }
        else{
            cout << "Invalid command" << endl;
            encoded.send = false;
        }
    }
    encoded.message += "\n";
    return encoded;
}


// Functions to process the messages from the server

// status [n_letters max_errors] - status = OK or NOK
void decodeStart(string message){
    string status;
    int space = message.find(" ");
    status = message.substr(0, space);
    if(superVerbose)cout << "Status: " << status << endl;
    if(status == "OK"){
        string n_letters;
        string max_errors;
        int space2 = message.find(" ", space + 1);
        n_letters = message.substr(space + 1, space2 - space - 1);
        max_errors = message.substr(space2 + 1);
        if(max_errors[max_errors.length() - 1] == '\n'){
            max_errors = max_errors.substr(0, max_errors.length() - 1);
            game = Game(stoi(n_letters), stoi(max_errors));
            cout << game.newGameMessage() << endl;
        }
        else{
            cout << "Invalid Communication" << endl;
        }
    }
    else if(status == "NOK\n"){
        cout << "There is already a game in process" << endl;
    }
    else{
        cout << "Invalid Communication" << endl;
    }
}

// status trial [n pos]\n - status = OK or WIN or DUP or NOK or OVR or INV or ERR
// n - number of occurenccess of the letter in the word
// pos - positions of the letter in the word, "pos1 pos2 pos3 ..."
// if OK then it will have n and pos
// if WIN then word was guessed
// if DUP then letter was already guessed
// if NOK the letter was not part of the word
// if OVR then the max number of errors was reached and game is over
// if INV then the trial number was invalid
// if ERR then there was an error and there is not trial number
// the message has to end with "\n", if not then the user should be informed by cout "Invalid Communication"
void decodePlay(string message){
    string status;
    string trial;
    string n;
    string pos;
    int space = message.find(" ");
    status = message.substr(0, space);
    if(status == "OK"){
        int space2 = message.find(" ", space + 1);
        trial = message.substr(space + 1, space2 - space - 1);
        if(superVerbose)cout << "Trial: " << trial << endl; //for testing
        int space3 = message.find(" ", space2 + 1);
        n = message.substr(space2 + 1, space3 - space2 - 1);
        if(superVerbose)cout << "N: " << n << endl; //for testing
        pos = message.substr(space3 + 1);
        if(superVerbose)cout << "Pos: " << pos << endl; //for testing
        if (pos[pos.length() - 1] != '\n'){
            cout << "Invalid Communication3" << endl;
        }
        else{
            game.trials++;
            vector<int> positions;
            int i = 0;
            int space4 = pos.find(" ");
            while(space4 != -1){
                positions.push_back(stoi(pos.substr(0, space4)));
                pos = pos.substr(space4 + 1);
                space4 = pos.find(" ");
                i++;
            }
            positions.push_back(stoi(pos.substr(0, pos.length() - 1)));
            cout << game.positivePlayMessage(game.lastLetter, positions) << endl;
        }
    }
    else if(status == "WIN"){
        int space2 = message.find(" ", space + 1);
        trial = message.substr(space + 1, space2 - space - 1);
        if(trial[trial.length() - 1] != '\n'){
            cout << "Invalid Communication" << endl;
        }
        else{
            game.trials++;
            game.completeWithLetter();
            cout << game.winPlayMessage() << endl;
        }
    }
    else if(status == "DUP"){
        int space2 = message.find(" ", space + 1);
        trial = message.substr(space + 1, space2 - space - 1);
        if(trial[trial.length() - 1] != '\n'){
            cout << "Invalid Communication" << endl;
        }
        else{
            cout << "Letter " << game.lastLetter << " was already played." << endl;
        }
    }
    else if(status == "NOK"){
        int space2 = message.find(" ", space + 1);
        trial = message.substr(space + 1, space2 - space - 1);
        if(trial[trial.length() - 1] != '\n'){
            cout << "Invalid Communication" << endl;
        }
        else{
            game.trials++;
            cout << game.negativePlayMessage(game.lastLetter) << endl;
        }
    }
    else if(status == "OVR"){
        int space2 = message.find(" ", space + 1);
        trial = message.substr(space + 1, space2 - space - 1);
        if(trial[trial.length() - 1] != '\n'){
            cout << "Invalid Communication" << endl;
        }
        else{
            cout << game.loseMessage() << endl;
        }
    }
    else if(status == "INV"){
        int space2 = message.find(" ", space + 1);
        trial = message.substr(space + 1, space2 - space - 1);
        if(trial[trial.length() - 1] != '\n'){
            cout << "Invalid Communication" << endl;
        }
        else{
            cout << "Invalid play." << endl;
        }
    }
    else if(status == "ERR\n"){
        cout << "Sintax Error. Try again." << endl;
    }
    else{
        cout << "Invalid Communication" << endl;
    }
}

//status trials
//status = WIN NOK OVR INV or ERR, trials must always end with "\n", if it doesnt, cout "Invalid Communication"
// for every different status, simply print 1, 2, 3, 4, 5 or 6 with 1 for win, 2 for nok, etc
// if the status is ERR, there is no trial number, so just print 6, ERR must still end with "\n"
void decodeGuess(string message){
    string status;
    string trial;
    int space = message.find(" ");
    status = message.substr(0, space);
    if(status == "WIN"){
        int space2 = message.find(" ", space + 1);
        trial = message.substr(space + 1, space2 - space - 1);
        if(trial[trial.length() - 1] != '\n'){
            cout << "Invalid Communication" << endl;
        }
        else{
            cout << game.positiveGuessMessage(game.lastWord) << endl;
        }
    }
    else if(status == "DUP"){
        int space2 = message.find(" ", space + 1);
        trial = message.substr(space + 1, space2 - space - 1);
        if(trial[trial.length() - 1] != '\n'){
            cout << "Invalid Communication" << endl;
        }
        else{
            cout << "Word: " << game.lastWord << " was already tried." << endl;
        }
    }
    else if(status == "NOK"){
        int space2 = message.find(" ", space + 1);
        trial = message.substr(space + 1, space2 - space - 1);
        if(trial[trial.length() - 1] != '\n'){
            cout << "Invalid Communication" << endl;
        }
        else{
            game.trials++;
            cout << game.negativeGuessMessage(game.lastWord) << endl;
        }
    }
    else if(status == "OVR"){
        int space2 = message.find(" ", space + 1);
        trial = message.substr(space + 1, space2 - space - 1);
        if(trial[trial.length() - 1] != '\n'){
            cout << "Invalid Communication" << endl;
        }
        else{
            cout << game.loseMessage() << endl;
        }
    }
    else if(status == "INV"){
        int space2 = message.find(" ", space + 1);
        trial = message.substr(space + 1, space2 - space - 1);
        if(trial[trial.length() - 1] != '\n'){
            cout << "Invalid Communication" << endl;
        }
        else{
            cout << "Invalid Guess." << endl;
        }
    }
    else if(status == "ERR\n"){
        cout << "6" << endl;
    }
    else{
        cout << "Invalid Communication" << endl;
    }
}

//status
//can be OK or ERR, must end with "\n", if it doesnt, cout "Invalid Communication"
//if OK, print 1, if ERR, print 2
void decodeQuit(string message){
    string status;
    int space = message.find(" ");
    status = message.substr(0, space);
    if(status == "OK\n"){
        cout << "Quit SuccessFull." << endl;
        return;
    }
    else if(status == "NOK\n"){
        cout << "Player has no ongoing games." << endl;
        return;
    }
    else if(status == "ERR\n"){
        cout << "Failuire to quit." << endl;
        return;
    }
    else{
        cout << "Invalid Communication" << endl;
        return;
    }
}

void decodeReveal(string message){
    cout << message << endl;
}

//type message
//type can be:
// RSG - start
// RLG - play
// RWG - guess
// RQT - quit
// RRV - reveal
//always return false except with quit
void protocolDecoder(string message){
    string type;
    int space = message.find(" ");
    type = message.substr(0, space);
    //create a string called secondaryMessage that is the message without the type
    string secondaryMessage = message.substr(space + 1, message.length() - space - 1);
    if(type == "RSG"){
        decodeStart(secondaryMessage);
        return;
    }
    else if(type == "RLG"){
        decodePlay(secondaryMessage);
        return;
    }
    else if(type == "RWG"){
        decodeGuess(secondaryMessage);
        return;
    }
    else if(type == "RQT"){
        decodeQuit(secondaryMessage);
        return;
    }
    else if(type == "RRV"){
        decodeReveal(secondaryMessage);
        return;
    }
    else{
        cout << "Invalid Communication" << endl;
        return;
    }
}





//  ---------------- = Socket Functions = ----------------

puppet openUDPSocket(string address, string port){
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

    memset(&hints,0,sizeof hints);

    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_DGRAM; //UDP socket

    char* addressChar = new char[address.length() + 1];
    strcpy(addressChar, address.c_str());
    char* portChar = new char[port.length() + 1];
    strcpy(portChar, port.c_str());

    if(superVerbose)cout << "Confirming, address and port: " << addressChar << " | " << portChar << endl;

    errcode = getaddrinfo(addressChar,portChar,&hints,&res);
    if(errcode!=0){
        cout << "Error getting address info" << endl;
        exit(1);
    }

    p.fd = fd;
    p.res = res;
    return p;
}

puppet openTCPSocket(string address, string port){
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

    memset(&hints,0,sizeof hints);

    hints.ai_family = AF_INET; //IPv4
    hints.ai_socktype = SOCK_STREAM; //TCP socket

    char* addressChar = new char[address.length() + 1];
    strcpy(addressChar, address.c_str());
    char* portChar = new char[port.length() + 1];
    strcpy(portChar, port.c_str());

    errcode = getaddrinfo(addressChar,portChar,&hints,&res);
    if(errcode!=0){
        cout << "Error getting address info" << endl;
        exit(1);
    }

    p.fd = fd;
    p.res = res;
    return p;
}

void closeSocket(puppet p){
    close(p.fd);
    freeaddrinfo(p.res);
}

void displayFile(string fname){
    string line;
    ifstream myfile (fname);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            cout << line << '\n';
        }
        myfile.close();
    }
    else cout << "Unable to open file";
}


/*This function will be given the information for a tcp socket as a puppet struct
it should read from this socket with a buffer and process the invormation retrieved.
The messages will be in one of the following formats:
RSB status [fname fsize fdata] - if status = EMPTY then it should cout "scoreboard is currently empty."
                                 if status = OK, then fname, fsize and fdata will be given a file with fname needs to be created or changed
                                 to have what is in fdata. This should be a text file.
RHL status [fname fsize fdata] - if status = NOK then it should cout "Problem receiving hint."
                                 if status = OK, then fname, fisize and fdata will be given and a file with fname needs to be created or changed
                                 to have what is in fdata. This should be an image file.
RST status [fname fsize fdata] - if status = NOK then it should cout "No state was found."
                                 if status = ACT then fname, fsize and fdata will be given and a file with fname needs to be created or changed
                                 to have what is in fdata. This should be a text file.
                                 if status = FIN then fname, fsize and fdata will be given and a file with fname needs to be created or changed
                                 to have what is in fdata. This should be a text file.
                                 if a file is received, it's information should also be presented for the user in the terminal in RST.
fdata can contain "\n" so, when reading it it should be read until the fsize is reached.
Use a buffer of size BUFFER_SIZE, the message in the tcp socket can be bigger than this buffer. This function does not use any others defined in this document.*/
void tcpProcessor(puppet tcp){
    char buffer[BUFFER_SIZE];

    int n = read(tcp.fd, buffer, BUFFER_SIZE);
    if(n < 0){
        cout << "Error reading from socket" << endl;
        exit(1);
    }

    if(superVerbose)cout << "tcpProcessor buffer: " << buffer << ".end." << endl;

    string message = buffer;

    if(superVerbose)cout << "tcpProcessor message: " << message << endl;

    string type = message.substr(0, 3);
    int space1 = message.find(" ");
    int space2 = message.find(" ", space1 + 1);
    string status = message.substr(space1 + 1, space2 - space1 - 1);
    if(type == "RSB"){
        //if status is EMPTY then cout "scoreboard is currently empty."
        if(status == "EMPTY"){
            cout << "scoreboard is currently empty." << endl;
        }
        else if(status == "OK"){
            string fname = processFileGetter(message, space2, tcp, buffer);
            displayFile(fname);
        }
        else{
            cout << "Invalid Communication" << endl;
        }
    }
    else if(type == "RHL"){
        //if status is NOK then cout "Problem receiving hint."
        if(status == "NOK"){
            cout << "Problem receiving hint." << endl;
        }
        else if(status == "OK"){
            string fname = processFileGetter(message, space2, tcp, buffer);
        }
    }
    else if(type == "RST"){
        if(status == "ACT"){
            cout << "An active game was found." << endl;
            string fname = processFileGetter(message, space2, tcp, buffer);
            displayFile(fname);
        }
        else if(status == "FIN"){
            cout << "A finished game was found." << endl;
            string fname = processFileGetter(message, space2, tcp, buffer);
            displayFile(fname);
            //game is assumed as finished
            game = Game();
        }
        else if(status == "NOK"){
            cout << "No state was found." << endl;
        }
        else{
            cout << "Invalid Communication" << endl;
        }
    }
    else{
        cout << "Invalid Communication" << endl;
    }
}

string processFileGetter(string message, int space2, puppet tcp, char buffer[BUFFER_SIZE]){
    int n;
    //find fname, it can be bigger than the buffer so it might be necessary to read more then once to complete the fname
    string fname = "playerDir/";
    int space3 = message.find(" ", space2 + 1);
    if (space3 == -1){
        while(1){
            n = read(tcp.fd, buffer, BUFFER_SIZE);
            if(n < 0){
                cout << "Error reading from socket" << endl;
                exit(1);
            }
            if(superVerbose)cout << "buffer: " << buffer << endl;
            message = buffer;
            space3 = message.find(" ");
            if(space3 != -1){
                fname += message.substr(0, space3);
                break;
            }
            else{
                fname += message;
            }
        }
    }
    else{
        fname = message.substr(space2 + 1, space3 - space2 - 1);
    }
    if(superVerbose)cout << "fname: " << fname << endl;
    //find fsize, same conditions as fname
    string fsize = "";
    int space4 = message.find(" ", space3 + 1);
    if (space4 == -1){
        while(1){
            n = read(tcp.fd, buffer, BUFFER_SIZE);
            if(n < 0){
                cout << "Error reading from socket3" << endl;
                exit(1);
            }
            message = buffer;
            space4 = message.find(" ");
            if(space4 != -1){
                fsize += message.substr(0, space4);
                break;
            }
            else{
                fsize += message;
            }
        }
    }
    else{
        fsize = message.substr(space3 + 1, space4 - space3 - 1);
    }
    if(superVerbose)cout << "fsize: " << fsize << endl;
    //fdata will be of fsize, it can contain " " "\n" and any other character, it should be read until the fsize is reached
    //as it is read it should be written into a file with fname, if this file doesnt exist it should be created, if it does exist it should be overwritten
    //the beggining of fdata can already be in the buffer, so it should be written to the file before reading more from the socket
    //as it is read it should be written to the file immediately
    //the file should be closed when the fsize is reached
    //if file of fname exists it should be overwritten
    ofstream file;
    file.open(fname, ios::binary | ios::out | ios::trunc);
    int fsizeInt = stoi(fsize);
    int writeAmmount = n - space4 - 1;
    if (writeAmmount > fsizeInt){
        writeAmmount = fsizeInt;
    }
    file.write(buffer + space4 + 1, writeAmmount);
    if(superVerbose)cout << "was written: " << buffer + space4 + 1 << ".end." << endl;
    if(superVerbose)cout << "here! fsize: " << fsize << endl;
    int fsizeRead = writeAmmount;
    while(fsizeRead < fsizeInt){
        n = read(tcp.fd, buffer, BUFFER_SIZE);
        if(n < 0){
            cout << "Error reading from socket" << endl;
            exit(1);
        }
        int toWrite = n;
        if(fsizeRead + toWrite > fsizeInt){
           toWrite = fsizeInt - fsizeRead;
        }
        file.write(buffer, toWrite);
        fsizeRead += toWrite;
    }
    if(superVerbose)cout << "fsizeInt: " << fsizeInt << endl;
    file.close();
    //fileName will be fname after the first /
    string fileName;
    int slash = fname.find("/");
    if(slash != -1){
        fileName = fname.substr(slash + 1);
    }
    else{
        fileName = fname;
    }
    cout << "File " << fileName << " received. Fsize = " << fsize << endl;
    return fname;
}

int timeLimitExceeded(int fd){
    fd_set set;
    struct timeval timeout;
    FD_ZERO(&set);
    FD_SET(fd, &set);
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;

    if(select(fd + 1, &set, NULL, NULL, &timeout) == 0){
        cout << "Time limit exceeded" << endl;
        return 1;
    }

    return 0;
}

//This function should send the message in protocol to the server and then receive the answer and pass it to protocolDecoder, returning its value
//if the server does not respond in 5 seconds, it should send the message again, up to 7 times
void udpHandler(puppet udp, protocol prot){
    int n;
    struct sockaddr_in addr;
    socklen_t addrlen;

    if(superVerbose)cout << "Sending message: " << prot.message << endl;

    struct timeval tv;
    tv.tv_sec = 5;
    tv.tv_usec = 0;
    setsockopt(udp.fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);
    //send message
    n = sendto(udp.fd, prot.message.c_str(), prot.message.length(), 0, udp.res->ai_addr, udp.res->ai_addrlen);
    if(n < 0){
        cout << "Error writing to socket" << endl;
        exit(1);
    }
    if(superVerbose)cout << "sent!" << endl;
    //receive answer
    char buffer[BUFFER_SIZE];
    //n = recvfrom(udp.fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &addr, &addrlen);
    
    int tries = 0;
    while(timeLimitExceeded(udp.fd)){
        if(tries < MAX_TIMEOUT_SEND_ATTEMPTS - 1){
            cout << "sending again" << endl;
            n = sendto(udp.fd, prot.message.c_str(), prot.message.length(), 0, udp.res->ai_addr, udp.res->ai_addrlen);
            if(n < 0){
                cout << "Error writing to socket" << endl;
                exit(1);
            }
            tries++;
        }
        else{
            cout << tries << " tries exceeded" << endl;
            exit(1);
        }
    }

    n = recvfrom(udp.fd, buffer, BUFFER_SIZE, 0, (struct sockaddr*) &addr, &addrlen);
    if(n < 0){
        cout << "Error reading from socket" << endl;
        exit(1);
    }


    if(superVerbose)cout << "received: " << buffer << endl; 
    //search the buffer for the first "\n" and convert the characters up to that point to a string ("\n" should be included)
    //if no "\n" is found, cout "Invalid Communication" and return true
    for(int i = 0; i < BUFFER_SIZE; i++){
        if(buffer[i] == '\n'){
            buffer[i + 1] = '\0';
            break;
        }
    }
    string message = buffer;
    if(superVerbose)cout << "received message: " << message << endl;
    //pass answer to protocolDecoder
    protocolDecoder(message);
    return;

}

//This function should establish conecntion with the server and send the message given in the parameter
//it should then call tcpProcessor to process the information received from the server and close the socket
void tcpHandler(puppet tcp, protocol prot){
    //establish connection with the server
    int n = connect(tcp.fd, tcp.res->ai_addr, tcp.res->ai_addrlen);
    if(n < 0){
        cout << "Error connecting to server" << endl;
        exit(1);
    }
    //send message
    n = write(tcp.fd, prot.message.c_str(), prot.message.length());
    if(n < 0){
        cout << "Error writing to socket" << endl;
        exit(1);
    }
    if(superVerbose)cout << "sent! -> " << prot.message << endl;
    //process information received from the server
    tcpProcessor(tcp);
    //close socket
    closeSocket(tcp);
}
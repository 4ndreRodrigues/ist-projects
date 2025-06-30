Grupo 27

IMPORTANT - Images are not included

File organization:
    - make should be done inside the main directory
    - the executable files should also be run inside the main directory
    - ./GS must be given a filename for the word file as a path, with the organization I am providing
        it should be server/words_eng.txt
    - The images should be inside the directory IMAGES which is inside of server, server/images

Makefile:
    - a simple make comand will clean the executables and create new ones
    - "make squeaky" will do the same as a simple make but also remove the directories GAMES and SCORES inside of server

Player code:
    - TIMEOUT_SECONDS is the number of seconds the player will wait for a response from the server
    - MAX_TIMEOUT_SEND_ATTEMPTS is the number of times the player will try to send a message to the server
    global variable flags:
        - superVerbose: when true will print out a lot of information, useful for debugging

Server code:
    - NSCORES_IN_SCOREBOARD is the number of scores that will be saved in the scoreboard
    global variable flags:
        - superVerbose: when true will print out a lot of information, useful for debugging
        - verbose: when true will print out simple information
        - keepTempFiles: when true will not delete the temporary files created by the server
    
Notes:
    - It is assumed that the value of score, once a game is finished is given by the percentage of total tries that were correct
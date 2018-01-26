# Pickupbox
**Systems Level Programming - Final Project**

by Cesar Mu & Alex Shevchenko pd. 5

## Description ##
Pickupbox (PUB) behaves just like Dropbox; the user can upload, store, and download files between their local directory and their PUB server directory. The PUB Client is a command shell where the user can browse through their local folders. Once the user wants to interact with their PUB directory, they can simply run a special "pub" command.

## Instructions ##
**Compiling**
1. Run ``make`` in terminal.

**Starting the program**
1. In one terminal session, call ``./server``. The PUB Server will load and wait for a client connection.
2. In another terminal session, call ``./client``. The PUB Client will load and connect to the PUB Server.

**Using the program**
1. The PUB Client will first ask for a username. If your username already exists, you will be signed into your existing account. Otherwise, a new PUB will be created for you.
2. Now, you will be in the command shell. Type ``pubhelp`` for a list of commands used to interact with your PUB.
3. To upload a file to your PUB, run ``pubup [filename] [optional new PUB name]``.
4. To view the files stored in your PUB, run ``publs``.
5. To download a file from your PUB, run ``pubdown [PUB file] [optional new file name]``.
6. To rename a file in your PUB, run ``pubrename [old name] [new name]``.
7. To delete a file from your PUB, run ``pubdel [file name]``.
8. To see the size or number of files in your PUB, run ``pubinfo [size or amt]``.
9. To see the current user you are logged in as, run ``pubuser``.
10. To switch to a different user, run ``pubswitch [username]``. This will switch to an existing user or create a new one.
11. To permanently delete all your PUB files, your PUB directory, and your PUB account, run ``pubremove``. You will be asked to confirm this action

**Cleaning up**
1. Run ``make clean`` in terminal
**OR**
2. Run ``make bleach`` in terminal to delete the "users" folder as well.

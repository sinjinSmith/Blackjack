
/* 
blackjack.cpp
Simple Blackjack Game in Console
Purpose: Trying learning more C++ principles including OOP, classes, vectors, etc.
Program currently works but could do with some optimization and minor improvements.

Game currently runs with one deck and shuffles it before every game, objective 
is to get as close to 21 as possible without not going over while being
higher than the dealer.
*/

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <random>
#include <chrono>
#include <thread>
#include <algorithm>
#include <vector>
//for outputing unicode16 chars
#include <fcntl.h>
#include <io.h>
#include <stdio.h>
//set colors for console
#define RED "\e[31;47m"   //red w/white bg
#define BLACK "\e[30;47m" //black w/white bg
#define RESET "\e[0m"     //resets all colors

using namespace std; 
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono;      // nanoseconds, system_clock, seconds


//create playing card class
class playing_card
{
    //card_name for displaying, card_number in deck, and card_value in game
    string card_name;
    int card_number, card_value, suite;
    char diplay_char;

public:
    void create_card(string card, int number, int value, int suite_num, char display);
    string name();
    int show_number();
    int retvalue();
    int retsuite();
    char display();
};

int playing_card::retsuite()
{
    return suite;
}

char playing_card::display()
{
    return diplay_char;
}

string playing_card::name()
{
    return card_name;
}

void playing_card::create_card(string card, int number, int value, int suite_num, char display)
{
    card_name = card;
    card_number = number;
    card_value = value;
    suite = suite_num;
    diplay_char = display;
}

int playing_card::show_number()
{
    return card_number;
}

int playing_card::retvalue()
{
    return card_value;
}
//end of playing_card methods


//create player class
class player
{
    int hand_value = 0;
    int ace = 0;

public:
    void dealt_card(int card_val);
    void reduce_ace_val();
    void has_ace();
    int retvalue();
    int retace();
};

void player::dealt_card(int card_val)
{
    hand_value += card_val;
}

void player::reduce_ace_val()
{
    hand_value = hand_value - 10;
    ace--;
}

int player::retace()
{
    return ace;
}

void player::has_ace()
{
    ace++;
}

int player::retvalue()
{
    return hand_value;
}
//end of player methods

//declaration of functions
bool start();
bool hit(int hand);
bool show_winner(int dealer_hand, int player_hand);
void show_card(int suite, char card, int suite2, char card2);
//function to help randomize deck each time
int myrandom(int i) { return rand() % i; }


//main function
int main()
{
    bool exit = false;
    while (!exit)
    {
        exit = start();
    }
    return 0;
}


//!!! NEED TO CLEAN UP, REMOVE UNNECESSARY CODE, OPTIMIZE, ADD MORE COMMENTS
//!!! MIGHT NEED TO SPLIT INTO MULTIPLE FUNCTIONS IF POSSIBLE
//handles most of the logic necessary to run the game
bool start()
{
    srand(time(NULL));
    player player1;
    player dealer;
    playing_card deck[52];

    int card, card1, card2, card3, player_hand, dealer_hand, next_card;
    int deck_pos = 0;
    bool hit_me;
    bool stand = false;
    bool dealer_stand = false;
    system("cls");

    int deck_limit = 52;
    string card_name, hearts, diamonds, spades, clubs;
    hearts = "Hearts";
    diamonds = "Diamonds";
    spades = "Spades";
    clubs = "Clubs";
    int card_num = 1;

    // creates vector for deck, adds 52 cards, then randomly shuffles the order
    vector<int> deck_order;

    for (int i = 1; i <= deck_limit; i++) deck_order.push_back(i);

    random_shuffle(deck_order.begin(), deck_order.end());
    random_shuffle(deck_order.begin(), deck_order.end(), myrandom);

    /*
    //Used for debugging to see order of deck

    cout << "Deck contains:";
    for (vector<int>::iterator it = deck_order.begin(); it != deck_order.end(); ++it)
    {
        cout << '\n';
        cout << ' ' << *it;
    }
    */

    //cout << deck_order.at(3) << endl;

    int card_value;
    string suite;
    int suite_val;
    char display_char;

    //creates deck with playing_card objects
    for (int i = 0; card_num <= deck_limit; i++)
    {
        //set aces value
        if (card_num == 1 || card_num == 14 || card_num == 27 || card_num == 40)
        {
            card_value = 11;
            card_name = "Ace";
            display_char = 'A';
        }
        //sets 2 - 10
        else if (1 < card_num && card_num < 11)
        {
            card_value = card_num;
            card_name = to_string(card_value);
            display_char = '0' + card_value;
        }
        //sets all face card values
        else if (card_num == 11 || card_num == 24 || card_num == 37 || card_num == 50)
        {
            card_value = 10;
            card_name = "Jack";
            display_char = 'J';
        }
        else if (card_num == 12 || card_num == 25 || card_num == 38 || card_num == 51)
        {
            card_value = 10;
            card_name = "Queen";
            display_char = 'Q';
        }
        else if (card_num == 13 || card_num == 26 || card_num == 39 || card_num == 52)
        {
            card_value = 10;
            card_name = "King";
            display_char = 'K';
        }
        //sets 2-10 on next suite
        else if (14 < card_num && card_num < 24)
        {
            card_value = card_num - 13;
            card_name = to_string(card_value);
            display_char = '0' + card_value;
        }
        //sets 2-10 on next suite
        else if (27 < card_num && card_num < 37)
        {
            card_value = card_num - 26;
            card_name = to_string(card_value);
            display_char = '0' + card_value;
        }
        //sets 2-10 on next suite
        else if (40 < card_num && card_num < 50)
        {
            card_value = card_num - 39;
            card_name = to_string(card_value);
            display_char = '0' + card_value;
        }
        
        suite = hearts;
        suite_val = 0;
        if (card_num > 13 && card_num <= 26)
        {
            suite = diamonds;
            suite_val = 1;
        }
        else if (card_num > 26 && card_num <= 39)
        {
            suite = spades;
            suite_val = 2;
        }
        else if (card_num > 39)
        {
            suite = clubs;
            suite_val = 3;
        }

        if (card_value == 10 && (card_num == 10 || card_num == 23 || card_num == 36 || card_num == 49))
        {
            display_char = 't';
        }
        
        deck[i].create_card(card_name + " of " + suite, card_num, card_value, suite_val, display_char);

        // cout << "\nvalue = " << deck[i].retvalue() << " num = " << deck[i].show_number();

        card_num++;
    }

    char dis1, dis2;
    int suite_no1, suite_no2;

    //player gets dealt card
    next_card = deck_order.at(deck_pos);
    card1 = deck[next_card].retvalue();
    player1.dealt_card(card1);
    suite_no1 = deck[next_card].retsuite();
    dis1 = deck[next_card].display();

    if (card1 == 11)
    {
        player1.has_ace();
    }
    cout << "You were dealt the " << deck[next_card].name();
    
    deck_pos++;

    sleep_for(1s);


    //player gets second card dealt
    next_card = deck_order.at(deck_pos);
    card2 = deck[next_card].retvalue();
    player1.dealt_card(card2);
    suite_no2 = deck[next_card].retsuite();
    dis2 = deck[next_card].display();


    if (card2 == 11)
    {
        player1.has_ace();
    }
    cout << "\nYou were dealt the " << deck[next_card].name();
    show_card(suite_no1, dis1, suite_no2, dis2);

    deck_pos++;

    sleep_for(1s);


    //dealer shows first card
    next_card = deck_order.at(deck_pos);
    card3 = deck[next_card].retvalue();
    dealer.dealt_card(card3);
    suite_no1 = deck[next_card].retsuite();
    dis1 = deck[next_card].display();

    if (card3 == 11)
    {
        dealer.has_ace();
    }
    cout << "\n\nDealer has the " << deck[next_card].name();
    show_card(suite_no1, dis1, 0, '0');

    deck_pos++;

    sleep_for(1s);


    //handles if player wants to hit or stand
    while (player1.retvalue() <= 21 && stand == false)
    {
        hit_me = hit(player1.retvalue());

        if (hit_me)
        {
            next_card = deck_order.at(deck_pos);
            card = deck[next_card].retvalue();
            player1.dealt_card(card);
            suite_no1 = deck[next_card].retsuite();
            dis1 = deck[next_card].display();
            deck_pos++;

            if (card == 11)
            {
                player1.has_ace();
            }
            cout << "\nYou were dealt the " << 
                     deck[next_card].name();
            show_card(suite_no1, dis1, 0, '0');
                }
        else
        {
            stand = true;
        }
        if (player1.retvalue() > 21 && player1.retace() > 0)
        {
            player1.reduce_ace_val();
            //cout << "\nyou had an ace so total has been reduced to " << player1.retvalue();
        }
    }
    
    if (player1.retvalue() > 21)
    {
        cout << "\n\nYour cards total "<< player1.retvalue() << " and went over 21, you lost.";
        cin.ignore();
        cin.get();
    }
    else 
    {
        while (!dealer_stand)
        {
            if (dealer.retvalue() < 17)
            {
                next_card = deck_order.at(deck_pos);
                card = deck[next_card].retvalue();
                dealer.dealt_card(card);
                suite_no1 = deck[next_card].retsuite();
                dis1 = deck[next_card].display();
                deck_pos++;

                if (card == 11)
                {
                    dealer.has_ace();
                }
                cout << "\n\nDealer has the " << deck[next_card].name();
                cout << "\nDealer Total = " << dealer.retvalue();
                show_card(suite_no1, dis1, 0, '0');
                sleep_for(2s);
            }
            else if (dealer.retvalue() > 21 && dealer.retace() > 0)
            {
                dealer.reduce_ace_val();
                //cout << "\nDealer had an ace so total has been reduced to " << dealer.retvalue();
            }
            else
            {
                dealer_stand = true;
            }
          
        }
        
    }
    //takes total values in hands and shows who won
    player_hand = player1.retvalue();
    dealer_hand = dealer.retvalue();
    bool exit = show_winner(dealer_hand, player_hand);
    return exit;
}


//asks player if they want to hit for another card or stand
bool hit(int hand)
{
    // need to implement error handling on bad inputs
    char option;
    cout << "\n\nYour hand is " << hand;
    cout << "\nWould you like to hit?(y/n): ";
    cin >> option;

    if (option == 'y')
    {
        return true;
    }
    else
    {
        return false;
    }
}


//!!! NEED TO CLEAN UP, OPTIMIZE, MAKE MORE READABLE, ADD COMMENTS
//converts mode to allow use of unicode characters and displays cards to the terminal
void show_card(int suite, char card, int suite2, char card2)
{
    //setsmode to allow unicode characters to print to console using wprintf
    _setmode(_fileno(stdout), _O_U16TEXT);
    
    // unicode characters used
    // \x2554 = top left corner
    // \x2557 = top right corner
    // \x2550 = across
    // \x2551 = side bar
    // \x255a = bottom left
    // \x255d = bottom right
    // \x2660 = spades
    // \x2665 = hearts
    // \x2663 = clubs
    // \x2666 = diamonds


    /*
        suite numbers
        0 = hearts
        1 = diamonds
        2 = spades
        3 = clubs
     */


    //top of card
    wprintf(BLACK L"\n\x2554\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2557");
    wprintf(RESET L"");


    if (card2 != '0') 
    {
        wprintf(RESET L"   ");
        wprintf(BLACK L"\x2554\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x2557");
        wprintf(RESET L"");
    }
    
    //body   (use if statements for suites and num 10)
    if (card == 't')
    {
        wprintf(BLACK L"\n\x2551%d     \x2551", 10);
        wprintf(RESET L"");
    }
    else
    {
        wprintf(BLACK L"\n\x2551%c      \x2551", card);
        wprintf(RESET L"");
    }

    if (card2 == 't')
    {
        wprintf(RESET L"   ");
        wprintf(BLACK L"\x2551%d     \x2551", 10);
        wprintf(RESET L"");
    }
    else if (card2 != '0' && card2 != 't')
    {
        wprintf(RESET L"   ");
        wprintf(BLACK L"\x2551%c      \x2551", card2);
        wprintf(RESET L"");
    }

    
    wprintf(BLACK L"\n\x2551       \x2551");
    wprintf(RESET L"");

    if (card2 != '0')
    {
        wprintf(RESET L"   ");
        wprintf(BLACK L"\x2551       \x2551");
        wprintf(RESET L"");
    }



    if (suite == 0)
    {
        wprintf(BLACK L"\n\x2551   ");
        wprintf(RED L"\x2665");
        wprintf(BLACK L"   \x2551");
        wprintf(RESET L"");
    }
    else if (suite == 1)
    {
        wprintf(BLACK L"\n\x2551   ");
        wprintf(RED L"\x2666");
        wprintf(BLACK L"   \x2551");
        wprintf(RESET L"");
    }
    else if (suite == 2)
    {
        wprintf(BLACK L"\n\x2551   \x2660   \x2551");
        wprintf(RESET L"");
    }
    else if (suite == 3)
    {
        wprintf(BLACK L"\n\x2551   \x2663   \x2551");
        wprintf(RESET L"");
    }


    if (card2 != '0')
    {
        if (suite2 == 0)
        {
            wprintf(RESET L"   ");
            wprintf(BLACK L"\x2551   ");
            wprintf(RED L"\x2665");
            wprintf(BLACK L"   \x2551");
            wprintf(RESET L"");
        }
        else if (suite2 == 1)
        {
            wprintf(RESET L"   ");
            wprintf(BLACK L"\x2551   ");
            wprintf(RED L"\x2666");
            wprintf(BLACK L"   \x2551");
            wprintf(RESET L"");
        }
        else if (suite2 == 2)
        {
            wprintf(RESET L"   ");
            wprintf(BLACK L"\x2551   \x2660   \x2551");
            wprintf(RESET L"");
        }
        else if (suite2 == 3)
        {
            wprintf(RESET L"   ");
            wprintf(BLACK L"\x2551   \x2663   \x2551");
            wprintf(RESET L"");
        }
    }

    wprintf(BLACK L"\n\x2551       \x2551");
    wprintf(RESET L"");

    if (card2 != '0')
    {
        wprintf(RESET L"   ");
        wprintf(BLACK L"\x2551       \x2551");
        wprintf(RESET L"");
    }

    if (card == 't')
    {
        wprintf(BLACK L"\n\x2551     %d\x2551", 10);
        wprintf(RESET L"");
    }
    else
    {
        wprintf(BLACK L"\n\x2551      %c\x2551", card);
        wprintf(RESET L"");
    }
    
    if (card2 == 't' && card2 != '0')
    {
        wprintf(RESET L"   ");
        wprintf(BLACK L"\x2551     %d\x2551", 10);
        wprintf(RESET L"");
    }
    else if (card2 != '0')
    {
        wprintf(RESET L"   ");
        wprintf(BLACK L"\x2551      %c\x2551", card2);
        wprintf(RESET L"");
    }

    //bottom
    wprintf(BLACK L"\n\x255a\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x255d");
    wprintf(RESET L"");
    if (card2 != '0')
    {
        wprintf(RESET L"   ");
        wprintf(BLACK L"\x255a\x2550\x2550\x2550\x2550\x2550\x2550\x2550\x255d");
        wprintf(RESET L"");
    }
    wprintf(RESET L"\n\n");
    //reverts mode back to original mode for cout, causes errors if this isn't done
    _setmode(_fileno(stdout), _O_TEXT);
}


//handles logic for figuring out and displaying who won
bool show_winner(int dealer_hand, int player_hand)
{
    cout << "\n\nPlayer Total = " << player_hand;
    cout << "\nDealer Total = " << dealer_hand;
    char restart;
    int shuffle_time = 3;
    string dot = ". ";
    string wait = ". ";

    if (player_hand > 21 || (21 >= dealer_hand && dealer_hand > player_hand))
    {
        cout << "\n\nDealer had " << dealer_hand <<
            " and you had " << player_hand << ", you lose.";
    }
    else if (21 >= dealer_hand && dealer_hand == player_hand)
    {
        cout << "\n\nYou both had " << player_hand << " so it's a push";
    }
    else
    {
        cout << "\n\nDealer had " << dealer_hand <<
            " and you had " << player_hand << ", you win.";
    }

    bool _restart = false;
    bool exit = false;

    while (!_restart)
    {
        cout << "\n\nWould you like to play again?(y/n): ";
        cin >> restart;

        if (restart == 'y')
        {
            for (int i = 0; i != shuffle_time; i++)
            {
                system("cls");
                cout << "Shuffling " << wait;
                wait = wait + dot;
                sleep_for(1s);
            }
            _restart = true;
        }
        else if (restart == 'n')
        {
            cout << "\nThanks for playing!!\n\n";
            _restart = true;
            exit = true;
        }
        else 
        {
            cout << "\nThat's not a valid response...\n";
        }
    }

    return exit;
}